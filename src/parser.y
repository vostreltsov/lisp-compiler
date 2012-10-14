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

%token LOOP
%token FOR
%token IN
%token DO
%token FROM
%token TO
%token WHILE
%token PROGN
%token IF

%token DEFUN
%token SLOTVALUE
%token OPEN
%token CLOSE
%token WITHOPENFILE
%token FUNCALL
%token DEFCLASS

%token INITFORM
%token READER
%token WRITER
%token ACCESSOR
%token INITARG
%token ALLOCATION
%token TYPE
%token DOCUMENTATION

%start program

%%

program : s_expr                        {root = $$ = create_program(++idCounter, $1);}
        ;

s_expr : INT                            {$$ = create_s_expr_int(++idCounter, $1);}
       | CHAR                           {$$ = create_s_expr_char(++idCounter, $1);}
       | STRING                         {$$ = create_s_expr_string(++idCounter, $1);}
       | BOOL                           {$$ = create_s_expr_bool(++idCounter, $1);}
       | ID                             {$$ = create_s_expr_id(++idCounter, $1);}
       | list                           {$$ = create_s_expr_list(++idCounter, $1);}
       ;

s_expr_seq : s_expr                     {$$ = create_s_expr_seq(++idCounter, $1);}
           | s_expr_seq s_expr          {$$ = add_to_s_expr_seq($1, $2);}
           ;

list : '(' ')'                                              {$$ = create_list(++idCounter, LIST_TYPE_EMPTY, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);}
     | '(' ID s_expr_seq ')'                                {$$ = create_list(++idCounter, LIST_TYPE_FCALL, $2, $3, NULL, NULL, NULL, NULL, NULL, NULL);}
     | '(' LOOP FOR ID IN s_expr DO s_expr ')'                {$$ = create_list(++idCounter, LIST_TYPE_LOOP_IN, $4, NULL, NULL, $6, NULL, NULL, $8, NULL);}
     | '(' LOOP FOR ID FROM s_expr TO s_expr DO s_expr ')'  {$$ = create_list(++idCounter, LIST_TYPE_LOOP_FROM_TO, $4, NULL, NULL, NULL, $6, $8, $10, NULL);}
     | '(' PROGN s_expr_seq ')'                             {$$ = create_list(++idCounter, LIST_TYPE_PROGN, NULL, $3, NULL, NULL, NULL, NULL, NULL, NULL);}
     | '(' IF s_expr s_expr ')'                             {$$ = create_list(++idCounter, LIST_TYPE_IF, NULL, NULL, $3, NULL, NULL, NULL, $4, NULL);}
     | '(' IF s_expr s_expr s_expr ')'                      {$$ = create_list(++idCounter, LIST_TYPE_IF, NULL, NULL, $3, NULL, NULL, NULL, $4, $5);}
     ;

%%

void yyerror(const char * str) {
    printf("error: %s\n", str);
}
