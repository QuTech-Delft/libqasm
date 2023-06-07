/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_CQASM_V1_LIBQASM_BUILD_DEBUG_SRC_V1_CQASM_PARSER_HPP_INCLUDED
# define YY_CQASM_V1_LIBQASM_BUILD_DEBUG_SRC_V1_CQASM_PARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef CQASM_V1_DEBUG
# if defined YYDEBUG
#if YYDEBUG
#   define CQASM_V1_DEBUG 1
#  else
#   define CQASM_V1_DEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define CQASM_V1_DEBUG 0
# endif /* ! defined YYDEBUG */
#endif  /* ! defined CQASM_V1_DEBUG */
#if CQASM_V1_DEBUG
extern int cqasm_v1_debug;
#endif
/* "%code requires" blocks.  */
#line 5 "libqasm/src/v1/cqasm-parser.y"

    /**
     * \file
     * Parser, generated by Bison.
     */
    #include <memory>
    #include <cstdio>
    #include <cstdint>
    #include "cqasm-annotations.hpp"
    #include "cqasm-string-builder.hpp"
    #include "v1/cqasm-ast.hpp"
    #include "v1/cqasm-parse-helper.hpp"
    using namespace cqasm::ast;  // for StringBuilder
    using namespace cqasm::v1::ast;
    typedef void* yyscan_t;

    #define YYSTYPE CQASM_V1_STYPE
    #define YYLTYPE CQASM_V1_LTYPE


#line 78 "libqasm/src/v1/cqasm-parser.hpp"

/* Token kinds.  */
#ifndef CQASM_V1_TOKENTYPE
# define CQASM_V1_TOKENTYPE
  enum cqasm_v1_tokentype
  {
    CQASM_V1_EMPTY = -2,
    CQASM_V1_EOF = 0,              /* "end of file"  */
    CQASM_V1_error = 256,          /* error  */
    CQASM_V1_UNDEF = 257,          /* "invalid token"  */
    NEWLINE = 258,                 /* NEWLINE  */
    VERSION = 259,                 /* VERSION  */
    QUBITS = 260,                  /* QUBITS  */
    MAP = 261,                     /* MAP  */
    VAR = 262,                     /* VAR  */
    CDASH = 263,                   /* CDASH  */
    COND = 264,                    /* COND  */
    IF = 265,                      /* IF  */
    ELSE = 266,                    /* ELSE  */
    FOR = 267,                     /* FOR  */
    FOREACH = 268,                 /* FOREACH  */
    WHILE = 269,                   /* WHILE  */
    REPEAT = 270,                  /* REPEAT  */
    UNTIL = 271,                   /* UNTIL  */
    CONTINUE = 272,                /* CONTINUE  */
    BREAK = 273,                   /* BREAK  */
    SET = 274,                     /* SET  */
    INT_LITERAL = 275,             /* INT_LITERAL  */
    FLOAT_LITERAL = 276,           /* FLOAT_LITERAL  */
    BAD_NUMBER = 277,              /* BAD_NUMBER  */
    STRING_OPEN = 278,             /* STRING_OPEN  */
    STRING_CLOSE = 279,            /* STRING_CLOSE  */
    JSON_OPEN = 280,               /* JSON_OPEN  */
    JSON_CLOSE = 281,              /* JSON_CLOSE  */
    STRBUILD_APPEND = 282,         /* STRBUILD_APPEND  */
    STRBUILD_ESCAPE = 283,         /* STRBUILD_ESCAPE  */
    IDENTIFIER = 284,              /* IDENTIFIER  */
    POWER = 285,                   /* POWER  */
    INT_DIV = 286,                 /* INT_DIV  */
    LOGIC_OR = 287,                /* LOGIC_OR  */
    LOGIC_AND = 288,               /* LOGIC_AND  */
    LOGIC_XOR = 289,               /* LOGIC_XOR  */
    CMP_GE = 290,                  /* CMP_GE  */
    CMP_LE = 291,                  /* CMP_LE  */
    CMP_EQ = 292,                  /* CMP_EQ  */
    CMP_NE = 293,                  /* CMP_NE  */
    SHL = 294,                     /* SHL  */
    ARITH_SHR = 295,               /* ARITH_SHR  */
    LOGIC_SHR = 296,               /* LOGIC_SHR  */
    ELLIPSIS = 297,                /* ELLIPSIS  */
    BAD_CHARACTER = 298,           /* BAD_CHARACTER  */
    END_OF_FILE = 299,             /* END_OF_FILE  */
    UMINUS = 300,                  /* UMINUS  */
    UCOMP = 301,                   /* UCOMP  */
    UNOT = 302                     /* UNOT  */
  };
  typedef enum cqasm_v1_tokentype cqasm_v1_token_kind_t;
#endif

/* Value type.  */
#if ! defined CQASM_V1_STYPE && ! defined CQASM_V1_STYPE_IS_DECLARED
union CQASM_V1_STYPE
{
#line 78 "libqasm/src/v1/cqasm-parser.y"

    char           	*str;
    IntegerLiteral  *ilit;
    FloatLiteral    *flit;
    MatrixLiteral   *mat;
    StringBuilder   *strb;
    StringLiteral   *slit;
    JsonLiteral     *jlit;
    Identifier      *idnt;
    FunctionCall    *func;
    Index           *indx;
    UnaryOp         *unop;
    BinaryOp        *biop;
    TernaryCond     *tcnd;
    Expression      *expr;
    ExpressionList  *expl;
    IndexItem       *idxi;
    IndexRange      *idxr;
    IndexEntry      *idxe;
    IndexList       *idxl;
    AnnotationData  *adat;
    Instruction     *inst;
    Bundle          *bun;
    Mapping         *map;
    Variables       *vars;
    Subcircuit      *sub;
    Assignment      *asgn;
    IfElse          *ifel;
    ForLoop         *forl;
    ForeachLoop     *fore;
    WhileLoop       *whil;
    RepeatUntilLoop *repu;
    BreakStatement  *brk;
    ContinueStatement *cont;
    Statement       *stmt;
    StatementList   *stms;
    Version         *vers;
    Program         *prog;

#line 182 "libqasm/src/v1/cqasm-parser.hpp"

};
typedef union CQASM_V1_STYPE CQASM_V1_STYPE;
# define CQASM_V1_STYPE_IS_TRIVIAL 1
# define CQASM_V1_STYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined CQASM_V1_LTYPE && ! defined CQASM_V1_LTYPE_IS_DECLARED
typedef struct CQASM_V1_LTYPE CQASM_V1_LTYPE;
struct CQASM_V1_LTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define CQASM_V1_LTYPE_IS_DECLARED 1
# define CQASM_V1_LTYPE_IS_TRIVIAL 1
#endif




int cqasm_v1_parse (yyscan_t scanner, cqasm::v1::parser::ParseHelper &helper);


#endif /* !YY_CQASM_V1_LIBQASM_BUILD_DEBUG_SRC_V1_CQASM_PARSER_HPP_INCLUDED  */
