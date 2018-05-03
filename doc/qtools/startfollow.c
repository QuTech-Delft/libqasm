/* startfollow.c */

/* written by Douglas Jones, July 2010,
   based on pieces of cruncher,
     written in Pascal by Douglas Jones, March 1990
     rewritten in C, Jan 2007
*/

/* traverse grammar and build start set and follow set for each nonterminal */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "grammar.h"
#include "reachable.h"
#include "startfollow.h"

/*
 * Global
 */

static bool changed; /* has a symbol been added */

/*
 * support routines for managing lists of symbols
 */

static void addsym( PELEMENT * es, PSYMBOL s) {
	/* add symbol s as an element of the set *es */

	while ((*es != NULL) && ((*es)->data != s)) es = &((*es)->next);
	if (*es == NULL) { /* APPEND SYMBOL TO LIST */
		*es = NEWELEMENT;
		(*es)->next = NULL;
		(*es)->data = s;
		changed = true;
	}
}

static void addsymbols( PELEMENT * head, PELEMENT e ) {
	/* union all symbol in set e into the set *head */

	while (e != NULL) {
		addsym( head, e->data );
		e = e->next;
	}
}

/*
 * package to find start set for each nonterminal
 */

static void dosymbol(PSYMBOL s) { /* process one symbol (or tree of symbols) */
	PPRODUCTION p;

	if (TERMINAL(s)) { /* terminals are their own start set */
		addsym( &(s->starter), s );
	} else { /* nonterminal start sets are union of sets for each rule */
		for (p = s->data; p != NULL; p = p->next) {
			addsymbols( &(s->starter), p->data->data->starter );
		}
	}
}

static void getstarts() {
	/* compute start sets of all symbols:
	   each terminal symbol is made into its own start set
	   nonterminals get start sets determine by each rule */
	PSYMBOL s;

	/* repeat the experiment until no additions to any start set */
	do {
		changed = false;

		/* build start sets */
		for (s = symlist; s != NULL; s = s->next) if(s->state==TOUCHED){
			dosymbol( s );
		}
	} while (changed);
}

/*
 * package to find follow set for each nonterminal
 */

static void getfollowrule( PPRODUCTION p ) {
	/* each element of a rule provides follow set for predecessor */
	PELEMENT e;	/* the current element */
	PELEMENT pe;	/* the previous element in this rule */
	PSYMBOL es;	/* the symbol referenced by e */
	PSYMBOL pes;	/* the symbol referenced by pe */

	pe = p->data;
	if (pe != NULL) {;
		pes = pe->data;
		for (e = pe->next; e != NULL; e = e->next) {
			es = e->data;
			/* for every pair of consecutive elements pe e
			   where es and pes are coresponding symbols */

			/* don't test terminality of es or pes */
			addsymbols( &(pes->follows), es->starter );

			pe = e;
			pes = es;
		}
	}
}

static void pushfollow( PPRODUCTION p, PSYMBOL s ) {
	/* follow set of nonterminal s applies to produciton rule p under s */
	PELEMENT e;	/* the current element */

	e = p->data;
	if (e != NULL) { /* should always be true */
		while (e->next != NULL) e = e->next;
		/* e is now last element of rule p */

		addsymbols( &(e->data->follows), s->follows );
	}
}

static void getfollows() { /* compute follow sets of all nonterminals */
	PSYMBOL s;
	PPRODUCTION p;

	/* first pass:  get local info on follow set from within rules */
	for (s = symlist; s != NULL; s = s->next) if (s->state == TOUCHED) {
		for (p = s->data; p != NULL; p = p->next) {
			/* for each production p hanging from every rule s */

			/* within rules, infer follow sets from start sets */
			getfollowrule( p );
		}
	}

	/* second pass:  push follow set down from each symbol */
	do {
		changed = false;
		for (s = symlist; s != NULL; s = s->next) if(s->state==TOUCHED){
			for (p = s->data; p != NULL; p = p->next) {
				/* for each production p of every symbol s */

				/* follow set of s is follow set of end of p */
				pushfollow( p, s );
			}
		}
	} while (changed == true);
}

/*
 * The interface
 */

void startfollow() { /* compute start set and follow set of each nonterminal */
	/* first learn what part of the grammar is reachable */
	reachsetup();
	if (head != NULL) reachtouch( head );

	/* then do the work */
	getstarts();
	getfollows();
}
