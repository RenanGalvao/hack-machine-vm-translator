#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "parser.h"
#include "code.h"
#include "defs.h"
#include "utils.h"

int scan(FILE *source_file, FILE *output_file) {
    regmatch_t pmatch[1];

    // can't pop to constant
    const char re_is_pop[] = "pop "
        "(argument|local|this|that|static|pointer|temp) "
        "[[:digit:]]";
    const char re_is_push[] = "push "
        "(argument|local|this|that|static|constant|pointer|temp) "
        "[[:digit:]]";
    const char re_is_arithmetic[] = "^(add|sub|eq|gt|lt|and|or|neg|not)$";

    char line[LINE_BUFF];
    int line_number = 0;


    vm_setup(output_file);

    while(fgets(line, LINE_BUFF, source_file) != NULL) {
        // prepare line
        clear_line(line);

        // ignore empty lines
        if(is_empty_string(line)) {
            line_number++;
            continue;
        }

        // pop
        re_match(re_is_pop, line, pmatch);
        if(pmatch[0].rm_so != -1) {
            char **parts = explode(line, " ");

            // validate instruction
            bool is_bad_instruction = false;

            if(arrlen(parts) != 3) {
                is_bad_instruction = true;
            } else if (strcmp(parts[1], "pointer") == 0 && atoi(parts[2]) > MAX_POINTER_INDEX) {
                is_bad_instruction = true;
            } else if(strcmp(parts[1], "temp") == 0 && atoi(parts[2]) > MAX_TEMP_INDEX) {
                is_bad_instruction = true;
            }

            if(is_bad_instruction) {
                bad_instruction(line, line_number);
                return -1;
            }

            write_pop_instruction(parts, output_file);
            free_exploded(parts);

            line_number++;
            continue;
        }

        // push
        re_match(re_is_push, line, pmatch);
        if(pmatch[0].rm_so != -1) {
            char **parts = explode(line, " ");

            // validate instruction
            bool is_bad_instruction = false;

            if(arrlen(parts) != 3) {
                is_bad_instruction = true;
            } else if (strcmp(parts[1], "pointer") == 0 && atoi(parts[2]) > MAX_POINTER_INDEX) {
                is_bad_instruction = true;
            } else if(strcmp(parts[1], "temp") == 0 && atoi(parts[2]) > MAX_TEMP_INDEX) {
                is_bad_instruction = true;
            } else if(strcmp(parts[1], "constant") == 0 && atoi(parts[2]) > MAX_CONSTANT_VALUE) {
                is_bad_instruction = true;
            }

            if(is_bad_instruction) {
                bad_instruction(line, line_number);
                return -1;
            }

            write_push_instruction(parts, output_file);
            free_exploded(parts);

            line_number++;
            continue;
        }

        // arithmetic
        re_match(re_is_arithmetic, line, pmatch);
        if(pmatch[0].rm_so != -1) {
            char **parts = explode(line, " ");

            // no need to check, regex does all the job
            write_arithmetic_instruction(line, output_file);
            free_exploded(parts);

            line_number++;
            continue;
        }

        // no match
        bad_instruction(line, line_number);
        return -1;
    }

    vm_end(output_file);

    return 0;
}

void bad_instruction(const char *line, int line_number) {
    fprintf(stderr, "Bad instruction at line %d: '%s'\n", line_number+1, line);
}
