#include "code.h"
#include "defs.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const int MAX_SEGMENTS_NAME_LENGTH = 4;
static const int MAX_DIGITS_RET_ADDRESS = 3;

static int ret_address_eq_next_index = 0;
static int ret_address_gt_next_index = 0;
static int ret_address_lt_next_index = 0;

extern char *g_file_name;

void static _write_pop_pointer(char **parts, FILE *output_file) {
    int pointer = 0;
    if (strcmp(parts[2], "0") == 0) {
        // THIS = RAM[3]
        pointer = 3;
    } else {
        // 1, THAT = RAM[4]
        pointer = 4;
    }

    const char sp_decrement[] =
        "@SP\n"
        "AM=M-1\n";
    write_output(sp_decrement, output_file);

    const char pop[] =
        "D=M\n"; // save stack's top value in D
    write_output(pop, output_file);

    const char base_update_pointer[] =
        "@%d\n"
        "M=D\n";
    const int final_length = strlen(base_update_pointer) + 1 + MAX_SEGMENTS_NAME_LENGTH;
    char *update_pointer = malloc(sizeof(char) * final_length); 
    snprintf(update_pointer, final_length, base_update_pointer, pointer);
    write_output(update_pointer, output_file);
    free(update_pointer);
}

void static _write_pop_static(char **parts, FILE *output_file) {
    const char sp_decrement[] =
        "@SP\n"
        "AM=M-1\n";
    write_output(sp_decrement, output_file);

    const char pop[] =
        "D=M\n"; // save stack's top value in D
    write_output(pop, output_file);

    const char base_update_static[] =
        "@%s.%s\n"
        "M=D\n";
    const int final_length = strlen(base_update_static) + 1 + strlen(g_file_name) + strlen(parts[2]);
    char *update_static = malloc(sizeof(char) * final_length);
    snprintf(update_static, final_length, base_update_static, g_file_name, parts[2]);
    write_output(update_static, output_file);
    free(update_static);
}

void static _write_pop_temp(char **parts, FILE *output_file) {
    const int temp_base_address = 5;
    const char base_segment_address_w_offset[] =
        "@%d\n"
        "D=A\n"   // temp base + index address
        "@R15\n"
        "M=D\n";    // save address in R15 as temp
    const int final_length = strlen(base_segment_address_w_offset) + 1 + MAX_SEGMENTS_NAME_LENGTH * 2;
    char *segment_address_w_offset = malloc(sizeof(char) * final_length);
    snprintf(
        segment_address_w_offset, 
        final_length,
        base_segment_address_w_offset,
        temp_base_address + atoi(parts[2])
    );
    write_output(segment_address_w_offset, output_file);
    free(segment_address_w_offset);

    const char sp_decrement[] =
        "@SP\n"
        "AM=M-1\n";
    write_output(sp_decrement, output_file);

    const char pop[] =
        "D=M\n"; // save stack's top value in D
    write_output(pop, output_file);

    const char update_segment_w_offset[] =
        "@R15\n"
        "A=M\n"
        "M=D\n"; // update segment with popped value
    write_output(update_segment_w_offset, output_file);
}

void static _write_pop_general(char **parts, FILE *output_file) {
    segment_to_label(parts[1]);
    if (strcmp(parts[1], "TEMP") == 0) {
        // base address for temp
        strcpy(parts[1], "5");
        printf("temp!!!\n");
    }
    const char base_segment_address_w_offset[] =
        "@%s\n"
        "D=M\n"     // segment base address
        "@%s\n"
        "D=D+A\n"   // append offset
        "@R15\n"
        "M=D\n";    // save address in R15 as temp
    const int final_length = strlen(base_segment_address_w_offset) + 1 + MAX_SEGMENTS_NAME_LENGTH * 2;
    char *segment_address_w_offset = malloc(sizeof(char) * final_length);
    snprintf(
        segment_address_w_offset, 
        final_length,
        base_segment_address_w_offset,
        parts[1],
        parts[2]
    );
    write_output(segment_address_w_offset, output_file);
    free(segment_address_w_offset);

    const char sp_decrement[] =
        "@SP\n"
        "AM=M-1\n";
    write_output(sp_decrement, output_file);

    const char pop[] =
        "D=M\n"; // save stack's top value in D
    write_output(pop, output_file);

    const char update_segment_w_offset[] =
        "@R15\n"
        "A=M\n"
        "M=D\n"; // update segment with popped value
    write_output(update_segment_w_offset, output_file);
}

void write_pop_instruction(char **parts, FILE *output_file) {
    if(strcmp(parts[1], "temp") == 0) {
        _write_pop_temp(parts, output_file);
    } else if(strcmp(parts[1], "static") == 0) {
        _write_pop_static(parts, output_file);
    } else if(strcmp(parts[1], "pointer") == 0) {
        _write_pop_pointer(parts, output_file);
    } else {
        _write_pop_general(parts, output_file);
    }
}

void write_push_instruction(char **parts, FILE *output_file){
    if(strcmp(parts[1], "constant") == 0) {
        const char base_constant[] =
            "@%s\n"
            "D=A\n"; // save value in D
        const int final_length = strlen(base_constant) + 1 + MAX_SEGMENTS_NAME_LENGTH;
        char *constant = malloc(sizeof(char) * final_length);
        snprintf(constant, final_length, base_constant, parts[2]);
        write_output(constant, output_file);
        free(constant);

    } else if(strcmp(parts[1], "temp") == 0) {
        const int temp_base_address = 5;
        const char base_temp[] =
            "@%d\n" // temp base + index
            "D=M\n"; 
        const int final_length = strlen(base_temp) + 1 + MAX_SEGMENTS_NAME_LENGTH;
        char *temp = malloc(sizeof(char) * final_length);
        snprintf(temp, final_length, base_temp, temp_base_address + atoi(parts[2]));
        write_output(temp, output_file);
        free(temp);

    } else if(strcmp(parts[1], "static") == 0)  {
        const char base_update_static[] =
            "@%s.%s\n" 
            "D=M\n"; 
        const int final_length = strlen(base_update_static) + 1 + strlen(g_file_name) + strlen(parts[2]);
        char *update_static = malloc(sizeof(char) * final_length);
        snprintf(update_static, final_length, base_update_static, g_file_name, parts[2]);
        write_output(update_static, output_file);
        free(update_static);

    } else if(strcmp(parts[1], "pointer") == 0) { 
        int pointer = 0;
        if(strcmp(parts[2], "0") == 0) {
            // THIS = RAM[3]
            pointer = 3;
        } else {
            // 1, THAT = RAM[4]
            pointer = 4;
        }

        const char base_update_pointer[] =
            "@%d\n" 
            "D=M\n"; 
        const int final_length = strlen(base_update_pointer) + 1 + MAX_SEGMENTS_NAME_LENGTH;
        char *update_pointer = malloc(sizeof(char) * final_length);
        snprintf(update_pointer, final_length, base_update_pointer, pointer);
        write_output(update_pointer, output_file);
        free(update_pointer);
    } else {
        segment_to_label(parts[1]);
        const char base_segment_w_offset_value[] = 
            "@%s\n"
            "D=M\n"     // segment base address
            "@%s\n"
            "A=D+A\n"   // append offset
            "D=M\n";    // save value in D
        const int final_length = strlen(base_segment_w_offset_value) + 1 + MAX_SEGMENTS_NAME_LENGTH * 2;
        char *segment_w_offset_value = malloc(sizeof(char) * final_length);
        snprintf(segment_w_offset_value, final_length, base_segment_w_offset_value, parts[1], parts[2]);
        write_output(segment_w_offset_value, output_file);
        free(segment_w_offset_value);
    }

    const char push_increment[] =
        "@SP\n"
        "A=M\n"
        "M=D\n" // D from above
        "@SP\n"
        "M=M+1\n";
    write_output(push_increment, output_file);
}

static void _write_arithmetic_native(const char *instruction, FILE *output_file) {
    const char *binary_operators[] = { "add", "sub", "and", "or", NULL };

    if(in_arr(instruction, binary_operators)) {
        const char pop_two_off_stack[] =
            "@SP\n"
            "AM=M-1\n"
            "D=M\n" // D = y
            "@SP\n"
            "AM=M-1\n";
        write_output(pop_two_off_stack, output_file);

        // M = x
        if(strcmp(instruction, "add") == 0) {
            write_output("D=M+D\n", output_file);
        } else if(strcmp(instruction, "sub") == 0) {
            write_output("D=M-D\n", output_file);
        } else if(strcmp(instruction, "and") == 0) {
            write_output("D=M&D\n", output_file);
        }
        // or
        else {
            write_output("D=M|D\n", output_file);
        }

    } else {
        const char pop_one_off_stack[] =
            "@SP\n"
            "AM=M-1\n"
            "D=M\n"; // D = y
        write_output(pop_one_off_stack, output_file);

        if(strcmp(instruction, "not") == 0) {
            write_output("D=!D\n", output_file);
        }
        // neg
        else {
            write_output("D=-D\n", output_file);
        }
    }

    const char push_increment[] =
        "@SP\n"
        "A=M\n"
        "M=D\n" // D from above
        "@SP\n"
        "M=M+1\n";
    write_output(push_increment, output_file);
}

static void _write_arithmetic_comparison(const char *instruction, FILE *output_file) {
    char base[LINE_BUFF];
    int *ret_address_next;
    const char common_template[] = 
        "@RET_ADDRESS_%s%%d\n"
        "D=A\n"
        "@START_%s\n"
        "0;JMP\n"
        "(RET_ADDRESS_%s%%d)\n";

    // computation is done in the respective code segments
    // see setup function
    if(strcmp(instruction, "eq") == 0) {
        char base_eq[LINE_BUFF];
        snprintf(base_eq, LINE_BUFF, common_template, "EQ", "EQ", "EQ");
        strcpy(base, base_eq);
        ret_address_next = &ret_address_eq_next_index;

    } else if(strcmp(instruction, "gt") == 0) {
        char base_gt[LINE_BUFF];
        snprintf(base_gt, LINE_BUFF, common_template, "GT", "GT", "GT");
        strcpy(base, base_gt);
        ret_address_next = &ret_address_gt_next_index;

    } else if(strcmp(instruction, "lt") == 0) {
        char base_lt[LINE_BUFF];
        snprintf(base_lt, LINE_BUFF, common_template, "LT", "LT", "LT");
        strcpy(base, base_lt);
        ret_address_next = &ret_address_lt_next_index;
    }

    const int final_length = strlen(base) + 1 + MAX_DIGITS_RET_ADDRESS * 2;
    char *instructions = malloc(sizeof(char) * final_length);
    snprintf(
        instructions,
        final_length,
        base,
        *ret_address_next,
        *ret_address_next
    );
    write_output(instructions, output_file);
    (*ret_address_next)++;
}

void write_arithmetic_instruction(const char *instruction, FILE *output_file) {
    const char *native_operations[] = {
        "add",
        "sub",
        "and",
        "or",
        "not",
        "neg",
        NULL
    };

    if(in_arr(instruction, native_operations)) {
        _write_arithmetic_native(instruction, output_file);
    } else {
        _write_arithmetic_comparison(instruction, output_file);
    }
}

void segment_to_label(char *s) {
    if(strcmp(s, "argument") == 0) {
        strcpy(s, "ARG");
    } else if(strcmp(s, "local") == 0) {
        strcpy(s, "LCL");
    // this, that and temp
    } else {
       strtoupper(s);
    }
}

void vm_setup(FILE *output_file) {
    const char setup[] =
        // @SP base at 256
        "@256\n"
        "D=A\n"
        "@SP\n"
        "M=D\n"
        "@START_PROG\n"
        "0;JMP\n"
        // EQ
        "(START_EQ)\n"
        "@R15\n"
        "M=D\n"
        "@SP\n"
        "AM=M-1\n"
        "D=M\n"
        "A=A-1\n"
        "D=M-D\n"
        "M=0\n"
        "@END_EQ\n"
        "D;JNE\n"
        "@SP\n"
        "A=M-1\n"
        "M=-1\n"
        "(END_EQ)\n"
        "@R15\n"
        "A=M\n"
        "0;JMP\n"
        // GT
        "(START_GT)\n"
        "@R15\n"
        "M=D\n"
        "@SP\n"
        "AM=M-1\n"
        "D=M\n"
        "A=A-1\n"
        "D=M-D\n"
        "M=0\n"
        "@END_GT\n"
        "D;JLE\n"
        "@SP\n"
        "A=M-1\n"
        "M=-1\n"
        "(END_GT)\n"
        "@R15\n"
        "A=M\n"
        "0;JMP\n"
        // LT
        "(START_LT)\n"
        "@R15\n"
        "M=D\n"
        "@SP\n"
        "AM=M-1\n"
        "D=M\n"
        "A=A-1\n"
        "D=M-D\n"
        "M=0\n"
        "@END_LT\n"
        "D;JGE\n"
        "@SP\n"
        "A=M-1\n"
        "M=-1\n"
        "(END_LT)\n"
        "@R15\n"
        "A=M\n"
        "0;JMP\n"
        "(START_PROG)\n";


    write_output(setup, output_file);
}

void vm_end(FILE *output_file) {
    const char end_asm[] =
        "(END)\n"
        "@END\n"
        "0;JMP\n";
    write_output(end_asm, output_file);
}
