#ifndef PARSER_FUNCS_H
#define PARSER_FUNCS_H

#include <malloc.h>
#include <string.h>
#include "parser_structs.h"

struct program_struct * create_program(struct s_expr_struct * s_expr);
struct s_expr_struct * create_s_expr_empty();
struct s_expr_struct * create_s_expr_int(long int value);
struct s_expr_struct * create_s_expr_char(char value);
struct s_expr_struct * create_s_expr_string(char * value);
struct s_expr_struct * create_s_expr_bool(int value);
struct s_expr_struct * create_s_expr_id(char * value);

#endif


