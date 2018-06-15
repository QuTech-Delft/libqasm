/* grammar.h */

/* written by Douglas Jones, June 2010,
   based on pieces of cruncher,
     written in Pascal by Douglas Jones, March 1990
     rewritten in C, Jan 2007
*/

/* all data structures used to represent the grammar are here
   there is no use of serious data abstraction in this code */

/*
 * Configuration constants
 */

/* Limit on the number of nonterminal characters */
#define STRINGLIMIT 10000

/* Limit on the number of chars in any symbol (T or NT) */
#define SYMLEN 50

/* Configurable syntactic details */
#define COMMENT '#'
#define RULESYM "::="

/* note:  RULESYM is for writeg, while readg accepts any of :, =, := or ::= */

/*
 * Types
 */

/* Handle on strings in the string table (0 .. STRINGLIMIT) */
typedef int STRINGPT;

/* Types of terminal symbols */
enum STYPE {
	ISEMPTY,	/* known to be equivalent to the empty symbol */
	CANBEEMPTY,	/* may be empty, marks optional syntactic elements */
	TOUCHED,	/* is reachable from the root of the grammar */
	UNTOUCHED,	/* has not yet been reached from the root */
	STYPES		/* NOT A TYPE, rather, the number of types permitted */
};

/* Pointer types */
typedef struct symbol *     PSYMBOL;
typedef struct production * PPRODUCTION;
typedef struct element *    PELEMENT;

struct symbol {
	STRINGPT name;    /* symbols have names */
	PSYMBOL next;     /* symbols may occur in lists of symbols */
	PPRODUCTION data; /* the head of the list of productions */
	enum STYPE state; /* the state of this symbol */
	PELEMENT starter; /* the head of the terminal list in the start set */
	PELEMENT follows; /* the head of the terminal list in the follow set */
	int line;         /* source line number on which symbol first seen */
};

struct production {
	PPRODUCTION next; /* each symbol has a linked list of productions */
	PELEMENT data;    /* the body of a production is a list of elements */
	enum STYPE state; /* the state of the production rule */
	PELEMENT starter; /* the head of the terminal list in the start set */
	PELEMENT ender;   /* the head of the terminal list in the follow set */
	int line;         /* source line number on which production starts */
};

struct element {
	PELEMENT next;    /* each production is a list of elements */
	PSYMBOL data;     /* an element is a handle on a symbol, never NULL */
	int line;         /* source line number on which element occurs */
};

/*
 * storage allocators
 */

#define NEWSYMBOL     (PSYMBOL) malloc(sizeof(struct symbol))
#define NEWPRODUCTION (PPRODUCTION) malloc(sizeof(struct production))
#define NEWELEMENT    (PELEMENT) malloc(sizeof(struct element))

/*
 * predicates
 */

#define TERMINAL(s)    ((s)->data == NULL)
#define NONTERMINAL(s) ((s)->data != NULL)

/*
 * global variables
 */

/* the tring table */
char     stringtab[STRINGLIMIT];
STRINGPT stringlim; /* lowest free element in the string table */

/* head and address of null pointer for the main list of all symbols */
PSYMBOL  symlist;
PSYMBOL * symlistend;

PSYMBOL  emptypt;   /* Identity of the empty symbol */
PSYMBOL  head;      /* Identity of the distinguished symbol in the grammar */
