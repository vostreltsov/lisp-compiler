#ifndef PARSER_FUNCS_H
#define PARSER_FUNCS_H

#include <malloc.h>
#include <string.h>
#include "parser_structs.h"

struct program_struct * create_program(int nodeId, struct s_expr_struct * s_expr);
struct s_expr_struct * create_s_expr_empty(int nodeId);
struct s_expr_struct * create_s_expr_int(int nodeId, long int value);
struct s_expr_struct * create_s_expr_char(int nodeId, char value);
struct s_expr_struct * create_s_expr_string(int nodeId, char * value);
struct s_expr_struct * create_s_expr_bool(int nodeId, int value);
struct s_expr_struct * create_s_expr_id(int nodeId, char * value);
struct s_expr_struct * create_s_expr_list(int nodeId, struct list_struct * value);
struct s_expr_seq_struct * create_s_expr_seq(int nodeId, struct s_expr_struct * first);
struct s_expr_seq_struct * add_to_s_expr_seq(struct s_expr_seq_struct * list, struct s_expr_struct * expr);

struct list_struct * create_list(int nodeId, enum list_type type, char * id, struct s_expr_seq_struct * ops,
                                                                             struct s_expr_struct * cond,
                                                                             struct s_expr_struct * container,
                                                                             struct s_expr_struct * from,
                                                                             struct s_expr_struct * to,
                                                                             struct s_expr_struct * body1,
                                                                             struct s_expr_struct * body2);

#endif


