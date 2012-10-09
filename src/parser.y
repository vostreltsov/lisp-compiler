%{

#include <stdio.h>
#include "parser.tab.h"
#include "parser_structs.h"
#include "parser_funcs.h"

extern int yylex(void);
void yyerror(const char * str);

struct program_struct * root;

%}

%union {
    int                     semantic_int;
    char                    semantic_char;
    char                  * semantic_string;
    int                     semantic_bool;
    char                  * semantic_id;
    struct program_struct * semantic_program;
    struct s_expr_struct  * semantic_s_expr;
}

%error-verbose

%type   <semantic_program>    program
%type   <semantic_s_expr>     s_expr

%token  <semantic_int>        INT
%token  <semantic_char>       CHAR
%token  <semantic_string>     STRING
%token  <semantic_bool>       BOOL
%token  <semantic_bool>       ID

%token GRTR_EQ
%token LESS_EQ
%token AND
%token OR
%token NOT

%token LOOP
%token FOR
%token IN
%token FROM
%token TO
%token PROGN
%token SETF
%token IF

%start program

%%

program : s_expr                        {root = $$ = create_program($1);}
        ;

s_expr : INT                            {$$ = create_s_expr_int($1);}
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

void yyerror(const char * str) {
    printf("error: %s\n", str);
}
