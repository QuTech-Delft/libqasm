/* writeg.c */

/* written by Douglas Jones, July 2010,
   based on pieces of cruncher,
     written in Pascal by Douglas Jones, March 1990
     rewritten in C, Jan 2007
*/

/* OUTPUT FORMAT:
   The distinguished symbol from which the grammar hangs is always output
   first, followed the designated empty symbol, if one was specified.
      |
      |> <distinguished>
      |/ <empty>
      |
   Terminal and nonterminal symbols are preserved unchanged, in whatever
   form they were input.  Rules are output as follows:
      |
      |<nonterminal> ::= <a> 'b' <c> 'd'
      |                  <continuation>
      |
   The delimiter ::= was used in the original Algol 60 report.  Indenting
   indicated continuation.  Multiple rules that reduce to the same
   nonterminal are output in succession, using the vertical bar to separate
   successive rules instead of re-stating the lefthand side of the rule.
      |
      |<nonterminal> ::= <a> 'b'
      |               |  <c> 'd'
      |
   If the start set and follow set of a nonterminal have been computed, these
   are included in the output grammar as comments after all of the rules for
   that nonterminal.
      |
      |<nonterminal> ::= <a> 'b'
      |               |  <c> 'd'
      |\ start:  'a' 'c'
      |\ follow: 'g' 'h'
      |
   At the end, unreachable symbols and unused production rules are listed.
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "grammar.h"
#include "writetool.h"
#include "writeg.h"

/*
 * global variables private to this package
 */

static int barcol; /* column number for vertical bar */
static int contcol; /* column number for continuation */

/*
 * printing utility
 */

static void outprod( PPRODUCTION p ) { /* put out RHS of rule p */
        PELEMENT e;
	PSYMBOL s;

	if (p != NULL) { /* empty rules should never happen, but be safe */
		for (e = p->data; e != NULL; e = e->next) {
			s = e->data;
			outspacesym( s, contcol, ' ' );
			outsymbol( s );
		}
	}
}

static void outprodgroup( PSYMBOL s ) { /* put out all rules with s on LHS */
	PPRODUCTION p;
	PELEMENT e;
	PSYMBOL ss;

	outline();
	outsymbol( s );
	outchar( ' ' );
	barcol = getoutcol() + 1; /* remember indent for next rule */
	outstring( RULESYM );
        contcol = getoutcol() + 1; /* remember indent for continuation */

	/* output first production on same line */
	p = s->data;
	outprod( p );

	while (p->next != NULL) { /* output successive productions */
		p = p->next;
		
		outline();
		outspaces( barcol );
		outstring( "| " );

		outprod( p );
	}

	if (s->starter != NULL) { /* output start set */
		outline();
		outchar( COMMENT );
		outstring( " start set:  " );
        	contcol = getoutcol() + 1; /* remember indent */

		for (e = s->starter; e != NULL; e = e->next) {
			ss = e->data;
			outspacesym( ss, contcol, COMMENT );
			outsymbol( ss );
		}
	}
	if (s->follows != NULL) { /* output follow set */
		outline();
		outchar( COMMENT );
		outstring( " follow set: " );
        	contcol = getoutcol() + 1; /* remember indent */

		for (e = s->follows; e != NULL; e = e->next) {
			ss = e->data;
			outspacesym( ss, contcol, COMMENT );
			outsymbol( ss );
		}
	}
	if ((s->follows != NULL) || (s->starter != NULL)) {
		/* output blank line to separate from the next rule */
		outline();
	}
}

static void outreachable( PSYMBOL s ) {
	/* recursively output reachable production*/

        /* handles used in list traversals */
        PPRODUCTION p;
        PELEMENT e;
        PSYMBOL ss;

	/* only output nonterminals with their rules */
	if (s->data != NULL) outprodgroup( s );

	/* mark that it is printed */
        s->state = TOUCHED;

        /* recursive walk through all of its productions */
        p = s->data;
        while (p != NULL) {
                /* walk through all elements of each production */
                e = p->data;
                while (e != NULL) {
                        /* touch the associated symbol */
                        ss = e->data;
                        if (ss->state == UNTOUCHED) outreachable( ss );

                        /* move to next element */
                        e = e->next;
                }

                /* move to next rule */
                p = p->next;
        }
}

/*
 * The interface
 */

void writeg() { /* write grammar structure documented in grammar.h */
	PSYMBOL s;
	bool header;

	outsetup();

	if (head != NULL) { /* there is a distinguished symbol */
		outstring( "> " );
		outsymbol( head );
	} else {
		outchar( COMMENT );
		outstring( " no distinguished symbol!" );
	}

	if (emptypt != NULL) { /* there is an empty symbol */
		outline();
		outstring( "/ " );
		outsymbol( emptypt );
	}

	for (s = symlist; s != NULL; s = s->next) s->state = UNTOUCHED;
	if (head != NULL) {
		outline();
		outreachable( head );
	}

	header = false;
	for (s = symlist; s != NULL; s = s->next) {
		if ((s->data == NULL) && (s->state == TOUCHED)) {
			if (!header) {
				outline();
				outline();
				outchar( COMMENT );
				outstring( " terminals:  " );
				contcol = getoutcol() + 1; /* remember indent */
				header = true;
			}
			outspacesym( s, contcol, COMMENT );
			outsymbol( s );
		}
	}

	header = false;
	for (s = symlist; s != NULL; s = s->next) {
		if ((s->data != NULL) && (s->state == UNTOUCHED)) {
			if (!header) {
				outline();
				outline();
				outchar( COMMENT );
				outstring( " unused productions" );
				header = true;
			}
			outprodgroup( s );
		}
	}

	header = false;
	for (s = symlist; s != NULL; s = s->next) {
		if ((s->data == NULL) && (s->state == UNTOUCHED)) {
			if (!header) {
				outline();
				outline();
				outchar( COMMENT );
				outstring( " unused terminals: " );
				contcol = getoutcol() + 1; /* remember indent */
				header = true;
			}
			outspacesym( s, contcol, COMMENT );
			outsymbol( s );
		}
	}
	outline();
}
