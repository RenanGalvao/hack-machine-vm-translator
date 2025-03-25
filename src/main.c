#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "parser.h"

char *g_file_name = NULL;

int main(int argc, char *argv[]) {
    if(argc != 2) {
        fprintf(stderr, "Usage: vm-translator <source-file>\n");
        exit(1);
    }

    FILE *source_file = open_source_file(argv[1]);
    FILE *output_file = open_output_file(argv[1], source_file);

    g_file_name = get_file_name(argv[1]);

    // doing all work calling code.h
    scan(source_file, output_file);

    fclose(source_file);
    fclose(output_file);

    return 0;
}
