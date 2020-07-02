/* writetool.c */

/* written by Douglas Jones, July 2010,
   based on pieces of cruncher,
     written in Pascal by Douglas Jones, March 1990
     rewritten in C, Jan 2007
*/

/* Tools for output of elements from a grammar */

#include <stdlib.h>
#include <stdio.h>

#include "grammar.h"
#include "writetool.h"

/*
 * global variables private to this package
 */

static int column; /* last column number filled on line */

/*
 * The interface
 */

void outsetup() { /* setup for package use */
	column = 0;
}

int getoutcol() { /* note what column we are on */
	return column;
}

void outline() { /* put out newline */
	putchar( '\n' );
	column = 0;
}

void outchar( char ch ) { /* put out one character */
	putchar( ch );
	column++;
}

void outstring( char * p ) { /* put out one null-terminated string */
	if (p != NULL) { /* insurance */
		while (*p != '\0') {
			outchar( *p );
			p++;
		}
	}
}

void outspaces( int c ) { /* put out spaces until column = c */
	while (column < c) outchar( ' ' );
}

void outspacesym( PSYMBOL s, int c, char ch ) {
	/* put out a space,
	   or if s won't fit, return to column c starting the line with ch */

	int len; /* length of s, in chars */
        STRINGPT pos; /* positiion of s in stringtab */

	pos = s->name;
	len = stringtab[pos]; /* string length is encoded in first char */

	/* does s fit on the line? */
	if ((column + 1 + len) > 80) { /* no, move to next line */
		outline();
		if (c > 1) outchar( ch );
		outspaces( c );
	} else { /* yes, output just one space */
		outchar( ' ' );
	}
}

void outsymbol( PSYMBOL s ) { /* put symbol to output */
	int len; /* length of s, in chars */
        STRINGPT pos; /* positiion of s in stringtab */

	pos = s->name;
	len = stringtab[pos];

	while (len != 0) {
		len = len - 1;
		pos = pos + 1;
		outchar( stringtab[pos] );
	}
}
