/* deebnf.c */

/* written by Douglas Jones, July 2013 */

/* convert Wirth style EBNF to pure BNF
     <a> ::= <b> [ <c> ] <d> { <e> | <f> } <g>
   is replaced with
     <a> ::= <b> <a-a> <d> <a-b> <g>
     <a-a> ::= <c>
            |  <empty>
     <a-b> ::= <e> <a-b>
            |  <f> <a-b>
            |  <empty>
   This is a kluge!  The metasymbols ( ) [ ] { } are parsed as terminals
   in the grammar by the readgram routine.

*/
   /* ------------- must carefully examime output on grambad -----*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "grammar.h"
#include "readg.h"
#include "deebnf.h"

/*
 * Global
 */

static PSYMBOL lparen;  /* ( */
static PSYMBOL rparen;  /* ) */
static PSYMBOL lsquare; /* [ */
static PSYMBOL rsquare; /* ] */
static PSYMBOL lcurly;  /* { */
static PSYMBOL rcurly;  /* } */

/*
 * Worker routines
 */

static PSYMBOL getdelsym( char * str ) {
        /* get symbol with name str and delete it from the master symbol list;
           str must begin with the length of the string, in characters;
           str must be NULL terminated so error messages work;
	   the symbol str must terminal; from now on, it is a metasymbol */

	PSYMBOL s; /* the symbol we are looking up */
	PSYMBOL * pss; /* tools for deletion */
	PSYMBOL ss;    /* tools for deletion */

	s = lookupsym( str );

	if (s == NULL) return NULL; /* nothing to do here */

	if (s->data != NULL) { /* it's nonterminal? */
		errormsg( "BRACE SHOULD BE NONTERMINAL", s->data->line );
	}

	/* now find pss, pointer to s in symlist -- we know it's there */
	pss = &symlist;
	ss = *pss;
	while (ss != s) {
		/* walk onward in symbol list */
		pss = &(ss->next);
		ss = *pss;
	}

	/* unlink s from symlist */
	if (s->next == NULL) { /* s was final element of list */
		symlistend = pss;
	}
	*pss = s->next;

	/* done */
	return s;
}

static PSYMBOL inventsymbol( PSYMBOL s, int * nsc ) {
	/* invent and return a new symbol; as initialized, it is a terminal
	   symbol a unique name derived from the name of s and *nsc.
	   Add rules to it and it will become nonterminal.
	*/
	char newname[SYMLEN];
	int i;      /* index into newname */
	STRINGPT j; /* stringpool index */
	int len;    /* length of symbol s */
	int dash;   /* location of dash in newname */
	int ext;    /* name extension */
	char quote; /* quotation mark at end of name */

	/* first, copy base name of new symbol into place */
	j = s->name;
	len = stringtab[j];
	for (i = 0; i <= len; i++) {
		newname[i] = stringtab[j];
		j++;
	}

	/* figure out what kind of quotes are in use, if any */
	if ( ((newname[1] == '<')  && (newname[len] == '>'))
	||   ((newname[1] == '"')  && (newname[len] == '"'))
	||   ((newname[1] == '\'') && (newname[len] == '\'')) ) {
		quote = newname[len];
		dash = len;
		/* extension will wipe out trailing quote */
	} else {
		quote = ' ';
		dash = len + 1;
		/* extension will be appended */
	}

	newname[dash] = '-';

	/* now, try to find a name extension that is not already in use */
	do {
		ext = *nsc;
		*nsc = ext + 1;

		/* create name extension of the form -a or -b */
		i = dash;
		do {
			i = i + 1;
			newname[i] = (ext % 26) + 'a';
			ext = ext / 26;
		} while (ext > 0);

		/* put back the quote that extension overwrote */
		if (quote != ' ') {
			i = i + 1;
			newname[i] = quote;
		}

		/* set the length to reflect extension etc*/
		newname[0] = i;

		/* keep trying until new name is genuinely new */
	} while (lookupsym( newname ) != NULL);

	/* newname is genuinely new */
	s = definesym( newname );
	return s;
}

static PSYMBOL extractuntil( PSYMBOL s, PPRODUCTION p, PELEMENT e,
			     PSYMBOL rsym, int * nsc ) {
	/* delete from e->next of production rule p and successors
	   until an element that references rsym; element e is edited
	   to refer to a new symbol with a name derived from the name of s
	   using *nsc and a rule holding the deleted parts, minus the
	   parentheses.  Returns a pointer to the new symbol, never NULL. */

	PELEMENT ee;    /* the element holding the end paren, if we find it */
	PELEMENT * eep; /* points to ee, once found, so we can delete it */

	PSYMBOL ns;     /* a new symbol */
	PPRODUCTION nr; /* the tail of the rule list hanging from ns */
	PELEMENT ne;    /* first element within parens starts body of rule */

	PSYMBOL lsym;   /* the left brace that balances rsym */
	int nest;       /* bracket nesting level */

	lsym = e->data; /* we were called with left brace current element */

	/* make new symbol to refer to a new set of rules */
	ns = inventsymbol( s, nsc );
	ns->line = e->line;

	/* make the first new rule that will hang under ns */
	nr = NEWPRODUCTION;
	nr->line = e->line;
	nr->state = UNTOUCHED;
	nr->next = NULL;
	nr->data = ne;

	/* set up preliminary linkage */
	ne = e->next;  /* the new rule begins after the opening bracket */
	nr->data = ne; /* the new rule begins after the opening bracket */
	ns->data = nr; /* hang the new rule in place */
	e->data = ns;  /* replace open bracket with new symbol */

	/* look for end paren of set of rules -- eep will point at it */
	nest = 0;
	eep = &(nr->data);
	ee = *eep;
	for (;;) {
		if (ee == NULL) {
			/* we hit the end of a rule, either because of
			   missing end bracket or bracketed alternatives */

			if (nr->data == NULL) { /* previous rule was empty! */
				errormsg( "EMPTY BRACKETED RULE", nr->line );
				/* add empty element to rule if possible */
				if (emptypt != NULL) {
					ne = NEWELEMENT;
					nr->data = ne;
					ne->line = nr->line;
					ne->next = NULL;
					ne->data = emptypt;
					ne = ee;
				}
			}

			if (p->next == NULL) break; /* quit loop */

			/* we have hit ( | and there is another production, so
			   swipe a rule from p's rule list, hang it under ns */
			nr->next = p->next;
			nr = nr->next;
			p->next = nr->next;
			nr->next = NULL;

			/* move to next rule looking for end paren */
			eep = &(nr->data);
			ee = *eep;
			ne = ee;

		} else { /* ee != NULL */
			/* scan for end paren, accounting for nesting */
			if (ee->data == rsym) {
				if (nest == 0) break; /* quit loop */
				nest = nest - 1;
			} else if (ee->data == lsym) {
				nest = nest + 1;
			}

			/* march down this rule looking for end paren */
			eep = &(ee->next);
			ee = *eep;
		}
	}

	if (ee != NULL) { /* we found the balancing parenthesis */
		*eep = NULL;        /* body of list does not include rsym */
		e->next = ee->next; /* snip bracketed body out of rule */

		if (nr->data == NULL) { /* final rule of set was empty! */
			errormsg( "EMPTY BRACKETED RULE", ee->line );
			/* add empty element to rule if possible */
			if (emptypt != NULL) {
				nr->data = NEWELEMENT;
				nr->data->line = nr->line;
				nr->data->next = NULL;
				nr->data->data = emptypt;
			}
		}
	} else { /* failure, unbalanced parens, treat as end paren */
		/* assert ee == *eep == NULL */
		e->next = NULL; /* snip body out of rule */

		if (rsym == rparen) {
			errormsg( "MISSING )", nr->line );
		} else if (rsym == rsquare) {
			errormsg( "MISSING ]", nr->line );
		} else /* rsym == rcurly */ {
			errormsg( "MISSING }", nr->line );
		}
		/* assert complaint about ne == NULL was already done */
	}
	return ns;
}

static void addemptyrule( PSYMBOL s ) {
	/* add an empty production to nonterminal s */
	PPRODUCTION p; /* a new production | <empty> */
	PELEMENT e;    /* a new element <empty> */

	if (emptypt == NULL) {
                errormsg( "EMPTY SYMBOL MUST BE DEFINED", s->data->line );
		return; /* quit if can't add empty rule */
        }
	
	e = NEWELEMENT;
	e->line = s->data->line; /* take the line number from its first rule */
	e->next = NULL;
	e->data = emptypt;

	p = NEWPRODUCTION;
	p->line = s->data->line; /* take the line number as above */
	p->next = s->data;
	p->data = e;
	p->state = UNTOUCHED;
	s->data = p;
}

static void makeiterative( PSYMBOL s ) { /* make nonterminal s iterate */
	PPRODUCTION p;  /* a production under s */
	PELEMENT e;	/* an element of p */
	PELEMENT * pe;  /* the pointer to e */
	int line;       /* best guess at source line number for empty element */

	/* add a self reference to end of each rule of s */
	for (p = s->data; p != NULL; p = p->next ) { /* for each production */
		/* find the end of the production */
		pe = &(p->data);
		line = p->line;
		e = *pe;
		while (e != NULL) {
			pe = &(e->next);
			line = e->line;
			e = *pe;
		}

		/* tack on a new element at the end that references s */
		e = NEWELEMENT;
		e->line = line;
		e->next = NULL;
		e->data = s;
		*pe = e;
	}
	
	/* add an empty production to s to terminate iteration */
	addemptyrule( s );
}

static void process( PSYMBOL s ) { /* process one symbol */
	PPRODUCTION p;    /* this production rule */
	PPRODUCTION * pp; /* the pointer to p so we can delete rules */
	PELEMENT e;       /* this element of p */
	PELEMENT * ep;    /* the pointer to e so we can delete elements */
	PSYMBOL ns;       /* a new symbol */
	int nsc;          /* count used to uniquely name new symbols */

	nsc = 0; /* any added symbols start with -a if possilbe */

	pp = &(s->data);
	p = *pp;
	while (p != NULL) {
		/* for each production rule of symbol, allowing for deletion */
		ep = &(p->data);
		e = *ep;
		while (e != NULL) {
			/* for each element of rule e */
			if (e->data == rparen) { /* syntax error */
				errormsg( "UNEXPECTED )", e->line );
				e = e->next;
				*ep = e; /* clip it from rule */

			} else if (e->data == rsquare) { /* syntax error */
				errormsg( "UNEXPECTED ]", e->line );
				e = e->next;
				*ep = e; /* clip it from rule */

			} else if (e->data == rcurly) { /* syntax error */
				errormsg( "UNEXPECTED }", e->line );
				e = e->next;
				*ep = e; /* clip it from rule */

			} else if (e->data == lparen) {
				ns = extractuntil( s, p, e, rparen, &nsc );
				/* above may delete rules following p */
				/* no extra work to be done */
				
				process(ns);

				/* move onward down the list */
				ep = &(e->next);
				e = *ep;

			} else if (e->data == lsquare) {
				ns = extractuntil( s, p, e, rsquare, &nsc );
				/* above may delete rules following p */

				process(ns);
				
				/* make new symbol possibly empty */
				addemptyrule( ns );

				/* move onward down the list */
				ep = &(e->next);
				e = *ep;
	
			} else if (e->data == lcurly) {
				ns = extractuntil( s, p, e, rcurly, &nsc );
				/* above may delete rules following p */

				process(ns);
				
				/* make new symbol possibly iterate */
				makeiterative( ns );

				/* move onward down the list */
				ep = &(e->next);
				e = *ep;

			} else {
				/* move onward down the list */
				ep = &(e->next);
				e = *ep;
			}
		}
		pp = &(p->next);
		p = *pp;
	}
	s->state = TOUCHED;
}

/*
 * The interface
 */

void deebnf() { /* remove Wirth-style EBNF features */
	PSYMBOL s;

	/* initializations */
	lparen  = getdelsym( "\x01(" );
	rparen  = getdelsym( "\x01)" );
	lsquare = getdelsym( "\x01[" );
	rsquare = getdelsym( "\x01]" );
	lcurly  = getdelsym( "\x01{" );
	rcurly  = getdelsym( "\x01}" );

	/* prevent duplicate processing */
	for (s = symlist; s != NULL; s = s->next) s->state = UNTOUCHED;

	/* now process all the symbols */
	s = symlist;
	while (s != NULL) {
		/* for all symbols s */
		if (s->state == UNTOUCHED) process( s );
		/* process may have added to tail of symlist */
		s = s->next;
	}
}
