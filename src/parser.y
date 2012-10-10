%{

#include <stdio.h>
#include "parser.tab.h"
#include "parser_structs.h"
#include "parser_funcs.h"

extern int yylex(void);
void yyerror(const char * str);

struct program_struct * root;

int idCounter = 0;

%}

%union {
    int                         semantic_int;
    char                        semantic_char;
    char                      * semantic_string;
    int                         semantic_bool;
    char                      * semantic_id;
    struct program_struct     * semantic_program;
    struct s_expr_struct      * semantic_s_expr;
    struct s_expr_seq_struct  * semantic_s_expr_seq;
    struct list_struct        * semantic_list;
}

%error-verbose

%type   <semantic_program>     program
%type   <semantic_s_expr>      s_expr
%type   <semantic_s_expr_seq>  s_expr_seq
%type   <semantic_list>        list

%token  <semantic_int>          INT
%token  <semantic_char>         CHAR
%token  <semantic_string>       STRING
%token  <semantic_bool>         BOOL
%token  <semantic_id>           ID

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

program : s_expr                        {root = $$ = create_program($1, ++idCounter);}
        ;

s_expr : INT                            {$$ = create_s_expr_int($1, ++idCounter);}
       | CHAR                           {$$ = create_s_expr_char($1, ++idCounter);}
       | STRING                         {$$ = create_s_expr_string($1, ++idCounter);}
       | BOOL                           {$$ = create_s_expr_bool($1, ++idCounter);}
       | ID                             {$$ = create_s_expr_id($1, ++idCounter);}
       | list                           {$$ = create_s_expr_list($1, ++idCounter);}
       ;

s_expr_seq : s_expr                     {$$ = create_s_expr_seq($1, ++idCounter);}
           | s_expr_seq s_expr          {$$ = add_to_s_expr_seq($1, $2);}
           ;

list : '(' ')'                                            {$$ = create_list_empty(++idCounter);}
     | '(' ID s_expr_seq ')'                              {$$ = create_list_id_s_expr_seq($2, $3, ++idCounter);}
     | '\'' s_expr                                        {}

     | '(' LOOP FOR ID IN list s_expr ')'                 {}
     | '(' LOOP FOR ID FROM s_expr TO s_expr s_expr ')'   {}
     | '(' PROGN s_expr_seq ')'                           {}
     | '(' SETF ID s_expr ')'                             {}
     | '(' IF s_expr s_expr ')'                           {}
     | '(' IF s_expr s_expr s_expr ')'                    {}
     ;

%%

void yyerror(const char * str) {
    printf("error: %s\n", str);
}
