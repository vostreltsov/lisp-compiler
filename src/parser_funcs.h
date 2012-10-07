#ifndef PARSER_FUNCS_H
#define PARSER_FUNCS_H

#include <malloc.h>
#include <string.h>
#include "parser_structs.h"

struct program_struct * create_program(struct s_expr_struct * s_expr);
struct atom_struct * create_atom_empty();
struct atom_struct * create_atom_int(long int value);
struct atom_struct * create_atom_char(char value);
struct atom_struct * create_atom_string(char * value);
struct atom_struct * create_atom_bool(int value);

#endif


