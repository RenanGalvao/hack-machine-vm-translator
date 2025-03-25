#include <stdio.h>
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
    const char re_is_arithmetic[] = "^(add|sub|eq|gt|lt|and|or|neg|not)";

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
            int size = arrlen(parts);
            if(size != 3) {
                bad_instruction(line, line_number);
                return -1;
            }
            // TODO validar uso do pointer(1) e temp(7),
            // já que os indíces são limitados

            // TODO treat pointer, temp e static a parte
            // funções para cada, verificar qual o segmento
            // e direcionar

            write_pop_instruction(parts, output_file);
            free_exploded(parts);
        }

        // push
        re_match(re_is_push, line, pmatch);
        if(pmatch[0].rm_so != -1) {
            char **parts = explode(line, " ");

            // validate instruction
            int size = arrlen(parts);
            if(size != 3) {
                bad_instruction(line, line_number);
                return -1;
            }

            write_push_instruction(parts, output_file);
            free_exploded(parts);
        }

        // arithmetic
        re_match(re_is_arithmetic, line, pmatch);
        if(pmatch[0].rm_so != -1) {
            char **parts = explode(line, " ");

            // validate instruction
            int size = arrlen(parts);
            if(size != 1) {
                bad_instruction(line, line_number);
                return -1;
            }

            write_arithmetic_instruction(line, output_file);
            free_exploded(parts);
        }
    }

    vm_end(output_file);

    return 0;
}

void bad_instruction(const char *line, int line_number) {
    fprintf(stderr, "Bad instruction at line %d: '%s'\n", line_number+1, line);
}
