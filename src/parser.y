%{

#include <stdio.h>
#include "parser.tab.h"
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

%token LOOP
%token FOR
%token IN
%token FROM
%token TO
%token PROGN
%token SETF
%token IF

%type   <semantic_program>    program

%start program

%%

program : s_expr                        {}
        ;

s_expr : INT                            {}
       | CHAR                           {}
       | STRING                         {}
       | BOOL                           {}
       | ID                             {}
       | list                           {}
       ;

s_expr_list : s_expr                    {}
            | s_expr_list s_expr        {}
            ;

list : '(' ')'                                            {}
     | '(' ID s_expr_list ')'                             {}
     | '\'' s_expr                                        {}

     | '(' LOOP FOR ID IN list s_expr ')'                 {}
     | '(' LOOP FOR ID FROM s_expr TO s_expr s_expr ')'   {}
     | '(' PROGN s_expr_list ')'                          {}
     | '(' SETF ID s_expr ')'                             {}
     | '(' IF s_expr s_expr ')'                           {}
     | '(' IF s_expr s_expr s_expr ')'                    {}
     ;

%%

void yyerror(const char * msg) {
    printf("error: %s\n", msg);
}
