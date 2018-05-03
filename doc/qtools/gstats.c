/* gstats.c */

/* written by Douglas Jones, June 2010,
   based on pieces of cruncher,
     written in Pascal by Douglas Jones, March 1990
     rewritten in C, Jan 2007
*/

/* main program to simply compute statistics about grammar */

#include "grammar.h"
#include "readg.h"
#include "gramstats.h"

int main( int argc, char *argv[] ) {
	readg();
 	gramstats();
}
