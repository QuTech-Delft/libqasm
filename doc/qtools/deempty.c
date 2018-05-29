/* deempty.c */

/* written by Douglas Jones, June 2010,
   based on pieces of cruncher,
     written in Pascal by Douglas Jones, March 1990
     rewritten in C, Jan 2007
*/

/* Remove empty symbol from grammar.  In this context,
      |
      |/ <empty>
      |
   Rewrite these rules
      |
      |<a> ::= <b> <empty> <c>
      |<b> ::= <d> | <empty>
      |
   as these equivalent rules
      |
      |<a> ::= <b> <c>
      |     |  <c>
      |<b> ::= <d>
      |
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "grammar.h"
#include "readg.h"
#include "samerule.h"
#include "deempty.h"

/*
 * Markings used to classify production rules and symbols in the grammar
 */

/* Values for states of symbols and rules */
/*      ISEMPTY    -- already defined */
/*      CANBEEMPTY -- already defined */
#define NONEMPTY      TOUCHED

/* rules end up either
	ISEMPTY    -- all elements symbols are ISEMPTY
	CANBEEMPTY -- all alements symbols are ISEMPTY or CANBEEMPTY
	NONEMPTY   -- some element is known to be NONEMPTY
 */
/* nonterminal symbols end up
	ISEMPTY    -- all rules of this symbol are ISEMPTY
	CANBEEMPTY -- some rule is ISEMPTY or CANBEEMPTY, one or more NONEMPTY
	NONEMPTY   -- all rules are NONEMPTY
 */

/*
 * Global
 */

static bool change; /* record that a change was made to the grammar */

/*
 * Support
 */

static void checkempty( PSYMBOL s ) { /* update emptyness of nonterminal s */
	PPRODUCTION p;
	PELEMENT e;

	bool anyempty;     /* are there any empty rules under s */
	bool anynonempty;  /* are there any non-empty rules under s */
	bool anycanbe;     /* are there any may-be-empty rules under s */
	bool anyunknown;   /* are there any unknown rules under s */
	enum STYPE sstate; /* conclusion about state of symbol s */

	enum STYPE pstate; /* conclusion about state of rule p */

	/* initially we know nothing about productions unmder symbol s */
	anyempty = false;
	anynonempty = false;
	anycanbe = false;

	for (p = s->data; p != NULL; p = p->next) {
		/* for each production rule p under nonterminal s */

		/* initially assume rule p is empty */
		/* pstate = ISEMPTY; */
		pstate = p->state;

		for (e = p->data; e != NULL; e = e->next) {
			/* for each element e of rule p */

			if(e->data->state == NONEMPTY) {
				/* p locked in as non-empty by any nonempty e */
				pstate = NONEMPTY;
			} else if (e->data->state == CANBEEMPTY) {
				if (pstate == ISEMPTY) {
					/* p may not be empty if e may not */
					pstate = CANBEEMPTY;
				}
			} /* else e->data->state == ISEMPTY,
			        no state change
			*/
		}

		if (p->state != pstate) { /* record new state of rule p */
			p->state = pstate;
			change = true;
		}

		/* record impact of above on what we know about symbol s */
		if (pstate == ISEMPTY) {
			anyempty = true;
		} else if (pstate == CANBEEMPTY) {
			anycanbe = true;
		} else /* pstate == NONEMPTY */ {
			anynonempty = true;
		}
	}

	/* we have looked at all the productions of s, draw conclusions */
	if (anyempty && anynonempty) {
		/* if both possibilities exist, s may be either */
		sstate = CANBEEMPTY;
	} else if (anycanbe) {
		/* if any rule may either, then s may be either */
		sstate = CANBEEMPTY;
	} else if (anyempty /* && !anynonempty && !anycanbe */ ) {
		/* we know they're all empty */
		sstate = ISEMPTY;
	} else if (anynonempty /* && !anyempty && !anycanbe */ ) {
		/* we know they're all non-empty */
		sstate = NONEMPTY;
	} else /* should never happen */ {
		errormsg( "ASSERTION FAILURE IN CHECKEMPTY", -1 );
		/* if this ever comes up, might help to get source line no */
	}

	if (s->state != sstate) { /* record new state of symbol s */
		s->state = sstate;
		change = true;
	}
}

static void dupminus( PSYMBOL s, PPRODUCTION p, PELEMENT e ) {
	/* duplicate production p minus element e under symbol s
	   duplicate is guaranteed non-empty, non-redundant,
	   it becomes the successor production of p */
	PELEMENT pe;    /* an element of p */
	PPRODUCTION np; /* the new production */
	PELEMENT ne;    /* a new element of np */
	PELEMENT * pne; /* place to hang ne under np */
	PPRODUCTION sp; /* a successor of p used for duplicate elimination */

	np = NEWPRODUCTION;
	pne = &(np->data);

	for (pe = p->data; pe != NULL; pe = pe->next) if (pe != e) {
		/* for all elements of p except e */
		ne = NEWELEMENT;
		ne->data = pe->data;
		*pne = ne;
		pne = &(ne->next);
	}

	/* mark end of new rule */
	*pne = NULL;

	/* avoid inserting duplicates of existing rules */
	sp = s->data;
	do {
		if (samerule( np, sp )) {
			np->data = NULL;
			sp = NULL;
		} else {
			sp = sp->next;
		}
	} while (sp != NULL);

	if (np->data != NULL) { /* if new rule is not empty */
		/* put new rule onto production list */
		np->next = p->next;
		np->state = p->state;
		p->next = np;
	}
}

static void cleanempty( PSYMBOL s ) { /* clean up rules of of nonterminal s */
	PPRODUCTION * pp;
	PPRODUCTION p;
	PELEMENT * pe;
	PELEMENT e;

	if (s->state == ISEMPTY) return;

	pp = &(s->data);
	while (*pp != NULL) {
		/* for all rules of symbol s, with deletions and additions */
		p = *pp;
		if (p->state == ISEMPTY) {
			/* delete this production rule instead of moving on */
			*pp = p->next;

		} else { /* CANBEEMPTY = NONEMPTY */
			/* clean up the rule, possibly adding new productions */

			pe = &(p->data);
			while (*pe != NULL) {
				/* for all elements of rule p */
				e = *pe;
				if (e->data->state == ISEMPTY) {
					/* delete e from rule p, don't move on*/
					*pe = e->next;

				} else if (e->data->state == CANBEEMPTY) {
					/* new copy of rule p minus element e */
					dupminus( s, p, e );

					/* move on to next production */
					pe = &(e->next);

				} else { /* NONEMPTY */
					/* move on to next production */
					pe = &(e->next);

				}
			}

			/* move on to next rule */
			pp = &(p->next);
		}
	}
}

/*
 * The interface
 */

void deempty() { /* eliminate references to the empty symol */

	/* handles used in list traversals */
	PSYMBOL s;
	PPRODUCTION p;

	if (emptypt == NULL) {
		errormsg( "EMPTY SYMBOL MUST BE DEFINED", -1 );
		return; /* quit if no analysis possible */
	}

	/* apply initial markings on all symbols */
	for (s = symlist; s != NULL; s = s->next) {
		if (TERMINAL(s)) {
			/* most terminals are nonempty */
			s->state = NONEMPTY;
		} else {
			/* nonterminals and rules are empty until proven not */
			s->state = ISEMPTY;
			for (p = s->data; p != NULL; p = p->next) {
				p->state = ISEMPTY;
			}
		}
	}
	/* the distinguished empty symbol is the exceptional terminal */
	emptypt->state = ISEMPTY; 

	do { /* keep trying until no change is made to the grammar */
		change = false;

		for (s = symlist; s != NULL; s = s->next) if (NONTERMINAL(s)) {
			/* for each nonterminal symbol s */

			checkempty( s );
		}
	} while (change);

	/* now use the markup to rewrite rules accounting for emptyness */
	for (s = symlist; s != NULL; s = s->next) if (NONTERMINAL(s)) {
		/* for each nonterminal symbol s */

		cleanempty( s );
	}

	/* finally, deal with possible empty distinguished symbol */
	if (head->state == ISEMPTY) {
		/* eliminate the distinguished symbol and the empty symbol! */
		head = NULL;
		emptypt = NULL;
	} else if (head->state == CANBEEMPTY) {
		/* we eliminated a bit too much, put it back! */
		p = NEWPRODUCTION;
		p->data = NEWELEMENT;
		p->data->next = NULL;
		p->data->data = emptypt;
		p->next = head->data;
		head->data = p;
		/* do not eliminate the empty symbol! */
	} else /* head->state == NONEMPTY */ {
		/* eliminate the empty symbol! */
		emptypt = NULL;
	}
}
