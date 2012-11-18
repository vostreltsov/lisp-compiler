/* A Bison parser, made by GNU Bison 2.6.5.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2012 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_PARSER_HPP_INCLUDED
# define YY_YY_PARSER_HPP_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     INT = 258,
     CHAR = 259,
     STRING = 260,
     BOOL = 261,
     ID = 262,
     LOOP = 263,
     FOR = 264,
     IN = 265,
     DO = 266,
     FROM = 267,
     TO = 268,
     WHILE = 269,
     PROGN = 270,
     IF = 271,
     DEFCLASS = 272,
     DEFUN = 273,
     FUNCALL = 274,
     INITFORM = 275,
     READER = 276,
     WRITER = 277,
     ACCESSOR = 278,
     ALLOCATION = 279,
     ALLOCINSTANCE = 280,
     ALLOCCLASS = 281,
     MAKEINSTANCE = 282,
     SLOTVALUE = 283,
     ERROR_UNCLOSED_COMMENT = 284,
     ERROR_UNCLOSED_STRING = 285,
     ERROR_UNEXPECTED_CHARACTER = 286
   };
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 2042 of yacc.c  */
#line 19 "parser.y"

    int                              semantic_int;
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


/* Line 2042 of yacc.c  */
#line 107 "parser.hpp"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_YY_PARSER_HPP_INCLUDED  */
