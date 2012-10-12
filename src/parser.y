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
%token DEFPARAMETER
%token SETF
%token LET
%token VECTOR
%token VECTORPUSH
%token VECTORPOP
%token ELT
%token LIST
%token LISTLENGTH
%token LENGTH
%token FIND
%token POSITION
%token REMOVE
%token SUBSTITUTE
%token CONCATENATE
%token IF
%token DOTIMES
%token DEFUN
%token SLOTVALUE
%token OPEN
%token CLOSE
%token WITHOPENFILE
%token FORMAT
%token FUNCALL
%token DEFCLASS

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

list : '(' ')'                                            {$$ = create_list(LIST_TYPE_EMPTY, NULL, NULL, ++idCounter);}
     | '(' ID s_expr_seq ')'                              {$$ = create_list(LIST_TYPE_FCALL, $2, $3, ++idCounter);}

     | '\'' s_expr                                        {/* TODO: ~ (list s_expr) */}

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
