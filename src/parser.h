#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>

int scan(FILE *source_file, FILE *output_file);
void bad_instruction(const char *line, int line_number);

#endif // PARSER_H
