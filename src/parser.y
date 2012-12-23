%{
#include <stdio.h>
#include "parser.hpp"
#include "parser_structs.h"
#include "parser_funcs.h"
#include "errors.h"

extern int yylex(void);
extern enum error_types errorCode;
extern char errorMessage[256];

void yyerror(const char * str);

struct program_struct * root = NULL;
int idCounter = 0;

%}

%union {
    int                              semantic_int;
    float                            semantic_float;
    char                             semantic_char;
    char                           * semantic_string;
    int                              semantic_bool;
    char                           * semantic_id;
    struct program_struct          * semantic_program;
    struct program_part_struct     * semantic_program_part;
    struct program_part_seq_struct * semantic_program_part_seq;
    struct s_expr_struct           * semantic_s_expr;
    struct s_expr_seq_struct       * semantic_s_expr_seq;
    struct slot_prop_struct        * semantic_slot_prop;
    struct slot_prop_seq_struct    * semantic_slot_prop_seq;
    struct slot_def_struct         * semantic_slot_def;
    struct slot_def_seq_struct     * semantic_slot_def_seq;
    struct def_struct              * semantic_def;
}

%output "parser.cpp"
%error-verbose

%type   <semantic_program>           program
%type   <semantic_program_part>      program_part
%type   <semantic_program_part_seq>  program_part_seq
%type   <semantic_s_expr>            s_expr
%type   <semantic_s_expr_seq>        s_expr_seq
%type   <semantic_slot_prop>         slot_prop
%type   <semantic_slot_prop_seq>     slot_prop_seq
%type   <semantic_slot_def>          slot_def
%type   <semantic_slot_def_seq>      slot_def_seq
%type   <semantic_def>               def

%token  <semantic_int>               INT
%token  <semantic_float>             FLOAT
%token  <semantic_char>              CHAR
%token  <semantic_string>            STRING
%token  <semantic_bool>              BOOL
%token  <semantic_id>                ID

%token LOOP
%token FOR
%token IN
%token DO
%token FROM
%token TO
%token DOWNTO
%token WHILE
%token PROGN
%token IF
%token DEFCLASS
%token DEFUN
%token INITFORM
%token READER
%token WRITER
%token ACCESSOR
%token ALLOCATION
%token ALLOCINSTANCE
%token ALLOCCLASS
%token MAKEINSTANCE
%token SLOTVALUE
%token ERROR_UNCLOSED_COMMENT
%token ERROR_UNCLOSED_STRING
%token ERROR_UNEXPECTED_CHARACTER

%start program

%%

program : program_part_seq                        {root = $$ = create_program(++idCounter, $1);}
        | /* empty */                             {root = $$ = NULL;}
        ;

lexerror : ERROR_UNCLOSED_COMMENT                 {root = NULL; errorCode = ERROR_LEXICAL_UNCLOSED_COMMENT;     strcpy(errorMessage, "Unclosed comment");         YYABORT;}
         | ERROR_UNCLOSED_STRING                  {root = NULL; errorCode = ERROR_LEXICAL_UNCLOSED_STRING;      strcpy(errorMessage, "Unclosed string constant"); YYABORT;}
         | ERROR_UNEXPECTED_CHARACTER             {root = NULL; errorCode = ERROR_LEXICAL_UNEXPECTED_CHARACTER; strcpy(errorMessage, "Unexpected character");     YYABORT;}
         ;

program_part : s_expr                             {$$ = create_program_part(++idCounter, PROGRAM_PART_TYPE_S_EXPR, $1,   NULL);}
             | def                                {$$ = create_program_part(++idCounter, PROGRAM_PART_TYPE_DEF,    NULL, $1);}
             ;

program_part_seq : program_part                   {$$ = create_program_part_seq(++idCounter, $1);}
                 | program_part_seq program_part  {$$ = add_to_program_part_seq($1, $2);}
                 ;

s_expr : INT                                                         {$$ = create_s_expr_simple(++idCounter, S_EXPR_TYPE_INT,     $1, 0.0,  '\0',  NULL,  0,  NULL);}
       | FLOAT                                                       {$$ = create_s_expr_simple(++idCounter, S_EXPR_TYPE_FLOAT,   0,  $1,   '\0',  NULL,  0,  NULL);}
       | CHAR                                                        {$$ = create_s_expr_simple(++idCounter, S_EXPR_TYPE_CHAR,    0,  0.0,  $1,    NULL,  0,  NULL);}
       | STRING                                                      {$$ = create_s_expr_simple(++idCounter, S_EXPR_TYPE_STRING,  0,  0.0,  '\0',  $1,    0,  NULL);}
       | BOOL                                                        {$$ = create_s_expr_simple(++idCounter, S_EXPR_TYPE_BOOL,    0,  0.0,  '\0',  NULL,  $1, NULL);}
       | ID                                                          {$$ = create_s_expr_simple(++idCounter, S_EXPR_TYPE_ID,      0,  0.0,  '\0',  NULL,  0,  $1);}
       | '(' ')'                                                     {$$ = create_s_expr_simple(++idCounter, S_EXPR_TYPE_EMPTY,   0,  0.0,  '\0',  NULL,  0,  NULL);}

       | '(' ID ')'                                                  {$$ = create_s_expr_funcall(++idCounter, S_EXPR_TYPE_FCALL, $2, NULL);}
       | '(' ID s_expr_seq ')'                                       {$$ = create_s_expr_funcall(++idCounter, S_EXPR_TYPE_FCALL, $2, $3);}

       | '(' LOOP FOR ID IN s_expr DO s_expr_seq ')'                 {$$ = create_s_expr_loop(++idCounter, S_EXPR_TYPE_LOOP_IN,          $4,   $8,  NULL, $6,   NULL, NULL);}
       | '(' LOOP FOR ID FROM s_expr TO s_expr DO s_expr_seq ')'     {$$ = create_s_expr_loop(++idCounter, S_EXPR_TYPE_LOOP_FROM_TO,     $4,   $10, NULL, NULL, $6,   $8);}
       | '(' LOOP FOR ID FROM s_expr DOWNTO s_expr DO s_expr_seq ')' {$$ = create_s_expr_loop(++idCounter, S_EXPR_TYPE_LOOP_FROM_DOWNTO, $4,   $10, NULL, NULL, $6,   $8);}
       | '(' LOOP WHILE s_expr DO s_expr_seq ')'                     {$$ = create_s_expr_loop(++idCounter, S_EXPR_TYPE_LOOP_WHILE,       NULL, $6,  $4,   NULL, NULL, NULL);}

       | '(' PROGN s_expr_seq ')'                                    {$$ = create_s_expr_progn(++idCounter, S_EXPR_TYPE_PROGN, $3);}

       | '(' IF s_expr s_expr ')'                                    {$$ = create_s_expr_if(++idCounter, S_EXPR_TYPE_IF, $3, $4, NULL);}
       | '(' IF s_expr s_expr s_expr ')'                             {$$ = create_s_expr_if(++idCounter, S_EXPR_TYPE_IF, $3, $4, $5);}

       | '(' MAKEINSTANCE '\'' ID ')'                                {$$ = create_s_expr_makeinstance(++idCounter, S_EXPR_TYPE_MAKEINSTANCE, $4);}
       | '(' SLOTVALUE ID '\'' ID ')'                                {$$ = create_s_expr_slotvalue(++idCounter, S_EXPR_TYPE_SLOTVALUE, $3, $5);}
       | lexerror                                                    {}
       ;

s_expr_seq : s_expr                        {$$ = create_s_expr_seq(++idCounter, $1);}
           | s_expr_seq s_expr             {$$ = add_to_s_expr_seq($1, $2);}
           ;

slot_prop : INITFORM s_expr                {$$ = create_slot_prop(++idCounter,  SLOT_PROP_TYPE_INITFORM,   $2,    NULL,  SLOT_ALLOC_TYPE_INSTANCE);}
          | READER ID                      {$$ = create_slot_prop(++idCounter,  SLOT_PROP_TYPE_READER,     NULL,  $2,    SLOT_ALLOC_TYPE_INSTANCE);}
          | WRITER ID                      {$$ = create_slot_prop(++idCounter,  SLOT_PROP_TYPE_WRITER,     NULL,  $2,    SLOT_ALLOC_TYPE_INSTANCE);}
          | ACCESSOR ID                    {$$ = create_slot_prop(++idCounter,  SLOT_PROP_TYPE_ACCESSOR,   NULL,  $2,    SLOT_ALLOC_TYPE_INSTANCE);}
          | ALLOCATION ALLOCINSTANCE       {$$ = create_slot_prop(++idCounter,  SLOT_PROP_TYPE_ALLOCATION, NULL,  NULL,  SLOT_ALLOC_TYPE_INSTANCE);}
          | ALLOCATION ALLOCCLASS          {$$ = create_slot_prop(++idCounter,  SLOT_PROP_TYPE_ALLOCATION, NULL,  NULL,  SLOT_ALLOC_TYPE_CLASS);}
          | lexerror                       {}
          ;

slot_prop_seq : slot_prop                  {$$ = create_slot_prop_seq(++idCounter, $1);}
              | slot_prop_seq slot_prop    {$$ = add_to_slot_prop_seq($1, $2);}
              ;

slot_def : '(' ID slot_prop_seq ')'        {$$ = create_slot_def(++idCounter, $2, $3);}
         | lexerror                        {}
         ;

slot_def_seq : slot_def                    {$$ = create_slot_def_seq(++idCounter, $1);}
             | slot_def_seq slot_def       {$$ = add_to_slot_def_seq($1, $2);}
             ;

def : '(' DEFUN ID '(' ')' s_expr_seq ')'                   {$$ = create_def(++idCounter, DEF_TYPE_FUNC,  $3, NULL, NULL, NULL, $6);}
    | '(' DEFUN ID '(' s_expr_seq ')' s_expr_seq ')'        {$$ = create_def(++idCounter, DEF_TYPE_FUNC,  $3, NULL, $5,   NULL, $7);}
    | '(' DEFCLASS ID '(' ')' '(' slot_def_seq ')' ')'      {$$ = create_def(++idCounter, DEF_TYPE_CLASS, $3, NULL, NULL, $7, NULL);}
    | '(' DEFCLASS ID '(' ID ')' '(' slot_def_seq ')' ')'   {$$ = create_def(++idCounter, DEF_TYPE_CLASS, $3, $5,   NULL, $8, NULL);}
    ;

%%

void yyerror(const char * str) {
    errorCode = ERROR_SYNTAX;
    strcpy(errorMessage, str);
    //printf("error: %s\n", str);
}
