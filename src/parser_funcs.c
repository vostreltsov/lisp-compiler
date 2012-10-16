#include "parser_funcs.h"

struct program_struct * create_program(int nodeId, struct s_expr_seq_struct * s_expr_seq) {
    struct program_struct * result = (struct program_struct *)malloc(sizeof(struct program_struct));
    result->s_expr_seq = s_expr_seq;
    result->nodeId = nodeId;
    return result;
}

struct s_expr_struct * create_s_expr(int nodeId, enum s_expr_type type, long int integer,
                                                                        char character,
                                                                        char * string,
                                                                        int boolean,
                                                                        char * id,
                                                                        struct list_struct * list) {

    struct s_expr_struct * result = (struct s_expr_struct *)malloc(sizeof(struct s_expr_struct));
    result->nodeId = nodeId;
    result->type = type;
    result->integer = integer;
    result->character = character;
    result->string = string;
    result->boolean = boolean;
    result->id = id;
    result->list = list;
    return result;
}

struct s_expr_seq_struct * create_s_expr_seq(int nodeId, struct s_expr_struct * first) {
    struct s_expr_seq_struct * result = (struct s_expr_seq_struct *)malloc(sizeof(struct s_expr_seq_struct));
    result->nodeId = nodeId;
    result->first = first;
    result->last = first;
    return result;
}

struct s_expr_seq_struct * add_to_s_expr_seq(struct s_expr_seq_struct * seq, struct s_expr_struct * expr) {
    seq->last->next = expr;
    seq->last = expr;
    return seq;
}

struct slot_def_struct * create_slot_def(int nodeId, struct s_expr_struct * initform,
                                                     char * reader,
                                                     char * writer,
                                                     char * accessor,
                                                     enum slot_alloc_type alloc) {
    struct slot_def_struct * result = (struct slot_def_struct *)malloc(sizeof(struct slot_def_struct));
    result->nodeId = nodeId;
    result->initform = initform;
    result->reader = reader;
    result->writer = writer;
    result->accessor = accessor;
    result->alloc = alloc;
    return result;
}

struct slot_def_seq_struct * create_slot_def_seq(int nodeId, struct slot_def_struct * first) {
    struct slot_def_seq_struct * result = (struct slot_def_seq_struct *)malloc(sizeof(struct slot_def_seq_struct));
    result->nodeId = nodeId;
    result->first = first;
    result->last = first;
    return result;
}

struct slot_def_seq_struct * add_to_slot_def_seq(struct slot_def_seq_struct * seq, struct slot_def_struct * def) {
    seq->last->next = def;
    seq->last = def;
    return seq;
}

struct list_struct * create_list(int nodeId, enum list_type type, char * id, struct s_expr_seq_struct * ops,
                                                                             struct s_expr_struct * cond,
                                                                             struct s_expr_struct * container,
                                                                             struct s_expr_struct * from,
                                                                             struct s_expr_struct * to,
                                                                             struct s_expr_struct * body1,
                                                                             struct s_expr_struct * body2,
                                                                             struct slot_def_seq_struct * slots) {

    struct list_struct * result = (struct list_struct *)malloc(sizeof(struct list_struct));
    result->nodeId = nodeId;
    result->type = type;
    result->id = id;
    result->ops = ops;
    result->cond = cond;
    result->container = container;
    result->from = from;
    result->to = to;
    result->body1 = body1;
    result->body2 = body2;
    result->slots = slots;
    return result;
}
