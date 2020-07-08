/* reachable.c */

/* written by Douglas Jones, June 2010,
   based on pieces of cruncher,
     written in Pascal by Douglas Jones, March 1990
     rewritten in C, Jan 2007
*/

/* mark reachable symbols in grammar as TOUCHED */

#include <stdlib.h>

#include "grammar.h"
#include "reachable.h"

/*
 * The interface
 */

void reachsetup() { /* setup for reachability analysis */
	PSYMBOL s;
	for (s = symlist; s != NULL; s = s->next) s->state = UNTOUCHED;
}

void reachtouch( PSYMBOL s ) { /* recursively touch reachable symbols */

	/* handles used in list traversals */
	PPRODUCTION p;
	PELEMENT e;
	PSYMBOL ss;

	s->state = TOUCHED;

	for (p = s->data; p != NULL; p = p->next) {
		/* for all production rules p */

		for (e = p->data; e != NULL; e = e->next) {
			/* for all elements e of rule p, touch the symbol */

			ss = e->data;
			if (ss->state == UNTOUCHED) reachtouch( ss );
		}
	}
}
