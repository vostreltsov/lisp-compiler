#include "parser_funcs.h"

struct program_struct * create_program(struct s_expr_struct * s_expr)
{
    struct program_struct * result = (struct program_struct *)malloc(sizeof(struct program_struct));
    result->s_expr = s_expr;
    return result;
}

struct atom_struct * create_atom_empty()
{
    struct atom_struct * result = (struct atom_struct *)malloc(sizeof(struct atom_struct));
    memset(result, 0, sizeof(struct atom_struct));
    return result;
}

struct atom_struct * create_atom_int(long int value)
{
    struct atom_struct * result = create_atom_empty();
    result->type = ATOM_TYPE_INT;
    result->const_int = value;
    return result;
}

struct atom_struct * create_atom_char(char value)
{
    struct atom_struct * result = create_atom_empty();
    result->type = ATOM_TYPE_CHAR;
    result->const_char = value;
    return result;
}

struct atom_struct * create_atom_string(char * value)
{
    struct atom_struct * result = create_atom_empty();
    result->type = ATOM_TYPE_STRING;
    result->const_string = (char *)malloc(sizeof(char) * strlen(value));
    strcpy(result->const_string, value);
    free(value);
    return result;
}

struct atom_struct * create_atom_bool(int value)
{
    struct atom_struct * result = create_atom_empty();
    result->type = ATOM_TYPE_BOOL;
    result->const_bool = value;
    return result;
}
