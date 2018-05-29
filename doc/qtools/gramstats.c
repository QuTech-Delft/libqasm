/* gramstats.c */

/* written by Douglas Jones, June 2010,
   based on pieces of cruncher,
     written in Pascal by Douglas Jones, March 1990
     rewritten in C, Jan 2007
*/

/* compute statistics about grammar */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "grammar.h"
#include "reachable.h"
#include "gramstats.h"

/*
 * The interface
 */

void gramstats() { /* traverse the grammar and get statistics about it */

	/* statistics we're collecting */
	int symbols;   /* count of symbols in the grammar */
	int terminals; /* count terminal symbols */
	int untouched; /* count untouched symbols */
	int rules;     /* count of production rules in the grammar */
	int unrules;   /* count of rules hanging from untouched symbols */

	/* handles used in list traversals */
	PSYMBOL s;
	PPRODUCTION p;

	/* initialize counters */
	symbols = 0;
	terminals = 0;
	untouched = 0;
	rules = 0;
	unrules = 0;

	/* count the symbols */
	for (s = symlist; s != NULL; s = s->next) {
		/* for all symbols s */

		symbols++;
		if (TERMINAL(s)) terminals++;

		/* count the productions hanging from the symbol */
		for (p = s->data; p != NULL; p = p->next) {
			/* for all production rules p umder s */

			rules++;
		}
	}

	/* touch all symbols reachable from the head */
	reachsetup();
	if (head != NULL) reachtouch( head );

	/* count symbols that remain untouched */
	for (s = symlist; s != NULL; s = s->next) {
		/* for all symbols s */

		if (s->state == UNTOUCHED) { /* gather statistics */
			untouched++;

			/* count unused rules for that symbol */
			for (p = s->data; p != NULL; p = p->next) {
				/* for all production rules p umder s */

				unrules++;
			}
		}
	}

	/* report the results */
	printf( " -- Total symbols:        %d\n", symbols );
	printf( " --   Terminal symbols:   %d\n", terminals );
	if (untouched != 0) {
		printf( " --   Extraneous symbols: %d\n", untouched );
	}
	printf( " -- Production rules:     %d\n", rules );
	if (unrules != 0) {
		printf( " --   Extraneous rules:   %d\n", unrules );
	}
}
