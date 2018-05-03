/* squeeze.c */

/* written by Douglas Jones, July 2010,
   based on pieces of cruncher,
     written in Pascal by Douglas Jones, March 1990
     rewritten in C, Jan 2007
*/

/* try to minimize grammar by squeezing out redundant rules and symbols
     <a> ::= <b>
          |  x <e>
          |  x <e>    -- squeezerules gets rid of this
     <b> ::= y <c>    -- squeezesymbols gets rid of this
   is replaced with
     <a> ::= y <c>
          |  x <e>
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "grammar.h"
#include "samerule.h"
#include "squeeze.h"

/*
 * Global
 */

static bool change; /* record that a change was made to the grammar */

/*
 * Worker routines
 */

static void squeezesymbols(){ /* squeeze out redundant symbols */
	/* handles used in list traversals */
	PSYMBOL s;
	PPRODUCTION p;
	PELEMENT e;
	PSYMBOL s1;
	PPRODUCTION p1;
	PELEMENT e1, e2;

	/* for all symbols */
	for (s = symlist; s != NULL; s = s->next ) {

		/* for all production rules of that symbol */
		for (p = s->data; p != NULL; p = p->next ) {

			/* for all elements of that production rule */
			for (e = p->data; e != NULL; e = e->next ) {

				s1 = e->data;
				p1 = s1->data;
				if ((p1 != NULL) && (p1->next == NULL)) {
					/* symbol s1 has just 1 rule p1
					   substitute that rule for s1 in p */

					e1 = p1->data;
					if (e1 != NULL) { /* if well formed */
						/* firse element of rule
						   overwrites first element */
						e->data = e1->data;
						change = true;

						/* now copy rest of rule */
						e1 = e1->next;
						while (e1 != NULL) {
							e2 = NEWELEMENT;
							e2->data = e1->data;
							e2->next = e->next;
							e->next = e2;
							e = e2;
							e1 = e1->next;
						}
					}
				}
			}
		}
	}
}

static void squeezerules(){ /* squeeze out redundant production rules */
	/* handles used in list traversals */
	PSYMBOL s;
	PPRODUCTION p;
	PPRODUCTION * qp; /* pointer to q */
	PPRODUCTION q;

	/* for all symbols */
	for (s = symlist; s != NULL; s = s->next ) {

		/* for all productions */
		for (p = s->data; p != NULL; p = p->next ) {

			/* for all additional productions of symbol s */
			qp = &(p->next);
			q = *qp; /* that is, q = p->next */
			while (q != NULL) {
				if (samerule( p, q )) {
					/* rule q is redundant, eliminate it */
					*qp = q->next;
					change = true;
				} else {
					/* move to next production */
					qp = &(q->next);
				}
				q = *qp;
			}
		}
	}
}

/*
 * The interface
 */

void squeeze() { /* squeeze out redundant rules and symbols */

	/* count the symbols and setup for reachability analysis */
	do {
		change = false;
		squeezerules();
		squeezesymbols();
	} while (change);
}
