#include "parser_funcs.h"

// Creates a program node.
struct program_struct * create_program(struct statement_list * stmts)
{
    struct program_struct * result = (struct program_struct *)malloc(sizeof(struct program_struct));
    result->stmts = stmts;
    return result;
}

struct expression_struct * create_expr_binary(enum expr_type type, struct expression_struct * left, struct expression_struct * right)
{
    struct expression_struct * result = (struct expression_struct *)malloc(sizeof(struct expression_struct));
    memset(result, 0, sizeof(struct expression_struct));
    result->type = type;
    result->left = left;
    result->right = right;
    return result;
}
