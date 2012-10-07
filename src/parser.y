%{

#include <stdio.h>
#include "parser_structs.h"
#include "parser_funcs.h"

struct program_struct * root;

%}

%union {
    int                     semantic_int;
    char                    semantic_char;
    char                  * semantic_string;
    int                     semantic_bool;
    char                  * semantic_id;
    struct program_struct * semantic_program;
}

%error-verbose

%token  <semantic_int>        INT
%token  <semantic_char>       CHAR
%token  <semantic_string>     STRING
%token  <semantic_bool>       BOOL
%token  <semantic_bool>       ID

%type   <semantic_program>    program

%start program

%%

program : /* empty */              {}
        | s_expr                   {}
        ;

atom : INT                         {}
     | CHAR                        {}
     | STRING                      {}
     | BOOL                        {}
     | ID                          {}
     ;

s_expr : atom                      {}
       | list                      {}
       ;

s_expr_list : s_expr               {}
            | s_expr_list s_expr   {}
            ;

list : '(' ')'                     {}
     | '\'' s_expr                 {}
     | '(' s_expr_list ')'         {}
     ;

%%
