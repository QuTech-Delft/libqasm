/* startfollow.h */

/* written by Douglas Jones, July 2010,
   based on pieces of cruncher,
     written in Pascal by Douglas Jones, March 1990
     rewritten in C, Jan 2007
*/

/* traverse grammar and build start set and follow set for each nonterminal */

/*
 * The interface
 */

void startfollow(); /* compute start set and follow set of each nonterminal */
