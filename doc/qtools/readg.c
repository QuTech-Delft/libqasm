/* readg.c */

/* written by Douglas Jones, June 2010,
   based on pieces of cruncher,
     written in Pascal by Douglas Jones, March 1990
     rewritten in C, Jan 2007
*/

/* INPUT FORMAT:
   Terminal symbols can be quoted (eg '+') and nonterminals can be
   bracketed as <nonterminal>, although in the processing does not depend
   on this -- nonterminals are those that appear on the left-hand side of
   production rules, terminals do not.  Bracketed alphanumeric strings may
   contain embedded spaces so long as there is no space immediately after
   the angle bracket (this allows < standing alone as a terminal symbol).
   Blanks delimit non-quoted or bracketed symbols.  Rules look like:
      |
      |<nonterminal> ::= <a> b "c" 'd'
      |
   The delimiter ::= was used in the original Algol 60 report.  Lazy users
   may shorten this to just a colon, just an equals, or the pair colon-equals.
   Indenting indicated continuation.  Multiple rules that reduce to the
   same nonterminal may (but need not) be written using the vertical bar
   separator as:
      |
      |<nonterminal> ::= <a> 'b'
      |               |  <c> 'd'
      |
   The distinguished symbol from which the grammar hangs must be documented,
   since the output is generally done by traversing the grammar starting at
   this distinguished symbol.  Also, if any symbol is used to stand for an
   empty string, it too must be documented.  These can be anywhere except
   in the middle of a rule.
      |
      |> <file>
      |/ <empty>
      |
   Comments may be included in the grammar, set off with a poundsign (or
   a user-selected comment character, as defined in grammar.h
      |
      |# this is a comment
      |
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "grammar.h"
#include "readg.h"

/*
 * global variables private to this package
 */

static int ch;   /* most recent char read from stdin (could be EOF) */
static int line; /* current line number on stdin, used in error reports */

/*
 * parsing utility
 */

bool endlist; /* set by nonblank at end of list, reset when understood */
bool endrule; /* set by nonblank at end of rule, reset when understood */

static void newline() { /* advance to next line, called when ch == '\n' */
	line = line + 1;
	ch = getchar();
}

static void skipline() { /* skip the rest of this line */
	while ((ch != '\n') && (ch != EOF)) {
		ch = getchar();
	};
	if (ch == '\n') newline();
}

static void skipwhite() { /* simple scan for a nonblank character in ch */
	while ((ch == '\t') || (ch == ' ')) ch = getchar();
}

static void nonblank() { /* fancy scan for a nonblank character in ch */
	while ((ch == '|')
	||     (ch == ' ')
	||     (ch == '\t')
	||     (ch == '\n')
	||     (ch == EOF)) {
		if (ch =='|') {
			endlist = true;
			ch = getchar();
			return;
		} else if (ch == EOF) {
			endrule = true;
			endlist = true;
			return;
		} else if (ch == '\n') {
			newline();
			if ((ch != ' ') && (ch != '\t')) {
				/* line starts with nonblank */
				endrule = true;
				endlist = true;
				return;
			}
		} else { /* must have been blank or tab */
			ch = getchar();
		}
	}
	return;
}

static PSYMBOL lookupordefine( char * str ) {
	/* lookup str in the main symbol list, and add it if required */

	PSYMBOL * ps = &symlist; /* reference to current symbol */
	PSYMBOL s; /* current symbol */
        int j,k; /* character indices */
	PSYMBOL m;

	s = lookupsym( str );

	if (s != NULL) return s;
	return definesym( str );
}

static void extendsym( int * len, char * str, char ch ) {
	/* str[ *len ] = ch; *len++ */
	if (*len >= SYMLEN) {
		errormsg( "SYMBOL TOO LONG", line );
	} else {
		*len = *len + 1;
		str[*len] = ch;
	}
}

static PSYMBOL getsymbol() { /* get symbol from input to str */

	char str[SYMLEN+1]; /* most recent symbol from stdin */
	int len;            /* index of last used space in str */
	/* string length is encoded in str[0] */

 	/* Must be called with ch nonblank, first char of symbol */
	len = 1;
	str[len] = ch;

	if (ch == '<') { /* may be a < quoted symbol */

		ch = getchar();
		if ( ((ch <= 'z') && (ch >= 'a'))
		||   ((ch <= 'Z') && (ch >= 'A'))
		||   ((ch <= '9') && (ch >= '0')) ) { /* definitely < quoted */

			for (;;) { /* consume bracketed symbol */
				extendsym( &len, str, ch );
				if (ch == '>') break;
				ch = getchar();
				if (ch == '\n') break;
				if (ch == EOF) break;
			}
			if (ch == '>') { /* normal end of symbol */
				ch = getchar(); /* skip trailing > */
			} else { /* abnormal end of symbol */
				errormsg( "MISSING CLOSING > MARK", line );

				/* fake it */
				extendsym( &len, str, '>' );
			}

		} else { /* symbol ends at next blank (broadly speaking) */

			for (;;) { /* symbol */
				extendsym( &len, str, ch );
				ch = getchar();
				if (ch == ' ') break;
				if (ch == '\t') break;
				if (ch == '\n') break;
				if (ch == EOF) break;
			}
		}

	} else if ((ch == '"') || (ch == '\'')) { /* quoted */

		ch = getchar();
		while ((ch != str[1]) && (ch != '\n') && (ch != EOF)) {
			extendsym( &len, str, ch );
			ch = getchar();
		}
		if (ch == str[1]) {
			extendsym( &len, str, ch );
			ch = getchar();
		} else {
			errormsg( "MISSING CLOSING QUOTE", line );

			/* fake it */
			extendsym( &len, str, str[1] );
		}

	} else { /* symbol did not begin with < or quote, ends with space */

		ch = getchar();
		while ((ch != ' ')
		&&     (ch != '\t')
		&&     (ch != '\n')
		&&     (ch != EOF )) {
			extendsym( &len, str, ch );
			ch = getchar();
		}
	}

	/* we now have have a symbol in str[1..len] ! */
	str[0] = len; /* record symbol length */
	return lookupordefine( str );
}

static PELEMENT getsymlist() { /* get the list of symbols on RHS of rule */
	PELEMENT s;

	nonblank();
	if (endlist) {
		endlist = false;
		return NULL;
	} else {
		s = NEWELEMENT;
		s->line = line;
		s->data = getsymbol();
		s->next = getsymlist();
		return s;
	}
}

static PPRODUCTION getprod() { /* get a list of production rules */
 	PPRODUCTION ph; /* the head of the production list */
 	PPRODUCTION p;  /* the current production */
 	PPRODUCTION np; /* the new production */

	ph = NULL;
	p = NULL;
	do {
		np = NEWPRODUCTION;
		np->line = line;
		nonblank();
		if (!endlist) { /* the normal case */
			np->data = getsymlist();

		} else { /* nothing there */
			errormsg( "EMPTY PRODUCTION RULE", np->line );
			if (emptypt != NULL) {
				np->data = NEWELEMENT;
				np->data->line = line;
				np->data->next = NULL;
				np->data->data = emptypt;
			} else {
				np->data = NULL;
			}
			endlist = false;
		}

		/* link it in place */
		if (ph == NULL) ph = np;
		if (p != NULL) {
			p->next = np;
		}
		p = np;
	} while (!endrule);
	endrule = false;
	endlist = false;
	p->next = NULL;
	return ph;
}

/*
 * The interface
 */

PSYMBOL lookupsym( char * str ) {
	/* lookup str in the main symbol list, return NULL if not found */
	/* str[0] is length of symbol, in characters */

	PSYMBOL s;
        int i;      /* character index in str */
        STRINGPT j; /* character index in stringtab */
        int len;    /* length of str */

	len = str[0];
	for (s = symlist; s != NULL; s = s->next) {
		i = 0;
		j = s->name;
		while (str[i] == stringtab[j]) {
			i = i+1;
			j = j+1;
		}
		if (i > len) return s; /* we found it */
		/* the above works because length is encoded as first byte */
	}
	/* we only get here if we don't find a symbol */
	return NULL;
}

void errormsg( char * msg, int line ) {
	/* output a error message msg attributed to the given source line */
	/* use -1 as a line number if line attribution does not work */

	fputs( " >>", stderr );
	fputs( msg, stderr );
	if (line > 0) {
		fprintf( stderr, " on line %d", line );
	}
	fputs( "<<\n", stderr );
}

PSYMBOL definesym( char * str ) {
	/* define str in the main symbol list, it must not be already there */

	PSYMBOL s;
        int i;      /* character index in str */
        int len;    /* length of string str */

	/* create the symbol itself and link it into place */
	s = NEWSYMBOL;
	*symlistend = s;
	s->line = line;
	s->name = stringlim;
	s->data = NULL;
	s->state = UNTOUCHED;
	s->starter = NULL;
	s->follows = NULL;
	s->next = NULL;
	symlistend = &(s->next);

	/* copy the characters of the symbol name into place */
	len = str[0];
	for (i = 0; i <= len; i++) {
		if (stringlim >= STRINGLIMIT) {
			errormsg( "STRING POOL OVERVLOW", line );
		} else {
			stringtab[stringlim] = str[i];
			stringlim = stringlim+1;
		}
	}
	return s;
}

void readg() { /* read grammar into global grammar structure in grammar.h */
	PSYMBOL s;
	PPRODUCTION p;
	bool ok;

	/* global initialization */
	stringlim = 0;  /* no characters have been put in stringtab */
	symlist = NULL; /* no symbols have been encountered */
	symlistend = &symlist;
	head = NULL;    /* we have no distinguished symbol */
	emptypt = NULL; /* we have no empty symbol */

	/* prime the input stream */
	line = 1;
	ch = getchar();
	endlist = false;
	endrule = false;

	while (ch != EOF) {
		while (ch == '\n') newline();
		if (ch == '>') { /* Identify distinguished symbol */
			if (head != NULL) {
				errormsg( "EXTRA DISTINGUISHED SYMBOL", line );
			} else {
				ch = getchar(); /* skip > */
				skipwhite();
				if ((ch == '\n') || (ch == EOF)) {
					errormsg( "NO DISTINGUISHED SYMBOL",
						line );
				} else {
					head = getsymbol();
				}
			}
			skipline();

		} else if (ch == '/') { /* Identify the empty (/)symbol */
			if (emptypt != NULL) {
				errormsg( "EXTRA EMPTY SYMBOL", line );
				skipline();
			} else {
				ch = getchar(); /* skip */
				skipwhite();
				if ((ch == '\n') || (ch == EOF)) {
					errormsg( "NO EMPTY SYMBOL", line );
				} else {
					emptypt = getsymbol();
				}
			}
			skipline();

		} else if (ch == COMMENT) { /* COMMENT */
			skipline();

		} else if (ch != EOF) { /*WE MIGHT HAVE A RULE*/
			s = getsymbol();
			skipwhite();

			ok = false;
			if (ch == ':') {  /* consume ::= or := or : or = */
				ok = true;
				ch = getchar();
				if (ch == ':') {
					ch = getchar();
					if (ch == '=') {
						ch = getchar();
					}
				} else if (ch == '=') {
					ch = getchar();
				}
			} else if (ch == '=') {
				ok = true;
				ch = getchar();
			}

			if (ok) {  /* WE HAVE A RULE s ::= rule */
				p = s->data;
				if (p == NULL) {
					s->data = getprod();
				} else {
					while (p->next != NULL) p = p->next;
					p->next = getprod();
				}
			} else { /* NOT A RULE, JUST s ...comment */
				errormsg( "MISSING ::= OR EQUIVALENT", line );
				skipline();
			}
		}
	}

	if (head == NULL) {
		errormsg( "DISTINGUISHED SYMBOL NOT GIVEN", -1 );
	} else if (TERMINAL(head)) {
		errormsg( "DISTINGUISHED SYMBOL IS TERMINAL", head->line );
	}
	if ((emptypt != NULL) && (NONTERMINAL(emptypt))) {
		errormsg( "EMPTY SYMBOL IS NONTERMINAL", emptypt->data->line );
	}
	line = -1; /* mark any new line numbers as fictional */
}
