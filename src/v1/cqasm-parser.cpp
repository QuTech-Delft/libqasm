/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* "%code top" blocks.  */
#line 31 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"


    /**
     * Attaches a source location annotation object to the given node pointer.
     */
    #define ADD_SOURCE_LOCATION(v)                              \
        v->set_annotation(cqasm::annotations::SourceLocation(   \
            helper.filename,                                    \
            yyloc.first_line,                                   \
            yyloc.first_column,                                 \
            yyloc.last_line,                                    \
            yyloc.last_column))

    /**
     * Constructs an empty, new node of type T and places it into v. v is almost
     * always $$ (the result location), but it can't be made part of the
     * definition because preprocessing happens after bison's substitution.
     */
    #define NEW(v, T)           \
        v = new T();            \
        ADD_SOURCE_LOCATION(v)

    /**
     * Moves previously constructed node s into t, where s is almost always one
     * of bison's input locations ($1, $2, etc) and t is almost always $$. The
     * purpose of this over just assigning t = s is that it extends the source
     * location annotation of the object to the extents of whatever rule is
     * being matched.
     */
    #define FROM(t, s)                                                                  \
        t = s;                                                                          \
        {                                                                               \
            auto *loc = t->get_annotation_ptr<cqasm::annotations::SourceLocation>();    \
            if (!loc) {                                                                 \
                ADD_SOURCE_LOCATION(t);                                                 \
            } else {                                                                    \
                loc->expand_to_include(yyloc.first_line, yyloc.first_column);           \
                loc->expand_to_include(yyloc.last_line, yyloc.last_column);             \
            }                                                                           \
        }


#line 111 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
/* Substitute the type names.  */
#define YYSTYPE         CQASM_V1_STYPE
#define YYLTYPE         CQASM_V1_LTYPE
/* Substitute the variable and function names.  */
#define yyparse         cqasm_v1_parse
#define yylex           cqasm_v1_lex
#define yyerror         cqasm_v1_error
#define yydebug         cqasm_v1_debug
#define yynerrs         cqasm_v1_nerrs


# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "../../include/v1/cqasm-parser.hpp"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_NEWLINE = 3,                    /* NEWLINE  */
  YYSYMBOL_VERSION = 4,                    /* VERSION  */
  YYSYMBOL_QUBITS = 5,                     /* QUBITS  */
  YYSYMBOL_MAP = 6,                        /* MAP  */
  YYSYMBOL_VAR = 7,                        /* VAR  */
  YYSYMBOL_CDASH = 8,                      /* CDASH  */
  YYSYMBOL_COND = 9,                       /* COND  */
  YYSYMBOL_IF = 10,                        /* IF  */
  YYSYMBOL_ELSE = 11,                      /* ELSE  */
  YYSYMBOL_FOR = 12,                       /* FOR  */
  YYSYMBOL_FOREACH = 13,                   /* FOREACH  */
  YYSYMBOL_WHILE = 14,                     /* WHILE  */
  YYSYMBOL_REPEAT = 15,                    /* REPEAT  */
  YYSYMBOL_UNTIL = 16,                     /* UNTIL  */
  YYSYMBOL_CONTINUE = 17,                  /* CONTINUE  */
  YYSYMBOL_BREAK = 18,                     /* BREAK  */
  YYSYMBOL_SET = 19,                       /* SET  */
  YYSYMBOL_INT_LITERAL = 20,               /* INT_LITERAL  */
  YYSYMBOL_FLOAT_LITERAL = 21,             /* FLOAT_LITERAL  */
  YYSYMBOL_BAD_NUMBER = 22,                /* BAD_NUMBER  */
  YYSYMBOL_STRING_OPEN = 23,               /* STRING_OPEN  */
  YYSYMBOL_STRING_CLOSE = 24,              /* STRING_CLOSE  */
  YYSYMBOL_JSON_OPEN = 25,                 /* JSON_OPEN  */
  YYSYMBOL_JSON_CLOSE = 26,                /* JSON_CLOSE  */
  YYSYMBOL_STRBUILD_APPEND = 27,           /* STRBUILD_APPEND  */
  YYSYMBOL_STRBUILD_ESCAPE = 28,           /* STRBUILD_ESCAPE  */
  YYSYMBOL_IDENTIFIER = 29,                /* IDENTIFIER  */
  YYSYMBOL_POWER = 30,                     /* POWER  */
  YYSYMBOL_INT_DIV = 31,                   /* INT_DIV  */
  YYSYMBOL_LOGIC_OR = 32,                  /* LOGIC_OR  */
  YYSYMBOL_LOGIC_AND = 33,                 /* LOGIC_AND  */
  YYSYMBOL_LOGIC_XOR = 34,                 /* LOGIC_XOR  */
  YYSYMBOL_CMP_GE = 35,                    /* CMP_GE  */
  YYSYMBOL_CMP_LE = 36,                    /* CMP_LE  */
  YYSYMBOL_CMP_EQ = 37,                    /* CMP_EQ  */
  YYSYMBOL_CMP_NE = 38,                    /* CMP_NE  */
  YYSYMBOL_SHL = 39,                       /* SHL  */
  YYSYMBOL_ARITH_SHR = 40,                 /* ARITH_SHR  */
  YYSYMBOL_LOGIC_SHR = 41,                 /* LOGIC_SHR  */
  YYSYMBOL_ELLIPSIS = 42,                  /* ELLIPSIS  */
  YYSYMBOL_BAD_CHARACTER = 43,             /* BAD_CHARACTER  */
  YYSYMBOL_END_OF_FILE = 44,               /* END_OF_FILE  */
  YYSYMBOL_45_ = 45,                       /* ','  */
  YYSYMBOL_46_ = 46,                       /* ':'  */
  YYSYMBOL_47_ = 47,                       /* '?'  */
  YYSYMBOL_48_ = 48,                       /* '|'  */
  YYSYMBOL_49_ = 49,                       /* '^'  */
  YYSYMBOL_50_ = 50,                       /* '&'  */
  YYSYMBOL_51_ = 51,                       /* '>'  */
  YYSYMBOL_52_ = 52,                       /* '<'  */
  YYSYMBOL_53_ = 53,                       /* '+'  */
  YYSYMBOL_54_ = 54,                       /* '-'  */
  YYSYMBOL_55_ = 55,                       /* '*'  */
  YYSYMBOL_56_ = 56,                       /* '/'  */
  YYSYMBOL_57_ = 57,                       /* '%'  */
  YYSYMBOL_UMINUS = 58,                    /* UMINUS  */
  YYSYMBOL_UCOMP = 59,                     /* UCOMP  */
  YYSYMBOL_UNOT = 60,                      /* UNOT  */
  YYSYMBOL_61_ = 61,                       /* '('  */
  YYSYMBOL_62_ = 62,                       /* '['  */
  YYSYMBOL_63_ = 63,                       /* '@'  */
  YYSYMBOL_64_ = 64,                       /* ']'  */
  YYSYMBOL_65_ = 65,                       /* ')'  */
  YYSYMBOL_66_ = 66,                       /* '~'  */
  YYSYMBOL_67_ = 67,                       /* '!'  */
  YYSYMBOL_68_ = 68,                       /* '.'  */
  YYSYMBOL_69_ = 69,                       /* '='  */
  YYSYMBOL_70_ = 70,                       /* '{'  */
  YYSYMBOL_71_ = 71,                       /* '}'  */
  YYSYMBOL_YYACCEPT = 72,                  /* $accept  */
  YYSYMBOL_Newline = 73,                   /* Newline  */
  YYSYMBOL_OptNewline = 74,                /* OptNewline  */
  YYSYMBOL_IntegerLiteral = 75,            /* IntegerLiteral  */
  YYSYMBOL_FloatLiteral = 76,              /* FloatLiteral  */
  YYSYMBOL_MatrixRows = 77,                /* MatrixRows  */
  YYSYMBOL_MatrixLiteral = 78,             /* MatrixLiteral  */
  YYSYMBOL_StringBuilder = 79,             /* StringBuilder  */
  YYSYMBOL_StringLiteral = 80,             /* StringLiteral  */
  YYSYMBOL_JsonLiteral = 81,               /* JsonLiteral  */
  YYSYMBOL_Identifier = 82,                /* Identifier  */
  YYSYMBOL_FunctionCall = 83,              /* FunctionCall  */
  YYSYMBOL_IndexItem = 84,                 /* IndexItem  */
  YYSYMBOL_IndexRange = 85,                /* IndexRange  */
  YYSYMBOL_IndexEntry = 86,                /* IndexEntry  */
  YYSYMBOL_IndexList = 87,                 /* IndexList  */
  YYSYMBOL_Index = 88,                     /* Index  */
  YYSYMBOL_IndexNP = 89,                   /* IndexNP  */
  YYSYMBOL_UnaryOp = 90,                   /* UnaryOp  */
  YYSYMBOL_BinaryOp = 91,                  /* BinaryOp  */
  YYSYMBOL_TernaryOp = 92,                 /* TernaryOp  */
  YYSYMBOL_UnaryOpNP = 93,                 /* UnaryOpNP  */
  YYSYMBOL_BinaryOpNP = 94,                /* BinaryOpNP  */
  YYSYMBOL_TernaryOpNP = 95,               /* TernaryOpNP  */
  YYSYMBOL_Expression = 96,                /* Expression  */
  YYSYMBOL_ExpressionNP = 97,              /* ExpressionNP  */
  YYSYMBOL_ExpressionList = 98,            /* ExpressionList  */
  YYSYMBOL_ExpressionListNP = 99,          /* ExpressionListNP  */
  YYSYMBOL_AnnotationName = 100,           /* AnnotationName  */
  YYSYMBOL_AnnotationData = 101,           /* AnnotationData  */
  YYSYMBOL_Instruction = 102,              /* Instruction  */
  YYSYMBOL_AnnotInstr = 103,               /* AnnotInstr  */
  YYSYMBOL_SLParInstrList = 104,           /* SLParInstrList  */
  YYSYMBOL_CBParInstrList = 105,           /* CBParInstrList  */
  YYSYMBOL_Mapping = 106,                  /* Mapping  */
  YYSYMBOL_VariableBody = 107,             /* VariableBody  */
  YYSYMBOL_Variable = 108,                 /* Variable  */
  YYSYMBOL_Subcircuit = 109,               /* Subcircuit  */
  YYSYMBOL_SubStatements = 110,            /* SubStatements  */
  YYSYMBOL_Assignment = 111,               /* Assignment  */
  YYSYMBOL_OptAssignment = 112,            /* OptAssignment  */
  YYSYMBOL_IfElse = 113,                   /* IfElse  */
  YYSYMBOL_ForLoop = 114,                  /* ForLoop  */
  YYSYMBOL_ForeachLoop = 115,              /* ForeachLoop  */
  YYSYMBOL_WhileLoop = 116,                /* WhileLoop  */
  YYSYMBOL_RepeatUntilLoop = 117,          /* RepeatUntilLoop  */
  YYSYMBOL_Continue = 118,                 /* Continue  */
  YYSYMBOL_Break = 119,                    /* Break  */
  YYSYMBOL_Statement = 120,                /* Statement  */
  YYSYMBOL_AnnotStatement = 121,           /* AnnotStatement  */
  YYSYMBOL_StatementList = 122,            /* StatementList  */
  YYSYMBOL_Statements = 123,               /* Statements  */
  YYSYMBOL_Version = 124,                  /* Version  */
  YYSYMBOL_Program = 125,                  /* Program  */
  YYSYMBOL_Root = 126                      /* Root  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;



/* Unqualified %code blocks.  */
#line 26 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"

    int yylex(YYSTYPE* yylvalp, YYLTYPE* yyllocp, yyscan_t scanner);
    void yyerror(YYLTYPE* yyllocp, yyscan_t scanner, cqasm::v1::parser::ParseHelper &helper, const char* msg);

#line 287 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"

#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined CQASM_V1_LTYPE_IS_TRIVIAL && CQASM_V1_LTYPE_IS_TRIVIAL \
             && defined CQASM_V1_STYPE_IS_TRIVIAL && CQASM_V1_STYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  9
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   2125

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  72
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  55
/* YYNRULES -- Number of rules.  */
#define YYNRULES  170
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  328

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   302


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    67,     2,     2,     2,    57,    50,     2,
      61,    65,    55,    53,    45,    54,    68,    56,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    46,     2,
      52,    69,    51,    47,    63,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    62,     2,    64,    49,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    70,    48,    71,    66,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      58,    59,    60
};

#if CQASM_V1_DEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   253,   253,   254,   258,   259,   263,   267,   271,   272,
     275,   276,   281,   282,   283,   287,   291,   295,   299,   300,
     304,   307,   310,   311,   314,   315,   318,   321,   325,   326,
     327,   330,   331,   332,   333,   334,   335,   336,   337,   338,
     339,   340,   341,   342,   343,   344,   345,   346,   347,   348,
     349,   350,   351,   354,   359,   360,   361,   364,   365,   366,
     367,   368,   369,   370,   371,   372,   373,   374,   375,   376,
     377,   378,   379,   380,   381,   382,   383,   384,   387,   391,
     392,   393,   394,   395,   396,   397,   398,   399,   400,   401,
     402,   403,   406,   407,   408,   409,   410,   411,   412,   413,
     414,   415,   416,   417,   418,   422,   423,   426,   427,   431,
     434,   435,   436,   441,   446,   451,   459,   465,   471,   482,
     483,   487,   488,   492,   493,   497,   498,   502,   503,   506,
     510,   511,   515,   516,   519,   522,   523,   526,   532,   538,
     547,   557,   567,   570,   573,   576,   580,   581,   582,   583,
     584,   585,   586,   587,   588,   589,   590,   591,   592,   599,
     600,   604,   605,   609,   610,   614,   615,   619,   622,   627,
     628
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "NEWLINE", "VERSION",
  "QUBITS", "MAP", "VAR", "CDASH", "COND", "IF", "ELSE", "FOR", "FOREACH",
  "WHILE", "REPEAT", "UNTIL", "CONTINUE", "BREAK", "SET", "INT_LITERAL",
  "FLOAT_LITERAL", "BAD_NUMBER", "STRING_OPEN", "STRING_CLOSE",
  "JSON_OPEN", "JSON_CLOSE", "STRBUILD_APPEND", "STRBUILD_ESCAPE",
  "IDENTIFIER", "POWER", "INT_DIV", "LOGIC_OR", "LOGIC_AND", "LOGIC_XOR",
  "CMP_GE", "CMP_LE", "CMP_EQ", "CMP_NE", "SHL", "ARITH_SHR", "LOGIC_SHR",
  "ELLIPSIS", "BAD_CHARACTER", "END_OF_FILE", "','", "':'", "'?'", "'|'",
  "'^'", "'&'", "'>'", "'<'", "'+'", "'-'", "'*'", "'/'", "'%'", "UMINUS",
  "UCOMP", "UNOT", "'('", "'['", "'@'", "']'", "')'", "'~'", "'!'", "'.'",
  "'='", "'{'", "'}'", "$accept", "Newline", "OptNewline",
  "IntegerLiteral", "FloatLiteral", "MatrixRows", "MatrixLiteral",
  "StringBuilder", "StringLiteral", "JsonLiteral", "Identifier",
  "FunctionCall", "IndexItem", "IndexRange", "IndexEntry", "IndexList",
  "Index", "IndexNP", "UnaryOp", "BinaryOp", "TernaryOp", "UnaryOpNP",
  "BinaryOpNP", "TernaryOpNP", "Expression", "ExpressionNP",
  "ExpressionList", "ExpressionListNP", "AnnotationName", "AnnotationData",
  "Instruction", "AnnotInstr", "SLParInstrList", "CBParInstrList",
  "Mapping", "VariableBody", "Variable", "Subcircuit", "SubStatements",
  "Assignment", "OptAssignment", "IfElse", "ForLoop", "ForeachLoop",
  "WhileLoop", "RepeatUntilLoop", "Continue", "Break", "Statement",
  "AnnotStatement", "StatementList", "Statements", "Version", "Program",
  "Root", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-239)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-137)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      78,  -239,  -239,    11,    13,  -239,    30,  -239,    15,  -239,
    -239,  -239,     9,    15,   773,  -239,  -239,  -239,  -239,   951,
     951,    21,    21,   -17,     3,    19,    23,    34,    29,  -239,
    -239,  1002,  -239,    21,    71,   592,  -239,    35,    76,  -239,
    -239,  -239,  -239,  -239,  -239,  -239,  -239,  -239,  -239,  -239,
      68,    71,  -239,  -239,  -239,  -239,   951,   951,    71,   951,
     951,  -239,  -239,  -239,  -239,  -239,    72,  -239,  -239,  -239,
    -239,  -239,  1040,   -33,  1520,    46,  -239,  1002,   951,   951,
       4,   951,   951,    71,    60,  -239,  1002,   951,  1002,  1002,
    -239,  -239,  -239,  -239,  -239,    72,  -239,  -239,  -239,  -239,
    -239,  1192,    73,   210,  1751,    80,    21,   210,    21,   129,
    -239,    28,    62,    83,  1232,   417,    83,    83,   647,   951,
     951,   951,   951,   951,   951,   951,   951,   951,   951,   951,
     951,   951,   951,   951,   951,   951,   951,   951,   951,   951,
     951,   951,   951,   797,  -239,   951,    21,    21,    21,  1652,
    1268,  1304,  1112,  -239,   137,  1152,  1340,   872,    88,    93,
    1376,    93,    93,  1002,  1002,  1002,  1002,  1002,  1002,  1002,
    1002,  1002,  1002,  1002,  1002,  1002,  1002,  1002,  1002,  1002,
    1002,  1002,  1002,  1002,  1002,   951,  1002,   951,    76,    71,
    1002,    89,    95,  -239,    35,  -239,    68,  -239,  -239,  -239,
    -239,  -239,  -239,    71,  1685,   115,  -239,   -39,   -20,   -20,
    1781,  1839,   471,   569,   569,  2035,  2035,    -8,    -8,    -8,
    1553,  1895,   281,  1979,   569,   569,   176,   176,   -20,   -20,
     -20,  -239,  -239,  -239,   -32,  1586,  1685,  -239,  -239,  -239,
    1002,    21,    29,   951,   951,   951,    29,  -239,    71,   951,
    -239,   -11,   -11,  1811,  1923,  1867,   793,   793,  2063,  2063,
      97,    97,    97,  1718,  1951,  2007,   793,   793,   180,   180,
     -11,   -11,   -11,   -30,  1751,  1412,    64,    90,  1751,    21,
     925,   220,    98,   951,  -239,   951,   951,  -239,   951,    80,
    1002,   152,  1685,  1079,  1619,  -239,    96,  1448,  1002,  -239,
    -239,    76,  -239,  -239,  -239,   -34,   115,  -239,  1685,  1685,
    -239,  1685,    80,    -1,   935,   951,  -239,  -239,  1751,  -239,
    -239,  -239,   104,  1484,    29,    29,  -239,  -239
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,   170,     3,     4,     0,   169,     0,     2,     0,     1,
       6,   166,     5,     0,     0,   164,   168,   165,   158,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   144,
     145,     0,    17,     0,     5,     0,   120,   122,   149,   146,
     147,   148,   151,   152,   153,   154,   155,   156,   157,   160,
     162,     5,    91,     7,    14,    14,     0,     0,     5,     0,
       0,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,     5,    84,     0,     0,   129,     0,     0,     0,
       0,     0,     0,     5,     0,   104,     0,     0,     0,     0,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,     0,   130,     0,   108,   114,     0,     0,     0,     0,
     163,     0,     0,    28,     0,     0,    29,    30,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   167,     0,     0,     0,     0,   116,
       0,     0,     0,   135,     0,     0,     0,     0,     0,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   124,     5,
       0,     0,   110,   119,   121,   159,   161,    15,    12,    13,
      16,    90,    11,     5,   106,     9,    18,     0,    31,    34,
      52,    50,    51,    44,    46,    41,    42,    38,    39,    40,
       0,    49,    48,    47,    43,    45,    36,    37,    32,    33,
      35,    22,    23,    25,     0,    20,   126,   125,   128,   127,
       0,     0,     0,     0,     0,     0,     0,   133,     5,     0,
     103,    57,    60,    77,    75,    76,    70,    72,    67,    68,
      64,    65,    66,     0,    74,    73,    69,    71,    62,    63,
      58,    59,    61,     0,   115,     0,     4,     0,   107,     0,
       0,     0,     0,     0,    19,     0,     0,    26,     0,   117,
       0,   137,   134,     0,     0,   142,     0,     0,     0,    27,
     131,   123,   150,   109,   111,     0,     8,    10,   105,    53,
      24,    21,   118,     0,     0,     0,   132,   143,    78,   112,
     139,   138,     0,     0,     0,     0,   140,   141
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -239,     6,   222,     8,   215,  -239,   371,   146,   399,   555,
     182,   585,  -239,  -239,   -81,   -15,  -239,  -239,  -239,  -239,
    -239,  -239,  -239,  -239,   -19,   741,  -116,  -237,  -239,   100,
    -239,   105,   -83,  -239,  -239,    67,  -239,  -239,  -238,  -239,
     -98,   -93,  -239,  -239,  -239,  -239,  -239,  -239,  -239,   118,
      77,   156,  -239,  -239,  -239
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     3,    15,    61,    62,   203,    63,   111,    64,    65,
      66,    67,   231,   232,   233,   234,    68,    97,    69,    70,
      71,    98,    99,   100,   204,   104,   205,   105,   192,   193,
      36,    37,    38,   189,    39,    76,    40,    41,    84,   153,
     154,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    16,    12,     5,     6
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      72,    74,   207,   289,   291,    52,   283,  -136,   295,    24,
     119,   283,     2,   286,     7,   286,    11,     8,    14,   163,
     188,    17,   119,   120,    10,    53,   284,    54,   118,    55,
       9,   319,   287,    32,   299,    10,   145,   113,   114,    90,
     116,   117,   142,    90,    78,   137,   138,   139,   140,   141,
      32,   185,   197,   312,   142,   198,   199,   109,    56,   150,
     151,   152,   155,   156,    79,    57,    58,     7,   160,    83,
      59,    60,    22,    23,     2,   320,   158,    13,   143,     1,
      80,     2,    -5,    31,    81,    90,   326,   327,   200,   198,
     199,   147,   148,    32,    90,    82,    90,    90,   106,    83,
     208,   209,   210,   211,   212,   213,   214,   215,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   235,   107,   190,   236,   163,   164,    -4,
      18,   108,     7,   118,   187,    20,    21,    22,    23,    24,
     244,    25,    26,    27,    28,   142,    29,    30,    31,   249,
     180,   181,   182,   183,   184,   185,   280,   279,    32,   185,
     283,   302,   307,   313,   305,   306,   235,   316,   275,   324,
     273,    90,    90,    90,    90,    90,    90,    90,    90,    90,
      90,    90,    90,    90,    90,    90,    90,    90,    90,    90,
      90,    90,    90,   301,    90,   276,    35,    33,    90,    34,
      -4,   112,    73,    75,    77,   310,   119,   120,   195,   281,
     163,   164,   194,    95,   238,   102,   322,    95,    22,    23,
     321,    52,     4,     7,   292,   293,   294,   196,   144,    31,
     297,   139,   140,   141,   248,   182,   183,   184,   142,    32,
      10,    53,   185,    54,     0,    55,    91,     0,    90,    32,
      91,     0,     0,     0,   109,     0,   103,     0,     0,    95,
       0,     0,     0,     0,   308,     0,   309,   235,    95,   311,
      95,    95,     0,   110,    56,     0,     0,     0,     0,     0,
     115,    57,    58,     0,    -4,    35,    59,    60,   191,    35,
     191,    35,    91,     0,     0,   152,   323,     0,    90,     0,
       0,    91,     0,    91,    91,   157,    90,     0,     0,     0,
       0,   119,   120,     0,     0,     0,   124,   125,   126,   127,
     128,   129,   130,     0,     0,    35,     0,     0,   237,    75,
     239,   134,   135,   136,   137,   138,   139,   140,   141,    35,
       0,     0,     0,   142,     0,    95,    95,    95,    95,    95,
      95,    95,    95,    95,    95,    95,    95,    95,    95,    95,
      95,    95,    95,    95,    95,    95,    95,     0,    95,     0,
       0,     0,    95,     0,     0,     0,     0,     0,    91,    91,
      91,    91,    91,    91,    91,    91,    91,    91,    91,    91,
      91,    91,    91,    91,    91,    91,    91,    91,    91,    91,
       0,    91,    92,     0,     0,    91,    92,     0,     0,     0,
       0,   277,     0,     0,     0,     0,     0,     0,    52,     0,
       0,     0,    95,   290,     0,   282,     0,     0,     0,     0,
      93,     0,     0,     0,    93,     0,     0,    10,    53,     0,
      54,     0,    55,     0,     0,     0,    32,     0,    92,     0,
       0,     0,     0,     0,     0,    91,     0,    92,    35,    92,
      92,   303,     0,     0,     0,     0,     0,     0,     0,     0,
     296,    56,    95,     0,     0,     0,    93,     0,    57,    58,
      95,   202,     0,    59,    60,    93,     0,    93,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   119,   120,     0,   122,    91,   124,   125,   126,   127,
     128,   129,   130,    91,     0,     0,     0,     0,     0,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,     0,
       0,     0,     0,   142,    92,    92,    92,    92,    92,    92,
      92,    92,    92,    92,    92,    92,    92,    92,    92,    92,
      92,    92,    92,    92,    92,    92,     0,    92,     0,     0,
       0,    92,    93,    93,    93,    93,    93,    93,    93,    93,
      93,    93,    93,    93,    93,    93,    93,    93,    93,    93,
      93,    93,    93,    93,     0,    93,    94,     0,     0,    93,
      94,     0,  -113,    85,     0,  -113,     0,     0,     0,   119,
     120,     0,     0,     0,     0,     0,     0,     0,   128,   129,
     130,    92,    10,    53,     0,    54,    96,    55,     0,     0,
      96,    32,   137,   138,   139,   140,   141,     0,     0,     0,
       0,   142,    94,     0,     0,     0,     0,     0,     0,    93,
    -113,    94,     0,    94,    94,     0,    86,     0,    52,     0,
       0,     0,     0,    87,    58,  -113,     0,     0,    88,    89,
       0,    92,    96,  -113,     0,     0,     0,    10,    53,    92,
      54,    96,    55,    96,    96,     0,    32,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
       0,    56,     0,     0,     0,     0,     0,     0,    57,    58,
       0,     0,   206,    59,    60,     0,     0,     0,    94,    94,
      94,    94,    94,    94,    94,    94,    94,    94,    94,    94,
      94,    94,    94,    94,    94,    94,    94,    94,    94,    94,
       0,    94,     0,     0,     0,    94,     0,     0,    96,    96,
      96,    96,    96,    96,    96,    96,    96,    96,    96,    96,
      96,    96,    96,    96,    96,    96,    96,    96,    96,    96,
       0,    96,   101,    -4,    18,    96,     7,     0,    19,    20,
      21,    22,    23,    24,     0,    25,    26,    27,    28,     0,
      29,    30,    31,     0,     0,    94,     0,    -4,    18,     0,
       7,     0,    32,    20,    21,    22,    23,    24,     0,    25,
      26,    27,    28,     0,    29,    30,    31,     0,   149,     0,
       0,     0,     0,   163,   164,    96,    32,   159,     0,   161,
     162,     0,   172,   173,   174,     0,     0,     0,     0,     0,
       0,    33,     0,    34,     0,    94,   180,   181,   182,   183,
     184,     0,     0,    94,     0,   185,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    33,     0,    34,     0,     0,
       0,     0,     0,    18,     0,    96,     0,     0,    20,    21,
      22,    23,    24,    96,    25,    26,    27,    28,     0,    29,
      30,    31,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    32,     0,     0,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,   271,   272,    52,   274,     0,     0,
       0,   278,     0,     0,     0,     0,    52,     0,     0,     0,
      33,     0,    34,   247,     0,    10,    53,     0,    54,     0,
      55,     0,    52,     0,    32,    10,    53,     0,    54,     0,
      55,     0,     0,     0,    32,     0,     0,     0,     0,     0,
       0,    10,    53,     0,    54,     0,    55,     0,     0,    56,
      32,     0,     0,     0,     0,     0,    57,    58,     0,    56,
     304,    59,    60,     0,     0,     0,    57,    58,     0,     0,
    -136,    59,    60,    85,     0,    56,     0,     0,     0,     0,
       0,     0,    57,    58,     0,     0,     0,    59,    60,     0,
       0,     0,    10,    53,     0,    54,     0,    55,     0,     0,
       0,    32,     0,     0,     0,     0,     0,     0,     0,   318,
       0,     0,     0,     2,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    86,     0,     0,     0,
       0,     0,     0,    87,    58,     0,     0,     0,    88,    89,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   314,     0,     0,     0,     0,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,     0,     0,
       0,     0,   142,     0,     0,     0,     0,     0,     0,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,     0,     0,     0,     0,     0,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,     0,     0,     0,
       0,   142,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,     0,     0,     0,     0,     0,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
       0,     0,     0,     0,   142,     0,     0,     0,     0,     0,
       0,   243,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,     0,     0,     0,     0,     0,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
       0,     0,     0,     0,   142,     0,     0,     0,     0,     0,
       0,   245,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,     0,     0,     0,     0,     0,   175,
       0,   176,   177,   178,   179,   180,   181,   182,   183,   184,
       0,     0,     0,     0,   185,     0,     0,     0,     0,     0,
       0,   186,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,     0,     0,     0,     0,     0,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
       0,     0,     0,     0,   142,     0,     0,   201,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
       0,     0,     0,     0,     0,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,     0,     0,     0,     0,
     142,     0,     0,   241,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,     0,     0,     0,     0,
       0,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,     0,     0,     0,     0,   142,     0,     0,   242,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,     0,     0,     0,     0,     0,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,     0,     0,
       0,     0,   142,     0,     0,   246,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,     0,     0,
       0,     0,     0,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,     0,     0,     0,     0,   142,     0,
       0,   250,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,     0,     0,     0,     0,     0,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
       0,     0,     0,     0,   142,     0,     0,   300,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
       0,     0,     0,     0,     0,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,     0,     0,     0,     0,
     142,     0,     0,   317,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,     0,     0,     0,     0,
       0,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,     0,     0,     0,     0,   142,     0,     0,   325,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,     0,     0,     0,   146,     0,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,     0,     0,
       0,     0,   142,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,     0,     0,     0,     0,   285,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,     0,     0,     0,     0,   142,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,     0,     0,
       0,     0,   288,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,     0,     0,     0,     0,   142,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   315,     0,     0,     0,     0,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,     0,     0,     0,
       0,   142,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,     0,     0,     0,   240,     0,   175,
       0,   176,   177,   178,   179,   180,   181,   182,   183,   184,
       0,     0,     0,     0,   185,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,     0,     0,     0,
       0,     0,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,     0,     0,     0,     0,   142,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
       0,     0,     0,     0,   298,   175,     0,   176,   177,   178,
     179,   180,   181,   182,   183,   184,     0,     0,     0,     0,
     185,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,     0,     0,     0,     0,     0,   175,     0,
     176,   177,   178,   179,   180,   181,   182,   183,   184,     0,
       0,   119,   120,   185,   122,   123,   124,   125,   126,   127,
     128,   129,   130,     0,     0,     0,     0,     0,     0,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,     0,
       0,   163,   164,   142,   166,   167,   168,   169,   170,   171,
     172,   173,   174,     0,     0,     0,     0,     0,     0,     0,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   119,
     120,     0,     0,   185,   124,   125,   126,   127,   128,   129,
     130,     0,     0,     0,     0,     0,     0,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   163,   164,     0,
     166,   142,   168,   169,   170,   171,   172,   173,   174,     0,
       0,     0,     0,     0,     0,     0,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   119,   120,     0,     0,   185,
     124,   125,   126,   127,   128,   129,   130,     0,     0,     0,
       0,     0,     0,     0,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   163,   164,     0,     0,   142,   168,   169,
     170,   171,   172,   173,   174,     0,     0,     0,     0,     0,
       0,     0,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   163,   164,     0,     0,   185,   168,   169,   170,   171,
     172,   173,   174,     0,     0,     0,     0,     0,     0,     0,
       0,   177,   178,   179,   180,   181,   182,   183,   184,   119,
     120,     0,     0,   185,   124,   125,   126,   127,   128,   129,
     130,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     135,   136,   137,   138,   139,   140,   141,   163,   164,     0,
       0,   142,   168,   169,   170,   171,   172,   173,   174,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   178,   179,
     180,   181,   182,   183,   184,   119,   120,     0,     0,   185,
     124,   125,     0,     0,   128,   129,   130,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   135,   136,   137,   138,
     139,   140,   141,   163,   164,     0,     0,   142,   168,   169,
       0,     0,   172,   173,   174,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   178,   179,   180,   181,   182,   183,
     184,     0,     0,     0,     0,   185
};

static const yytype_int16 yycheck[] =
{
      19,    20,   118,   240,   242,     1,    45,     3,   246,    10,
      30,    45,     3,    45,     3,    45,     8,     4,    12,    30,
     103,    13,    30,    31,    20,    21,    65,    23,    61,    25,
       0,    65,    64,    29,    64,    20,    69,    56,    57,    31,
      59,    60,    62,    35,    61,    53,    54,    55,    56,    57,
      29,    62,    24,   290,    62,    27,    28,    51,    54,    78,
      79,    80,    81,    82,    61,    61,    62,     3,    87,    70,
      66,    67,     8,     9,     3,   313,    16,    68,    72,     1,
      61,     3,     4,    19,    61,    77,   324,   325,    26,    27,
      28,    45,    46,    29,    86,    61,    88,    89,    63,    70,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,    48,    45,   145,    30,    31,     0,
       1,    63,     3,    61,    61,     6,     7,     8,     9,    10,
       3,    12,    13,    14,    15,    62,    17,    18,    19,    61,
      53,    54,    55,    56,    57,    62,    61,    68,    29,    62,
      45,    71,    64,    11,   280,   281,   185,    71,   187,    65,
     185,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   276,   186,   189,    14,    68,   190,    70,
      71,    55,    20,    21,    22,   286,    30,    31,   108,   203,
      30,    31,   107,    31,   147,    33,   314,    35,     8,     9,
     313,     1,     0,     3,   243,   244,   245,   109,    72,    19,
     249,    55,    56,    57,   157,    55,    56,    57,    62,    29,
      20,    21,    62,    23,    -1,    25,    31,    -1,   240,    29,
      35,    -1,    -1,    -1,   248,    -1,    34,    -1,    -1,    77,
      -1,    -1,    -1,    -1,   283,    -1,   285,   286,    86,   288,
      88,    89,    -1,    51,    54,    -1,    -1,    -1,    -1,    -1,
      58,    61,    62,    -1,    64,   103,    66,    67,   106,   107,
     108,   109,    77,    -1,    -1,   314,   315,    -1,   290,    -1,
      -1,    86,    -1,    88,    89,    83,   298,    -1,    -1,    -1,
      -1,    30,    31,    -1,    -1,    -1,    35,    36,    37,    38,
      39,    40,    41,    -1,    -1,   143,    -1,    -1,   146,   147,
     148,    50,    51,    52,    53,    54,    55,    56,    57,   157,
      -1,    -1,    -1,    62,    -1,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,    -1,   186,    -1,
      -1,    -1,   190,    -1,    -1,    -1,    -1,    -1,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
      -1,   186,    31,    -1,    -1,   190,    35,    -1,    -1,    -1,
      -1,   189,    -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,
      -1,    -1,   240,   241,    -1,   203,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    35,    -1,    -1,    20,    21,    -1,
      23,    -1,    25,    -1,    -1,    -1,    29,    -1,    77,    -1,
      -1,    -1,    -1,    -1,    -1,   240,    -1,    86,   276,    88,
      89,   279,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     248,    54,   290,    -1,    -1,    -1,    77,    -1,    61,    62,
     298,    64,    -1,    66,    67,    86,    -1,    88,    89,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    30,    31,    -1,    33,   290,    35,    36,    37,    38,
      39,    40,    41,   298,    -1,    -1,    -1,    -1,    -1,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    -1,
      -1,    -1,    -1,    62,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,    -1,   186,    -1,    -1,
      -1,   190,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,    -1,   186,    31,    -1,    -1,   190,
      35,    -1,     0,     1,    -1,     3,    -1,    -1,    -1,    30,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    40,
      41,   240,    20,    21,    -1,    23,    31,    25,    -1,    -1,
      35,    29,    53,    54,    55,    56,    57,    -1,    -1,    -1,
      -1,    62,    77,    -1,    -1,    -1,    -1,    -1,    -1,   240,
      48,    86,    -1,    88,    89,    -1,    54,    -1,     1,    -1,
      -1,    -1,    -1,    61,    62,    63,    -1,    -1,    66,    67,
      -1,   290,    77,    71,    -1,    -1,    -1,    20,    21,   298,
      23,    86,    25,    88,    89,    -1,    29,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   290,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   298,    -1,    -1,
      -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    61,    62,
      -1,    -1,    65,    66,    67,    -1,    -1,    -1,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
      -1,   186,    -1,    -1,    -1,   190,    -1,    -1,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
      -1,   186,    31,     0,     1,   190,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    12,    13,    14,    15,    -1,
      17,    18,    19,    -1,    -1,   240,    -1,     0,     1,    -1,
       3,    -1,    29,     6,     7,     8,     9,    10,    -1,    12,
      13,    14,    15,    -1,    17,    18,    19,    -1,    77,    -1,
      -1,    -1,    -1,    30,    31,   240,    29,    86,    -1,    88,
      89,    -1,    39,    40,    41,    -1,    -1,    -1,    -1,    -1,
      -1,    68,    -1,    70,    -1,   290,    53,    54,    55,    56,
      57,    -1,    -1,   298,    -1,    62,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    68,    -1,    70,    -1,    -1,
      -1,    -1,    -1,     1,    -1,   290,    -1,    -1,     6,     7,
       8,     9,    10,   298,    12,    13,    14,    15,    -1,    17,
      18,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    29,    -1,    -1,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,     1,   186,    -1,    -1,
      -1,   190,    -1,    -1,    -1,    -1,     1,    -1,    -1,    -1,
      68,    -1,    70,    71,    -1,    20,    21,    -1,    23,    -1,
      25,    -1,     1,    -1,    29,    20,    21,    -1,    23,    -1,
      25,    -1,    -1,    -1,    29,    -1,    -1,    -1,    -1,    -1,
      -1,    20,    21,    -1,    23,    -1,    25,    -1,    -1,    54,
      29,    -1,    -1,    -1,    -1,    -1,    61,    62,    -1,    54,
      65,    66,    67,    -1,    -1,    -1,    61,    62,    -1,    -1,
      65,    66,    67,     1,    -1,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    62,    -1,    -1,    -1,    66,    67,    -1,
      -1,    -1,    20,    21,    -1,    23,    -1,    25,    -1,    -1,
      -1,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   298,
      -1,    -1,    -1,     3,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    62,    -1,    -1,    -1,    66,    67,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,     3,    -1,    -1,    -1,    -1,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    -1,    -1,
      -1,    -1,    62,    -1,    -1,    -1,    -1,    -1,    -1,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    -1,    -1,    -1,    -1,    -1,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    -1,    -1,    -1,
      -1,    62,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    -1,    -1,    -1,    -1,    -1,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      -1,    -1,    -1,    -1,    62,    -1,    -1,    -1,    -1,    -1,
      -1,    69,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    -1,    -1,    -1,    -1,    -1,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      -1,    -1,    -1,    -1,    62,    -1,    -1,    -1,    -1,    -1,
      -1,    69,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    -1,    -1,    -1,    -1,    -1,    47,
      -1,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      -1,    -1,    -1,    -1,    62,    -1,    -1,    -1,    -1,    -1,
      -1,    69,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    -1,    -1,    -1,    -1,    -1,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      -1,    -1,    -1,    -1,    62,    -1,    -1,    65,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      -1,    -1,    -1,    -1,    -1,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    -1,    -1,    -1,    -1,
      62,    -1,    -1,    65,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    -1,    -1,    -1,    -1,
      -1,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    -1,    -1,    -1,    -1,    62,    -1,    -1,    65,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    -1,    -1,    -1,    -1,    -1,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    -1,    -1,
      -1,    -1,    62,    -1,    -1,    65,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    -1,    -1,
      -1,    -1,    -1,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    -1,    -1,    -1,    -1,    62,    -1,
      -1,    65,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    -1,    -1,    -1,    -1,    -1,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      -1,    -1,    -1,    -1,    62,    -1,    -1,    65,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      -1,    -1,    -1,    -1,    -1,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    -1,    -1,    -1,    -1,
      62,    -1,    -1,    65,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    -1,    -1,    -1,    -1,
      -1,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    -1,    -1,    -1,    -1,    62,    -1,    -1,    65,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    -1,    -1,    -1,    45,    -1,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    -1,    -1,
      -1,    -1,    62,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    -1,    -1,    -1,    -1,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    -1,    -1,    -1,    -1,    62,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    -1,    -1,
      -1,    -1,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    -1,    -1,    -1,    -1,    62,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    -1,    -1,    -1,    -1,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    -1,    -1,    -1,
      -1,    62,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    -1,    -1,    -1,    45,    -1,    47,
      -1,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      -1,    -1,    -1,    -1,    62,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    -1,    -1,    -1,
      -1,    -1,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    -1,    -1,    -1,    -1,    62,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      -1,    -1,    -1,    -1,    46,    47,    -1,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    -1,    -1,    -1,    -1,
      62,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    -1,    -1,    -1,    -1,    -1,    47,    -1,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    -1,
      -1,    30,    31,    62,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    -1,    -1,    -1,    -1,    -1,    -1,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    -1,
      -1,    30,    31,    62,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    30,
      31,    -1,    -1,    62,    35,    36,    37,    38,    39,    40,
      41,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    30,    31,    -1,
      33,    62,    35,    36,    37,    38,    39,    40,    41,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    30,    31,    -1,    -1,    62,
      35,    36,    37,    38,    39,    40,    41,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    30,    31,    -1,    -1,    62,    35,    36,
      37,    38,    39,    40,    41,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    30,    31,    -1,    -1,    62,    35,    36,    37,    38,
      39,    40,    41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    50,    51,    52,    53,    54,    55,    56,    57,    30,
      31,    -1,    -1,    62,    35,    36,    37,    38,    39,    40,
      41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      51,    52,    53,    54,    55,    56,    57,    30,    31,    -1,
      -1,    62,    35,    36,    37,    38,    39,    40,    41,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    51,    52,
      53,    54,    55,    56,    57,    30,    31,    -1,    -1,    62,
      35,    36,    -1,    -1,    39,    40,    41,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    51,    52,    53,    54,
      55,    56,    57,    30,    31,    -1,    -1,    62,    35,    36,
      -1,    -1,    39,    40,    41,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    51,    52,    53,    54,    55,    56,
      57,    -1,    -1,    -1,    -1,    62
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     1,     3,    73,    74,   125,   126,     3,     4,     0,
      20,    75,   124,    68,    73,    74,   123,    75,     1,     5,
       6,     7,     8,     9,    10,    12,    13,    14,    15,    17,
      18,    19,    29,    68,    70,    82,   102,   103,   104,   106,
     108,   109,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,     1,    21,    23,    25,    54,    61,    62,    66,
      67,    75,    76,    78,    80,    81,    82,    83,    88,    90,
      91,    92,    96,    82,    96,    82,   107,    82,    61,    61,
      61,    61,    61,    70,   110,     1,    54,    61,    66,    67,
      75,    76,    78,    80,    81,    82,    83,    89,    93,    94,
      95,    97,    82,    74,    97,    99,    63,    48,    63,    73,
      74,    79,    79,    96,    96,    74,    96,    96,    61,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    62,    73,   123,    69,    45,    45,    46,    97,
      96,    96,    96,   111,   112,    96,    96,    74,    16,    97,
      96,    97,    97,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    47,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    62,    69,    61,   104,   105,
      45,    82,   100,   101,   103,   101,   121,    24,    27,    28,
      26,    65,    64,    77,    96,    98,    65,    98,    96,    96,
      96,    96,    96,    96,    96,    96,    96,    96,    96,    96,
      96,    96,    96,    96,    96,    96,    96,    96,    96,    96,
      96,    84,    85,    86,    87,    96,    96,    82,   107,    82,
      45,    65,    65,    69,     3,    69,    65,    71,   122,    61,
      65,    97,    97,    97,    97,    97,    97,    97,    97,    97,
      97,    97,    97,    97,    97,    97,    97,    97,    97,    97,
      97,    97,    97,    87,    97,    96,    73,    74,    97,    68,
      61,    73,    74,    45,    65,    46,    45,    64,    46,    99,
      82,   110,    96,    96,    96,   110,    74,    96,    46,    64,
      65,   104,    71,    82,    65,    98,    98,    64,    96,    96,
      86,    96,    99,    11,     3,    42,    71,    65,    97,    65,
     110,   113,   112,    96,    65,    65,   110,   110
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    72,    73,    73,    74,    74,    75,    76,    77,    77,
      78,    78,    79,    79,    79,    80,    81,    82,    83,    83,
      84,    85,    86,    86,    87,    87,    88,    89,    90,    90,
      90,    91,    91,    91,    91,    91,    91,    91,    91,    91,
      91,    91,    91,    91,    91,    91,    91,    91,    91,    91,
      91,    91,    91,    92,    93,    93,    93,    94,    94,    94,
      94,    94,    94,    94,    94,    94,    94,    94,    94,    94,
      94,    94,    94,    94,    94,    94,    94,    94,    95,    96,
      96,    96,    96,    96,    96,    96,    96,    96,    96,    96,
      96,    96,    97,    97,    97,    97,    97,    97,    97,    97,
      97,    97,    97,    97,    97,    98,    98,    99,    99,   100,
     101,   101,   101,   102,   102,   102,   102,   102,   102,   103,
     103,   104,   104,   105,   105,   106,   106,   107,   107,   108,
     109,   109,   110,   110,   111,   112,   112,   113,   113,   113,
     114,   115,   116,   117,   118,   119,   120,   120,   120,   120,
     120,   120,   120,   120,   120,   120,   120,   120,   120,   121,
     121,   122,   122,   123,   123,   124,   124,   125,   125,   126,
     126
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     1,     1,     0,     1,     1,     3,     1,
       5,     3,     2,     2,     0,     3,     3,     1,     3,     4,
       1,     3,     1,     1,     3,     1,     4,     4,     2,     2,
       2,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     5,     2,     2,     2,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     5,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     1,     3,     1,     3,     1,     3,
       1,     3,     4,     1,     2,     4,     3,     5,     6,     3,
       1,     3,     1,     3,     1,     4,     4,     3,     3,     2,
       2,     5,     5,     3,     3,     1,     0,     5,     7,     7,
       9,     9,     5,     6,     1,     1,     1,     1,     1,     1,
       5,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       1,     3,     1,     3,     1,     3,     1,     7,     4,     1,
       1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = CQASM_V1_EMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == CQASM_V1_EMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (&yylloc, scanner, helper, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use CQASM_V1_error or CQASM_V1_UNDEF. */
#define YYERRCODE CQASM_V1_UNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if CQASM_V1_DEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined CQASM_V1_LTYPE_IS_TRIVIAL && CQASM_V1_LTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location, scanner, helper); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, yyscan_t scanner, cqasm::v1::parser::ParseHelper &helper)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  YY_USE (scanner);
  YY_USE (helper);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, yyscan_t scanner, cqasm::v1::parser::ParseHelper &helper)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp, scanner, helper);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule, yyscan_t scanner, cqasm::v1::parser::ParseHelper &helper)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]), scanner, helper);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, scanner, helper); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !CQASM_V1_DEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !CQASM_V1_DEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
  YYLTYPE *yylloc;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyctx->yyssp];
  if (!yypact_value_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yyx;
      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
# endif
#endif

#ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;
      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
#endif


static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yytnamerr (YY_NULLPTR, yytname[yyarg[yyi]]);
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yytname[yyarg[yyi++]]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, yyscan_t scanner, cqasm::v1::parser::ParseHelper &helper)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  YY_USE (scanner);
  YY_USE (helper);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}






/*----------.
| yyparse.  |
`----------*/

int
yyparse (yyscan_t scanner, cqasm::v1::parser::ParseHelper &helper)
{
/* Lookahead token kind.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

/* Location data for the lookahead symbol.  */
static YYLTYPE yyloc_default
# if defined CQASM_V1_LTYPE_IS_TRIVIAL && CQASM_V1_LTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

    /* Number of syntax errors so far.  */
    int yynerrs = 0;

    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = CQASM_V1_EMPTY; /* Cause a token to be read.  */

  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == CQASM_V1_EMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex (&yylval, &yylloc, scanner);
    }

  if (yychar <= CQASM_V1_EOF)
    {
      yychar = CQASM_V1_EOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == CQASM_V1_error)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = CQASM_V1_UNDEF;
      yytoken = YYSYMBOL_YYerror;
      yyerror_range[1] = yylloc;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = CQASM_V1_EMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 6: /* IntegerLiteral: INT_LITERAL  */
#line 263 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.ilit), IntegerLiteral); (yyval.ilit)->value = std::strtol((yyvsp[0].str), nullptr, 0); std::free((yyvsp[0].str)); }
#line 2233 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 7: /* FloatLiteral: FLOAT_LITERAL  */
#line 267 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.flit), FloatLiteral); (yyval.flit)->value = std::strtod((yyvsp[0].str), nullptr); std::free((yyvsp[0].str)); }
#line 2239 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 8: /* MatrixRows: MatrixRows Newline ExpressionList  */
#line 271 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.mat), (yyvsp[-2].mat)); (yyval.mat)->rows.add_raw((yyvsp[0].expl)); }
#line 2245 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 9: /* MatrixRows: ExpressionList  */
#line 272 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.mat), MatrixLiteral); (yyval.mat)->rows.add_raw((yyvsp[0].expl)); }
#line 2251 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 10: /* MatrixLiteral: '[' OptNewline MatrixRows OptNewline ']'  */
#line 275 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.mat), (yyvsp[-2].mat)); }
#line 2257 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 11: /* MatrixLiteral: '[' OptNewline ']'  */
#line 276 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.mat), MatrixLiteral); (yyval.mat)->rows.add_raw(new ExpressionList()); }
#line 2263 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 12: /* StringBuilder: StringBuilder STRBUILD_APPEND  */
#line 281 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.strb), (yyvsp[-1].strb)); (yyval.strb)->push_string(std::string((yyvsp[0].str))); std::free((yyvsp[0].str)); }
#line 2269 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 13: /* StringBuilder: StringBuilder STRBUILD_ESCAPE  */
#line 282 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.strb), (yyvsp[-1].strb)); (yyval.strb)->push_escape(std::string((yyvsp[0].str))); std::free((yyvsp[0].str)); }
#line 2275 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 14: /* StringBuilder: %empty  */
#line 283 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.strb), StringBuilder); }
#line 2281 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 15: /* StringLiteral: STRING_OPEN StringBuilder STRING_CLOSE  */
#line 287 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.slit), StringLiteral); (yyval.slit)->value = (yyvsp[-1].strb)->stream.str(); delete (yyvsp[-1].strb); }
#line 2287 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 16: /* JsonLiteral: JSON_OPEN StringBuilder JSON_CLOSE  */
#line 291 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.jlit), JsonLiteral); (yyval.jlit)->value = (yyvsp[-1].strb)->stream.str(); delete (yyvsp[-1].strb); }
#line 2293 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 17: /* Identifier: IDENTIFIER  */
#line 295 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.idnt), Identifier); (yyval.idnt)->name = std::string((yyvsp[0].str)); std::free((yyvsp[0].str)); }
#line 2299 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 18: /* FunctionCall: Identifier '(' ')'  */
#line 299 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.func), FunctionCall); (yyval.func)->name.set_raw((yyvsp[-2].idnt)); (yyval.func)->arguments.set_raw(new ExpressionList()); }
#line 2305 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 19: /* FunctionCall: Identifier '(' ExpressionList ')'  */
#line 300 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.func), FunctionCall); (yyval.func)->name.set_raw((yyvsp[-3].idnt)); (yyval.func)->arguments.set_raw((yyvsp[-1].expl)); }
#line 2311 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 20: /* IndexItem: Expression  */
#line 304 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.idxi), IndexItem); (yyval.idxi)->index.set_raw((yyvsp[0].expr)); }
#line 2317 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 21: /* IndexRange: Expression ':' Expression  */
#line 307 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.idxr), IndexRange); (yyval.idxr)->first.set_raw((yyvsp[-2].expr)); (yyval.idxr)->last.set_raw((yyvsp[0].expr)); }
#line 2323 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 22: /* IndexEntry: IndexItem  */
#line 310 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.idxe), (yyvsp[0].idxi)); }
#line 2329 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 23: /* IndexEntry: IndexRange  */
#line 311 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.idxe), (yyvsp[0].idxr)); }
#line 2335 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 24: /* IndexList: IndexList ',' IndexEntry  */
#line 314 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.idxl), (yyvsp[-2].idxl)); (yyval.idxl)->items.add_raw((yyvsp[0].idxe)); }
#line 2341 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 25: /* IndexList: IndexEntry  */
#line 315 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.idxl), IndexList); (yyval.idxl)->items.add_raw((yyvsp[0].idxe)); }
#line 2347 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 26: /* Index: Expression '[' IndexList ']'  */
#line 318 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.indx), Index); (yyval.indx)->expr.set_raw((yyvsp[-3].expr)); (yyval.indx)->indices.set_raw((yyvsp[-1].idxl)); }
#line 2353 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 27: /* IndexNP: ExpressionNP '[' IndexList ']'  */
#line 321 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.indx), Index); (yyval.indx)->expr.set_raw((yyvsp[-3].expr)); (yyval.indx)->indices.set_raw((yyvsp[-1].idxl)); }
#line 2359 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 28: /* UnaryOp: '-' Expression  */
#line 325 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.unop), Negate); (yyval.unop)->expr.set_raw((yyvsp[0].expr)); }
#line 2365 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 29: /* UnaryOp: '~' Expression  */
#line 326 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.unop), BitwiseNot); (yyval.unop)->expr.set_raw((yyvsp[0].expr)); }
#line 2371 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 30: /* UnaryOp: '!' Expression  */
#line 327 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.unop), LogicalNot); (yyval.unop)->expr.set_raw((yyvsp[0].expr)); }
#line 2377 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 31: /* BinaryOp: Expression POWER Expression  */
#line 330 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.biop), Power);           (yyval.biop)->lhs.set_raw((yyvsp[-2].expr)); (yyval.biop)->rhs.set_raw((yyvsp[0].expr)); }
#line 2383 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 32: /* BinaryOp: Expression '*' Expression  */
#line 331 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.biop), Multiply);        (yyval.biop)->lhs.set_raw((yyvsp[-2].expr)); (yyval.biop)->rhs.set_raw((yyvsp[0].expr)); }
#line 2389 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 33: /* BinaryOp: Expression '/' Expression  */
#line 332 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.biop), Divide);          (yyval.biop)->lhs.set_raw((yyvsp[-2].expr)); (yyval.biop)->rhs.set_raw((yyvsp[0].expr)); }
#line 2395 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 34: /* BinaryOp: Expression INT_DIV Expression  */
#line 333 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.biop), IntDivide);       (yyval.biop)->lhs.set_raw((yyvsp[-2].expr)); (yyval.biop)->rhs.set_raw((yyvsp[0].expr)); }
#line 2401 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 35: /* BinaryOp: Expression '%' Expression  */
#line 334 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.biop), Modulo);          (yyval.biop)->lhs.set_raw((yyvsp[-2].expr)); (yyval.biop)->rhs.set_raw((yyvsp[0].expr)); }
#line 2407 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 36: /* BinaryOp: Expression '+' Expression  */
#line 335 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.biop), Add);             (yyval.biop)->lhs.set_raw((yyvsp[-2].expr)); (yyval.biop)->rhs.set_raw((yyvsp[0].expr)); }
#line 2413 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 37: /* BinaryOp: Expression '-' Expression  */
#line 336 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.biop), Subtract);        (yyval.biop)->lhs.set_raw((yyvsp[-2].expr)); (yyval.biop)->rhs.set_raw((yyvsp[0].expr)); }
#line 2419 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 38: /* BinaryOp: Expression SHL Expression  */
#line 337 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.biop), ShiftLeft);       (yyval.biop)->lhs.set_raw((yyvsp[-2].expr)); (yyval.biop)->rhs.set_raw((yyvsp[0].expr)); }
#line 2425 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 39: /* BinaryOp: Expression ARITH_SHR Expression  */
#line 338 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.biop), ShiftRightArith); (yyval.biop)->lhs.set_raw((yyvsp[-2].expr)); (yyval.biop)->rhs.set_raw((yyvsp[0].expr)); }
#line 2431 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 40: /* BinaryOp: Expression LOGIC_SHR Expression  */
#line 339 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.biop), ShiftRightLogic); (yyval.biop)->lhs.set_raw((yyvsp[-2].expr)); (yyval.biop)->rhs.set_raw((yyvsp[0].expr)); }
#line 2437 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 41: /* BinaryOp: Expression CMP_EQ Expression  */
#line 340 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.biop), CmpEq);           (yyval.biop)->lhs.set_raw((yyvsp[-2].expr)); (yyval.biop)->rhs.set_raw((yyvsp[0].expr)); }
#line 2443 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 42: /* BinaryOp: Expression CMP_NE Expression  */
#line 341 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.biop), CmpNe);           (yyval.biop)->lhs.set_raw((yyvsp[-2].expr)); (yyval.biop)->rhs.set_raw((yyvsp[0].expr)); }
#line 2449 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 43: /* BinaryOp: Expression '>' Expression  */
#line 342 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.biop), CmpGt);           (yyval.biop)->lhs.set_raw((yyvsp[-2].expr)); (yyval.biop)->rhs.set_raw((yyvsp[0].expr)); }
#line 2455 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 44: /* BinaryOp: Expression CMP_GE Expression  */
#line 343 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.biop), CmpGe);           (yyval.biop)->lhs.set_raw((yyvsp[-2].expr)); (yyval.biop)->rhs.set_raw((yyvsp[0].expr)); }
#line 2461 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 45: /* BinaryOp: Expression '<' Expression  */
#line 344 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.biop), CmpLt);           (yyval.biop)->lhs.set_raw((yyvsp[-2].expr)); (yyval.biop)->rhs.set_raw((yyvsp[0].expr)); }
#line 2467 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 46: /* BinaryOp: Expression CMP_LE Expression  */
#line 345 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.biop), CmpLe);           (yyval.biop)->lhs.set_raw((yyvsp[-2].expr)); (yyval.biop)->rhs.set_raw((yyvsp[0].expr)); }
#line 2473 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 47: /* BinaryOp: Expression '&' Expression  */
#line 346 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.biop), BitwiseAnd);      (yyval.biop)->lhs.set_raw((yyvsp[-2].expr)); (yyval.biop)->rhs.set_raw((yyvsp[0].expr)); }
#line 2479 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 48: /* BinaryOp: Expression '^' Expression  */
#line 347 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.biop), BitwiseXor);      (yyval.biop)->lhs.set_raw((yyvsp[-2].expr)); (yyval.biop)->rhs.set_raw((yyvsp[0].expr)); }
#line 2485 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 49: /* BinaryOp: Expression '|' Expression  */
#line 348 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.biop), BitwiseOr);       (yyval.biop)->lhs.set_raw((yyvsp[-2].expr)); (yyval.biop)->rhs.set_raw((yyvsp[0].expr)); }
#line 2491 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 50: /* BinaryOp: Expression LOGIC_AND Expression  */
#line 349 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.biop), LogicalAnd);      (yyval.biop)->lhs.set_raw((yyvsp[-2].expr)); (yyval.biop)->rhs.set_raw((yyvsp[0].expr)); }
#line 2497 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 51: /* BinaryOp: Expression LOGIC_XOR Expression  */
#line 350 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.biop), LogicalXor);      (yyval.biop)->lhs.set_raw((yyvsp[-2].expr)); (yyval.biop)->rhs.set_raw((yyvsp[0].expr)); }
#line 2503 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 52: /* BinaryOp: Expression LOGIC_OR Expression  */
#line 351 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.biop), LogicalOr);       (yyval.biop)->lhs.set_raw((yyvsp[-2].expr)); (yyval.biop)->rhs.set_raw((yyvsp[0].expr)); }
#line 2509 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 53: /* TernaryOp: Expression '?' Expression ':' Expression  */
#line 354 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.tcnd), TernaryCond);     (yyval.tcnd)->cond.set_raw((yyvsp[-4].expr)); (yyval.tcnd)->if_true.set_raw((yyvsp[-2].expr)); (yyval.tcnd)->if_false.set_raw((yyvsp[0].expr)); }
#line 2515 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 54: /* UnaryOpNP: '-' ExpressionNP  */
#line 359 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.unop), Negate); (yyval.unop)->expr.set_raw((yyvsp[0].expr)); }
#line 2521 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 55: /* UnaryOpNP: '~' ExpressionNP  */
#line 360 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.unop), BitwiseNot); (yyval.unop)->expr.set_raw((yyvsp[0].expr)); }
#line 2527 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 56: /* UnaryOpNP: '!' ExpressionNP  */
#line 361 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.unop), LogicalNot); (yyval.unop)->expr.set_raw((yyvsp[0].expr)); }
#line 2533 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 57: /* BinaryOpNP: ExpressionNP POWER ExpressionNP  */
#line 364 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.biop), Power);           (yyval.biop)->lhs.set_raw((yyvsp[-2].expr)); (yyval.biop)->rhs.set_raw((yyvsp[0].expr)); }
#line 2539 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 58: /* BinaryOpNP: ExpressionNP '*' ExpressionNP  */
#line 365 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.biop), Multiply);        (yyval.biop)->lhs.set_raw((yyvsp[-2].expr)); (yyval.biop)->rhs.set_raw((yyvsp[0].expr)); }
#line 2545 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 59: /* BinaryOpNP: ExpressionNP '/' ExpressionNP  */
#line 366 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.biop), Divide);          (yyval.biop)->lhs.set_raw((yyvsp[-2].expr)); (yyval.biop)->rhs.set_raw((yyvsp[0].expr)); }
#line 2551 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 60: /* BinaryOpNP: ExpressionNP INT_DIV ExpressionNP  */
#line 367 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.biop), IntDivide);       (yyval.biop)->lhs.set_raw((yyvsp[-2].expr)); (yyval.biop)->rhs.set_raw((yyvsp[0].expr)); }
#line 2557 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 61: /* BinaryOpNP: ExpressionNP '%' ExpressionNP  */
#line 368 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.biop), Modulo);          (yyval.biop)->lhs.set_raw((yyvsp[-2].expr)); (yyval.biop)->rhs.set_raw((yyvsp[0].expr)); }
#line 2563 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 62: /* BinaryOpNP: ExpressionNP '+' ExpressionNP  */
#line 369 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.biop), Add);             (yyval.biop)->lhs.set_raw((yyvsp[-2].expr)); (yyval.biop)->rhs.set_raw((yyvsp[0].expr)); }
#line 2569 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 63: /* BinaryOpNP: ExpressionNP '-' ExpressionNP  */
#line 370 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.biop), Subtract);        (yyval.biop)->lhs.set_raw((yyvsp[-2].expr)); (yyval.biop)->rhs.set_raw((yyvsp[0].expr)); }
#line 2575 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 64: /* BinaryOpNP: ExpressionNP SHL ExpressionNP  */
#line 371 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.biop), ShiftLeft);       (yyval.biop)->lhs.set_raw((yyvsp[-2].expr)); (yyval.biop)->rhs.set_raw((yyvsp[0].expr)); }
#line 2581 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 65: /* BinaryOpNP: ExpressionNP ARITH_SHR ExpressionNP  */
#line 372 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.biop), ShiftRightArith); (yyval.biop)->lhs.set_raw((yyvsp[-2].expr)); (yyval.biop)->rhs.set_raw((yyvsp[0].expr)); }
#line 2587 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 66: /* BinaryOpNP: ExpressionNP LOGIC_SHR ExpressionNP  */
#line 373 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.biop), ShiftRightLogic); (yyval.biop)->lhs.set_raw((yyvsp[-2].expr)); (yyval.biop)->rhs.set_raw((yyvsp[0].expr)); }
#line 2593 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 67: /* BinaryOpNP: ExpressionNP CMP_EQ ExpressionNP  */
#line 374 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.biop), CmpEq);           (yyval.biop)->lhs.set_raw((yyvsp[-2].expr)); (yyval.biop)->rhs.set_raw((yyvsp[0].expr)); }
#line 2599 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 68: /* BinaryOpNP: ExpressionNP CMP_NE ExpressionNP  */
#line 375 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.biop), CmpNe);           (yyval.biop)->lhs.set_raw((yyvsp[-2].expr)); (yyval.biop)->rhs.set_raw((yyvsp[0].expr)); }
#line 2605 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 69: /* BinaryOpNP: ExpressionNP '>' ExpressionNP  */
#line 376 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.biop), CmpGt);           (yyval.biop)->lhs.set_raw((yyvsp[-2].expr)); (yyval.biop)->rhs.set_raw((yyvsp[0].expr)); }
#line 2611 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 70: /* BinaryOpNP: ExpressionNP CMP_GE ExpressionNP  */
#line 377 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.biop), CmpGe);           (yyval.biop)->lhs.set_raw((yyvsp[-2].expr)); (yyval.biop)->rhs.set_raw((yyvsp[0].expr)); }
#line 2617 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 71: /* BinaryOpNP: ExpressionNP '<' ExpressionNP  */
#line 378 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.biop), CmpLt);           (yyval.biop)->lhs.set_raw((yyvsp[-2].expr)); (yyval.biop)->rhs.set_raw((yyvsp[0].expr)); }
#line 2623 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 72: /* BinaryOpNP: ExpressionNP CMP_LE ExpressionNP  */
#line 379 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.biop), CmpLe);           (yyval.biop)->lhs.set_raw((yyvsp[-2].expr)); (yyval.biop)->rhs.set_raw((yyvsp[0].expr)); }
#line 2629 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 73: /* BinaryOpNP: ExpressionNP '&' ExpressionNP  */
#line 380 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.biop), BitwiseAnd);      (yyval.biop)->lhs.set_raw((yyvsp[-2].expr)); (yyval.biop)->rhs.set_raw((yyvsp[0].expr)); }
#line 2635 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 74: /* BinaryOpNP: ExpressionNP '^' ExpressionNP  */
#line 381 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.biop), BitwiseXor);      (yyval.biop)->lhs.set_raw((yyvsp[-2].expr)); (yyval.biop)->rhs.set_raw((yyvsp[0].expr)); }
#line 2641 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 75: /* BinaryOpNP: ExpressionNP LOGIC_AND ExpressionNP  */
#line 382 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.biop), LogicalAnd);      (yyval.biop)->lhs.set_raw((yyvsp[-2].expr)); (yyval.biop)->rhs.set_raw((yyvsp[0].expr)); }
#line 2647 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 76: /* BinaryOpNP: ExpressionNP LOGIC_XOR ExpressionNP  */
#line 383 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.biop), LogicalXor);      (yyval.biop)->lhs.set_raw((yyvsp[-2].expr)); (yyval.biop)->rhs.set_raw((yyvsp[0].expr)); }
#line 2653 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 77: /* BinaryOpNP: ExpressionNP LOGIC_OR ExpressionNP  */
#line 384 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.biop), LogicalOr);       (yyval.biop)->lhs.set_raw((yyvsp[-2].expr)); (yyval.biop)->rhs.set_raw((yyvsp[0].expr)); }
#line 2659 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 78: /* TernaryOpNP: ExpressionNP '?' ExpressionNP ':' ExpressionNP  */
#line 387 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.tcnd), TernaryCond);     (yyval.tcnd)->cond.set_raw((yyvsp[-4].expr)); (yyval.tcnd)->if_true.set_raw((yyvsp[-2].expr)); (yyval.tcnd)->if_false.set_raw((yyvsp[0].expr)); }
#line 2665 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 79: /* Expression: IntegerLiteral  */
#line 391 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.expr), (yyvsp[0].ilit)); }
#line 2671 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 80: /* Expression: FloatLiteral  */
#line 392 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.expr), (yyvsp[0].flit)); }
#line 2677 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 81: /* Expression: MatrixLiteral  */
#line 393 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.expr), (yyvsp[0].mat)); }
#line 2683 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 82: /* Expression: StringLiteral  */
#line 394 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.expr), (yyvsp[0].slit)); }
#line 2689 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 83: /* Expression: JsonLiteral  */
#line 395 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.expr), (yyvsp[0].jlit)); }
#line 2695 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 84: /* Expression: Identifier  */
#line 396 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.expr), (yyvsp[0].idnt)); }
#line 2701 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 85: /* Expression: FunctionCall  */
#line 397 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.expr), (yyvsp[0].func)); }
#line 2707 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 86: /* Expression: Index  */
#line 398 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.expr), (yyvsp[0].indx)); }
#line 2713 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 87: /* Expression: UnaryOp  */
#line 399 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.expr), (yyvsp[0].unop)); }
#line 2719 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 88: /* Expression: BinaryOp  */
#line 400 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.expr), (yyvsp[0].biop)); }
#line 2725 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 89: /* Expression: TernaryOp  */
#line 401 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.expr), (yyvsp[0].tcnd)); }
#line 2731 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 90: /* Expression: '(' Expression ')'  */
#line 402 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.expr), (yyvsp[-1].expr)); }
#line 2737 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 91: /* Expression: error  */
#line 403 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.expr), ErroneousExpression); }
#line 2743 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 92: /* ExpressionNP: IntegerLiteral  */
#line 406 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.expr), (yyvsp[0].ilit)); }
#line 2749 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 93: /* ExpressionNP: FloatLiteral  */
#line 407 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.expr), (yyvsp[0].flit)); }
#line 2755 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 94: /* ExpressionNP: MatrixLiteral  */
#line 408 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.expr), (yyvsp[0].mat)); }
#line 2761 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 95: /* ExpressionNP: StringLiteral  */
#line 409 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.expr), (yyvsp[0].slit)); }
#line 2767 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 96: /* ExpressionNP: JsonLiteral  */
#line 410 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.expr), (yyvsp[0].jlit)); }
#line 2773 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 97: /* ExpressionNP: Identifier  */
#line 411 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.expr), (yyvsp[0].idnt)); }
#line 2779 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 98: /* ExpressionNP: FunctionCall  */
#line 412 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.expr), (yyvsp[0].func)); }
#line 2785 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 99: /* ExpressionNP: IndexNP  */
#line 413 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.expr), (yyvsp[0].indx)); }
#line 2791 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 100: /* ExpressionNP: UnaryOpNP  */
#line 414 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.expr), (yyvsp[0].unop)); }
#line 2797 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 101: /* ExpressionNP: BinaryOpNP  */
#line 415 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.expr), (yyvsp[0].biop)); }
#line 2803 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 102: /* ExpressionNP: TernaryOpNP  */
#line 416 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.expr), (yyvsp[0].tcnd)); }
#line 2809 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 103: /* ExpressionNP: '(' Expression ')'  */
#line 417 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.expr), (yyvsp[-1].expr)); }
#line 2815 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 104: /* ExpressionNP: error  */
#line 418 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.expr), ErroneousExpression); }
#line 2821 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 105: /* ExpressionList: ExpressionList ',' Expression  */
#line 422 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.expl), (yyvsp[-2].expl)); (yyval.expl)->items.add_raw((yyvsp[0].expr)); }
#line 2827 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 106: /* ExpressionList: Expression  */
#line 423 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.expl), ExpressionList); (yyval.expl)->items.add_raw((yyvsp[0].expr)); }
#line 2833 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 107: /* ExpressionListNP: ExpressionListNP ',' ExpressionNP  */
#line 426 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.expl), (yyvsp[-2].expl)); (yyval.expl)->items.add_raw((yyvsp[0].expr)); }
#line 2839 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 108: /* ExpressionListNP: ExpressionNP  */
#line 427 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.expl), ExpressionList); (yyval.expl)->items.add_raw((yyvsp[0].expr)); }
#line 2845 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 109: /* AnnotationName: Identifier '.' Identifier  */
#line 431 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.adat), AnnotationData); (yyval.adat)->interface.set_raw((yyvsp[-2].idnt)); (yyval.adat)->operation.set_raw((yyvsp[0].idnt)); }
#line 2851 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 110: /* AnnotationData: AnnotationName  */
#line 434 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.adat), (yyvsp[0].adat)); (yyval.adat)->operands.set_raw(new ExpressionList()); }
#line 2857 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 111: /* AnnotationData: AnnotationName '(' ')'  */
#line 435 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.adat), (yyvsp[-2].adat)); (yyval.adat)->operands.set_raw(new ExpressionList()); }
#line 2863 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 112: /* AnnotationData: AnnotationName '(' ExpressionList ')'  */
#line 436 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.adat), (yyvsp[-3].adat)); (yyval.adat)->operands.set_raw((yyvsp[-1].expl)); }
#line 2869 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 113: /* Instruction: Identifier  */
#line 441 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                {
                                                                                    NEW((yyval.inst), Instruction);
                                                                                    (yyval.inst)->name.set_raw((yyvsp[0].idnt));
                                                                                    (yyval.inst)->operands.set_raw(new ExpressionList());
                                                                                }
#line 2879 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 114: /* Instruction: Identifier ExpressionListNP  */
#line 446 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                {
                                                                                    NEW((yyval.inst), Instruction);
                                                                                    (yyval.inst)->name.set_raw((yyvsp[-1].idnt));
                                                                                    (yyval.inst)->operands.set_raw((yyvsp[0].expl));
                                                                                }
#line 2889 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 115: /* Instruction: SET ExpressionNP '=' ExpressionNP  */
#line 451 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                {
                                                                                    NEW((yyval.inst), Instruction);
                                                                                    (yyval.inst)->name.set_raw(new Identifier());
                                                                                    (yyval.inst)->name->name = "set";
                                                                                    (yyval.inst)->operands.set_raw(new ExpressionList());
                                                                                    (yyval.inst)->operands->items.add_raw((yyvsp[-2].expr));
                                                                                    (yyval.inst)->operands->items.add_raw((yyvsp[0].expr));
                                                                                }
#line 2902 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 116: /* Instruction: CDASH Identifier ExpressionNP  */
#line 459 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                {
                                                                                    NEW((yyval.inst), Instruction);
                                                                                    (yyval.inst)->name.set_raw((yyvsp[-1].idnt));
                                                                                    (yyval.inst)->condition.set_raw((yyvsp[0].expr));
                                                                                    (yyval.inst)->operands.set_raw(new ExpressionList());
                                                                                }
#line 2913 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 117: /* Instruction: CDASH Identifier ExpressionNP ',' ExpressionListNP  */
#line 465 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                {
                                                                                    NEW((yyval.inst), Instruction);
                                                                                    (yyval.inst)->name.set_raw((yyvsp[-3].idnt));
                                                                                    (yyval.inst)->condition.set_raw((yyvsp[-2].expr));
                                                                                    (yyval.inst)->operands.set_raw((yyvsp[0].expl));
                                                                                }
#line 2924 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 118: /* Instruction: COND '(' Expression ')' Identifier ExpressionListNP  */
#line 471 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                {
                                                                                    NEW((yyval.inst), Instruction);
                                                                                    (yyval.inst)->name.set_raw((yyvsp[-1].idnt));
                                                                                    (yyval.inst)->condition.set_raw((yyvsp[-3].expr));
                                                                                    (yyval.inst)->operands.set_raw((yyvsp[0].expl));
                                                                                }
#line 2935 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 119: /* AnnotInstr: AnnotInstr '@' AnnotationData  */
#line 482 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.inst), (yyvsp[-2].inst)); (yyval.inst)->annotations.add_raw((yyvsp[0].adat)); }
#line 2941 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 120: /* AnnotInstr: Instruction  */
#line 483 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.inst), (yyvsp[0].inst)); }
#line 2947 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 121: /* SLParInstrList: SLParInstrList '|' AnnotInstr  */
#line 487 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.bun), (yyvsp[-2].bun)); (yyval.bun)->items.add_raw((yyvsp[0].inst)); }
#line 2953 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 122: /* SLParInstrList: AnnotInstr  */
#line 488 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.bun), Bundle); (yyval.bun)->items.add_raw((yyvsp[0].inst)); }
#line 2959 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 123: /* CBParInstrList: CBParInstrList Newline SLParInstrList  */
#line 492 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.bun), (yyvsp[-2].bun)); (yyval.bun)->items.extend((yyvsp[0].bun)->items); delete (yyvsp[0].bun); }
#line 2965 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 124: /* CBParInstrList: SLParInstrList  */
#line 493 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.bun), (yyvsp[0].bun)); }
#line 2971 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 125: /* Mapping: MAP Expression ',' Identifier  */
#line 497 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.map), Mapping); (yyval.map)->expr.set_raw((yyvsp[-2].expr)); (yyval.map)->alias.set_raw((yyvsp[0].idnt)); }
#line 2977 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 126: /* Mapping: MAP Identifier '=' Expression  */
#line 498 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.map), Mapping); (yyval.map)->alias.set_raw((yyvsp[-2].idnt)); (yyval.map)->expr.set_raw((yyvsp[0].expr)); }
#line 2983 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 127: /* VariableBody: Identifier ':' Identifier  */
#line 502 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.vars), Variables); (yyval.vars)->names.add_raw((yyvsp[-2].idnt)); (yyval.vars)->typ.set_raw((yyvsp[0].idnt)); }
#line 2989 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 128: /* VariableBody: Identifier ',' VariableBody  */
#line 503 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.vars), (yyvsp[0].vars)); (yyval.vars)->names.add_raw((yyvsp[-2].idnt)); }
#line 2995 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 129: /* Variable: VAR VariableBody  */
#line 506 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.vars), (yyvsp[0].vars)); }
#line 3001 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 130: /* Subcircuit: '.' Identifier  */
#line 510 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.sub), Subcircuit); (yyval.sub)->name.set_raw((yyvsp[0].idnt)); }
#line 3007 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 131: /* Subcircuit: '.' Identifier '(' Expression ')'  */
#line 511 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.sub), Subcircuit); (yyval.sub)->name.set_raw((yyvsp[-3].idnt)); (yyval.sub)->iterations.set_raw((yyvsp[-1].expr)); }
#line 3013 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 132: /* SubStatements: '{' OptNewline StatementList OptNewline '}'  */
#line 515 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.stms), (yyvsp[-2].stms)); }
#line 3019 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 133: /* SubStatements: '{' OptNewline '}'  */
#line 516 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.stms), StatementList); }
#line 3025 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 134: /* Assignment: Expression '=' Expression  */
#line 519 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.asgn), Assignment); (yyval.asgn)->lhs.set_raw((yyvsp[-2].expr)); (yyval.asgn)->rhs.set_raw((yyvsp[0].expr)); }
#line 3031 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 135: /* OptAssignment: Assignment  */
#line 522 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.asgn), (yyvsp[0].asgn)); }
#line 3037 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 136: /* OptAssignment: %empty  */
#line 523 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { (yyval.asgn) = nullptr; }
#line 3043 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 137: /* IfElse: IF '(' Expression ')' SubStatements  */
#line 526 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                {
                                                                                    NEW((yyval.ifel), IfElse);
                                                                                    (yyval.ifel)->branches.add_raw(new IfElseBranch());
                                                                                    (yyval.ifel)->branches[0]->condition.set_raw((yyvsp[-2].expr));
                                                                                    (yyval.ifel)->branches[0]->body.set_raw((yyvsp[0].stms));
                                                                                }
#line 3054 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 138: /* IfElse: IF '(' Expression ')' SubStatements ELSE IfElse  */
#line 532 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                {
                                                                                    FROM((yyval.ifel), (yyvsp[0].ifel));
                                                                                    (yyval.ifel)->branches.add_raw(new IfElseBranch(), 0);
                                                                                    (yyval.ifel)->branches[0]->condition.set_raw((yyvsp[-4].expr));
                                                                                    (yyval.ifel)->branches[0]->body.set_raw((yyvsp[-2].stms));
                                                                                }
#line 3065 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 139: /* IfElse: IF '(' Expression ')' SubStatements ELSE SubStatements  */
#line 538 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                {
                                                                                    NEW((yyval.ifel), IfElse);
                                                                                    (yyval.ifel)->branches.add_raw(new IfElseBranch());
                                                                                    (yyval.ifel)->branches[0]->condition.set_raw((yyvsp[-4].expr));
                                                                                    (yyval.ifel)->branches[0]->body.set_raw((yyvsp[-2].stms));
                                                                                    (yyval.ifel)->otherwise.set_raw((yyvsp[0].stms));
                                                                                }
#line 3077 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 140: /* ForLoop: FOR '(' OptAssignment NEWLINE Expression NEWLINE OptAssignment ')' SubStatements  */
#line 548 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                {
                                                                                    NEW((yyval.forl), ForLoop);
                                                                                    if ((yyvsp[-6].asgn)) (yyval.forl)->initialize.set_raw((yyvsp[-6].asgn));
                                                                                    (yyval.forl)->condition.set_raw((yyvsp[-4].expr));
                                                                                    if ((yyvsp[-2].asgn)) (yyval.forl)->update.set_raw((yyvsp[-2].asgn));
                                                                                    (yyval.forl)->body.set_raw((yyvsp[0].stms));
                                                                                }
#line 3089 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 141: /* ForeachLoop: FOREACH '(' Expression '=' Expression ELLIPSIS Expression ')' SubStatements  */
#line 558 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                {
                                                                                    NEW((yyval.fore), ForeachLoop);
                                                                                    (yyval.fore)->lhs.set_raw((yyvsp[-6].expr));
                                                                                    (yyval.fore)->frm.set_raw((yyvsp[-4].expr));
                                                                                    (yyval.fore)->to.set_raw((yyvsp[-2].expr));
                                                                                    (yyval.fore)->body.set_raw((yyvsp[0].stms));
                                                                                }
#line 3101 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 142: /* WhileLoop: WHILE '(' Expression ')' SubStatements  */
#line 567 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.whil), WhileLoop); (yyval.whil)->condition.set_raw((yyvsp[-2].expr)); (yyval.whil)->body.set_raw((yyvsp[0].stms)); }
#line 3107 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 143: /* RepeatUntilLoop: REPEAT SubStatements UNTIL '(' Expression ')'  */
#line 570 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.repu), RepeatUntilLoop); (yyval.repu)->body.set_raw((yyvsp[-4].stms)); (yyval.repu)->condition.set_raw((yyvsp[-1].expr)); }
#line 3113 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 144: /* Continue: CONTINUE  */
#line 573 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.cont), ContinueStatement); }
#line 3119 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 145: /* Break: BREAK  */
#line 576 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.brk), BreakStatement); }
#line 3125 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 146: /* Statement: Mapping  */
#line 580 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.stmt), (yyvsp[0].map)); }
#line 3131 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 147: /* Statement: Variable  */
#line 581 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.stmt), (yyvsp[0].vars)); }
#line 3137 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 148: /* Statement: Subcircuit  */
#line 582 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.stmt), (yyvsp[0].sub)); }
#line 3143 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 149: /* Statement: SLParInstrList  */
#line 583 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.stmt), (yyvsp[0].bun)); }
#line 3149 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 150: /* Statement: '{' OptNewline CBParInstrList OptNewline '}'  */
#line 584 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.stmt), (yyvsp[-2].bun)); }
#line 3155 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 151: /* Statement: IfElse  */
#line 585 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.stmt), (yyvsp[0].ifel)); }
#line 3161 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 152: /* Statement: ForLoop  */
#line 586 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.stmt), (yyvsp[0].forl)); }
#line 3167 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 153: /* Statement: ForeachLoop  */
#line 587 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.stmt), (yyvsp[0].fore)); }
#line 3173 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 154: /* Statement: WhileLoop  */
#line 588 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.stmt), (yyvsp[0].whil)); }
#line 3179 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 155: /* Statement: RepeatUntilLoop  */
#line 589 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.stmt), (yyvsp[0].repu)); }
#line 3185 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 156: /* Statement: Continue  */
#line 590 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.stmt), (yyvsp[0].cont)); }
#line 3191 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 157: /* Statement: Break  */
#line 591 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.stmt), (yyvsp[0].brk)); }
#line 3197 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 158: /* Statement: error  */
#line 592 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.stmt), ErroneousStatement); }
#line 3203 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 159: /* AnnotStatement: AnnotStatement '@' AnnotationData  */
#line 599 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.stmt), (yyvsp[-2].stmt)); (yyval.stmt)->annotations.add_raw((yyvsp[0].adat)); }
#line 3209 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 160: /* AnnotStatement: Statement  */
#line 600 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.stmt), (yyvsp[0].stmt)); }
#line 3215 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 161: /* StatementList: StatementList Newline AnnotStatement  */
#line 604 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.stms), (yyvsp[-2].stms)); (yyval.stms)->items.add_raw((yyvsp[0].stmt)); }
#line 3221 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 162: /* StatementList: AnnotStatement  */
#line 605 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.stms), StatementList); (yyval.stms)->items.add_raw((yyvsp[0].stmt)); }
#line 3227 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 163: /* Statements: Newline StatementList OptNewline  */
#line 609 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.stms), (yyvsp[-1].stms)); }
#line 3233 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 164: /* Statements: OptNewline  */
#line 610 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.stms), StatementList); }
#line 3239 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 165: /* Version: Version '.' IntegerLiteral  */
#line 614 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { FROM((yyval.vers), (yyvsp[-2].vers)); (yyval.vers)->items.push_back((yyvsp[0].ilit)->value); delete (yyvsp[0].ilit); }
#line 3245 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 166: /* Version: IntegerLiteral  */
#line 615 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.vers), Version); (yyval.vers)->items.clear(); (yyval.vers)->items.push_back((yyvsp[0].ilit)->value); delete (yyvsp[0].ilit); }
#line 3251 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 167: /* Program: OptNewline VERSION Version Newline QUBITS Expression Statements  */
#line 621 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.prog), Program); (yyval.prog)->version.set_raw((yyvsp[-4].vers)); (yyval.prog)->num_qubits.set_raw((yyvsp[-1].expr)); (yyval.prog)->statements.set_raw((yyvsp[0].stms)); }
#line 3257 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 168: /* Program: OptNewline VERSION Version Statements  */
#line 623 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { NEW((yyval.prog), Program); (yyval.prog)->version.set_raw((yyvsp[-1].vers)); (yyval.prog)->statements.set_raw((yyvsp[0].stms)); }
#line 3263 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 169: /* Root: Program  */
#line 627 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { helper.result.root.set_raw((yyvsp[0].prog)); }
#line 3269 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;

  case 170: /* Root: error  */
#line 628 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"
                                                                                { helper.result.root.set_raw(new ErroneousProgram()); }
#line 3275 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"
    break;


#line 3279 "/mnt/c/Projects/libqasm/build/Debug/src/v1/cqasm-parser.cpp"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == CQASM_V1_EMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yytoken, &yylloc};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (&yylloc, scanner, helper, yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
    }

  yyerror_range[1] = yylloc;
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= CQASM_V1_EOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == CQASM_V1_EOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc, scanner, helper);
          yychar = CQASM_V1_EMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp, scanner, helper);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, scanner, helper, YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != CQASM_V1_EMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc, scanner, helper);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp, scanner, helper);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 631 "/mnt/c/Projects/libqasm/src/v1/cqasm-parser.y"


void yyerror(YYLTYPE* yyllocp, yyscan_t unused, cqasm::v1::parser::ParseHelper &helper, const char* msg) {
    (void)unused;
    std::ostringstream sb;
    sb << helper.filename
       << ":"  << yyllocp->first_line
       << ":"  << yyllocp->first_column
       << ": " << msg;
    helper.push_error(sb.str());
}
