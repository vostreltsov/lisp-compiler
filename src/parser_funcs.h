#ifndef PARSER_FUNCS_H
#define PARSER_FUNCS_H

#include <malloc.h>
#include <string.h>
#include "parser_structs.h"

struct program_struct * create_program(struct s_expr_struct * s_expr, int id);
struct s_expr_struct * create_s_expr_empty(int id);
struct s_expr_struct * create_s_expr_int(long int value, int id);
struct s_expr_struct * create_s_expr_char(char value, int id);
struct s_expr_struct * create_s_expr_string(char * value, int id);
struct s_expr_struct * create_s_expr_bool(int value, int id);
struct s_expr_struct * create_s_expr_id(char * value, int id);
struct s_expr_struct * create_s_expr_list(struct list_struct * value, int id);

#endif


