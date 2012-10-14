#ifndef DOTCODE_H
#define DOTCODE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../parser_structs.h"

#define BUFFER_SIZE 128

void exec_dot(const char * dotFileName, const char * pngFileName);

void dot_for_program(FILE * file, struct program_struct * program);
void dot_for_s_expr(FILE * file, char * lastNode, char * label, struct s_expr_struct * expr);
void dot_for_s_expr_seq(FILE * file, char * lastNode, char * label, struct s_expr_seq_struct * expr_list);
void dot_for_list(FILE * file, char * lastNode, char * label, struct list_struct * list);

#endif
