/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 3 "library/grammar.y" /* yacc.c:339  */

    #include <math.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <iostream>
    #include <vector>
    #include <string>
    #include "qasm_ast.hpp"
    int yylex(void);
    void yyerror (char const *);
    compiler::Bits bits_identified;
    compiler::NumericalIdentifiers buffer_indices;
    compiler::SubCircuits subcircuits_object;
    compiler::QasmRepresentation qasm_representation;
    std::string buffer_gate;

#line 83 "grammar.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "grammar.tab.h".  */
#ifndef YY_YY_GRAMMAR_TAB_H_INCLUDED
# define YY_YY_GRAMMAR_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 1 "library/grammar.y" /* yacc.c:355  */
 #include "qasm_ast.hpp" 

#line 116 "grammar.tab.c" /* yacc.c:355  */

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
    COMMENT = 274,
    ROTATIONS = 275,
    AXIS = 276,
    QUBITS = 277,
    SINGLE_QUBIT_GATES = 278,
    TWO_QUBIT_GATES = 279,
    CR = 280,
    TOFFOLI = 281,
    CDASH = 282,
    NOT_TOKEN = 283,
    MAPKEY = 284,
    PREP = 285,
    MEASURE = 286,
    MEASUREPARITY = 287,
    MEASUREALL = 288,
    WAIT = 289,
    DISPLAY = 290,
    RESET_AVERAGING = 291,
    QBITHEAD = 292,
    BITHEAD = 293
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 21 "library/grammar.y" /* yacc.c:355  */

    int ival;
    double dval;
    char* sval;
    compiler::Qubits* qval;
    compiler::Bits* bval;

#line 175 "grammar.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_GRAMMAR_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 192 "grammar.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

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

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
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


#if ! defined yyoverflow || YYERROR_VERBOSE

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
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  7
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   297

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  39
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  48
/* YYNRULES -- Number of rules.  */
#define YYNRULES  102
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  214

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   293

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
      35,    36,    37,    38
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    48,    48,    49,    51,    52,    53,    54,    56,    57,
      58,    59,    61,    62,    64,    65,    66,    67,    68,    69,
      71,    72,    73,    74,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    94,    95,
      97,    99,   100,   102,   103,   105,   109,   113,   117,   118,
     125,   136,   137,   144,   156,   160,   165,   170,   174,   180,
     180,   182,   185,   185,   187,   192,   193,   197,   202,   205,
     207,   210,   212,   215,   216,   217,   218,   219,   229,   230,
     231,   232,   233,   234,   236,   238,   240,   242,   244,   246,
     250,   252,   252,   254,   254,   258,   258,   258,   260,   260,
     262,   264,   264
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "NAME", "INTEGER", "FLOAT",
  "COMMA_SEPARATOR", "PARALLEL_SEPARATOR", "BRA", "KET", "DOT", "SBRA",
  "SKET", "CBRA", "CKET", "LS_SEP", "NEWLINE", "WS", "COLON", "COMMENT",
  "ROTATIONS", "AXIS", "QUBITS", "SINGLE_QUBIT_GATES", "TWO_QUBIT_GATES",
  "CR", "TOFFOLI", "CDASH", "NOT_TOKEN", "MAPKEY", "PREP", "MEASURE",
  "MEASUREPARITY", "MEASUREALL", "WAIT", "DISPLAY", "RESET_AVERAGING",
  "QBITHEAD", "BITHEAD", "$accept", "qasm-file", "circuits", "circuit",
  "subcircuit", "statements", "comments", "qasm-line", "line-separator",
  "indices", "numerical-identifiers", "numerical-identifier-list",
  "numerical-identifier-range", "qubit-register", "qubit", "qubit-nomap",
  "bit", "bit-nomap", "single-qubit-operation",
  "single-qubit-operation-args", "map-operation", "single-qubit-gate",
  "parameterized-single-qubit-gate", "prep_measure-ops",
  "measure-parity-operation", "measure-parity-command",
  "measureall-operation", "two-qubit-operation",
  "two-qubit-operation-args", "two-qubit-gates", "two-qubit-gate-args",
  "toffoli-operation", "toffoli-gate", "regular-operations",
  "binary-controlled-operations", "bit-single-qubit-operation",
  "bit-single-qubit-operation-args", "bit-two-qubit-operation",
  "bit-two-qubit-operation-args", "bit-toffoli-operation",
  "negate-binary-operation", "parallel-operations", "all-valid-operations",
  "parallelizable-ops", "special-operations", "display-operation",
  "wait-operation", "reset-averaging-operation", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293
};
# endif

#define YYPACT_NINF -121

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-121)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      13,  -121,    38,    30,     9,    51,    34,  -121,  -121,    51,
      23,    51,   118,  -121,    55,  -121,  -121,   118,    70,   257,
     199,  -121,  -121,  -121,  -121,  -121,  -121,    27,    58,    64,
    -121,  -121,  -121,  -121,    68,  -121,  -121,   145,  -121,    91,
      51,    25,    51,  -121,  -121,  -121,    69,    71,    73,  -121,
      77,  -121,  -121,  -121,    79,    80,  -121,    81,  -121,  -121,
    -121,  -121,  -121,  -121,  -121,  -121,  -121,  -121,    88,  -121,
      93,   145,  -121,  -121,  -121,  -121,    22,   223,    91,    51,
    -121,  -121,  -121,  -121,  -121,  -121,  -121,    96,   112,   113,
     115,   116,    16,    19,   132,  -121,    25,   136,    18,    51,
      55,   172,    55,    21,    21,    21,    21,    21,    21,    21,
      16,    21,   257,  -121,  -121,  -121,  -121,  -121,  -121,  -121,
    -121,    51,    16,    16,    16,    16,    16,  -121,   146,  -121,
    -121,   146,   153,   154,  -121,   147,   155,  -121,  -121,  -121,
    -121,   161,  -121,   177,   178,   180,   181,  -121,  -121,  -121,
     184,   185,   188,   204,   205,   157,  -121,  -121,   210,   211,
    -121,   212,   194,    21,    21,    21,    21,    21,    21,    21,
      21,   200,   209,   231,   232,  -121,  -121,  -121,   233,  -121,
     234,   235,  -121,   236,   239,   255,   256,   259,  -121,   157,
     157,    21,   260,    21,   261,    21,    21,    21,  -121,  -121,
    -121,   262,  -121,  -121,  -121,  -121,   263,   264,   244,   267,
      21,  -121,  -121,  -121
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    20,     0,     0,     0,     0,     0,     1,    38,    21,
       0,     0,     0,    47,    23,    39,    22,     0,     0,     0,
       0,    61,    59,    60,    69,    70,    72,     0,     0,     0,
      62,    63,    65,    66,     0,    98,   101,     2,     4,    15,
       9,     6,    14,    73,    74,    24,     0,     0,     0,    26,
       0,    25,    75,    76,     0,     0,    77,     0,    27,    28,
      78,    79,    80,    81,    82,    83,    29,    30,    95,    96,
      97,     3,    12,    91,    92,    93,     0,     0,    15,    11,
      24,    26,    25,    27,    28,    29,    30,     0,     0,     0,
       0,     0,     0,     0,     0,     5,     7,     0,    15,     8,
      17,     0,    16,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    90,    31,    33,    32,    34,    35,    36,
      37,    10,     0,     0,     0,     0,     0,    52,     0,    89,
      51,     0,     0,     0,   100,     0,    19,    18,    49,    54,
      48,     0,    55,     0,     0,     0,     0,    99,   102,    94,
       0,     0,     0,     0,     0,     0,    53,    50,     0,     0,
      13,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    43,     0,    41,    42,    57,    58,    56,     0,    67,
       0,     0,    84,     0,     0,     0,     0,     0,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    45,    44,
      46,     0,    68,    71,    85,    86,     0,     0,     0,     0,
       0,    64,    87,    88
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -121,  -121,   250,   -31,    -6,    -2,    12,   189,     4,   148,
    -120,  -121,  -121,   282,  -104,   196,   150,   198,  -121,  -121,
       0,   265,   266,  -121,     1,  -121,     2,  -121,  -121,   268,
     269,  -121,   270,    -9,    -3,  -121,  -121,  -121,  -121,  -121,
    -121,     3,   182,  -121,     7,  -121,  -121,  -121
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,    37,    38,    39,    40,    41,    42,   101,   156,
     172,   173,   174,     5,   139,   140,   129,   130,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    75,    76,    67,    68,    69,    70
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
     141,   142,   143,   144,   145,   146,    95,   148,    10,    12,
      73,    83,     4,    14,    78,    17,    74,    84,    79,   127,
      80,    81,    82,    85,   138,     8,    97,    86,     9,   112,
       7,     2,     1,    98,     8,     2,   113,    99,    13,    15,
      95,     8,    16,   100,     9,    10,   102,    21,    22,    96,
      23,    24,    25,    26,   128,     6,   131,   128,   131,   179,
     180,   181,   182,   183,   184,   185,   186,     8,   117,   199,
     200,    15,    98,    72,   118,    92,   121,   114,   115,   116,
     119,    93,   100,    96,   120,    94,   103,   201,   104,   203,
     105,   205,   206,   207,   106,   136,   107,   108,   109,    97,
      10,    18,   100,    73,    19,   110,   213,     8,    77,    74,
     111,    21,    22,   122,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    18,   123,
     124,    19,   125,   126,    15,    20,   134,     1,    21,    22,
     135,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    18,   160,   155,    19,   158,
     159,   171,    20,    97,     1,    21,    22,   161,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    18,   162,   163,    19,   164,   165,    15,    77,
     166,   167,    21,    22,   168,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    18,
     169,   170,    19,   175,   176,   178,    77,   177,   187,    21,
      22,   188,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    19,   189,   190,   191,
     192,   193,   194,    21,    22,   195,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
     147,   196,   197,   198,   202,   211,   204,    71,   208,   209,
     210,   212,   150,   151,   152,   153,   154,    21,    22,   157,
      23,    24,    25,    26,    27,    28,    11,    30,    31,   132,
     137,   133,    87,    88,   149,    89,    90,    91
};

static const yytype_uint8 yycheck[] =
{
     104,   105,   106,   107,   108,   109,    37,   111,     4,     5,
      19,    20,     0,     9,    20,    11,    19,    20,    20,     3,
      20,    20,    20,    20,     3,    16,     8,    20,    19,     7,
       0,    22,    19,    39,    16,    22,    14,    39,     4,    16,
      71,    16,    19,    39,    19,    41,    42,    20,    21,    37,
      23,    24,    25,    26,    38,    17,    37,    38,    37,   163,
     164,   165,   166,   167,   168,   169,   170,    16,    77,   189,
     190,    16,    78,     3,    77,    17,    78,    77,    77,    77,
      77,    17,    78,    71,    77,    17,    17,   191,    17,   193,
      17,   195,   196,   197,    17,   101,    17,    17,    17,     8,
      96,    10,    98,   112,    13,    17,   210,    16,    17,   112,
      17,    20,    21,    17,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    10,    17,
      17,    13,    17,    17,    16,    17,     4,    19,    20,    21,
       4,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    10,     9,    11,    13,     6,
       6,     4,    17,     8,    19,    20,    21,     6,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    10,     6,     6,    13,     6,     6,    16,    17,
       6,     6,    20,    21,     6,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    10,
       6,     6,    13,     3,     3,    21,    17,     5,    18,    20,
      21,    12,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    13,     6,     6,     6,
       6,     6,     6,    20,    21,     6,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
     110,     6,     6,     4,     4,    21,     5,    17,     6,     6,
       6,     4,   122,   123,   124,   125,   126,    20,    21,   131,
      23,    24,    25,    26,    27,    28,     4,    30,    31,    93,
     101,    93,    27,    27,   112,    27,    27,    27
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    19,    22,    40,    45,    52,    17,     0,    16,    19,
      47,    52,    47,     4,    47,    16,    19,    47,    10,    13,
      17,    20,    21,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    41,    42,    43,
      44,    45,    46,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    83,    84,    85,
      86,    41,     3,    72,    73,    81,    82,    17,    43,    44,
      59,    63,    65,    72,    73,    80,    83,    60,    61,    68,
      69,    71,    17,    17,    17,    42,    45,     8,    43,    44,
      47,    47,    47,    17,    17,    17,    17,    17,    17,    17,
      17,    17,     7,    14,    59,    63,    65,    72,    73,    80,
      83,    44,    17,    17,    17,    17,    17,     3,    38,    55,
      56,    37,    54,    56,     4,     4,    43,    46,     3,    53,
      54,    53,    53,    53,    53,    53,    53,    55,    53,    81,
      55,    55,    55,    55,    55,    11,    48,    48,     6,     6,
       9,     6,     6,     6,     6,     6,     6,     6,     6,     6,
       6,     4,    49,    50,    51,     3,     3,     5,    21,    53,
      53,    53,    53,    53,    53,    53,    53,    18,    12,     6,
       6,     6,     6,     6,     6,     6,     6,     6,     4,    49,
      49,    53,     4,    53,     5,    53,    53,    53,     6,     6,
       6,    21,     4,    53
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    39,    40,    40,    41,    41,    41,    41,    42,    42,
      42,    42,    43,    43,    44,    44,    44,    44,    44,    44,
      45,    45,    45,    45,    46,    46,    46,    46,    46,    46,
      46,    46,    46,    46,    46,    46,    46,    46,    47,    47,
      48,    49,    49,    50,    50,    51,    51,    52,    53,    53,
      54,    55,    55,    56,    57,    57,    58,    59,    59,    60,
      60,    61,    62,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    72,    72,    72,    72,    73,    73,
      73,    73,    73,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    81,    82,    82,    83,    83,    83,    84,    84,
      85,    86,    86
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     3,     4,     1,     2,     1,     2,     2,     1,
       3,     2,     2,     4,     1,     1,     2,     2,     3,     3,
       1,     2,     3,     3,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     2,     2,     2,     2,     2,     1,     2,
       3,     1,     1,     1,     3,     3,     3,     3,     1,     1,
       2,     1,     1,     2,     3,     3,     5,     5,     5,     1,
       1,     1,     1,     1,     9,     1,     1,     5,     7,     1,
       1,     7,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     6,     8,     8,    10,    10,     3,
       3,     1,     1,     1,     3,     1,     1,     1,     1,     3,
       3,     1,     3
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


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


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
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
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
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
            /* Fall through.  */
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

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
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
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
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
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
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
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

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

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
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

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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
| yyreduce -- Do a reduction.  |
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


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 48 "library/grammar.y" /* yacc.c:1646  */
    {qasm_representation.getSubCircuits() = subcircuits_object;}
#line 1444 "grammar.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 49 "library/grammar.y" /* yacc.c:1646  */
    {qasm_representation.getSubCircuits() = subcircuits_object;}
#line 1450 "grammar.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 61 "library/grammar.y" /* yacc.c:1646  */
    { subcircuits_object.addSubCircuit( compiler::SubCircuit ((yyvsp[0].sval),subcircuits_object.numberOfSubCircuits()) ); }
#line 1456 "grammar.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 62 "library/grammar.y" /* yacc.c:1646  */
    { subcircuits_object.lastSubCircuit().numberIterations((yyvsp[-1].ival)); }
#line 1462 "grammar.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 97 "library/grammar.y" /* yacc.c:1646  */
    {}
#line 1468 "grammar.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 99 "library/grammar.y" /* yacc.c:1646  */
    {}
#line 1474 "grammar.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 100 "library/grammar.y" /* yacc.c:1646  */
    {}
#line 1480 "grammar.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 102 "library/grammar.y" /* yacc.c:1646  */
    {buffer_indices.addToVector((yyvsp[0].ival));}
#line 1486 "grammar.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 106 "library/grammar.y" /* yacc.c:1646  */
    {
                                buffer_indices.addToVector((yyvsp[-2].ival),(yyvsp[0].ival)); 
                             }
#line 1494 "grammar.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 113 "library/grammar.y" /* yacc.c:1646  */
    {qasm_representation.qubitRegister((yyvsp[0].ival));}
#line 1500 "grammar.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 117 "library/grammar.y" /* yacc.c:1646  */
    {(yyval.qval)=(yyvsp[0].qval);}
#line 1506 "grammar.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 119 "library/grammar.y" /* yacc.c:1646  */
    {
            buffer_indices = qasm_representation.getMappedIndices( std::string((yyvsp[0].sval)), true );
            (yyval.qval) = new compiler::Qubits (buffer_indices);
        }
#line 1515 "grammar.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 126 "library/grammar.y" /* yacc.c:1646  */
    {
                 buffer_indices.removeDuplicates();
                 (yyval.qval) = new compiler::Qubits (buffer_indices);
                 buffer_indices.clear();
              }
#line 1525 "grammar.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 138 "library/grammar.y" /* yacc.c:1646  */
    {
           buffer_indices = qasm_representation.getMappedIndices( std::string((yyvsp[0].sval)), false );
           (yyval.bval) = new compiler::Bits (buffer_indices);
       }
#line 1534 "grammar.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 145 "library/grammar.y" /* yacc.c:1646  */
    {
                 buffer_indices.removeDuplicates();
                 (yyval.bval) = new compiler::Bits (buffer_indices);
                 buffer_indices.clear();
            }
#line 1544 "grammar.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 157 "library/grammar.y" /* yacc.c:1646  */
    {
                            subcircuits_object.lastSubCircuit().addOperation( new compiler::Operation(buffer_gate, *((yyvsp[0].qval)) ) );
                         }
#line 1552 "grammar.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 161 "library/grammar.y" /* yacc.c:1646  */
    {
                            subcircuits_object.lastSubCircuit().addOperation( new compiler::Operation(buffer_gate, *((yyvsp[0].qval)) ) );
                         }
#line 1560 "grammar.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 166 "library/grammar.y" /* yacc.c:1646  */
    {
                                  subcircuits_object.lastSubCircuit().addOperation( new compiler::Operation(buffer_gate, *((yyvsp[-2].qval)) ,(yyvsp[0].dval)) );
                              }
#line 1568 "grammar.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 171 "library/grammar.y" /* yacc.c:1646  */
    {
                    qasm_representation.addMappings(std::string((yyvsp[0].sval)), (yyvsp[-2].qval)->getSelectedQubits(), true );
                }
#line 1576 "grammar.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 175 "library/grammar.y" /* yacc.c:1646  */
    {
                    qasm_representation.addMappings(std::string((yyvsp[0].sval)), (yyvsp[-2].bval)->getSelectedBits(), false );
                }
#line 1584 "grammar.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 180 "library/grammar.y" /* yacc.c:1646  */
    {buffer_gate = std::string((yyvsp[0].sval));}
#line 1590 "grammar.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 180 "library/grammar.y" /* yacc.c:1646  */
    {buffer_gate = std::string((yyvsp[0].sval));}
#line 1596 "grammar.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 182 "library/grammar.y" /* yacc.c:1646  */
    {buffer_gate = std::string((yyvsp[0].sval));}
#line 1602 "grammar.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 185 "library/grammar.y" /* yacc.c:1646  */
    {buffer_gate = std::string((yyvsp[0].sval));}
#line 1608 "grammar.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 185 "library/grammar.y" /* yacc.c:1646  */
    {buffer_gate = std::string((yyvsp[0].sval));}
#line 1614 "grammar.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 188 "library/grammar.y" /* yacc.c:1646  */
    {
                               subcircuits_object.lastSubCircuit().addOperation( new compiler::Operation( buffer_gate, *((yyvsp[-6].qval)) , std::string((yyvsp[-4].sval),1) , *((yyvsp[-2].qval)) , std::string((yyvsp[0].sval),1) ) );
                           }
#line 1622 "grammar.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 192 "library/grammar.y" /* yacc.c:1646  */
    {buffer_gate = std::string((yyvsp[0].sval));}
#line 1628 "grammar.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 193 "library/grammar.y" /* yacc.c:1646  */
    {subcircuits_object.lastSubCircuit().addOperation( new compiler::Operation(std::string((yyvsp[0].sval))) );}
#line 1634 "grammar.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 198 "library/grammar.y" /* yacc.c:1646  */
    {
                          subcircuits_object.lastSubCircuit().addOperation( new compiler::Operation( buffer_gate, *((yyvsp[-2].qval)) , *((yyvsp[0].qval)) ) );
                      }
#line 1642 "grammar.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 205 "library/grammar.y" /* yacc.c:1646  */
    {buffer_gate = std::string((yyvsp[0].sval));}
#line 1648 "grammar.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 207 "library/grammar.y" /* yacc.c:1646  */
    {buffer_gate = std::string((yyvsp[0].sval));}
#line 1654 "grammar.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 212 "library/grammar.y" /* yacc.c:1646  */
    {buffer_gate = std::string((yyvsp[0].sval));}
#line 1660 "grammar.tab.c" /* yacc.c:1646  */
    break;


#line 1664 "grammar.tab.c" /* yacc.c:1646  */
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
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

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

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 267 "library/grammar.y" /* yacc.c:1906  */


void yyerror(char const *x)
{
    printf("Error %s\n",x);
    exit(1);
}
