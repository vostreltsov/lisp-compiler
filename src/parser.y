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
    int                          semantic_int;
    char                         semantic_char;
    char                       * semantic_string;
    int                          semantic_bool;
    char                       * semantic_id;
    struct program_struct      * semantic_program;
    struct s_expr_struct       * semantic_s_expr;
    struct s_expr_seq_struct   * semantic_s_expr_seq;
    struct slot_def_struct     * semantic_slot_def;
    struct slot_def_seq_struct * semantic_slot_def_seq;
    struct list_struct         * semantic_list;
}

%error-verbose

%type   <semantic_program>      program
%type   <semantic_s_expr>       s_expr
%type   <semantic_s_expr_seq>   s_expr_seq
%type   <semantic_slot_def>     slot_def
%type   <semantic_slot_def_seq> slot_def_seq
%type   <semantic_list>         list

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
%token ALLOCATION
%token ALLOCINSTANCE
%token ALLOCCLASS

%start program

%%

program : s_expr_seq                    {root = $$ = create_program(++idCounter, $1);}
        ;

s_expr : INT                            {$$ = create_s_expr(++idCounter,  S_EXPR_TYPE_INT,     $1,  (char)0,  NULL,  0,   NULL,  NULL);}
       | CHAR                           {$$ = create_s_expr(++idCounter,  S_EXPR_TYPE_CHAR,    0,   $1,       NULL,  0,   NULL,  NULL);}
       | STRING                         {$$ = create_s_expr(++idCounter,  S_EXPR_TYPE_STRING,  0,   (char)0,  $1,    0,   NULL,  NULL);}
       | BOOL                           {$$ = create_s_expr(++idCounter,  S_EXPR_TYPE_BOOL,    0,   (char)0,  NULL,  $1,  NULL,  NULL);}
       | ID                             {$$ = create_s_expr(++idCounter,  S_EXPR_TYPE_ID,      0,   (char)0,  NULL,  0,   $1,    NULL);}
       | list                           {$$ = create_s_expr(++idCounter,  S_EXPR_TYPE_LIST,    0,   (char)0,  NULL,  0,   NULL,  $1);}
       ;

s_expr_seq : s_expr                     {$$ = create_s_expr_seq(++idCounter, $1);}
           | s_expr_seq s_expr          {$$ = add_to_s_expr_seq($1, $2);}
           ;

slot_def : INITFORM s_expr              {$$ = create_slot_def(++idCounter,  $2,    NULL,  NULL,  NULL,  SLOT_ALLOC_TYPE_RESERVED);}
         | READER ID                    {$$ = create_slot_def(++idCounter,  NULL,  $2,    NULL,  NULL,  SLOT_ALLOC_TYPE_RESERVED);}
         | WRITER ID                    {$$ = create_slot_def(++idCounter,  NULL,  NULL,  $2,    NULL,  SLOT_ALLOC_TYPE_RESERVED);}
         | ACCESSOR ID                  {$$ = create_slot_def(++idCounter,  NULL,  NULL,  NULL,  $2,    SLOT_ALLOC_TYPE_RESERVED);}
         | ALLOCATION ALLOCINSTANCE     {$$ = create_slot_def(++idCounter,  NULL,  NULL,  NULL,  NULL,  SLOT_ALLOC_TYPE_INSTANCE);}
         | ALLOCATION ALLOCCLASS        {$$ = create_slot_def(++idCounter,  NULL,  NULL,  NULL,  NULL,  SLOT_ALLOC_TYPE_CLASS);}
         ;

slot_def_seq : slot_def                 {$$ = create_slot_def_seq(++idCounter, $1);}
             | slot_def_seq slot_def    {$$ = add_to_slot_def_seq($1, $2);}
             ;

list : '(' ')'                                              {$$ = create_list(++idCounter, LIST_TYPE_EMPTY,         NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL);}
     | '(' ID s_expr_seq ')'                                {$$ = create_list(++idCounter, LIST_TYPE_FCALL,         $2,    $3,    NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL);}
     | '(' LOOP FOR ID IN s_expr DO s_expr ')'              {$$ = create_list(++idCounter, LIST_TYPE_LOOP_IN,       $4,    NULL,  NULL,  $6,    NULL,  NULL,  $8,    NULL,  NULL,  NULL);}
     | '(' LOOP FOR ID FROM s_expr TO s_expr DO s_expr ')'  {$$ = create_list(++idCounter, LIST_TYPE_LOOP_FROM_TO,  $4,    NULL,  NULL,  NULL,  $6,    $8,    $10,   NULL,  NULL,  NULL);}
     | '(' PROGN s_expr_seq ')'                             {$$ = create_list(++idCounter, LIST_TYPE_PROGN,         NULL,  $3,    NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL);}
     | '(' IF s_expr s_expr ')'                             {$$ = create_list(++idCounter, LIST_TYPE_IF,            NULL,  NULL,  $3,    NULL,  NULL,  NULL,  $4,    NULL,  NULL,  NULL);}
     | '(' IF s_expr s_expr s_expr ')'                      {$$ = create_list(++idCounter, LIST_TYPE_IF,            NULL,  NULL,  $3,    NULL,  NULL,  NULL,  $4,    $5,    NULL,  NULL);}
     | '(' ID slot_def_seq ')'                              {$$ = create_list(++idCounter, LIST_TYPE_SLOTDEF,       $2,    NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  $3,    NULL);}
     | '(' DEFUN ID '(' ')' s_expr ')'                      {$$ = create_list(++idCounter, LIST_TYPE_DEFUN,         $3,    NULL,  NULL,  NULL,  NULL,  NULL,  $6,    NULL,  NULL,  NULL);}
     | '(' DEFUN ID '(' s_expr_seq ')' s_expr ')'           {$$ = create_list(++idCounter, LIST_TYPE_DEFUN,         $3,    $5,    NULL,  NULL,  NULL,  NULL,  $7,    NULL,  NULL,  NULL);}
     | '(' DEFCLASS ID '(' ')' '(' s_expr_seq ')' ')'       {$$ = create_list(++idCounter, LIST_TYPE_DEFCLASS,      $3,    $7,    NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL);}
     | '(' DEFCLASS ID '(' ID ')' '(' s_expr_seq ')' ')'    {$$ = create_list(++idCounter, LIST_TYPE_DEFCLASS,      $3,    $8,    NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  $5);}
     ;

%%

void yyerror(const char * str) {
    printf("error: %s\n", str);
}
