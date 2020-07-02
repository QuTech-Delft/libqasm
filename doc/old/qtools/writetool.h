/* writetool.h */

/* written by Douglas Jones, July 2010,
   based on pieces of cruncher,
     written in Pascal by Douglas Jones, March 1990
     rewritten in C, Jan 2007
*/

/* Tools for output of elements from a grammar */

/*
 * The interface
 */

void outsetup(); /* setup for package use */

int getoutcol(); /* note what column we are on */

void outline(); /* put out newline */

void outchar( char ch ); /* put out one character */

void outstring( char * p ); /* put out one null-terminated string */

void outspaces( int c ); /* put out spaces until column = c */

void outspacesym( PSYMBOL s, int c, char ch );
        /* put out a space, 
           or if s won't fit, return to column c starting the line with ch */

void outsymbol( PSYMBOL s ); /* put symbol to output */
