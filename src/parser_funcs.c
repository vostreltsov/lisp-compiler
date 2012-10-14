#include "parser_funcs.h"

struct program_struct * create_program(int nodeId, struct s_expr_struct * s_expr) {
    struct program_struct * result = (struct program_struct *)malloc(sizeof(struct program_struct));
    result->s_expr = s_expr;
    result->nodeId = nodeId;
    return result;
}

struct s_expr_struct * create_s_expr_empty(int nodeId) {
    struct s_expr_struct * result = (struct s_expr_struct *)malloc(sizeof(struct s_expr_struct));
    memset(result, 0, sizeof(struct s_expr_struct));
    result->nodeId = nodeId;
    return result;
}

struct s_expr_struct * create_s_expr_int(int nodeId, long int value) {
    struct s_expr_struct * result = create_s_expr_empty(nodeId);
    result->type = S_EXPR_TYPE_INT;
    result->integer = value;
    return result;
}

struct s_expr_struct * create_s_expr_char(int nodeId, char value) {
    struct s_expr_struct * result = create_s_expr_empty(nodeId);
    result->type = S_EXPR_TYPE_CHAR;
    result->character = value;
    return result;
}

struct s_expr_struct * create_s_expr_string(int nodeId, char * value) {
    struct s_expr_struct * result = create_s_expr_empty(nodeId);
    result->type = S_EXPR_TYPE_STRING;
    result->string = value;
    return result;
}

struct s_expr_struct * create_s_expr_bool(int nodeId, int value) {
    struct s_expr_struct * result = create_s_expr_empty(nodeId);
    result->type = S_EXPR_TYPE_BOOL;
    result->boolean = value;
    return result;
}

struct s_expr_struct * create_s_expr_id(int nodeId, char * value) {
    struct s_expr_struct * result = create_s_expr_empty(nodeId);
    result->type = S_EXPR_TYPE_ID;
    result->id = value;
    return result;
}

struct s_expr_struct * create_s_expr_list(int nodeId, struct list_struct * value) {
    struct s_expr_struct * result = create_s_expr_empty(nodeId);
    result->type = S_EXPR_TYPE_LIST;
    result->list = value;
    return result;
}

struct s_expr_seq_struct * create_s_expr_seq(int nodeId, struct s_expr_struct * first) {
    struct s_expr_seq_struct * result = (struct s_expr_seq_struct *)malloc(sizeof(struct s_expr_seq_struct));
    result->first = first;
    result->last = first;
    result->nodeId = nodeId;
    return result;
}

struct s_expr_seq_struct * add_to_s_expr_seq(struct s_expr_seq_struct * list, struct s_expr_struct * expr) {
    list->last->next = expr;
    list->last = expr;
    return list;
}

struct list_struct * create_list(int nodeId, enum list_type type, char * id, struct s_expr_seq_struct * ops,
                                                                             struct s_expr_struct * cond,
                                                                             struct s_expr_struct * container,
                                                                             struct s_expr_struct * from,
                                                                             struct s_expr_struct * to,
                                                                             struct s_expr_struct * body1,
                                                                             struct s_expr_struct * body2) {

    struct list_struct * result = (struct list_struct *)malloc(sizeof(struct list_struct));
    memset(result, 0, sizeof(struct list_struct));
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
    return result;
}
