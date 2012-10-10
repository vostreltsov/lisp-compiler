#include "parser_funcs.h"

struct program_struct * create_program(struct s_expr_struct * s_expr, int id) {
    struct program_struct * result = (struct program_struct *)malloc(sizeof(struct program_struct));
    result->s_expr = s_expr;
    result->nodeId = id;
    return result;
}

struct s_expr_struct * create_s_expr_empty(int id) {
    struct s_expr_struct * result = (struct s_expr_struct *)malloc(sizeof(struct s_expr_struct));
    memset(result, 0, sizeof(struct s_expr_struct));
    result->nodeId = id;
    return result;
}

struct s_expr_struct * create_s_expr_int(long int value, int id) {
    struct s_expr_struct * result = create_s_expr_empty(id);
    result->type = S_EXPR_TYPE_INT;
    result->integer = value;
    return result;
}

struct s_expr_struct * create_s_expr_char(char value, int id) {
    struct s_expr_struct * result = create_s_expr_empty(id);
    result->type = S_EXPR_TYPE_CHAR;
    result->character = value;
    return result;
}

struct s_expr_struct * create_s_expr_string(char * value, int id) {
    struct s_expr_struct * result = create_s_expr_empty(id);
    result->type = S_EXPR_TYPE_STRING;
    result->string = (char *)malloc(sizeof(char) * strlen(value));
    strcpy(result->string, value);
    free(value);
    return result;
}

struct s_expr_struct * create_s_expr_bool(int value, int id) {
    struct s_expr_struct * result = create_s_expr_empty(id);
    result->type = S_EXPR_TYPE_BOOL;
    result->boolean = value;
    return result;
}

struct s_expr_struct * create_s_expr_id(char * value, int id) {
    struct s_expr_struct * result = create_s_expr_empty(id);
    result->type = S_EXPR_TYPE_ID;
    result->identifier = (char *)malloc(sizeof(char) * strlen(value));
    strcpy(result->identifier, value);
    free(value);
    return result;
}

struct s_expr_struct * create_s_expr_list(struct list_struct * value, int id) {
    struct s_expr_struct * result = create_s_expr_empty(id);
    result->type = S_EXPR_TYPE_LIST;
    result->list = value;
    return result;
}
