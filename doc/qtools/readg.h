/* readg.h */

/* written by Douglas Jones, June 2010,
   based on pieces of cruncher,
     written in Pascal by Douglas Jones, March 1990
     rewritten in C, Jan 2007
*/

/*
 * The interface
 */

void errormsg( char * msg, int line );
	/* output a error message msg attributed to the given source line */
	/* use -1 as a line number if line attribution does not work */

PSYMBOL lookupsym( char * str );
	/* lookup str in the main symbol list, return NULL if not found */
	/* str[0] is length of the symbol, in chars */

PSYMBOL definesym( char * str );
	/* define str in the main symbol list, it must not be already there */

void readg(); /* read grammar into global grammar structure in grammar.h */
	      /* see readgram.c for the syntax used to express grammars */
