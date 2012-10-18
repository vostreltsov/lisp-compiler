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
    result->next = NULL;
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
    result->next = NULL;
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
                                                                             struct slot_def_seq_struct * slotdefs,
                                                                             char * parent) {

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
    result->slotdefs = slotdefs;
    result->parent = parent;
    return result;
}

void free_program(struct program_struct * program) {
    if (program != NULL) {
        free_s_expr_seq(program->s_expr_seq);
        free(program);
    }
}

void free_s_expr(struct s_expr_struct * s_expr) {
    if (s_expr != NULL) {
        if (s_expr->string != NULL) {
            free(s_expr->string);
        }
        if (s_expr->id != NULL) {
            free(s_expr->id);
        }
        free_list(s_expr->list);
        free(s_expr);
    }
}

void free_s_expr_seq(struct s_expr_seq_struct * s_expr_seq) {
    if (s_expr_seq != NULL) {
        struct s_expr_struct * expr = s_expr_seq->first;
        while (expr != NULL) {
            struct s_expr_struct * tmp = expr;
            expr = expr->next;
            free_s_expr(tmp);
        }
        free(s_expr_seq);
    }
}

void free_slot_def(struct slot_def_struct * slot_def) {
    if (slot_def != NULL) {
        free_s_expr(slot_def->initform);
        if (slot_def->reader != NULL) {
            free(slot_def->reader);
        }
        if (slot_def->writer != NULL) {
            free(slot_def->writer);
        }
        if (slot_def->accessor != NULL) {
            free(slot_def->accessor);
        }
        free(slot_def);
    }
}

void free_slot_def_seq(struct slot_def_seq_struct * slot_def_seq) {
    if (slot_def_seq != NULL) {
        struct slot_def_struct * slot_def = slot_def_seq->first;
        while (slot_def != NULL) {
            struct s_expr_struct * tmp = slot_def;
            slot_def = slot_def->next;
            free_slot_def(tmp);
        }
        free(slot_def_seq);
    }
}

void free_list(struct list_struct * list) {
    if (list != NULL) {
        if (list->id != NULL) {
            free(list->id);
        }
        free_s_expr_seq(list->ops);
        free_s_expr(list->cond);
        free_s_expr(list->container);
        free_s_expr(list->from);
        free_s_expr(list->to);
        free_s_expr(list->body1);
        free_s_expr(list->body2);
        free_slot_def_seq(list->slotdefs);
        if (list->parent != NULL) {
            free(list->parent);
        }
        free(list);
    }
}

