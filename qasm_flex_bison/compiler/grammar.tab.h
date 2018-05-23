/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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

#ifndef YY_YY_GRAMMAR_TAB_H_INCLUDED
# define YY_YY_GRAMMAR_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    NAME = 258,
    INTEGER = 259,
    FLOAT = 260,
    COMMA_SEPARATOR = 261,
    PARALLEL_SEPARATOR = 262,
    BRA = 263,
    KET = 264,
    DOT = 265,
    SBRA = 266,
    SKET = 267,
    CBRA = 268,
    CKET = 269,
    LS_SEP = 270,
    NEWLINE = 271,
    WS = 272,
    COLON = 273,
    ROTATIONS = 274,
    AXIS = 275,
    QUBITS = 276,
    SINGLE_QUBIT_GATES = 277,
    TWO_QUBIT_GATES = 278,
    CR = 279,
    TOFFOLI = 280,
    CDASH = 281,
    NOT_TOKEN = 282,
    MAPKEY = 283,
    PREP = 284,
    MEASURE = 285,
    MEASUREPARITY = 286,
    MEASUREALL = 287,
    WAIT = 288,
    DISPLAY = 289,
    RESET_AVERAGING = 290,
    QBITHEAD = 291,
    BITHEAD = 292
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_GRAMMAR_TAB_H_INCLUDED  */
