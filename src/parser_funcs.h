#ifndef PARSER_FUNCS_H
#define PARSER_FUNCS_H

#include <malloc.h>
#include <string.h>
#include "parser_structs.h"

struct program_struct * create_program(int nodeId, struct s_expr_seq_struct * s_expr_seq);

struct s_expr_struct * create_s_expr(int nodeId, enum s_expr_type type, int integer,
                                                                        char character,
                                                                        char * string,
                                                                        int boolean,
                                                                        char * id,
                                                                        struct list_struct * list);

struct s_expr_seq_struct * create_s_expr_seq(int nodeId, struct s_expr_struct * first);

struct s_expr_seq_struct * add_to_s_expr_seq(struct s_expr_seq_struct * seq, struct s_expr_struct * expr);

struct slot_def_struct * create_slot_def(int nodeId, enum slot_def_type type,
                                                     struct s_expr_struct * initform,
                                                     char * id,
                                                     enum slot_alloc_type alloc);

struct slot_def_seq_struct * create_slot_def_seq(int nodeId, struct slot_def_struct * first);

struct slot_def_seq_struct * add_to_slot_def_seq(struct slot_def_seq_struct * seq, struct slot_def_struct * def);

struct list_struct * create_list(int nodeId, enum list_type type, char * id, struct s_expr_seq_struct * ops,
                                                                             struct s_expr_struct * cond,
                                                                             struct s_expr_struct * container,
                                                                             struct s_expr_struct * from,
                                                                             struct s_expr_struct * to,
                                                                             struct s_expr_seq_struct * body,
                                                                             struct s_expr_struct * body1,
                                                                             struct s_expr_struct * body2,
                                                                             struct slot_def_seq_struct * slotdefs,
                                                                             char * parent);

void free_program(struct program_struct * program);
void free_s_expr(struct s_expr_struct * s_expr);
void free_s_expr_seq(struct s_expr_seq_struct * s_expr_seq);
void free_slot_def(struct slot_def_struct * slot_def);
void free_slot_def_seq(struct slot_def_seq_struct * slot_def_seq);
void free_list(struct list_struct * list);

#endif


