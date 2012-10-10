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

struct list_struct * create_list_empty(int nodeId);
struct list_struct * create_list_id_s_expr_seq(char * id, struct s_expr_seq_struct * s_expr_seq, int nodeId);

struct s_expr_seq_struct * add_to_s_expr_seq(struct s_expr_seq_struct * list, struct s_expr_struct * expr);

#endif


