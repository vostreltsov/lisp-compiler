#ifndef PARSER_FUNCS_H
#define PARSER_FUNCS_H

#include <malloc.h>
#include "parser_structs.h"

// Creates a program node.
struct program_struct * create_program(struct statement_list * stmts);

struct expression_struct * create_expr_empty();
struct expression_struct * create_expr_int(long int value);
struct expression_struct * create_expr_char(char value);
struct expression_struct * create_expr_string(char * value);   // memory for value is free after this function.
struct expression_struct * create_expr_bool(int value);
struct expression_struct * create_expr_binary(enum expr_type type, struct expression_struct * left, struct expression_struct * right);

#endif


