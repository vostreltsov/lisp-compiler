%{

#include <stdio.h>
#include "parser_structs.h"
#include "parser_funcs.h"

struct program_struct * root;

%}

%union {
    int    semantic_int;
    char   semantic_char;
    char * semantic_string;
    int    semantic_bool;

    struct variable_struct   * semantic_var;
    struct expression_struct * semantic_expr;
    struct expression_list   * semantic_expr_list;
    struct statement_list    * semantic_stmt_list;
    struct program_struct    * semantic_program;
}

%error-verbose

%type<semantic_program> program
%type<semantic_stmt_list> stmt_list

%token LOOP
%token FOR
%token IN
%token DO
%token FROM
%token TO
%token REPEAT
%token WHILE
%token UNTIL
%token PROGN
%token DEFPARAMETER
%token SETF
%token LET
%token VECTOR
%token VECTOR_PUSH
%token VECTOR_POP
%token ELT
%token LIST
%token LIST_LENGTH
%token LENGTH
%token FIND
%token POSITION
%token REMOVE
%token SUBSTITUTE
%token CONCATENATE
%token IF
%token DOTIMES
%token DEFUN
%token SLOT_VALUE
%token OPEN
%token CLOSE
%token WITH_OPEN_FILE
%token FORMAT
%token FUNCALL
%token DEFCLASS
%token PRINT
%token INITFORM
%token READER
%token WRITER
%token ACCESSOR
%token INITARG
%token ALLOCATION
%token TYPE
%token DOCUMENTATION

%left '+' '-' '*' '/' UPLUS UMINUS
%left '<' '>' LESSEQ GRTREQ

%start program

%%

program : stmt_list { $$ = root = create_program($1); }
        ;

expr : '+' expr {}
     ;

stmt_list : expr {}
          ;

%%


