#include "parser_funcs.h"

struct program_struct * create_program(struct statement_list * stmts)
{
    struct program_struct * result = (struct program_struct *)malloc(sizeof(struct program_struct));
    result->stmts = stmts;
    return result;
}

struct expression_struct * create_expr_empty()
{
    struct expression_struct * result = (struct expression_struct *)malloc(sizeof(struct expression_struct));
    memset(result, 0, sizeof(struct expression_struct));
    return result;
}

struct expression_struct * create_expr_int(long int value)
{
    struct expression_struct * result = create_expr_empty();
    result->type = EXPR_TYPE_INT;
    result->const_int = value;
    return result;
}

struct expression_struct * create_expr_char(char value)
{
    struct expression_struct * result = create_expr_empty();
    result->type = EXPR_TYPE_CHAR;
    result->const_char = value;
    return result;
}

struct expression_struct * create_expr_string(char * value)
{
    struct expression_struct * result = create_expr_empty();
    result->type = EXPR_TYPE_STRING;
    result->const_string = (char *)malloc(sizeof(char) * strlen(value));
    strcpy(result->const_string, value);
    free(value);
    return result;
}

struct expression_struct * create_expr_bool(int value)
{
    struct expression_struct * result = create_expr_empty();
    result->type = EXPR_TYPE_BOOL;
    result->const_bool = value;
    return result;
}

struct expression_struct * create_expr_binary(enum expr_type type, struct expression_struct * left, struct expression_struct * right)
{
    struct expression_struct * result = create_expr_empty();
    result->type = type;
    result->left = left;
    result->right = right;
    return result;
}
