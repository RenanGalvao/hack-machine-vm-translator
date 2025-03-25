#ifndef CODE_H
#define CODE_H

#include <stdio.h>

void write_pop_instruction(char **parts, FILE *output_file);
void write_push_instruction(char **parts, FILE *output_file);
void write_arithmetic_instruction(const char *instruction, FILE *output_file);
void segment_to_label(char *s);
void vm_setup(FILE *output_file);
void vm_end(FILE *output_file);

#endif // CODE_H
