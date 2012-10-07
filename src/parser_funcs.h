#ifndef PARSER_FUNCS_H
#define PARSER_FUNCS_H

#include <malloc.h>
#include "parser_structs.h"

// Creates a program node.
struct program_struct * create_program(struct statement_list * stmts);

// Creates a binary operator node.
struct expression_struct * create_expr_binary(enum expr_type type, struct expression_struct * left, struct expression_struct * right);

#endif


