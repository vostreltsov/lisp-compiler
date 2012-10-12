#ifndef PARSER_FUNCS_H
#define PARSER_FUNCS_H

#include <malloc.h>
#include <string.h>
#include "parser_structs.h"

struct program_struct * create_program(struct s_expr_struct * s_expr, int nodeId);
struct s_expr_struct * create_s_expr_empty(int nodeId);
struct s_expr_struct * create_s_expr_int(long int value, int nodeId);
struct s_expr_struct * create_s_expr_char(char value, int nodeId);
struct s_expr_struct * create_s_expr_string(char * value, int nodeId);
struct s_expr_struct * create_s_expr_bool(int value, int nodeId);
struct s_expr_struct * create_s_expr_id(char * value, int nodeId);
struct s_expr_struct * create_s_expr_list(struct list_struct * value, int nodeId);
struct s_expr_seq_struct * create_s_expr_seq(struct s_expr_struct * first, int nodeId);
struct s_expr_seq_struct * add_to_s_expr_seq(struct s_expr_seq_struct * list, struct s_expr_struct * expr);

struct list_struct * create_list(enum list_type type, char * id, struct s_expr_seq_struct * ops, int nodeId);


#endif


