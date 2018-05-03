/* gdeempty.c */

/* written by Douglas Jones, July 2013,
   based on pieces of cruncher,
     written in Pascal by Douglas Jones, March 1990
     rewritten in C, Jan 2007
*/

/* main program to remove empty symbols from a grammar */

#include "grammar.h"
#include "readg.h"
#include "deempty.h"
#include "writeg.h"

int main( int argc, char *argv[] ) {
	readg();
	deempty();
 	writeg();
}
