#include "code.h"
#include "utils.h"
#include "unity.h"
#include "defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

int pop_file;
FILE *pop_output_file = NULL;
const char pop_output_file_path[] = "test/files/write_pop_instruction.asm";

int push_file;
FILE *push_output_file = NULL;
const char push_output_file_path[] = "test/files/write_push_instruction.asm";

int arithmetic_file;
FILE *arithmetic_output_file = NULL;
const char arithmetic_output_file_path[] = "test/files/write_arithmetic_instruction.asm";

char line[LINE_BUFF];
char *g_file_name = NULL; 

void setUp(void) {
    pop_output_file = fopen(pop_output_file_path, "w");
    pop_file = open(pop_output_file_path, O_RDONLY);

    push_output_file = fopen(push_output_file_path, "w");
    push_file = open(push_output_file_path, O_RDONLY);

    arithmetic_output_file = fopen(arithmetic_output_file_path, "w");
    arithmetic_file = open(arithmetic_output_file_path, O_RDONLY);

    g_file_name = get_file_name("some/buried/folder/Foo.vm");
}

void tearDown(void) {
    close(pop_file);
    fclose(pop_output_file);
    remove(pop_output_file_path);

    close(push_file);
    fclose(push_output_file);
    remove(push_output_file_path);

    close(arithmetic_file);
    fclose(arithmetic_output_file);
    remove(arithmetic_output_file_path);
}

void test_write_pop_instruction(void) {
    const char pop_base_asm[] = 
        "@%s\n"
        "D=M\n"
        "@%d\n"
        "D=D+A\n"
        "@R15\n"
        "M=D\n"
        "@SP\n"
        "AM=M-1\n"
        "D=M\n"
        "@R15\n"
        "A=M\n"
        "M=D\n";

    // should generate correct asm code
    // pop argument 2
    char **parts = explode("pop argument 2", " ");
    char pop_argument_2_asm[LINE_BUFF];
    snprintf(pop_argument_2_asm, LINE_BUFF, pop_base_asm, "ARG", 2);

    write_pop_instruction(parts, pop_output_file);
    free_exploded(parts);
    rewind(pop_output_file);

    read(pop_file, line, LINE_BUFF);
    TEST_ASSERT_EQUAL_STRING(pop_argument_2_asm, line);
    fseek(pop_output_file, strlen(pop_argument_2_asm), SEEK_SET);

    // pop local 120
    parts = explode("pop local 120", " ");
    char pop_local_120_asm[LINE_BUFF];
    snprintf(pop_local_120_asm, LINE_BUFF, pop_base_asm, "LCL", 120);

    write_pop_instruction(parts, pop_output_file);
    free_exploded(parts);
    fseek(pop_output_file, -strlen(pop_local_120_asm), SEEK_CUR);

    memset(line, 0, LINE_BUFF);
    read(pop_file, line, LINE_BUFF);
    TEST_ASSERT_EQUAL_STRING(pop_local_120_asm, line);
    fseek(pop_output_file, strlen(pop_local_120_asm), SEEK_CUR);

    // pop this 3
    parts = explode("pop this 3", " ");
    char pop_this_3_asm[LINE_BUFF];
    snprintf(pop_this_3_asm, LINE_BUFF, pop_base_asm, "THIS", 3);

    write_pop_instruction(parts, pop_output_file);
    free_exploded(parts);
    fseek(pop_output_file, -strlen(pop_this_3_asm), SEEK_CUR);

    memset(line, 0, LINE_BUFF);
    read(pop_file, line, LINE_BUFF);
    TEST_ASSERT_EQUAL_STRING(pop_this_3_asm, line);
    fseek(pop_output_file, strlen(pop_this_3_asm), SEEK_CUR);

    // pop that 10
    parts = explode("pop that 10", " ");
    char pop_that_10_asm[LINE_BUFF];
    snprintf(pop_that_10_asm, LINE_BUFF, pop_base_asm, "THAT", 10);

    write_pop_instruction(parts, pop_output_file);
    free_exploded(parts);
    fseek(pop_output_file, -strlen(pop_that_10_asm), SEEK_CUR);

    memset(line, 0, LINE_BUFF);
    read(pop_file, line, LINE_BUFF);
    TEST_ASSERT_EQUAL_STRING(pop_that_10_asm, line);
    fseek(pop_output_file, strlen(pop_that_10_asm), SEEK_CUR);

    const char pop_static_base[] =
        "@SP\n"
        "AM=M-1\n"
        "D=M\n"
        "@%s.%d\n"
        "M=D\n";

    // pop static 100
    parts = explode("pop static 100", " ");
    char pop_static_100_asm[LINE_BUFF];
    snprintf(pop_static_100_asm, LINE_BUFF, pop_static_base, g_file_name, 100);

    write_pop_instruction(parts, pop_output_file);
    free_exploded(parts);
    fseek(pop_output_file, -strlen(pop_static_100_asm), SEEK_CUR);

    memset(line, 0, LINE_BUFF);
    read(pop_file, line, LINE_BUFF);
    TEST_ASSERT_EQUAL_STRING(pop_static_100_asm, line);
    fseek(pop_output_file, strlen(pop_static_100_asm), SEEK_CUR);

    // pop pointer 0 == RAM[3] = RAM[--SP]
    const char pop_pointer_base[] =
        "@SP\n"
        "AM=M-1\n"
        "D=M\n"
        "@%d\n"
        "M=D\n";

    // pop pointer 0
    parts = explode("pop pointer 0", " ");
    char pop_pointer_0_asm[LINE_BUFF];
    snprintf(pop_pointer_0_asm, LINE_BUFF, pop_pointer_base, 3);

    write_pop_instruction(parts, pop_output_file);
    free_exploded(parts);
    fseek(pop_output_file, -strlen(pop_pointer_0_asm), SEEK_CUR);

    memset(line, 0, LINE_BUFF);
    read(pop_file, line, LINE_BUFF);
    TEST_ASSERT_EQUAL_STRING(pop_pointer_0_asm, line);
    fseek(pop_output_file, strlen(pop_pointer_0_asm), SEEK_CUR);

    // pop pointer 1 == RAM[4] = RAM[--SP]
    parts = explode("pop pointer 1", " ");
    char pop_pointer_1_asm[LINE_BUFF];
    snprintf(pop_pointer_1_asm, LINE_BUFF, pop_pointer_base, 4);

    write_pop_instruction(parts, pop_output_file);
    free_exploded(parts);
    fseek(pop_output_file, -strlen(pop_pointer_1_asm), SEEK_CUR);

    memset(line, 0, LINE_BUFF);
    read(pop_file, line, LINE_BUFF);
    TEST_ASSERT_EQUAL_STRING(pop_pointer_1_asm, line);
    fseek(pop_output_file, strlen(pop_pointer_1_asm), SEEK_CUR);

    // pop temp i == RAM[5+i] = RAM[--SP]
    const char temp_base_asm[] = 
        "@%d\n"
        "D=A\n"
        "@R15\n"
        "M=D\n"
        "@SP\n"
        "AM=M-1\n"
        "D=M\n"
        "@R15\n"
        "A=M\n"
        "M=D\n";
    const int temp_base_address = 5;

    parts = explode("pop temp 0", " ");
    char pop_temp_0_asm[LINE_BUFF];
    snprintf(pop_temp_0_asm, LINE_BUFF, temp_base_asm, temp_base_address+0);

    write_pop_instruction(parts, pop_output_file);
    free_exploded(parts);
    fseek(pop_output_file, -strlen(pop_temp_0_asm), SEEK_CUR);

    memset(line, 0, LINE_BUFF);
    read(pop_file, line, LINE_BUFF);
    TEST_ASSERT_EQUAL_STRING(pop_temp_0_asm, line);
    fseek(pop_output_file, strlen(pop_temp_0_asm), SEEK_CUR);
}

void test_write_push_instruction(void) {
    const char push_base_asm[]=
        "@%s\n"
        "D=M\n"
        "@%d\n"
        "A=D+A\n"
        "D=M\n"
        "@SP\n"
        "A=M\n"
        "M=D\n"
        "@SP\n"
        "M=M+1\n";

    // push argument 2
    char **parts = explode("push argument 2", " ");
    char push_argument_2_asm[LINE_BUFF];
    snprintf(push_argument_2_asm, LINE_BUFF, push_base_asm, "ARG", 2);

    write_push_instruction(parts, push_output_file);
    free_exploded(parts);
    rewind(push_output_file);

    memset(line, 0, LINE_BUFF);
    read(push_file, line, LINE_BUFF);
    TEST_ASSERT_EQUAL_STRING(push_argument_2_asm, line);
    fseek(push_output_file, strlen(push_argument_2_asm), SEEK_SET);

    // push local 20
    parts = explode("push local 20", " ");
    char push_local_20_asm[LINE_BUFF];
    snprintf(push_local_20_asm, LINE_BUFF, push_base_asm, "LCL", 20);

    write_push_instruction(parts, push_output_file);
    free_exploded(parts);
    fseek(push_output_file, -strlen(push_local_20_asm), SEEK_CUR);

    memset(line, 0, LINE_BUFF);
    read(push_file, line, LINE_BUFF);
    TEST_ASSERT_EQUAL_STRING(push_local_20_asm, line);
    fseek(push_output_file, strlen(push_local_20_asm), SEEK_CUR);

    // push this 10
    parts = explode("push this 10", " ");
    char push_this_10_asm[LINE_BUFF];
    snprintf(push_this_10_asm, LINE_BUFF, push_base_asm, "THIS", 10);

    write_push_instruction(parts, push_output_file);
    free_exploded(parts);
    fseek(push_output_file, -strlen(push_this_10_asm), SEEK_CUR);

    memset(line, 0, LINE_BUFF);
    read(push_file, line, LINE_BUFF);
    TEST_ASSERT_EQUAL_STRING(push_this_10_asm, line);
    fseek(push_output_file, strlen(push_this_10_asm), SEEK_CUR);

    // push that 5
    parts = explode("push that 5", " ");
    char push_that_5_asm[LINE_BUFF];
    snprintf(push_that_5_asm, LINE_BUFF, push_base_asm, "THAT", 5);

    write_push_instruction(parts, push_output_file);
    free_exploded(parts);
    fseek(push_output_file, -strlen(push_that_5_asm), SEEK_CUR);

    memset(line, 0, LINE_BUFF);
    read(push_file, line, LINE_BUFF);
    TEST_ASSERT_EQUAL_STRING(push_that_5_asm, line);
    fseek(push_output_file, strlen(push_that_5_asm), SEEK_CUR);

    // push constant 7
    const char constant_base_asm[] =
        "@%d\n"
        "D=A\n"
        "@SP\n"
        "A=M\n"
        "M=D\n"
        "@SP\n"
        "M=M+1\n";

    parts = explode("push constant 7", " ");
    char push_constant_7_asm[LINE_BUFF];
    snprintf(push_constant_7_asm, LINE_BUFF, constant_base_asm, 7);

    write_push_instruction(parts, push_output_file);
    free_exploded(parts);
    fseek(push_output_file, -strlen(push_constant_7_asm), SEEK_CUR);

    memset(line, 0, LINE_BUFF);
    read(push_file, line, LINE_BUFF);
    TEST_ASSERT_EQUAL_STRING(push_constant_7_asm, line);
    fseek(push_output_file, strlen(push_constant_7_asm), SEEK_CUR);

    // push static 3
    const char static_base_asm[] =
        "@%s.%s\n"
        "D=M\n"
        "@SP\n"
        "A=M\n"
        "M=D\n"
        "@SP\n"
        "M=M+1\n";

    parts = explode("push static 3", " ");
    char push_static_3_asm[LINE_BUFF];
    snprintf(push_static_3_asm, LINE_BUFF, static_base_asm, g_file_name, "3");

    write_push_instruction(parts, push_output_file);
    free_exploded(parts);
    fseek(push_output_file, -strlen(push_static_3_asm), SEEK_CUR);

    memset(line, 0, LINE_BUFF);
    read(push_file, line, LINE_BUFF);
    TEST_ASSERT_EQUAL_STRING(push_static_3_asm, line);
    fseek(push_output_file, strlen(push_static_3_asm), SEEK_CUR);

    // push pointer 0
    const char pointer_base_asm[] =
        "@%d\n"
        "D=M\n"
        "@SP\n"
        "A=M\n"
        "M=D\n"
        "@SP\n"
        "M=M+1\n";

    parts = explode("push pointer 0", " ");
    char push_pointer_0_asm[LINE_BUFF];
    snprintf(push_pointer_0_asm, LINE_BUFF, pointer_base_asm, 3);

    write_push_instruction(parts, push_output_file);
    free_exploded(parts);
    fseek(push_output_file, -strlen(push_pointer_0_asm), SEEK_CUR);

    memset(line, 0, LINE_BUFF);
    read(push_file, line, LINE_BUFF);
    TEST_ASSERT_EQUAL_STRING(push_pointer_0_asm, line);
    fseek(push_output_file, strlen(push_pointer_0_asm), SEEK_CUR);

    // push pointer 1
    parts = explode("push pointer 1", " ");
    char push_pointer_1_asm[LINE_BUFF];
    snprintf(push_pointer_1_asm, LINE_BUFF, pointer_base_asm, 4);

    write_push_instruction(parts, push_output_file);
    free_exploded(parts);
    fseek(push_output_file, -strlen(push_pointer_1_asm), SEEK_CUR);

    memset(line, 0, LINE_BUFF);
    read(push_file, line, LINE_BUFF);
    TEST_ASSERT_EQUAL_STRING(push_pointer_1_asm, line);
    fseek(push_output_file, strlen(push_pointer_1_asm), SEEK_CUR);

    // push temp 3
    const char temp_base_asm[] =
        "@%d\n"
        "D=M\n"
        "@SP\n"
        "A=M\n"
        "M=D\n"
        "@SP\n"
        "M=M+1\n";
    const int temp_base_address = 5;

    parts = explode("push temp 3", " ");
    char push_temp_3_asm[LINE_BUFF];
    snprintf(push_temp_3_asm, LINE_BUFF, temp_base_asm, temp_base_address+3);

    write_push_instruction(parts, push_output_file);
    free_exploded(parts);
    fseek(push_output_file, -strlen(push_temp_3_asm), SEEK_CUR);

    memset(line, 0, LINE_BUFF);
    read(push_file, line, LINE_BUFF);
    TEST_ASSERT_EQUAL_STRING(push_temp_3_asm, line);
    fseek(push_output_file, strlen(push_temp_3_asm), SEEK_CUR);
}

void test_write_arithmetic_instruction(void) {
    const char pop_two_off_stack[] =
        "@SP\n"
        "AM=M-1\n"
        "D=M\n"
        "@SP\n"
        "AM=M-1\n";
    const char pop_one_off_stack[] =
        "@SP\n"
        "AM=M-1\n"
        "D=M\n";
    const char push_increment[] =
        "@SP\n"
        "A=M\n"
        "M=D\n"
        "@SP\n"
        "M=M+1\n";
    const char comparision_template[] = 
        "@RET_ADDRESS_%s%d\n"
        "D=A\n"
        "@START_%s\n"
        "0;JMP\n"
        "(RET_ADDRESS_%s%d)\n";

    // add
    char add_asm[LINE_BUFF];
    snprintf(add_asm, LINE_BUFF, "%sD=M+D\n%s", pop_two_off_stack, push_increment);

    write_arithmetic_instruction("add", arithmetic_output_file);
    rewind(arithmetic_output_file);

    memset(line, 0, LINE_BUFF);
    read(arithmetic_file, line, LINE_BUFF);
    TEST_ASSERT_EQUAL_STRING(add_asm, line);
    fseek(arithmetic_output_file, strlen(add_asm), SEEK_SET);

    // sub
    char sub_asm[LINE_BUFF];
    snprintf(sub_asm, LINE_BUFF, "%sD=M-D\n%s", pop_two_off_stack, push_increment);

    write_arithmetic_instruction("sub", arithmetic_output_file);
    fseek(arithmetic_output_file, -strlen(sub_asm), SEEK_CUR);

    memset(line, 0, LINE_BUFF);
    read(arithmetic_file, line, LINE_BUFF);
    TEST_ASSERT_EQUAL_STRING(sub_asm, line);
    fseek(arithmetic_output_file, strlen(sub_asm), SEEK_CUR);

    // and
    char and_asm[LINE_BUFF];
    snprintf(and_asm, LINE_BUFF, "%sD=M&D\n%s", pop_two_off_stack, push_increment);

    write_arithmetic_instruction("and", arithmetic_output_file);
    fseek(arithmetic_output_file, -strlen(and_asm), SEEK_CUR);

    memset(line, 0, LINE_BUFF);
    read(arithmetic_file, line, LINE_BUFF);
    TEST_ASSERT_EQUAL_STRING(and_asm, line);
    fseek(arithmetic_output_file, strlen(and_asm), SEEK_CUR);

    // or
    char or_asm[LINE_BUFF];
    snprintf(or_asm, LINE_BUFF, "%sD=M|D\n%s", pop_two_off_stack, push_increment);

    write_arithmetic_instruction("or", arithmetic_output_file);
    fseek(arithmetic_output_file, -strlen(or_asm), SEEK_CUR);

    memset(line, 0, LINE_BUFF);
    read(arithmetic_file, line, LINE_BUFF);
    TEST_ASSERT_EQUAL_STRING(or_asm, line);
    fseek(arithmetic_output_file, strlen(or_asm), SEEK_CUR);

    // not
    char not_asm[LINE_BUFF];
    snprintf(not_asm, LINE_BUFF, "%sD=!D\n%s", pop_one_off_stack, push_increment);

    write_arithmetic_instruction("not", arithmetic_output_file);
    fseek(arithmetic_output_file, -strlen(not_asm), SEEK_CUR);

    memset(line, 0, LINE_BUFF);
    read(arithmetic_file, line, LINE_BUFF);
    TEST_ASSERT_EQUAL_STRING(not_asm, line);
    fseek(arithmetic_output_file, strlen(not_asm), SEEK_CUR);

    // neg
    char neg_asm[LINE_BUFF];
    snprintf(neg_asm, LINE_BUFF, "%sD=-D\n%s", pop_one_off_stack, push_increment);

    write_arithmetic_instruction("neg", arithmetic_output_file);
    fseek(arithmetic_output_file, -strlen(neg_asm), SEEK_CUR);

    memset(line, 0, LINE_BUFF);
    read(arithmetic_file, line, LINE_BUFF);
    TEST_ASSERT_EQUAL_STRING(neg_asm, line);
    fseek(arithmetic_output_file, strlen(neg_asm), SEEK_CUR);

    // eq
    char eq_asm[LINE_BUFF];
    snprintf(eq_asm, LINE_BUFF, comparision_template, "EQ", 0, "EQ", "EQ", 0);

    write_arithmetic_instruction("eq", arithmetic_output_file);
    fseek(arithmetic_output_file, -strlen(eq_asm), SEEK_CUR);

    memset(line, 0, LINE_BUFF);
    read(arithmetic_file, line, LINE_BUFF);
    TEST_ASSERT_EQUAL_STRING(eq_asm, line);
    fseek(arithmetic_output_file, strlen(eq_asm), SEEK_CUR);

    // gt
    char gt_asm[LINE_BUFF];
    snprintf(gt_asm, LINE_BUFF, comparision_template, "GT", 0, "GT", "GT", 0);

    write_arithmetic_instruction("gt", arithmetic_output_file);
    fseek(arithmetic_output_file, -strlen(gt_asm), SEEK_CUR);

    memset(line, 0, LINE_BUFF);
    read(arithmetic_file, line, LINE_BUFF);
    TEST_ASSERT_EQUAL_STRING(gt_asm, line);
    fseek(arithmetic_output_file, strlen(gt_asm), SEEK_CUR);

    // lt
    char lt_asm[LINE_BUFF];
    snprintf(lt_asm, LINE_BUFF, comparision_template, "LT", 0, "LT", "LT", 0);

    write_arithmetic_instruction("lt", arithmetic_output_file);
    fseek(arithmetic_output_file, -strlen(lt_asm), SEEK_CUR);

    memset(line, 0, LINE_BUFF);
    read(arithmetic_file, line, LINE_BUFF);
    TEST_ASSERT_EQUAL_STRING(lt_asm, line);
    fseek(arithmetic_output_file, strlen(lt_asm), SEEK_CUR);
}

void test_segment_to_label(void) {
    // should return 'ARG'
    char s1[] = "argument";
    char s1_expected[] = "ARG";
    segment_to_label(s1);
    TEST_ASSERT_EQUAL_STRING(s1_expected, s1);

    // should return 'LCL'
    char s2[] = "local";
    char s2_expected[] = "LCL";
    segment_to_label(s2);
    TEST_ASSERT_EQUAL_STRING(s2_expected, s2);

    // should return 'THIS'
    char s3[] = "this";
    char s3_expected[] = "THIS";
    segment_to_label(s3);
    TEST_ASSERT_EQUAL_STRING(s3_expected, s3);

    // should return 'THAT'
    char s4[] = "that";
    char s4_expected[] = "THAT";
    segment_to_label(s4);
    TEST_ASSERT_EQUAL_STRING(s4_expected, s4);

    // should return 'TEMP'
    char s5[] = "temp";
    char s5_expected[] = "TEMP";
    segment_to_label(s5);
    TEST_ASSERT_EQUAL_STRING(s5_expected, s5);
}

