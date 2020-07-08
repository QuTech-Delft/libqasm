/* gsample.c */

/* written by Douglas Jones, July 2013,
   based on pieces of cruncher,
     written in Pascal by Douglas Jones, March 1990
     rewritten in C, Jan 2007
*/

/* main program to sample the strings in a grammar */

#include "grammar.h"
#include "readg.h"
#include "sample.h"

int main( int argc, char *argv[] ) {
	readg();
 	sample();
}
