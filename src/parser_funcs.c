#include "parser_funcs.h"

struct program_struct * create_program(struct s_expr_struct * s_expr)
{
    struct program_struct * result = (struct program_struct *)malloc(sizeof(struct program_struct));
    result->s_expr = s_expr;
    return result;
}

struct s_expr_struct * create_s_expr_empty()
{
    struct s_expr_struct * result = (struct s_expr_struct *)malloc(sizeof(struct s_expr_struct));
    memset(result, 0, sizeof(struct s_expr_struct));
    return result;
}

struct s_expr_struct * create_s_expr_int(long int value)
{
    struct s_expr_struct * result = create_s_expr_empty();
    //result->type = ATOM_TYPE_INT;
    result->const_int = value;
    return result;
}

struct s_expr_struct * create_s_expr_char(char value)
{
    struct s_expr_struct * result = create_s_expr_empty();
    //result->type = ATOM_TYPE_CHAR;
    result->const_char = value;
    return result;
}

struct s_expr_struct * create_s_expr_string(char * value)
{
    struct s_expr_struct * result = create_s_expr_empty();
    //result->type = ATOM_TYPE_STRING;
    result->const_string = (char *)malloc(sizeof(char) * strlen(value));
    strcpy(result->const_string, value);
    free(value);
    return result;
}

struct s_expr_struct * create_s_expr_bool(int value)
{
    struct s_expr_struct * result = create_s_expr_empty();
    //result->type = ATOM_TYPE_BOOL;
    result->const_bool = value;
    return result;
}

struct s_expr_struct * create_s_expr_id(char * value)
{
    struct s_expr_struct * result = create_s_expr_empty();
    //result->type = ATOM_TYPE_STRING;
    result->const_id = (char *)malloc(sizeof(char) * strlen(value));
    strcpy(result->const_id, value);
    free(value);
    return result;
}
