#include "parser_funcs.h"

struct program_struct * create_program(struct s_expr_struct * s_expr, int nodeId) {
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

struct s_expr_struct * create_s_expr_int(long int value, int nodeId) {
    struct s_expr_struct * result = create_s_expr_empty(nodeId);
    result->type = S_EXPR_TYPE_INT;
    result->integer = value;
    return result;
}

struct s_expr_struct * create_s_expr_char(char value, int nodeId) {
    struct s_expr_struct * result = create_s_expr_empty(nodeId);
    result->type = S_EXPR_TYPE_CHAR;
    result->character = value;
    return result;
}

struct s_expr_struct * create_s_expr_string(char * value, int nodeId) {
    struct s_expr_struct * result = create_s_expr_empty(nodeId);
    result->type = S_EXPR_TYPE_STRING;
    result->string = value;
    return result;
}

struct s_expr_struct * create_s_expr_bool(int value, int nodeId) {
    struct s_expr_struct * result = create_s_expr_empty(nodeId);
    result->type = S_EXPR_TYPE_BOOL;
    result->boolean = value;
    return result;
}

struct s_expr_struct * create_s_expr_id(char * value, int nodeId) {
    struct s_expr_struct * result = create_s_expr_empty(nodeId);
    result->type = S_EXPR_TYPE_ID;
    result->id = value;
    return result;
}

struct s_expr_struct * create_s_expr_list(struct list_struct * value, int nodeId) {
    struct s_expr_struct * result = create_s_expr_empty(nodeId);
    result->type = S_EXPR_TYPE_LIST;
    result->list = value;
    return result;
}

struct s_expr_seq_struct * create_s_expr_seq(struct s_expr_struct * first, int nodeId) {
    struct s_expr_seq_struct * result = (struct s_expr_seq_struct *)malloc(sizeof(struct s_expr_seq_struct));
    result->first = first;
    result->last = first;
    result->nodeId = nodeId;
    return result;
}

struct list_struct * create_list_empty(int nodeId) {
    struct list_struct * result = (struct list_struct *)malloc(sizeof(struct list_struct));
    memset(result, 0, sizeof(struct list_struct));
    result->nodeId = nodeId;
    return result;
}

struct list_struct * create_list_id_s_expr_seq(char * id, struct s_expr_seq_struct * s_expr_seq, int nodeId) {
    struct list_struct * result = create_list_empty(nodeId);
    result->id = id;
    result->s_expr_seq = s_expr_seq;
    return result;
}

struct s_expr_seq_struct * add_to_s_expr_seq(struct s_expr_seq_struct * list, struct s_expr_struct * expr) {
    list->last->next = expr;
    list->last = expr;
    return list;
}
