/* A Bison parser, made by GNU Bison 2.6.4.  */

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
     DEFUN = 272,
     SLOTVALUE = 273,
     OPEN = 274,
     CLOSE = 275,
     WITHOPENFILE = 276,
     FUNCALL = 277,
     DEFCLASS = 278,
     INITFORM = 279,
     READER = 280,
     WRITER = 281,
     ACCESSOR = 282,
     ALLOCATION = 283,
     ALLOCINSTANCE = 284,
     ALLOCCLASS = 285
   };
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 2077 of yacc.c  */
#line 18 "parser.y"

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


/* Line 2077 of yacc.c  */
#line 102 "parser.hpp"
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
