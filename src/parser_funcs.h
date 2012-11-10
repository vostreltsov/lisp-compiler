#ifndef PARSER_FUNCS_H
#define PARSER_FUNCS_H

#include <malloc.h>
#include <string.h>
#include "parser_structs.h"

struct program_struct * create_program_empty();
struct program_part_struct * create_program_part_empty();
struct program_part_seq_struct * create_program_part_seq_empty();
struct s_expr_struct * create_s_expr_empty();
struct s_expr_seq_struct * create_s_expr_seq_empty();
struct slot_prop_struct * create_slot_prop_empty();
struct slot_prop_seq_struct * create_slot_prop_seq_empty();
struct slot_def_struct * create_slot_def_empty();
struct slot_def_seq_struct * create_slot_def_seq_empty();
struct def_struct * create_def_empty();

struct program_struct * create_program(int nodeId,
	                                   struct program_part_seq_struct * parts);

struct program_part_struct * create_program_part(int nodeId,
	                                             enum program_part_type type,
	                                             struct s_expr_struct * s_expr,
	                                             struct def_struct * def);

struct program_part_seq_struct * create_program_part_seq(int nodeId,
	                                                     struct program_part_struct * first);

struct program_part_seq_struct * add_to_program_part_seq(struct program_part_seq_struct * seq,
	                                                     struct program_part_struct * part);

struct s_expr_struct * create_s_expr_simple(int nodeId,
	                                        enum s_expr_type type,
	                                        int integer,
	                                        char character,
	                                        char * string,
	                                        int boolean,
	                                        char * id);

struct s_expr_struct * create_s_expr_funcall(int nodeId,
	                                         enum s_expr_type type,
	                                         char * id,
	                                         struct s_expr_seq_struct * args);

struct s_expr_struct * create_s_expr_loop(int nodeId,
	                                      enum s_expr_type type,
	                                      char * id,
	                                      struct s_expr_struct * container,
	                                      struct s_expr_struct * from,
	                                      struct s_expr_struct * to,
	                                      struct s_expr_struct * body);

struct s_expr_struct * create_s_expr_progn(int nodeId,
	                                       enum s_expr_type type,
                                           struct s_expr_seq_struct * args);

struct s_expr_struct * create_s_expr_if(int nodeId,
	                                    enum s_expr_type type,
                                        struct s_expr_struct * cond,
                                        struct s_expr_struct * body1,
                                        struct s_expr_struct * body2);

struct s_expr_seq_struct * create_s_expr_seq(int nodeId,
	                                         struct s_expr_struct * first);

struct s_expr_seq_struct * add_to_s_expr_seq(struct s_expr_seq_struct * seq,
	                                         struct s_expr_struct * expr);

struct slot_prop_struct * create_slot_prop(int nodeId,
	                                       enum slot_prop_type type,
	                                       struct s_expr_struct * initform,
	                                       char * id,
	                                       enum slot_alloc_type alloc);

struct slot_prop_seq_struct * create_slot_prop_seq(int nodeId,
	                                               struct slot_prop_struct * first);

struct slot_prop_seq_struct * add_to_slot_prop_seq(struct slot_prop_seq_struct * seq,
	                                               struct slot_prop_struct * prop);

struct slot_def_struct * create_slot_def(int nodeId,
	                                     char * id,
	                                     struct slot_prop_seq_struct * props);

struct slot_def_seq_struct * create_slot_def_seq(int nodeId,
	                                             struct slot_def_struct * first);

struct slot_def_seq_struct * add_to_slot_def_seq(struct slot_def_seq_struct * seq,
	                                             struct slot_def_struct * def);

struct def_struct * create_def(int nodeId,
	                           enum def_type type,
	                           char * id,
	                           char * parent,
	                           struct s_expr_seq_struct * args,
	                           struct slot_def_seq_struct * slotdefs,
	                           struct s_expr_seq_struct * body);

void free_char(char * ch);

void free_program(struct program_struct * program);

void free_program_part(struct program_part_struct * program_part);

void free_program_part_seq(struct program_part_seq_struct * program_part_seq);

void free_s_expr(struct s_expr_struct * s_expr);

void free_s_expr_seq(struct s_expr_seq_struct * s_expr_seq);

void free_slot_prop(struct slot_prop_struct * slot_prop);

void free_slot_prop_seq(struct slot_prop_seq_struct * slot_prop_seq);

void free_slot_def(struct slot_def_struct * slot_def);

void free_slot_def_seq(struct slot_def_seq_struct * slot_def_seq);

void free_def(struct def_struct * def);

#endif
