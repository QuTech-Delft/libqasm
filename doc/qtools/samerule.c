/* samerule.c */

/* written by Douglas Jones, July 2010,
   based on pieces of cruncher,
     written in Pascal by Douglas Jones, March 1990
     rewritten in C, Jan 2007
*/

/* compare two produciton rules */

#include "stdlib.h"
#include "stdbool.h"

#include "grammar.h"
#include "samerule.h"

/*
 * Interface
 */

bool samerule( PPRODUCTION p, PPRODUCTION q ) { /* compare rules p,q */
	PELEMENT pe, qe; /* pointers to elements of p and q */

	/* for successive elements */
	pe = p->data;
	qe = q->data;
	while ((pe != NULL) && (qe != NULL)) {
		if (pe->data != qe->data) return false; /* they differ */
		pe = pe->next;
		qe = qe->next;
	}
	return ((pe == NULL) && (qe == NULL)); /* identical and same length */
}
