/* reachable.h */

/* written by Douglas Jones, June 2010,
   based on pieces of cruncher,
     written in Pascal by Douglas Jones, March 1990
     rewritten in C, Jan 2007
*/

/* mark reachable symbols in grammar as TOUCHED */

/*
 * The interface
 */

void reachsetup(); /* setup for reachability analysis */

void reachtouch( PSYMBOL s ); /* recursively touch reachable symbols */
