%{

#include <stdio.h>
#include <malloc.h>
#include "parser_structs.h"
#include "parser_funcs.h"

struct program_struct * root;

%}

%error-verbose
%union {
    struct variable_struct * var;
    struct statement_list * stmt_l;
    struct program_struct * prog;
}


%%


%%
