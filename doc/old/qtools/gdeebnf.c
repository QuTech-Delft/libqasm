/* gdeebnf.c */

/* written by Douglas Jones, August 2013,
   based on pieces of cruncher,
     written in Pascal by Douglas Jones, March 1990
     rewritten in C, Jan 2007
*/

/* main program to convert a grammar from EBNF to BNF */

#include "grammar.h"
#include "readg.h"
#include "deebnf.h"
#include "writeg.h"

int main( int argc, char *argv[] ) {
	readg();
	deebnf();
 	writeg();
}
