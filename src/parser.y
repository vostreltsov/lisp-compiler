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
    struct expression_node   * semantic_expr_node;
    struct statement_node    * semantic_stmt_node;
    struct program_struct    * semantic_program;
}

%error-verbose


%type   <semantic_var>        var
%type   <semantic_expr>       expr
%type   <semantic_expr_node>  expr_list
%type   <semantic_stmt_node>  stmt_list
%type   <semantic_program>    program

%token  <semantic_int>        int_const
%token  <semantic_char>       char_const
%token  <semantic_string>     string_const
%token  <semantic_bool>       bool_const

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

%nonassoc '+' '-' '*' '/'
%nonassoc '<' '>' LESSEQ GRTREQ
%nonassoc AND OR
%nonassoc UPLUS UMINUS NOT
%nonassoc SETF

%start program

%%

program : stmt_list {$$ = root = create_program($1);}
        ;

expr : int_const                {$$ = create_expr_int($1);}
     | char_const               {$$ = create_expr_char($1);}
     | string_const             {$$ = create_expr_string($1);}
     | bool_const               {$$ = create_expr_bool($1);}
     | '+' expr                 {}
     ;

expr_list : expr                {}
          | expr_list expr      {}
          ;

stmt_list : stmt                {}
          | stmt_list stmt      {}
          ;

%%


