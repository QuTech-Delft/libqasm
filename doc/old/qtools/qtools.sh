# This is a shell archive.  Save it in a file, remove anything before
# this line, and then unpack it by entering "sh file".  Note, it may
# create directories; files and directories will be owned by you and
# have default permissions.
#
# This archive contains:
#
#	README
#	Makefile
#	deebnf.c
#	deempty.c
#	gcopy.c
#	gdeebnf.c
#	gdeempty.c
#	gramstats.c
#	gsample.c
#	gsqueeze.c
#	gstartfollow.c
#	gstats.c
#	reachable.c
#	readg.c
#	samerule.c
#	sample.c
#	squeeze.c
#	startfollow.c
#	writeg.c
#	writetool.c
#	deebnf.h
#	deempty.h
#	grammar.h
#	gramstats.h
#	reachable.h
#	readg.h
#	samerule.h
#	sample.h
#	squeeze.h
#	startfollow.h
#	writeg.h
#	writetool.h
#	bnf.gr
#	ebnf.gr
#	errors.gr
#	gram.gr
#
echo x - README
sed 's/^X//' >README << 'END-of-README'
XREADME
X  grammar tools, a set of tools for manipulating BNF grammars.
X
XThe tools
X  gcopy 	-- copy (and pretty print) a grammar, stdin to stdout
X  gdeebnf	-- remove Wirth-style EBNF extensions
X  gdeempty	-- rewrite grammar to remove empty symbol, if possible
X  gsqueeze	-- like gramcopy but also eliminate redundant rules and symbols
X  gsample	-- output one random string from the grammar, stdin to stdout
X  gstartfollow 	-- output the start set and follow set of all symbols
X  gstats	-- gather basic statistics on grammar, stdin to stdout
X
X  Makefile	-- make one or more of the above, or make with no args for help
X
XSource files for main programs that weave together components
X  gcopy.c	-- gcopy
X  gdeebnf.c	-- gdeebnf
X  gdeempty.c	-- gdeempty
X  gsqueeze.c	-- gsqueeze
X  gsample.c	-- gsample
X  gstartfollow 	-- gstartfollow
X  gstats.c	-- gstats
X
XSource files for components
X  grammar.h	-- framework
X
X  readgram.c	-- read grammar from stdin
X  readgram.h
X  reachable.c	-- mark reachable symbols in the grammar
X  reachable.h
X  writetool.c	-- tools for formatting output to stdout
X  writetool.h
X  writegram.c	-- pretty print grammar to stdout
X  writegram.h
X  gramstats.c	-- statistics gathering mechanism
X  gramstats.h
X  squeezegram.c	-- eliminate redundant production rules and nonterminals
X  squeezegram.h
X  samplegram.c	-- generate a random string from the grammar
X  samplegram.h
X  deempty.c	-- rewrite grammar to remove empty symbol, if possible
X  deempty.h
X  deebnf.c	-- remove Wirth-style EBNF extensions
X  deebnf.h
X
XSome example grammars
X  gram.gr	-- the EBNF grammar of the BNF and EBNF notation used here
X  errors.gr	-- generate as many error messages as possible
X  bnf.gr	-- simple BNF for expressions
X  ebnf.gr	-- extended BNF for expressions
X
XWarranty
X  The gtools code is basically junk, so it is offered here as freeware.
X  You get what you pay for, and if it does anything you consider
X  useful, that is good, but if it breaks, you bear the entire burden  
X  of responsibility for using such junk.
X
XLicense
X  The gtools code is basically junk, so there's no point to dolling it
X  up with fancy license agreements.  Basically, it's free; if you can
X  make something useful from it, you are welcome to assert any rights
X  you wish to your improvements, so long as you credit me (Douglas W.
X  Jones at the University of Iowa) for the foundation on which you
X  built.  Feel free to redistribute this code to anyone, as is or with
X  your additions and improvements.
END-of-README
echo x - Makefile
sed 's/^X//' >Makefile << 'END-of-Makefile'
X# Makefile
X
X###################
X# decoy target gives help message
X
Xhelp:
X	# Specify a make target, one of the grammar processors:
X	# [gstats, gcopy, gsqueeze, gsample, gstartfollow, gdeempty gdeebnf]
X	# make all         -- Makes all of the above.
X	# make clean       -- Deletes all intermediate files.
X	# make veryclean   -- Deletes all executables and intermediate files.
X	# make gtools.shar -- Bundles everything here into a shell archive.
X
X###################
X# top level targets
X
XREADWRITE = readg.o writetool.o writeg.o
X
Xgstats: readg.o reachable.o gramstats.o gstats.o
X	cc -o gstats readg.o reachable.o gramstats.o gstats.o
X
Xgcopy: $(READWRITE) gcopy.o
X	cc -o gcopy $(READWRITE) gcopy.o
X
Xgsqueeze: $(READWRITE) samerule.o squeeze.o gsqueeze.o
X	cc -o gsqueeze $(READWRITE) samerule.o squeeze.o gsqueeze.o
X
Xgsample: readg.o writetool.o sample.o gsample.o
X	cc -o gsample readg.o writetool.o sample.o gsample.o
X
Xgstartfollow: $(READWRITE) reachable.o startfollow.o gstartfollow.o
X	cc -o gstartfollow $(READWRITE) reachable.o startfollow.o gstartfollow.o
X
Xgdeempty: $(READWRITE) samerule.o deempty.o gdeempty.o
X	cc -o gdeempty $(READWRITE) samerule.o deempty.o gdeempty.o
X
Xgdeebnf: $(READWRITE) deebnf.o gdeebnf.o
X	cc -o gdeebnf $(READWRITE) deebnf.o gdeebnf.o
X
X###################
X# main programs (these just bind together components, without any computing)
X
Xgstats.o: gstats.c readg.h gramstats.h grammar.h
X	cc -c gstats.c
X
Xgcopy.o: gcopy.c readg.h writeg.h grammar.h
X	cc -c gcopy.c
X
Xgsqueeze.o: gsqueeze.c readg.h squeeze.h writeg.h grammar.h
X	cc -c gsqueeze.c
X
Xgsample.o: gsample.c readg.h sample.h grammar.h
X	cc -c gsample.c
X
Xgstartfollow.o: gstartfollow.c readg.h startfollow.h writeg.h grammar.h
X	cc -c gstartfollow.c
X
Xgdeempty.o: gdeempty.c readg.h deempty.h writeg.h grammar.h
X	cc -c gdeempty.c
X
Xgdeebnf.o: gdeebnf.c readg.h deebnf.h writeg.h grammar.h
X	cc -c gdeebnf.c
X
X
X###################
X# bottom level components
X
Xreadg.o: readg.c readg.h grammar.h
X	cc -c readg.c
X
Xreachable.o: reachable.c reachable.h grammar.h
X	cc -c reachable.c
X
Xwriteg.o: writeg.c writetool.h writeg.h grammar.h
X	cc -c writeg.c
X
Xwritetool.o: writetool.c writetool.h grammar.h
X	cc -c writetool.c
X
Xgramstats.o: gramstats.c gramstats.h grammar.h
X	cc -c gramstats.c
X
Xsamerule.o: samerule.c samerule.h grammar.h
X	cc -c samerule.c
X
Xsqueeze.o: squeeze.c samerule.h squeeze.h grammar.h
X	cc -c squeeze.c
X
Xsample.o: sample.c writetool.h sample.h grammar.h
X	cc -c sample.c
X
Xstartfollow.o: startfollow.c startfollow.h grammar.h
X	cc -c startfollow.c
X
Xdeempty.o: deempty.c deempty.h readg.h samerule.h grammar.h
X	cc -c deempty.c
X
Xdeebnf.o: deebnf.c deebnf.h readg.h grammar.h
X	cc -c deebnf.c
X
X###################
X# utility 
X
Xall: gstats gcopy gsqueeze gsample gstartfollow gdeempty gdeebnf
X
Xclean:
X	rm -f -v *.o
X
Xveryclean: clean
X	rm -f -v gstats gcopy gsqueeze gsample gstartfollow gdeempty gdeebnf
X
Xgtools.shar: README Makefile *.c *.h *.gr
X	shar README Makefile *.c *.h *.gr > gtools.shar
END-of-Makefile
echo x - deebnf.c
sed 's/^X//' >deebnf.c << 'END-of-deebnf.c'
X/* deebnf.c */
X
X/* written by Douglas Jones, July 2013 */
X
X/* convert Wirth style EBNF to pure BNF
X     <a> ::= <b> [ <c> ] <d> { <e> | <f> } <g>
X   is replaced with
X     <a> ::= <b> <a-a> <d> <a-b> <g>
X     <a-a> ::= <c>
X            |  <empty>
X     <a-b> ::= <e> <a-b>
X            |  <f> <a-b>
X            |  <empty>
X   This is a kluge!  The metasymbols ( ) [ ] { } are parsed as terminals
X   in the grammar by the readgram routine.
X
X*/
X   /* ------------- must carefully examime output on grambad -----*/
X
X#include <stdlib.h>
X#include <stdio.h>
X#include <stdbool.h>
X
X#include "grammar.h"
X#include "readg.h"
X#include "deebnf.h"
X
X/*
X * Global
X */
X
Xstatic PSYMBOL lparen;  /* ( */
Xstatic PSYMBOL rparen;  /* ) */
Xstatic PSYMBOL lsquare; /* [ */
Xstatic PSYMBOL rsquare; /* ] */
Xstatic PSYMBOL lcurly;  /* { */
Xstatic PSYMBOL rcurly;  /* } */
X
X/*
X * Worker routines
X */
X
Xstatic PSYMBOL getdelsym( char * str ) {
X        /* get symbol with name str and delete it from the master symbol list;
X           str must begin with the length of the string, in characters;
X           str must be NULL terminated so error messages work;
X	   the symbol str must terminal; from now on, it is a metasymbol */
X
X	PSYMBOL s; /* the symbol we are looking up */
X	PSYMBOL * pss; /* tools for deletion */
X	PSYMBOL ss;    /* tools for deletion */
X
X	s = lookupsym( str );
X
X	if (s == NULL) return NULL; /* nothing to do here */
X
X	if (s->data != NULL) { /* it's nonterminal? */
X		errormsg( "BRACE SHOULD BE NONTERMINAL", s->data->line );
X	}
X
X	/* now find pss, pointer to s in symlist -- we know it's there */
X	pss = &symlist;
X	ss = *pss;
X	while (ss != s) {
X		/* walk onward in symbol list */
X		pss = &(ss->next);
X		ss = *pss;
X	}
X
X	/* unlink s from symlist */
X	if (s->next == NULL) { /* s was final element of list */
X		symlistend = pss;
X	}
X	*pss = s->next;
X
X	/* done */
X	return s;
X}
X
Xstatic PSYMBOL inventsymbol( PSYMBOL s, int * nsc ) {
X	/* invent and return a new symbol; as initialized, it is a terminal
X	   symbol a unique name derived from the name of s and *nsc.
X	   Add rules to it and it will become nonterminal.
X	*/
X	char newname[SYMLEN];
X	int i;      /* index into newname */
X	STRINGPT j; /* stringpool index */
X	int len;    /* length of symbol s */
X	int dash;   /* location of dash in newname */
X	int ext;    /* name extension */
X	char quote; /* quotation mark at end of name */
X
X	/* first, copy base name of new symbol into place */
X	j = s->name;
X	len = stringtab[j];
X	for (i = 0; i <= len; i++) {
X		newname[i] = stringtab[j];
X		j++;
X	}
X
X	/* figure out what kind of quotes are in use, if any */
X	if ( ((newname[1] == '<')  && (newname[len] == '>'))
X	||   ((newname[1] == '"')  && (newname[len] == '"'))
X	||   ((newname[1] == '\'') && (newname[len] == '\'')) ) {
X		quote = newname[len];
X		dash = len;
X		/* extension will wipe out trailing quote */
X	} else {
X		quote = ' ';
X		dash = len + 1;
X		/* extension will be appended */
X	}
X
X	newname[dash] = '-';
X
X	/* now, try to find a name extension that is not already in use */
X	do {
X		ext = *nsc;
X		*nsc = ext + 1;
X
X		/* create name extension of the form -a or -b */
X		i = dash;
X		do {
X			i = i + 1;
X			newname[i] = (ext % 26) + 'a';
X			ext = ext / 26;
X		} while (ext > 0);
X
X		/* put back the quote that extension overwrote */
X		if (quote != ' ') {
X			i = i + 1;
X			newname[i] = quote;
X		}
X
X		/* set the length to reflect extension etc*/
X		newname[0] = i;
X
X		/* keep trying until new name is genuinely new */
X	} while (lookupsym( newname ) != NULL);
X
X	/* newname is genuinely new */
X	s = definesym( newname );
X	return s;
X}
X
Xstatic PSYMBOL extractuntil( PSYMBOL s, PPRODUCTION p, PELEMENT e,
X			     PSYMBOL rsym, int * nsc ) {
X	/* delete from e->next of production rule p and successors
X	   until an element that references rsym; element e is edited
X	   to refer to a new symbol with a name derived from the name of s
X	   using *nsc and a rule holding the deleted parts, minus the
X	   parentheses.  Returns a pointer to the new symbol, never NULL. */
X
X	PELEMENT ee;    /* the element holding the end paren, if we find it */
X	PELEMENT * eep; /* points to ee, once found, so we can delete it */
X
X	PSYMBOL ns;     /* a new symbol */
X	PPRODUCTION nr; /* the tail of the rule list hanging from ns */
X	PELEMENT ne;    /* first element within parens starts body of rule */
X
X	PSYMBOL lsym;   /* the left brace that balances rsym */
X	int nest;       /* bracket nesting level */
X
X	lsym = e->data; /* we were called with left brace current element */
X
X	/* make new symbol to refer to a new set of rules */
X	ns = inventsymbol( s, nsc );
X	ns->line = e->line;
X
X	/* make the first new rule that will hang under ns */
X	nr = NEWPRODUCTION;
X	nr->line = e->line;
X	nr->state = UNTOUCHED;
X	nr->next = NULL;
X	nr->data = ne;
X
X	/* set up preliminary linkage */
X	ne = e->next;  /* the new rule begins after the opening bracket */
X	nr->data = ne; /* the new rule begins after the opening bracket */
X	ns->data = nr; /* hang the new rule in place */
X	e->data = ns;  /* replace open bracket with new symbol */
X
X	/* look for end paren of set of rules -- eep will point at it */
X	nest = 0;
X	eep = &(nr->data);
X	ee = *eep;
X	for (;;) {
X		if (ee == NULL) {
X			/* we hit the end of a rule, either because of
X			   missing end bracket or bracketed alternatives */
X
X			if (nr->data == NULL) { /* previous rule was empty! */
X				errormsg( "EMPTY BRACKETED RULE", nr->line );
X				/* add empty element to rule if possible */
X				if (emptypt != NULL) {
X					ne = NEWELEMENT;
X					nr->data = ne;
X					ne->line = nr->line;
X					ne->next = NULL;
X					ne->data = emptypt;
X					ne = ee;
X				}
X			}
X
X			if (p->next == NULL) break; /* quit loop */
X
X			/* we have hit ( | and there is another production, so
X			   swipe a rule from p's rule list, hang it under ns */
X			nr->next = p->next;
X			nr = nr->next;
X			p->next = nr->next;
X			nr->next = NULL;
X
X			/* move to next rule looking for end paren */
X			eep = &(nr->data);
X			ee = *eep;
X			ne = ee;
X
X		} else { /* ee != NULL */
X			/* scan for end paren, accounting for nesting */
X			if (ee->data == rsym) {
X				if (nest == 0) break; /* quit loop */
X				nest = nest - 1;
X			} else if (ee->data == lsym) {
X				nest = nest + 1;
X			}
X
X			/* march down this rule looking for end paren */
X			eep = &(ee->next);
X			ee = *eep;
X		}
X	}
X
X	if (ee != NULL) { /* we found the balancing parenthesis */
X		*eep = NULL;        /* body of list does not include rsym */
X		e->next = ee->next; /* snip bracketed body out of rule */
X
X		if (nr->data == NULL) { /* final rule of set was empty! */
X			errormsg( "EMPTY BRACKETED RULE", ee->line );
X			/* add empty element to rule if possible */
X			if (emptypt != NULL) {
X				nr->data = NEWELEMENT;
X				nr->data->line = nr->line;
X				nr->data->next = NULL;
X				nr->data->data = emptypt;
X			}
X		}
X	} else { /* failure, unbalanced parens, treat as end paren */
X		/* assert ee == *eep == NULL */
X		e->next = NULL; /* snip body out of rule */
X
X		if (rsym == rparen) {
X			errormsg( "MISSING )", nr->line );
X		} else if (rsym == rsquare) {
X			errormsg( "MISSING ]", nr->line );
X		} else /* rsym == rcurly */ {
X			errormsg( "MISSING }", nr->line );
X		}
X		/* assert complaint about ne == NULL was already done */
X	}
X	return ns;
X}
X
Xstatic void addemptyrule( PSYMBOL s ) {
X	/* add an empty production to nonterminal s */
X	PPRODUCTION p; /* a new production | <empty> */
X	PELEMENT e;    /* a new element <empty> */
X
X	if (emptypt == NULL) {
X                errormsg( "EMPTY SYMBOL MUST BE DEFINED", s->data->line );
X		return; /* quit if can't add empty rule */
X        }
X	
X	e = NEWELEMENT;
X	e->line = s->data->line; /* take the line number from its first rule */
X	e->next = NULL;
X	e->data = emptypt;
X
X	p = NEWPRODUCTION;
X	p->line = s->data->line; /* take the line number as above */
X	p->next = s->data;
X	p->data = e;
X	p->state = UNTOUCHED;
X	s->data = p;
X}
X
Xstatic void makeiterative( PSYMBOL s ) { /* make nonterminal s iterate */
X	PPRODUCTION p;  /* a production under s */
X	PELEMENT e;	/* an element of p */
X	PELEMENT * pe;  /* the pointer to e */
X	int line;       /* best guess at source line number for empty element */
X
X	/* add a self reference to end of each rule of s */
X	for (p = s->data; p != NULL; p = p->next ) { /* for each production */
X		/* find the end of the production */
X		pe = &(p->data);
X		line = p->line;
X		e = *pe;
X		while (e != NULL) {
X			pe = &(e->next);
X			line = e->line;
X			e = *pe;
X		}
X
X		/* tack on a new element at the end that references s */
X		e = NEWELEMENT;
X		e->line = line;
X		e->next = NULL;
X		e->data = s;
X		*pe = e;
X	}
X	
X	/* add an empty production to s to terminate iteration */
X	addemptyrule( s );
X}
X
Xstatic void process( PSYMBOL s ) { /* process one symbol */
X	PPRODUCTION p;    /* this production rule */
X	PPRODUCTION * pp; /* the pointer to p so we can delete rules */
X	PELEMENT e;       /* this element of p */
X	PELEMENT * ep;    /* the pointer to e so we can delete elements */
X	PSYMBOL ns;       /* a new symbol */
X	int nsc;          /* count used to uniquely name new symbols */
X
X	nsc = 0; /* any added symbols start with -a if possilbe */
X
X	pp = &(s->data);
X	p = *pp;
X	while (p != NULL) {
X		/* for each production rule of symbol, allowing for deletion */
X		ep = &(p->data);
X		e = *ep;
X		while (e != NULL) {
X			/* for each element of rule e */
X			if (e->data == rparen) { /* syntax error */
X				errormsg( "UNEXPECTED )", e->line );
X				e = e->next;
X				*ep = e; /* clip it from rule */
X
X			} else if (e->data == rsquare) { /* syntax error */
X				errormsg( "UNEXPECTED ]", e->line );
X				e = e->next;
X				*ep = e; /* clip it from rule */
X
X			} else if (e->data == rcurly) { /* syntax error */
X				errormsg( "UNEXPECTED }", e->line );
X				e = e->next;
X				*ep = e; /* clip it from rule */
X
X			} else if (e->data == lparen) {
X				ns = extractuntil( s, p, e, rparen, &nsc );
X				/* above may delete rules following p */
X				/* no extra work to be done */
X				
X				process(ns);
X
X				/* move onward down the list */
X				ep = &(e->next);
X				e = *ep;
X
X			} else if (e->data == lsquare) {
X				ns = extractuntil( s, p, e, rsquare, &nsc );
X				/* above may delete rules following p */
X
X				process(ns);
X				
X				/* make new symbol possibly empty */
X				addemptyrule( ns );
X
X				/* move onward down the list */
X				ep = &(e->next);
X				e = *ep;
X	
X			} else if (e->data == lcurly) {
X				ns = extractuntil( s, p, e, rcurly, &nsc );
X				/* above may delete rules following p */
X
X				process(ns);
X				
X				/* make new symbol possibly iterate */
X				makeiterative( ns );
X
X				/* move onward down the list */
X				ep = &(e->next);
X				e = *ep;
X
X			} else {
X				/* move onward down the list */
X				ep = &(e->next);
X				e = *ep;
X			}
X		}
X		pp = &(p->next);
X		p = *pp;
X	}
X	s->state = TOUCHED;
X}
X
X/*
X * The interface
X */
X
Xvoid deebnf() { /* remove Wirth-style EBNF features */
X	PSYMBOL s;
X
X	/* initializations */
X	lparen  = getdelsym( "\x01(" );
X	rparen  = getdelsym( "\x01)" );
X	lsquare = getdelsym( "\x01[" );
X	rsquare = getdelsym( "\x01]" );
X	lcurly  = getdelsym( "\x01{" );
X	rcurly  = getdelsym( "\x01}" );
X
X	/* prevent duplicate processing */
X	for (s = symlist; s != NULL; s = s->next) s->state = UNTOUCHED;
X
X	/* now process all the symbols */
X	s = symlist;
X	while (s != NULL) {
X		/* for all symbols s */
X		if (s->state == UNTOUCHED) process( s );
X		/* process may have added to tail of symlist */
X		s = s->next;
X	}
X}
END-of-deebnf.c
echo x - deempty.c
sed 's/^X//' >deempty.c << 'END-of-deempty.c'
X/* deempty.c */
X
X/* written by Douglas Jones, June 2010,
X   based on pieces of cruncher,
X     written in Pascal by Douglas Jones, March 1990
X     rewritten in C, Jan 2007
X*/
X
X/* Remove empty symbol from grammar.  In this context,
X      |
X      |/ <empty>
X      |
X   Rewrite these rules
X      |
X      |<a> ::= <b> <empty> <c>
X      |<b> ::= <d> | <empty>
X      |
X   as these equivalent rules
X      |
X      |<a> ::= <b> <c>
X      |     |  <c>
X      |<b> ::= <d>
X      |
X */
X
X#include <stdlib.h>
X#include <stdio.h>
X#include <stdbool.h>
X
X#include "grammar.h"
X#include "readg.h"
X#include "samerule.h"
X#include "deempty.h"
X
X/*
X * Markings used to classify production rules and symbols in the grammar
X */
X
X/* Values for states of symbols and rules */
X/*      ISEMPTY    -- already defined */
X/*      CANBEEMPTY -- already defined */
X#define NONEMPTY      TOUCHED
X
X/* rules end up either
X	ISEMPTY    -- all elements symbols are ISEMPTY
X	CANBEEMPTY -- all alements symbols are ISEMPTY or CANBEEMPTY
X	NONEMPTY   -- some element is known to be NONEMPTY
X */
X/* nonterminal symbols end up
X	ISEMPTY    -- all rules of this symbol are ISEMPTY
X	CANBEEMPTY -- some rule is ISEMPTY or CANBEEMPTY, one or more NONEMPTY
X	NONEMPTY   -- all rules are NONEMPTY
X */
X
X/*
X * Global
X */
X
Xstatic bool change; /* record that a change was made to the grammar */
X
X/*
X * Support
X */
X
Xstatic void checkempty( PSYMBOL s ) { /* update emptyness of nonterminal s */
X	PPRODUCTION p;
X	PELEMENT e;
X
X	bool anyempty;     /* are there any empty rules under s */
X	bool anynonempty;  /* are there any non-empty rules under s */
X	bool anycanbe;     /* are there any may-be-empty rules under s */
X	bool anyunknown;   /* are there any unknown rules under s */
X	enum STYPE sstate; /* conclusion about state of symbol s */
X
X	enum STYPE pstate; /* conclusion about state of rule p */
X
X	/* initially we know nothing about productions unmder symbol s */
X	anyempty = false;
X	anynonempty = false;
X	anycanbe = false;
X
X	for (p = s->data; p != NULL; p = p->next) {
X		/* for each production rule p under nonterminal s */
X
X		/* initially assume rule p is empty */
X		/* pstate = ISEMPTY; */
X		pstate = p->state;
X
X		for (e = p->data; e != NULL; e = e->next) {
X			/* for each element e of rule p */
X
X			if(e->data->state == NONEMPTY) {
X				/* p locked in as non-empty by any nonempty e */
X				pstate = NONEMPTY;
X			} else if (e->data->state == CANBEEMPTY) {
X				if (pstate == ISEMPTY) {
X					/* p may not be empty if e may not */
X					pstate = CANBEEMPTY;
X				}
X			} /* else e->data->state == ISEMPTY,
X			        no state change
X			*/
X		}
X
X		if (p->state != pstate) { /* record new state of rule p */
X			p->state = pstate;
X			change = true;
X		}
X
X		/* record impact of above on what we know about symbol s */
X		if (pstate == ISEMPTY) {
X			anyempty = true;
X		} else if (pstate == CANBEEMPTY) {
X			anycanbe = true;
X		} else /* pstate == NONEMPTY */ {
X			anynonempty = true;
X		}
X	}
X
X	/* we have looked at all the productions of s, draw conclusions */
X	if (anyempty && anynonempty) {
X		/* if both possibilities exist, s may be either */
X		sstate = CANBEEMPTY;
X	} else if (anycanbe) {
X		/* if any rule may either, then s may be either */
X		sstate = CANBEEMPTY;
X	} else if (anyempty /* && !anynonempty && !anycanbe */ ) {
X		/* we know they're all empty */
X		sstate = ISEMPTY;
X	} else if (anynonempty /* && !anyempty && !anycanbe */ ) {
X		/* we know they're all non-empty */
X		sstate = NONEMPTY;
X	} else /* should never happen */ {
X		errormsg( "ASSERTION FAILURE IN CHECKEMPTY", -1 );
X		/* if this ever comes up, might help to get source line no */
X	}
X
X	if (s->state != sstate) { /* record new state of symbol s */
X		s->state = sstate;
X		change = true;
X	}
X}
X
Xstatic void dupminus( PSYMBOL s, PPRODUCTION p, PELEMENT e ) {
X	/* duplicate production p minus element e under symbol s
X	   duplicate is guaranteed non-empty, non-redundant,
X	   it becomes the successor production of p */
X	PELEMENT pe;    /* an element of p */
X	PPRODUCTION np; /* the new production */
X	PELEMENT ne;    /* a new element of np */
X	PELEMENT * pne; /* place to hang ne under np */
X	PPRODUCTION sp; /* a successor of p used for duplicate elimination */
X
X	np = NEWPRODUCTION;
X	pne = &(np->data);
X
X	for (pe = p->data; pe != NULL; pe = pe->next) if (pe != e) {
X		/* for all elements of p except e */
X		ne = NEWELEMENT;
X		ne->data = pe->data;
X		*pne = ne;
X		pne = &(ne->next);
X	}
X
X	/* mark end of new rule */
X	*pne = NULL;
X
X	/* avoid inserting duplicates of existing rules */
X	sp = s->data;
X	do {
X		if (samerule( np, sp )) {
X			np->data = NULL;
X			sp = NULL;
X		} else {
X			sp = sp->next;
X		}
X	} while (sp != NULL);
X
X	if (np->data != NULL) { /* if new rule is not empty */
X		/* put new rule onto production list */
X		np->next = p->next;
X		np->state = p->state;
X		p->next = np;
X	}
X}
X
Xstatic void cleanempty( PSYMBOL s ) { /* clean up rules of of nonterminal s */
X	PPRODUCTION * pp;
X	PPRODUCTION p;
X	PELEMENT * pe;
X	PELEMENT e;
X
X	if (s->state == ISEMPTY) return;
X
X	pp = &(s->data);
X	while (*pp != NULL) {
X		/* for all rules of symbol s, with deletions and additions */
X		p = *pp;
X		if (p->state == ISEMPTY) {
X			/* delete this production rule instead of moving on */
X			*pp = p->next;
X
X		} else { /* CANBEEMPTY = NONEMPTY */
X			/* clean up the rule, possibly adding new productions */
X
X			pe = &(p->data);
X			while (*pe != NULL) {
X				/* for all elements of rule p */
X				e = *pe;
X				if (e->data->state == ISEMPTY) {
X					/* delete e from rule p, don't move on*/
X					*pe = e->next;
X
X				} else if (e->data->state == CANBEEMPTY) {
X					/* new copy of rule p minus element e */
X					dupminus( s, p, e );
X
X					/* move on to next production */
X					pe = &(e->next);
X
X				} else { /* NONEMPTY */
X					/* move on to next production */
X					pe = &(e->next);
X
X				}
X			}
X
X			/* move on to next rule */
X			pp = &(p->next);
X		}
X	}
X}
X
X/*
X * The interface
X */
X
Xvoid deempty() { /* eliminate references to the empty symol */
X
X	/* handles used in list traversals */
X	PSYMBOL s;
X	PPRODUCTION p;
X
X	if (emptypt == NULL) {
X		errormsg( "EMPTY SYMBOL MUST BE DEFINED", -1 );
X		return; /* quit if no analysis possible */
X	}
X
X	/* apply initial markings on all symbols */
X	for (s = symlist; s != NULL; s = s->next) {
X		if (TERMINAL(s)) {
X			/* most terminals are nonempty */
X			s->state = NONEMPTY;
X		} else {
X			/* nonterminals and rules are empty until proven not */
X			s->state = ISEMPTY;
X			for (p = s->data; p != NULL; p = p->next) {
X				p->state = ISEMPTY;
X			}
X		}
X	}
X	/* the distinguished empty symbol is the exceptional terminal */
X	emptypt->state = ISEMPTY; 
X
X	do { /* keep trying until no change is made to the grammar */
X		change = false;
X
X		for (s = symlist; s != NULL; s = s->next) if (NONTERMINAL(s)) {
X			/* for each nonterminal symbol s */
X
X			checkempty( s );
X		}
X	} while (change);
X
X	/* now use the markup to rewrite rules accounting for emptyness */
X	for (s = symlist; s != NULL; s = s->next) if (NONTERMINAL(s)) {
X		/* for each nonterminal symbol s */
X
X		cleanempty( s );
X	}
X
X	/* finally, deal with possible empty distinguished symbol */
X	if (head->state == ISEMPTY) {
X		/* eliminate the distinguished symbol and the empty symbol! */
X		head = NULL;
X		emptypt = NULL;
X	} else if (head->state == CANBEEMPTY) {
X		/* we eliminated a bit too much, put it back! */
X		p = NEWPRODUCTION;
X		p->data = NEWELEMENT;
X		p->data->next = NULL;
X		p->data->data = emptypt;
X		p->next = head->data;
X		head->data = p;
X		/* do not eliminate the empty symbol! */
X	} else /* head->state == NONEMPTY */ {
X		/* eliminate the empty symbol! */
X		emptypt = NULL;
X	}
X}
END-of-deempty.c
echo x - gcopy.c
sed 's/^X//' >gcopy.c << 'END-of-gcopy.c'
X/* gcopy.c */
X
X/* written by Douglas Jones, July 2013,
X   based on pieces of cruncher,
X     written in Pascal by Douglas Jones, March 1990
X     rewritten in C, Jan 2007
X*/
X
X/* main program to copy a grammar */
X
X#include "grammar.h"
X#include "readg.h"
X#include "writeg.h"
X
Xint main( int argc, char *argv[] ) {
X	readg();
X 	writeg();
X}
END-of-gcopy.c
echo x - gdeebnf.c
sed 's/^X//' >gdeebnf.c << 'END-of-gdeebnf.c'
X/* gdeebnf.c */
X
X/* written by Douglas Jones, August 2013,
X   based on pieces of cruncher,
X     written in Pascal by Douglas Jones, March 1990
X     rewritten in C, Jan 2007
X*/
X
X/* main program to convert a grammar from EBNF to BNF */
X
X#include "grammar.h"
X#include "readg.h"
X#include "deebnf.h"
X#include "writeg.h"
X
Xint main( int argc, char *argv[] ) {
X	readg();
X	deebnf();
X 	writeg();
X}
END-of-gdeebnf.c
echo x - gdeempty.c
sed 's/^X//' >gdeempty.c << 'END-of-gdeempty.c'
X/* gdeempty.c */
X
X/* written by Douglas Jones, July 2013,
X   based on pieces of cruncher,
X     written in Pascal by Douglas Jones, March 1990
X     rewritten in C, Jan 2007
X*/
X
X/* main program to remove empty symbols from a grammar */
X
X#include "grammar.h"
X#include "readg.h"
X#include "deempty.h"
X#include "writeg.h"
X
Xint main( int argc, char *argv[] ) {
X	readg();
X	deempty();
X 	writeg();
X}
END-of-gdeempty.c
echo x - gramstats.c
sed 's/^X//' >gramstats.c << 'END-of-gramstats.c'
X/* gramstats.c */
X
X/* written by Douglas Jones, June 2010,
X   based on pieces of cruncher,
X     written in Pascal by Douglas Jones, March 1990
X     rewritten in C, Jan 2007
X*/
X
X/* compute statistics about grammar */
X
X#include <stdlib.h>
X#include <stdio.h>
X#include <stdbool.h>
X
X#include "grammar.h"
X#include "reachable.h"
X#include "gramstats.h"
X
X/*
X * The interface
X */
X
Xvoid gramstats() { /* traverse the grammar and get statistics about it */
X
X	/* statistics we're collecting */
X	int symbols;   /* count of symbols in the grammar */
X	int terminals; /* count terminal symbols */
X	int untouched; /* count untouched symbols */
X	int rules;     /* count of production rules in the grammar */
X	int unrules;   /* count of rules hanging from untouched symbols */
X
X	/* handles used in list traversals */
X	PSYMBOL s;
X	PPRODUCTION p;
X
X	/* initialize counters */
X	symbols = 0;
X	terminals = 0;
X	untouched = 0;
X	rules = 0;
X	unrules = 0;
X
X	/* count the symbols */
X	for (s = symlist; s != NULL; s = s->next) {
X		/* for all symbols s */
X
X		symbols++;
X		if (TERMINAL(s)) terminals++;
X
X		/* count the productions hanging from the symbol */
X		for (p = s->data; p != NULL; p = p->next) {
X			/* for all production rules p umder s */
X
X			rules++;
X		}
X	}
X
X	/* touch all symbols reachable from the head */
X	reachsetup();
X	if (head != NULL) reachtouch( head );
X
X	/* count symbols that remain untouched */
X	for (s = symlist; s != NULL; s = s->next) {
X		/* for all symbols s */
X
X		if (s->state == UNTOUCHED) { /* gather statistics */
X			untouched++;
X
X			/* count unused rules for that symbol */
X			for (p = s->data; p != NULL; p = p->next) {
X				/* for all production rules p umder s */
X
X				unrules++;
X			}
X		}
X	}
X
X	/* report the results */
X	printf( " -- Total symbols:        %d\n", symbols );
X	printf( " --   Terminal symbols:   %d\n", terminals );
X	if (untouched != 0) {
X		printf( " --   Extraneous symbols: %d\n", untouched );
X	}
X	printf( " -- Production rules:     %d\n", rules );
X	if (unrules != 0) {
X		printf( " --   Extraneous rules:   %d\n", unrules );
X	}
X}
END-of-gramstats.c
echo x - gsample.c
sed 's/^X//' >gsample.c << 'END-of-gsample.c'
X/* gsample.c */
X
X/* written by Douglas Jones, July 2013,
X   based on pieces of cruncher,
X     written in Pascal by Douglas Jones, March 1990
X     rewritten in C, Jan 2007
X*/
X
X/* main program to sample the strings in a grammar */
X
X#include "grammar.h"
X#include "readg.h"
X#include "sample.h"
X
Xint main( int argc, char *argv[] ) {
X	readg();
X 	sample();
X}
END-of-gsample.c
echo x - gsqueeze.c
sed 's/^X//' >gsqueeze.c << 'END-of-gsqueeze.c'
X/* gqueeze.c */
X
X/* written by Douglas Jones, July 2013,
X   based on pieces of cruncher,
X     written in Pascal by Douglas Jones, March 1990
X     rewritten in C, Jan 2007
X*/
X
X/* main program to copy a grammar */
X
X#include "grammar.h"
X#include "readg.h"
X#include "squeeze.h"
X#include "writeg.h"
X
Xint main( int argc, char *argv[] ) {
X	readg();
X	squeeze();
X 	writeg();
X}
END-of-gsqueeze.c
echo x - gstartfollow.c
sed 's/^X//' >gstartfollow.c << 'END-of-gstartfollow.c'
X/* gstartstop.c */
X
X/* written by Douglas Jones, July 2013,
X   based on pieces of cruncher,
X     written in Pascal by Douglas Jones, March 1990
X     rewritten in C, Jan 2007
X*/
X
X/* main program to compute start and follow sets for a grammar */
X
X#include "grammar.h"
X#include "readg.h"
X#include "writeg.h"
X#include "startfollow.h"
X
Xint main( int argc, char *argv[] ) {
X	readg();
X	startfollow();
X 	writeg();
X}
END-of-gstartfollow.c
echo x - gstats.c
sed 's/^X//' >gstats.c << 'END-of-gstats.c'
X/* gstats.c */
X
X/* written by Douglas Jones, June 2010,
X   based on pieces of cruncher,
X     written in Pascal by Douglas Jones, March 1990
X     rewritten in C, Jan 2007
X*/
X
X/* main program to simply compute statistics about grammar */
X
X#include "grammar.h"
X#include "readg.h"
X#include "gramstats.h"
X
Xint main( int argc, char *argv[] ) {
X	readg();
X 	gramstats();
X}
END-of-gstats.c
echo x - reachable.c
sed 's/^X//' >reachable.c << 'END-of-reachable.c'
X/* reachable.c */
X
X/* written by Douglas Jones, June 2010,
X   based on pieces of cruncher,
X     written in Pascal by Douglas Jones, March 1990
X     rewritten in C, Jan 2007
X*/
X
X/* mark reachable symbols in grammar as TOUCHED */
X
X#include <stdlib.h>
X
X#include "grammar.h"
X#include "reachable.h"
X
X/*
X * The interface
X */
X
Xvoid reachsetup() { /* setup for reachability analysis */
X	PSYMBOL s;
X	for (s = symlist; s != NULL; s = s->next) s->state = UNTOUCHED;
X}
X
Xvoid reachtouch( PSYMBOL s ) { /* recursively touch reachable symbols */
X
X	/* handles used in list traversals */
X	PPRODUCTION p;
X	PELEMENT e;
X	PSYMBOL ss;
X
X	s->state = TOUCHED;
X
X	for (p = s->data; p != NULL; p = p->next) {
X		/* for all production rules p */
X
X		for (e = p->data; e != NULL; e = e->next) {
X			/* for all elements e of rule p, touch the symbol */
X
X			ss = e->data;
X			if (ss->state == UNTOUCHED) reachtouch( ss );
X		}
X	}
X}
END-of-reachable.c
echo x - readg.c
sed 's/^X//' >readg.c << 'END-of-readg.c'
X/* readg.c */
X
X/* written by Douglas Jones, June 2010,
X   based on pieces of cruncher,
X     written in Pascal by Douglas Jones, March 1990
X     rewritten in C, Jan 2007
X*/
X
X/* INPUT FORMAT:
X   Terminal symbols can be quoted (eg '+') and nonterminals can be
X   bracketed as <nonterminal>, although in the processing does not depend
X   on this -- nonterminals are those that appear on the left-hand side of
X   production rules, terminals do not.  Bracketed alphanumeric strings may
X   contain embedded spaces so long as there is no space immediately after
X   the angle bracket (this allows < standing alone as a terminal symbol).
X   Blanks delimit non-quoted or bracketed symbols.  Rules look like:
X      |
X      |<nonterminal> ::= <a> b "c" 'd'
X      |
X   The delimiter ::= was used in the original Algol 60 report.  Lazy users
X   may shorten this to just a colon, just an equals, or the pair colon-equals.
X   Indenting indicated continuation.  Multiple rules that reduce to the
X   same nonterminal may (but need not) be written using the vertical bar
X   separator as:
X      |
X      |<nonterminal> ::= <a> 'b'
X      |               |  <c> 'd'
X      |
X   The distinguished symbol from which the grammar hangs must be documented,
X   since the output is generally done by traversing the grammar starting at
X   this distinguished symbol.  Also, if any symbol is used to stand for an
X   empty string, it too must be documented.  These can be anywhere except
X   in the middle of a rule.
X      |
X      |> <file>
X      |/ <empty>
X      |
X   Comments may be included in the grammar, set off with a poundsign (or
X   a user-selected comment character, as defined in grammar.h
X      |
X      |# this is a comment
X      |
X*/
X
X#include <stdlib.h>
X#include <stdio.h>
X#include <stdbool.h>
X
X#include "grammar.h"
X#include "readg.h"
X
X/*
X * global variables private to this package
X */
X
Xstatic int ch;   /* most recent char read from stdin (could be EOF) */
Xstatic int line; /* current line number on stdin, used in error reports */
X
X/*
X * parsing utility
X */
X
Xbool endlist; /* set by nonblank at end of list, reset when understood */
Xbool endrule; /* set by nonblank at end of rule, reset when understood */
X
Xstatic void newline() { /* advance to next line, called when ch == '\n' */
X	line = line + 1;
X	ch = getchar();
X}
X
Xstatic void skipline() { /* skip the rest of this line */
X	while ((ch != '\n') && (ch != EOF)) {
X		ch = getchar();
X	};
X	if (ch == '\n') newline();
X}
X
Xstatic void skipwhite() { /* simple scan for a nonblank character in ch */
X	while ((ch == '\t') || (ch == ' ')) ch = getchar();
X}
X
Xstatic void nonblank() { /* fancy scan for a nonblank character in ch */
X	while ((ch == '|')
X	||     (ch == ' ')
X	||     (ch == '\t')
X	||     (ch == '\n')
X	||     (ch == EOF)) {
X		if (ch =='|') {
X			endlist = true;
X			ch = getchar();
X			return;
X		} else if (ch == EOF) {
X			endrule = true;
X			endlist = true;
X			return;
X		} else if (ch == '\n') {
X			newline();
X			if ((ch != ' ') && (ch != '\t')) {
X				/* line starts with nonblank */
X				endrule = true;
X				endlist = true;
X				return;
X			}
X		} else { /* must have been blank or tab */
X			ch = getchar();
X		}
X	}
X	return;
X}
X
Xstatic PSYMBOL lookupordefine( char * str ) {
X	/* lookup str in the main symbol list, and add it if required */
X
X	PSYMBOL * ps = &symlist; /* reference to current symbol */
X	PSYMBOL s; /* current symbol */
X        int j,k; /* character indices */
X	PSYMBOL m;
X
X	s = lookupsym( str );
X
X	if (s != NULL) return s;
X	return definesym( str );
X}
X
Xstatic void extendsym( int * len, char * str, char ch ) {
X	/* str[ *len ] = ch; *len++ */
X	if (*len >= SYMLEN) {
X		errormsg( "SYMBOL TOO LONG", line );
X	} else {
X		*len = *len + 1;
X		str[*len] = ch;
X	}
X}
X
Xstatic PSYMBOL getsymbol() { /* get symbol from input to str */
X
X	char str[SYMLEN+1]; /* most recent symbol from stdin */
X	int len;            /* index of last used space in str */
X	/* string length is encoded in str[0] */
X
X 	/* Must be called with ch nonblank, first char of symbol */
X	len = 1;
X	str[len] = ch;
X
X	if (ch == '<') { /* may be a < quoted symbol */
X
X		ch = getchar();
X		if ( ((ch <= 'z') && (ch >= 'a'))
X		||   ((ch <= 'Z') && (ch >= 'A'))
X		||   ((ch <= '9') && (ch >= '0')) ) { /* definitely < quoted */
X
X			for (;;) { /* consume bracketed symbol */
X				extendsym( &len, str, ch );
X				if (ch == '>') break;
X				ch = getchar();
X				if (ch == '\n') break;
X				if (ch == EOF) break;
X			}
X			if (ch == '>') { /* normal end of symbol */
X				ch = getchar(); /* skip trailing > */
X			} else { /* abnormal end of symbol */
X				errormsg( "MISSING CLOSING > MARK", line );
X
X				/* fake it */
X				extendsym( &len, str, '>' );
X			}
X
X		} else { /* symbol ends at next blank (broadly speaking) */
X
X			for (;;) { /* symbol */
X				extendsym( &len, str, ch );
X				ch = getchar();
X				if (ch == ' ') break;
X				if (ch == '\t') break;
X				if (ch == '\n') break;
X				if (ch == EOF) break;
X			}
X		}
X
X	} else if ((ch == '"') || (ch == '\'')) { /* quoted */
X
X		ch = getchar();
X		while ((ch != str[1]) && (ch != '\n') && (ch != EOF)) {
X			extendsym( &len, str, ch );
X			ch = getchar();
X		}
X		if (ch == str[1]) {
X			extendsym( &len, str, ch );
X			ch = getchar();
X		} else {
X			errormsg( "MISSING CLOSING QUOTE", line );
X
X			/* fake it */
X			extendsym( &len, str, str[1] );
X		}
X
X	} else { /* symbol did not begin with < or quote, ends with space */
X
X		ch = getchar();
X		while ((ch != ' ')
X		&&     (ch != '\t')
X		&&     (ch != '\n')
X		&&     (ch != EOF )) {
X			extendsym( &len, str, ch );
X			ch = getchar();
X		}
X	}
X
X	/* we now have have a symbol in str[1..len] ! */
X	str[0] = len; /* record symbol length */
X	return lookupordefine( str );
X}
X
Xstatic PELEMENT getsymlist() { /* get the list of symbols on RHS of rule */
X	PELEMENT s;
X
X	nonblank();
X	if (endlist) {
X		endlist = false;
X		return NULL;
X	} else {
X		s = NEWELEMENT;
X		s->line = line;
X		s->data = getsymbol();
X		s->next = getsymlist();
X		return s;
X	}
X}
X
Xstatic PPRODUCTION getprod() { /* get a list of production rules */
X 	PPRODUCTION ph; /* the head of the production list */
X 	PPRODUCTION p;  /* the current production */
X 	PPRODUCTION np; /* the new production */
X
X	ph = NULL;
X	p = NULL;
X	do {
X		np = NEWPRODUCTION;
X		np->line = line;
X		nonblank();
X		if (!endlist) { /* the normal case */
X			np->data = getsymlist();
X
X		} else { /* nothing there */
X			errormsg( "EMPTY PRODUCTION RULE", np->line );
X			if (emptypt != NULL) {
X				np->data = NEWELEMENT;
X				np->data->line = line;
X				np->data->next = NULL;
X				np->data->data = emptypt;
X			} else {
X				np->data = NULL;
X			}
X			endlist = false;
X		}
X
X		/* link it in place */
X		if (ph == NULL) ph = np;
X		if (p != NULL) {
X			p->next = np;
X		}
X		p = np;
X	} while (!endrule);
X	endrule = false;
X	endlist = false;
X	p->next = NULL;
X	return ph;
X}
X
X/*
X * The interface
X */
X
XPSYMBOL lookupsym( char * str ) {
X	/* lookup str in the main symbol list, return NULL if not found */
X	/* str[0] is length of symbol, in characters */
X
X	PSYMBOL s;
X        int i;      /* character index in str */
X        STRINGPT j; /* character index in stringtab */
X        int len;    /* length of str */
X
X	len = str[0];
X	for (s = symlist; s != NULL; s = s->next) {
X		i = 0;
X		j = s->name;
X		while (str[i] == stringtab[j]) {
X			i = i+1;
X			j = j+1;
X		}
X		if (i > len) return s; /* we found it */
X		/* the above works because length is encoded as first byte */
X	}
X	/* we only get here if we don't find a symbol */
X	return NULL;
X}
X
Xvoid errormsg( char * msg, int line ) {
X	/* output a error message msg attributed to the given source line */
X	/* use -1 as a line number if line attribution does not work */
X
X	fputs( " >>", stderr );
X	fputs( msg, stderr );
X	if (line > 0) {
X		fprintf( stderr, " on line %d", line );
X	}
X	fputs( "<<\n", stderr );
X}
X
XPSYMBOL definesym( char * str ) {
X	/* define str in the main symbol list, it must not be already there */
X
X	PSYMBOL s;
X        int i;      /* character index in str */
X        int len;    /* length of string str */
X
X	/* create the symbol itself and link it into place */
X	s = NEWSYMBOL;
X	*symlistend = s;
X	s->line = line;
X	s->name = stringlim;
X	s->data = NULL;
X	s->state = UNTOUCHED;
X	s->starter = NULL;
X	s->follows = NULL;
X	s->next = NULL;
X	symlistend = &(s->next);
X
X	/* copy the characters of the symbol name into place */
X	len = str[0];
X	for (i = 0; i <= len; i++) {
X		if (stringlim >= STRINGLIMIT) {
X			errormsg( "STRING POOL OVERVLOW", line );
X		} else {
X			stringtab[stringlim] = str[i];
X			stringlim = stringlim+1;
X		}
X	}
X	return s;
X}
X
Xvoid readg() { /* read grammar into global grammar structure in grammar.h */
X	PSYMBOL s;
X	PPRODUCTION p;
X	bool ok;
X
X	/* global initialization */
X	stringlim = 0;  /* no characters have been put in stringtab */
X	symlist = NULL; /* no symbols have been encountered */
X	symlistend = &symlist;
X	head = NULL;    /* we have no distinguished symbol */
X	emptypt = NULL; /* we have no empty symbol */
X
X	/* prime the input stream */
X	line = 1;
X	ch = getchar();
X	endlist = false;
X	endrule = false;
X
X	while (ch != EOF) {
X		while (ch == '\n') newline();
X		if (ch == '>') { /* Identify distinguished symbol */
X			if (head != NULL) {
X				errormsg( "EXTRA DISTINGUISHED SYMBOL", line );
X			} else {
X				ch = getchar(); /* skip > */
X				skipwhite();
X				if ((ch == '\n') || (ch == EOF)) {
X					errormsg( "NO DISTINGUISHED SYMBOL",
X						line );
X				} else {
X					head = getsymbol();
X				}
X			}
X			skipline();
X
X		} else if (ch == '/') { /* Identify the empty (/)symbol */
X			if (emptypt != NULL) {
X				errormsg( "EXTRA EMPTY SYMBOL", line );
X				skipline();
X			} else {
X				ch = getchar(); /* skip */
X				skipwhite();
X				if ((ch == '\n') || (ch == EOF)) {
X					errormsg( "NO EMPTY SYMBOL", line );
X				} else {
X					emptypt = getsymbol();
X				}
X			}
X			skipline();
X
X		} else if (ch == COMMENT) { /* COMMENT */
X			skipline();
X
X		} else if (ch != EOF) { /*WE MIGHT HAVE A RULE*/
X			s = getsymbol();
X			skipwhite();
X
X			ok = false;
X			if (ch == ':') {  /* consume ::= or := or : or = */
X				ok = true;
X				ch = getchar();
X				if (ch == ':') {
X					ch = getchar();
X					if (ch == '=') {
X						ch = getchar();
X					}
X				} else if (ch == '=') {
X					ch = getchar();
X				}
X			} else if (ch == '=') {
X				ok = true;
X				ch = getchar();
X			}
X
X			if (ok) {  /* WE HAVE A RULE s ::= rule */
X				p = s->data;
X				if (p == NULL) {
X					s->data = getprod();
X				} else {
X					while (p->next != NULL) p = p->next;
X					p->next = getprod();
X				}
X			} else { /* NOT A RULE, JUST s ...comment */
X				errormsg( "MISSING ::= OR EQUIVALENT", line );
X				skipline();
X			}
X		}
X	}
X
X	if (head == NULL) {
X		errormsg( "DISTINGUISHED SYMBOL NOT GIVEN", -1 );
X	} else if (TERMINAL(head)) {
X		errormsg( "DISTINGUISHED SYMBOL IS TERMINAL", head->line );
X	}
X	if ((emptypt != NULL) && (NONTERMINAL(emptypt))) {
X		errormsg( "EMPTY SYMBOL IS NONTERMINAL", emptypt->data->line );
X	}
X	line = -1; /* mark any new line numbers as fictional */
X}
END-of-readg.c
echo x - samerule.c
sed 's/^X//' >samerule.c << 'END-of-samerule.c'
X/* samerule.c */
X
X/* written by Douglas Jones, July 2010,
X   based on pieces of cruncher,
X     written in Pascal by Douglas Jones, March 1990
X     rewritten in C, Jan 2007
X*/
X
X/* compare two produciton rules */
X
X#include "stdlib.h"
X#include "stdbool.h"
X
X#include "grammar.h"
X#include "samerule.h"
X
X/*
X * Interface
X */
X
Xbool samerule( PPRODUCTION p, PPRODUCTION q ) { /* compare rules p,q */
X	PELEMENT pe, qe; /* pointers to elements of p and q */
X
X	/* for successive elements */
X	pe = p->data;
X	qe = q->data;
X	while ((pe != NULL) && (qe != NULL)) {
X		if (pe->data != qe->data) return false; /* they differ */
X		pe = pe->next;
X		qe = qe->next;
X	}
X	return ((pe == NULL) && (qe == NULL)); /* identical and same length */
X}
END-of-samerule.c
echo x - sample.c
sed 's/^X//' >sample.c << 'END-of-sample.c'
X/* sample.c */
X
X/* written by Douglas Jones, July 2010,
X   based on pieces of cruncher,
X     written in Pascal by Douglas Jones, March 1990
X     rewritten in C, Jan 2007
X*/
X
X/* Output a random derivation from the grammar */
X
X#include <stdlib.h>
X#include <stdio.h>
X
X#include "grammar.h"
X#include "writetool.h"
X#include "sample.h"
X
X/*
X * printing utility
X */
X
Xstatic void outsym( PSYMBOL s ); /* forward declaration */
X
Xstatic void outprod( PPRODUCTION p ) { /* put the symbols on RHS of rule p */
X        PELEMENT e;
X	PSYMBOL s;
X
X	if (p != NULL) { /* empty rules should never happen, but be safe */
X		e = p->data;
X		while (e != NULL) {
X			s = e->data;
X			if (s != emptypt) outsym( s );
X
X			e = e->next;
X		}
X	}
X}
X
Xstatic void outsym( PSYMBOL s ) { /* output a symbol or pick a rule */
X	PPRODUCTION p;
X	int pcount;
X	int pnum;
X
X	if (TERMINAL(s)) {
X		outspacesym( s, 1, ' ' );
X		outsymbol( s );
X	} else { /* nonterminal symbol */
X
X		/* how many alternatives are there? */
X		pcount = 0;
X		for (p = s->data; p != NULL; p = p->next) pcount++;
X
X		/* pick an alternative */
X		p = s->data;
X		for (pnum = random() % pcount; pnum > 0; pnum--) {
X			p = p->next;
X		}
X
X		/* output that alternative */
X		outprod( p );
X	}
X}
X
X/*
X * The interface
X */
X
Xvoid sample() { /* write a sample string generated by grammar */
X	outsetup();
X	srandom( time( NULL ) );
X	if (head != NULL) { /* there is a distinguished symbol */
X		outsym( head );
X	}
X	outline();
X}
END-of-sample.c
echo x - squeeze.c
sed 's/^X//' >squeeze.c << 'END-of-squeeze.c'
X/* squeeze.c */
X
X/* written by Douglas Jones, July 2010,
X   based on pieces of cruncher,
X     written in Pascal by Douglas Jones, March 1990
X     rewritten in C, Jan 2007
X*/
X
X/* try to minimize grammar by squeezing out redundant rules and symbols
X     <a> ::= <b>
X          |  x <e>
X          |  x <e>    -- squeezerules gets rid of this
X     <b> ::= y <c>    -- squeezesymbols gets rid of this
X   is replaced with
X     <a> ::= y <c>
X          |  x <e>
X*/
X
X#include <stdlib.h>
X#include <stdio.h>
X#include <stdbool.h>
X
X#include "grammar.h"
X#include "samerule.h"
X#include "squeeze.h"
X
X/*
X * Global
X */
X
Xstatic bool change; /* record that a change was made to the grammar */
X
X/*
X * Worker routines
X */
X
Xstatic void squeezesymbols(){ /* squeeze out redundant symbols */
X	/* handles used in list traversals */
X	PSYMBOL s;
X	PPRODUCTION p;
X	PELEMENT e;
X	PSYMBOL s1;
X	PPRODUCTION p1;
X	PELEMENT e1, e2;
X
X	/* for all symbols */
X	for (s = symlist; s != NULL; s = s->next ) {
X
X		/* for all production rules of that symbol */
X		for (p = s->data; p != NULL; p = p->next ) {
X
X			/* for all elements of that production rule */
X			for (e = p->data; e != NULL; e = e->next ) {
X
X				s1 = e->data;
X				p1 = s1->data;
X				if ((p1 != NULL) && (p1->next == NULL)) {
X					/* symbol s1 has just 1 rule p1
X					   substitute that rule for s1 in p */
X
X					e1 = p1->data;
X					if (e1 != NULL) { /* if well formed */
X						/* firse element of rule
X						   overwrites first element */
X						e->data = e1->data;
X						change = true;
X
X						/* now copy rest of rule */
X						e1 = e1->next;
X						while (e1 != NULL) {
X							e2 = NEWELEMENT;
X							e2->data = e1->data;
X							e2->next = e->next;
X							e->next = e2;
X							e = e2;
X							e1 = e1->next;
X						}
X					}
X				}
X			}
X		}
X	}
X}
X
Xstatic void squeezerules(){ /* squeeze out redundant production rules */
X	/* handles used in list traversals */
X	PSYMBOL s;
X	PPRODUCTION p;
X	PPRODUCTION * qp; /* pointer to q */
X	PPRODUCTION q;
X
X	/* for all symbols */
X	for (s = symlist; s != NULL; s = s->next ) {
X
X		/* for all productions */
X		for (p = s->data; p != NULL; p = p->next ) {
X
X			/* for all additional productions of symbol s */
X			qp = &(p->next);
X			q = *qp; /* that is, q = p->next */
X			while (q != NULL) {
X				if (samerule( p, q )) {
X					/* rule q is redundant, eliminate it */
X					*qp = q->next;
X					change = true;
X				} else {
X					/* move to next production */
X					qp = &(q->next);
X				}
X				q = *qp;
X			}
X		}
X	}
X}
X
X/*
X * The interface
X */
X
Xvoid squeeze() { /* squeeze out redundant rules and symbols */
X
X	/* count the symbols and setup for reachability analysis */
X	do {
X		change = false;
X		squeezerules();
X		squeezesymbols();
X	} while (change);
X}
END-of-squeeze.c
echo x - startfollow.c
sed 's/^X//' >startfollow.c << 'END-of-startfollow.c'
X/* startfollow.c */
X
X/* written by Douglas Jones, July 2010,
X   based on pieces of cruncher,
X     written in Pascal by Douglas Jones, March 1990
X     rewritten in C, Jan 2007
X*/
X
X/* traverse grammar and build start set and follow set for each nonterminal */
X
X#include <stdlib.h>
X#include <stdio.h>
X#include <stdbool.h>
X
X#include "grammar.h"
X#include "reachable.h"
X#include "startfollow.h"
X
X/*
X * Global
X */
X
Xstatic bool changed; /* has a symbol been added */
X
X/*
X * support routines for managing lists of symbols
X */
X
Xstatic void addsym( PELEMENT * es, PSYMBOL s) {
X	/* add symbol s as an element of the set *es */
X
X	while ((*es != NULL) && ((*es)->data != s)) es = &((*es)->next);
X	if (*es == NULL) { /* APPEND SYMBOL TO LIST */
X		*es = NEWELEMENT;
X		(*es)->next = NULL;
X		(*es)->data = s;
X		changed = true;
X	}
X}
X
Xstatic void addsymbols( PELEMENT * head, PELEMENT e ) {
X	/* union all symbol in set e into the set *head */
X
X	while (e != NULL) {
X		addsym( head, e->data );
X		e = e->next;
X	}
X}
X
X/*
X * package to find start set for each nonterminal
X */
X
Xstatic void dosymbol(PSYMBOL s) { /* process one symbol (or tree of symbols) */
X	PPRODUCTION p;
X
X	if (TERMINAL(s)) { /* terminals are their own start set */
X		addsym( &(s->starter), s );
X	} else { /* nonterminal start sets are union of sets for each rule */
X		for (p = s->data; p != NULL; p = p->next) {
X			addsymbols( &(s->starter), p->data->data->starter );
X		}
X	}
X}
X
Xstatic void getstarts() {
X	/* compute start sets of all symbols:
X	   each terminal symbol is made into its own start set
X	   nonterminals get start sets determine by each rule */
X	PSYMBOL s;
X
X	/* repeat the experiment until no additions to any start set */
X	do {
X		changed = false;
X
X		/* build start sets */
X		for (s = symlist; s != NULL; s = s->next) if(s->state==TOUCHED){
X			dosymbol( s );
X		}
X	} while (changed);
X}
X
X/*
X * package to find follow set for each nonterminal
X */
X
Xstatic void getfollowrule( PPRODUCTION p ) {
X	/* each element of a rule provides follow set for predecessor */
X	PELEMENT e;	/* the current element */
X	PELEMENT pe;	/* the previous element in this rule */
X	PSYMBOL es;	/* the symbol referenced by e */
X	PSYMBOL pes;	/* the symbol referenced by pe */
X
X	pe = p->data;
X	if (pe != NULL) {;
X		pes = pe->data;
X		for (e = pe->next; e != NULL; e = e->next) {
X			es = e->data;
X			/* for every pair of consecutive elements pe e
X			   where es and pes are coresponding symbols */
X
X			/* don't test terminality of es or pes */
X			addsymbols( &(pes->follows), es->starter );
X
X			pe = e;
X			pes = es;
X		}
X	}
X}
X
Xstatic void pushfollow( PPRODUCTION p, PSYMBOL s ) {
X	/* follow set of nonterminal s applies to produciton rule p under s */
X	PELEMENT e;	/* the current element */
X
X	e = p->data;
X	if (e != NULL) { /* should always be true */
X		while (e->next != NULL) e = e->next;
X		/* e is now last element of rule p */
X
X		addsymbols( &(e->data->follows), s->follows );
X	}
X}
X
Xstatic void getfollows() { /* compute follow sets of all nonterminals */
X	PSYMBOL s;
X	PPRODUCTION p;
X
X	/* first pass:  get local info on follow set from within rules */
X	for (s = symlist; s != NULL; s = s->next) if (s->state == TOUCHED) {
X		for (p = s->data; p != NULL; p = p->next) {
X			/* for each production p hanging from every rule s */
X
X			/* within rules, infer follow sets from start sets */
X			getfollowrule( p );
X		}
X	}
X
X	/* second pass:  push follow set down from each symbol */
X	do {
X		changed = false;
X		for (s = symlist; s != NULL; s = s->next) if(s->state==TOUCHED){
X			for (p = s->data; p != NULL; p = p->next) {
X				/* for each production p of every symbol s */
X
X				/* follow set of s is follow set of end of p */
X				pushfollow( p, s );
X			}
X		}
X	} while (changed == true);
X}
X
X/*
X * The interface
X */
X
Xvoid startfollow() { /* compute start set and follow set of each nonterminal */
X	/* first learn what part of the grammar is reachable */
X	reachsetup();
X	if (head != NULL) reachtouch( head );
X
X	/* then do the work */
X	getstarts();
X	getfollows();
X}
END-of-startfollow.c
echo x - writeg.c
sed 's/^X//' >writeg.c << 'END-of-writeg.c'
X/* writeg.c */
X
X/* written by Douglas Jones, July 2010,
X   based on pieces of cruncher,
X     written in Pascal by Douglas Jones, March 1990
X     rewritten in C, Jan 2007
X*/
X
X/* OUTPUT FORMAT:
X   The distinguished symbol from which the grammar hangs is always output
X   first, followed the designated empty symbol, if one was specified.
X      |
X      |> <distinguished>
X      |/ <empty>
X      |
X   Terminal and nonterminal symbols are preserved unchanged, in whatever
X   form they were input.  Rules are output as follows:
X      |
X      |<nonterminal> ::= <a> 'b' <c> 'd'
X      |                  <continuation>
X      |
X   The delimiter ::= was used in the original Algol 60 report.  Indenting
X   indicated continuation.  Multiple rules that reduce to the same
X   nonterminal are output in succession, using the vertical bar to separate
X   successive rules instead of re-stating the lefthand side of the rule.
X      |
X      |<nonterminal> ::= <a> 'b'
X      |               |  <c> 'd'
X      |
X   If the start set and follow set of a nonterminal have been computed, these
X   are included in the output grammar as comments after all of the rules for
X   that nonterminal.
X      |
X      |<nonterminal> ::= <a> 'b'
X      |               |  <c> 'd'
X      |\ start:  'a' 'c'
X      |\ follow: 'g' 'h'
X      |
X   At the end, unreachable symbols and unused production rules are listed.
X*/
X
X#include <stdlib.h>
X#include <stdio.h>
X#include <stdbool.h>
X
X#include "grammar.h"
X#include "writetool.h"
X#include "writeg.h"
X
X/*
X * global variables private to this package
X */
X
Xstatic int barcol; /* column number for vertical bar */
Xstatic int contcol; /* column number for continuation */
X
X/*
X * printing utility
X */
X
Xstatic void outprod( PPRODUCTION p ) { /* put out RHS of rule p */
X        PELEMENT e;
X	PSYMBOL s;
X
X	if (p != NULL) { /* empty rules should never happen, but be safe */
X		for (e = p->data; e != NULL; e = e->next) {
X			s = e->data;
X			outspacesym( s, contcol, ' ' );
X			outsymbol( s );
X		}
X	}
X}
X
Xstatic void outprodgroup( PSYMBOL s ) { /* put out all rules with s on LHS */
X	PPRODUCTION p;
X	PELEMENT e;
X	PSYMBOL ss;
X
X	outline();
X	outsymbol( s );
X	outchar( ' ' );
X	barcol = getoutcol() + 1; /* remember indent for next rule */
X	outstring( RULESYM );
X        contcol = getoutcol() + 1; /* remember indent for continuation */
X
X	/* output first production on same line */
X	p = s->data;
X	outprod( p );
X
X	while (p->next != NULL) { /* output successive productions */
X		p = p->next;
X		
X		outline();
X		outspaces( barcol );
X		outstring( "| " );
X
X		outprod( p );
X	}
X
X	if (s->starter != NULL) { /* output start set */
X		outline();
X		outchar( COMMENT );
X		outstring( " start set:  " );
X        	contcol = getoutcol() + 1; /* remember indent */
X
X		for (e = s->starter; e != NULL; e = e->next) {
X			ss = e->data;
X			outspacesym( ss, contcol, COMMENT );
X			outsymbol( ss );
X		}
X	}
X	if (s->follows != NULL) { /* output follow set */
X		outline();
X		outchar( COMMENT );
X		outstring( " follow set: " );
X        	contcol = getoutcol() + 1; /* remember indent */
X
X		for (e = s->follows; e != NULL; e = e->next) {
X			ss = e->data;
X			outspacesym( ss, contcol, COMMENT );
X			outsymbol( ss );
X		}
X	}
X	if ((s->follows != NULL) || (s->starter != NULL)) {
X		/* output blank line to separate from the next rule */
X		outline();
X	}
X}
X
Xstatic void outreachable( PSYMBOL s ) {
X	/* recursively output reachable production*/
X
X        /* handles used in list traversals */
X        PPRODUCTION p;
X        PELEMENT e;
X        PSYMBOL ss;
X
X	/* only output nonterminals with their rules */
X	if (s->data != NULL) outprodgroup( s );
X
X	/* mark that it is printed */
X        s->state = TOUCHED;
X
X        /* recursive walk through all of its productions */
X        p = s->data;
X        while (p != NULL) {
X                /* walk through all elements of each production */
X                e = p->data;
X                while (e != NULL) {
X                        /* touch the associated symbol */
X                        ss = e->data;
X                        if (ss->state == UNTOUCHED) outreachable( ss );
X
X                        /* move to next element */
X                        e = e->next;
X                }
X
X                /* move to next rule */
X                p = p->next;
X        }
X}
X
X/*
X * The interface
X */
X
Xvoid writeg() { /* write grammar structure documented in grammar.h */
X	PSYMBOL s;
X	bool header;
X
X	outsetup();
X
X	if (head != NULL) { /* there is a distinguished symbol */
X		outstring( "> " );
X		outsymbol( head );
X	} else {
X		outchar( COMMENT );
X		outstring( " no distinguished symbol!" );
X	}
X
X	if (emptypt != NULL) { /* there is an empty symbol */
X		outline();
X		outstring( "/ " );
X		outsymbol( emptypt );
X	}
X
X	for (s = symlist; s != NULL; s = s->next) s->state = UNTOUCHED;
X	if (head != NULL) {
X		outline();
X		outreachable( head );
X	}
X
X	header = false;
X	for (s = symlist; s != NULL; s = s->next) {
X		if ((s->data == NULL) && (s->state == TOUCHED)) {
X			if (!header) {
X				outline();
X				outline();
X				outchar( COMMENT );
X				outstring( " terminals:  " );
X				contcol = getoutcol() + 1; /* remember indent */
X				header = true;
X			}
X			outspacesym( s, contcol, COMMENT );
X			outsymbol( s );
X		}
X	}
X
X	header = false;
X	for (s = symlist; s != NULL; s = s->next) {
X		if ((s->data != NULL) && (s->state == UNTOUCHED)) {
X			if (!header) {
X				outline();
X				outline();
X				outchar( COMMENT );
X				outstring( " unused productions" );
X				header = true;
X			}
X			outprodgroup( s );
X		}
X	}
X
X	header = false;
X	for (s = symlist; s != NULL; s = s->next) {
X		if ((s->data == NULL) && (s->state == UNTOUCHED)) {
X			if (!header) {
X				outline();
X				outline();
X				outchar( COMMENT );
X				outstring( " unused terminals: " );
X				contcol = getoutcol() + 1; /* remember indent */
X				header = true;
X			}
X			outspacesym( s, contcol, COMMENT );
X			outsymbol( s );
X		}
X	}
X	outline();
X}
END-of-writeg.c
echo x - writetool.c
sed 's/^X//' >writetool.c << 'END-of-writetool.c'
X/* writetool.c */
X
X/* written by Douglas Jones, July 2010,
X   based on pieces of cruncher,
X     written in Pascal by Douglas Jones, March 1990
X     rewritten in C, Jan 2007
X*/
X
X/* Tools for output of elements from a grammar */
X
X#include <stdlib.h>
X#include <stdio.h>
X
X#include "grammar.h"
X#include "writetool.h"
X
X/*
X * global variables private to this package
X */
X
Xstatic int column; /* last column number filled on line */
X
X/*
X * The interface
X */
X
Xvoid outsetup() { /* setup for package use */
X	column = 0;
X}
X
Xint getoutcol() { /* note what column we are on */
X	return column;
X}
X
Xvoid outline() { /* put out newline */
X	putchar( '\n' );
X	column = 0;
X}
X
Xvoid outchar( char ch ) { /* put out one character */
X	putchar( ch );
X	column++;
X}
X
Xvoid outstring( char * p ) { /* put out one null-terminated string */
X	if (p != NULL) { /* insurance */
X		while (*p != '\0') {
X			outchar( *p );
X			p++;
X		}
X	}
X}
X
Xvoid outspaces( int c ) { /* put out spaces until column = c */
X	while (column < c) outchar( ' ' );
X}
X
Xvoid outspacesym( PSYMBOL s, int c, char ch ) {
X	/* put out a space,
X	   or if s won't fit, return to column c starting the line with ch */
X
X	int len; /* length of s, in chars */
X        STRINGPT pos; /* positiion of s in stringtab */
X
X	pos = s->name;
X	len = stringtab[pos]; /* string length is encoded in first char */
X
X	/* does s fit on the line? */
X	if ((column + 1 + len) > 80) { /* no, move to next line */
X		outline();
X		if (c > 1) outchar( ch );
X		outspaces( c );
X	} else { /* yes, output just one space */
X		outchar( ' ' );
X	}
X}
X
Xvoid outsymbol( PSYMBOL s ) { /* put symbol to output */
X	int len; /* length of s, in chars */
X        STRINGPT pos; /* positiion of s in stringtab */
X
X	pos = s->name;
X	len = stringtab[pos];
X
X	while (len != 0) {
X		len = len - 1;
X		pos = pos + 1;
X		outchar( stringtab[pos] );
X	}
X}
END-of-writetool.c
echo x - deebnf.h
sed 's/^X//' >deebnf.h << 'END-of-deebnf.h'
X/* deebnf.c */
X
X/* written by Douglas Jones, July 2013 */
X
X/* convert Wirth style EBNF to pure BNF
X     <a> ::= <b> [ <c> ] <d> { <e> } <f>
X   is replaced with
X     <a> ::= <b> <q01q> <d> <q02q> <f>
X     <q01q> ::= <c>
X             |  <empty>
X     <q02q> ::= <e>
X             |  <e> <q02q>
X             |  <empty>
X   This is a kluge!  The metasymbols ( ) [ ] { } are parsed as terminals
X   in the grammar by the readgram routine.
X*/
X
X/*
X * The interface
X */
X
Xvoid deebnf(); /* remove Wirth-style EBNF features */
END-of-deebnf.h
echo x - deempty.h
sed 's/^X//' >deempty.h << 'END-of-deempty.h'
X/* deempty.h */
X
X/* written by Douglas Jones, July 2010,
X   based on pieces of cruncher,
X     written in Pascal by Douglas Jones, March 1990
X     rewritten in C, Jan 2007
X*/
X
X/* Remove empty symbol from grammar. */
X
X/*
X * The interface
X */
X
Xvoid deempty(); /* eliminate references to the empty symol */
END-of-deempty.h
echo x - grammar.h
sed 's/^X//' >grammar.h << 'END-of-grammar.h'
X/* grammar.h */
X
X/* written by Douglas Jones, June 2010,
X   based on pieces of cruncher,
X     written in Pascal by Douglas Jones, March 1990
X     rewritten in C, Jan 2007
X*/
X
X/* all data structures used to represent the grammar are here
X   there is no use of serious data abstraction in this code */
X
X/*
X * Configuration constants
X */
X
X/* Limit on the number of nonterminal characters */
X#define STRINGLIMIT 10000
X
X/* Limit on the number of chars in any symbol (T or NT) */
X#define SYMLEN 50
X
X/* Configurable syntactic details */
X#define COMMENT '#'
X#define RULESYM "::="
X
X/* note:  RULESYM is for writeg, while readg accepts any of :, =, := or ::= */
X
X/*
X * Types
X */
X
X/* Handle on strings in the string table (0 .. STRINGLIMIT) */
Xtypedef int STRINGPT;
X
X/* Types of terminal symbols */
Xenum STYPE {
X	ISEMPTY,	/* known to be equivalent to the empty symbol */
X	CANBEEMPTY,	/* may be empty, marks optional syntactic elements */
X	TOUCHED,	/* is reachable from the root of the grammar */
X	UNTOUCHED,	/* has not yet been reached from the root */
X	STYPES		/* NOT A TYPE, rather, the number of types permitted */
X};
X
X/* Pointer types */
Xtypedef struct symbol *     PSYMBOL;
Xtypedef struct production * PPRODUCTION;
Xtypedef struct element *    PELEMENT;
X
Xstruct symbol {
X	STRINGPT name;    /* symbols have names */
X	PSYMBOL next;     /* symbols may occur in lists of symbols */
X	PPRODUCTION data; /* the head of the list of productions */
X	enum STYPE state; /* the state of this symbol */
X	PELEMENT starter; /* the head of the terminal list in the start set */
X	PELEMENT follows; /* the head of the terminal list in the follow set */
X	int line;         /* source line number on which symbol first seen */
X};
X
Xstruct production {
X	PPRODUCTION next; /* each symbol has a linked list of productions */
X	PELEMENT data;    /* the body of a production is a list of elements */
X	enum STYPE state; /* the state of the production rule */
X	PELEMENT starter; /* the head of the terminal list in the start set */
X	PELEMENT ender;   /* the head of the terminal list in the follow set */
X	int line;         /* source line number on which production starts */
X};
X
Xstruct element {
X	PELEMENT next;    /* each production is a list of elements */
X	PSYMBOL data;     /* an element is a handle on a symbol, never NULL */
X	int line;         /* source line number on which element occurs */
X};
X
X/*
X * storage allocators
X */
X
X#define NEWSYMBOL     (PSYMBOL) malloc(sizeof(struct symbol))
X#define NEWPRODUCTION (PPRODUCTION) malloc(sizeof(struct production))
X#define NEWELEMENT    (PELEMENT) malloc(sizeof(struct element))
X
X/*
X * predicates
X */
X
X#define TERMINAL(s)    ((s)->data == NULL)
X#define NONTERMINAL(s) ((s)->data != NULL)
X
X/*
X * global variables
X */
X
X/* the tring table */
Xchar     stringtab[STRINGLIMIT];
XSTRINGPT stringlim; /* lowest free element in the string table */
X
X/* head and address of null pointer for the main list of all symbols */
XPSYMBOL  symlist;
XPSYMBOL * symlistend;
X
XPSYMBOL  emptypt;   /* Identity of the empty symbol */
XPSYMBOL  head;      /* Identity of the distinguished symbol in the grammar */
END-of-grammar.h
echo x - gramstats.h
sed 's/^X//' >gramstats.h << 'END-of-gramstats.h'
X/* gramstats.h */
X
X/* written by Douglas Jones, June 2010,
X   based on pieces of cruncher,
X     written in Pascal by Douglas Jones, March 1990
X     rewritten in C, Jan 2007
X*/
X
Xvoid gramstats(); /* traverse the grammar and get statistics about it */
END-of-gramstats.h
echo x - reachable.h
sed 's/^X//' >reachable.h << 'END-of-reachable.h'
X/* reachable.h */
X
X/* written by Douglas Jones, June 2010,
X   based on pieces of cruncher,
X     written in Pascal by Douglas Jones, March 1990
X     rewritten in C, Jan 2007
X*/
X
X/* mark reachable symbols in grammar as TOUCHED */
X
X/*
X * The interface
X */
X
Xvoid reachsetup(); /* setup for reachability analysis */
X
Xvoid reachtouch( PSYMBOL s ); /* recursively touch reachable symbols */
END-of-reachable.h
echo x - readg.h
sed 's/^X//' >readg.h << 'END-of-readg.h'
X/* readg.h */
X
X/* written by Douglas Jones, June 2010,
X   based on pieces of cruncher,
X     written in Pascal by Douglas Jones, March 1990
X     rewritten in C, Jan 2007
X*/
X
X/*
X * The interface
X */
X
Xvoid errormsg( char * msg, int line );
X	/* output a error message msg attributed to the given source line */
X	/* use -1 as a line number if line attribution does not work */
X
XPSYMBOL lookupsym( char * str );
X	/* lookup str in the main symbol list, return NULL if not found */
X	/* str[0] is length of the symbol, in chars */
X
XPSYMBOL definesym( char * str );
X	/* define str in the main symbol list, it must not be already there */
X
Xvoid readg(); /* read grammar into global grammar structure in grammar.h */
X	      /* see readgram.c for the syntax used to express grammars */
END-of-readg.h
echo x - samerule.h
sed 's/^X//' >samerule.h << 'END-of-samerule.h'
X/* samerule.h */
X
X/* written by Douglas Jones, July 2010,
X   based on pieces of cruncher,
X     written in Pascal by Douglas Jones, March 1990
X     rewritten in C, Jan 2007
X*/
X
X/* compare two produciton rules */
X
X/*
X * Interface
X */
X
Xbool samerule( PPRODUCTION p, PPRODUCTION q ); /* compare rules p,q */
END-of-samerule.h
echo x - sample.h
sed 's/^X//' >sample.h << 'END-of-sample.h'
X/* sample.h */
X
X/* written by Douglas Jones, July 2010,
X   based on pieces of cruncher,
X     written in Pascal by Douglas Jones, March 1990
X     rewritten in C, Jan 2007
X*/
X
X/* Output a random derivation from the grammar */
X
X/*
X * The interface
X */
X
Xvoid sample(); /* write a sample string generated by grammar */
END-of-sample.h
echo x - squeeze.h
sed 's/^X//' >squeeze.h << 'END-of-squeeze.h'
X/* squeeze.h */
X
X/* written by Douglas Jones, June 2010,
X   based on pieces of cruncher,
X     written in Pascal by Douglas Jones, March 1990
X     rewritten in C, Jan 2007
X*/
X
X/*
X * The interface
X */
X
Xvoid squeeze(); /* squeeze out redundant rules and symbols */
X	        /* see squeezegram.c for the changes it makes */
END-of-squeeze.h
echo x - startfollow.h
sed 's/^X//' >startfollow.h << 'END-of-startfollow.h'
X/* startfollow.h */
X
X/* written by Douglas Jones, July 2010,
X   based on pieces of cruncher,
X     written in Pascal by Douglas Jones, March 1990
X     rewritten in C, Jan 2007
X*/
X
X/* traverse grammar and build start set and follow set for each nonterminal */
X
X/*
X * The interface
X */
X
Xvoid startfollow(); /* compute start set and follow set of each nonterminal */
END-of-startfollow.h
echo x - writeg.h
sed 's/^X//' >writeg.h << 'END-of-writeg.h'
X/* writeg.h */
X
X/* written by Douglas Jones, July 2010,
X   based on pieces of cruncher,
X     written in Pascal by Douglas Jones, March 1990
X     rewritten in C, Jan 2007
X*/
X
X/*
X * The interface
X */
X
Xvoid writeg(); /* write grammar structure documented in grammar.h */
END-of-writeg.h
echo x - writetool.h
sed 's/^X//' >writetool.h << 'END-of-writetool.h'
X/* writetool.h */
X
X/* written by Douglas Jones, July 2010,
X   based on pieces of cruncher,
X     written in Pascal by Douglas Jones, March 1990
X     rewritten in C, Jan 2007
X*/
X
X/* Tools for output of elements from a grammar */
X
X/*
X * The interface
X */
X
Xvoid outsetup(); /* setup for package use */
X
Xint getoutcol(); /* note what column we are on */
X
Xvoid outline(); /* put out newline */
X
Xvoid outchar( char ch ); /* put out one character */
X
Xvoid outstring( char * p ); /* put out one null-terminated string */
X
Xvoid outspaces( int c ); /* put out spaces until column = c */
X
Xvoid outspacesym( PSYMBOL s, int c, char ch );
X        /* put out a space, 
X           or if s won't fit, return to column c starting the line with ch */
X
Xvoid outsymbol( PSYMBOL s ); /* put symbol to output */
END-of-writetool.h
echo x - bnf.gr
sed 's/^X//' >bnf.gr << 'END-of-bnf.gr'
X# bnf.gr -- an example classical BNF grammar for expressions
X> <expression>
X<expression> ::= <term> | <expression> + <term> | <expression> - <term>
X<term> ::= <factor> | <term> * <factor> | <term> / <factor>
X<factor> ::= <element> | - <element>
X<element> ::= <number> | <identifier> | ( <expression> )
END-of-bnf.gr
echo x - ebnf.gr
sed 's/^X//' >ebnf.gr << 'END-of-ebnf.gr'
X# ebnf.gr -- an example extended BNF grammar for expressions
X> expression
X/ ''
Xexpression = term { ( '+' | '-' ) term }
Xterm = factor { ( '*' | '/' ) factor }
Xfactor = [ '-' ] ( number | identifier | '(' expression ')' )
END-of-ebnf.gr
echo x - errors.gr
sed 's/^X//' >errors.gr << 'END-of-errors.gr'
X# errors.gr
X# grammar full of errors, trying to evoke as many error
X# messages as possible from one source file.
X
X# The first batch relates to to readg.c
X/ <reempty>
X/ <empty>
X/
X<reempty> ::= <anything
X>
X> <rehead>
X> <head>
X<head>
X<head> <tail>
X<head> ::=
X<head> ::= something else
X 	|
X# the following errors relate to deebnf.c
X) ::= 'something'
X<tail> ::= [ ]
X<tail> ::= [ | <head> ]
X<tail> ::= <extra a> )
X	|  <extra b> ]
X	|  <extra c> }
X	|  ( <miss a>
X	|  { <miss c>
X	|  ( | <anything> )
X	|  [ <miss b>
X'something' ::= (
X
END-of-errors.gr
echo x - gram.gr
sed 's/^X//' >gram.gr << 'END-of-gram.gr'
X# gram.gr
X# the grammar for grammars supported by the gtools package
X# note that the .gr ending is a convenience, grammars are
X# plain text files
X
X> grammar   the start or sentence symbol of this grammar
X/ empty     the empty symbol of this grammar
X
Xgrammar = { metarule | rule | comment }
X
X# In EBNF, curly braces denote zero or more repetitions.
X# In BNF, vertical bars separate alternatives.
X
Xmetarule = ( '>' | '/' ) spaces symbol anything newline
X
X# Metarules specify the head and empty symbols (if any)
X# There may never be more than one of each.
X
Xspaces = { space }
Xspace = ' ' | tab
X
X# Zero or more spaces or tabs may separate the metarule
X# symbol from the initial character.
X
Xsymbol = '<' alphanumeric { anything-but-rbrack } '>'
X       | '"' { anything-but-dquote } '"'
X       | "'" { anything-but-squote } "'"
X       | anything-but-quote { nonblank }
X
X# Symbols may be enclosed in <angle brackets> or "quoted"
X# in single or double quotes, or they may consist of any
X# sequence of characters followed by a blank, so long as
X# the first character is not a quote.
X
X# The restriction on <angle brackets> that the first
X# bracketed symbol must be alphanumeric allows symbols
X# such as < <= <- and <> to be used without quotation
X# but it prevents < > from being treated as a bracketed
X# blank.  As presented, this grammar is ambiguous; we can
X# informally fix this by saying that the first rule
X# encountered applies in cases where two rules both match;
X# a formal fix is left as an exercise.
X
X# By convention, <symbol> is a nonterminal symbol; if this
X# convention is followed strictly, no quotes are needed
X# around most terminal symbols.
X
X# By convention, 'symbol' is a terminal symbol; if this
X# convention is followed strictly, no angle brackets are
X# needed around nonterminal symbols.
X
X# Neither of the above conventions is enforced here.
X
Xanything = nonblank | space
Xnonblank = alphanumeric | punctuation
Xalphanumeric = alpha | numeric
Xalpha = a-z | A-Z
Xnumeric = 0-9
Xpunctuation = '>' | '"' | "'" | other-nonalpha
Xanything-but-quote = alphanumeric | '>' | other-nonalpha | spaces
Xanything-but-rbrack = alphanumeric | '"' | "'" | other-nonalpha | spaces
Xanything-but-dquote = "'" | anything-but-quote
Xanything-but-squote = '"' | anything-but-quote
X
X# The above syntactic categories are messy but self explanatory.
X# A more formal treatment of the sets of characters would require
X# enumerating the full character set, something that is extremely
X# difficult in this age of Unicode.
X
X# Note that, more by accident than by design, UTF-8 characters
X# above 0070 end up classified as other-nonalphanumeric.
X
Xrule = lhs ( ':' | '=' | ':=' | '::=' ) rhs newline
X
X# Production rules may use any of several separators
X# between the righthand side and the lefthand side:
X# ::= -- as in conventional BNF
X# :=  -- a lazy variant of the above
X# =   -- as used in Wirth's version of EBNF for Pascal
X# :   -- sometimes encountered
X
X# each rule ends with a newline
X
Xlhs = symbol spaces
X
X# The occurance of a symbol on the left-hand side of a rule
X# defines that symbol to be nonterminal; any symbol that is
X# not used on the left-hand side of any rule is terminal.
X
Xrhs = alternative { continuation '|' alternative }
Xcontinuation = { { newline } space spaces }
X
X# The right-hand side consists of a sequence of alternatives
X# separated by vertical bars, which may be broken across
X# multiple lines so long as each nonblank continuation line
X# begins with at least one space character (blank or tab).
X
Xalternative = { continuation } item { [ continuation ] item }
X
X# Each alternative consists of a list of one or more items,
X# possibly continued on successive lines.
X
Xitem = symbol
X
X# In plain BNF, each item is a symbol and the symbols ( { [
X# ] } and ) have no special meaning.  If these are needed as
X# terminal symbols in EBNF, they must be quoted because of the
X# following added rules:
X
Xitem = '(' rhs ')'
X     | '[' rhs ']'
X     | '{' rhs '}'
X
X# In extended BNF or EBNF, items may be entire right-hand
X# sides enclosed in ( parentheses ), for grouping, [ square
X# braces ], indicating optional groups, or { curly braces }
X# indicating zero or more repetitions of the indicated group.
X
X# Note that EBNF adds no additional expressive power to the
X# grammar -- every EBNF grammar may be mechanically
X# transformed to a plain BNF grammar, for example, by using
X# the gdeebnf tool provided here.
X
X# Note that two different rules may begin with the same lhs
X# (the same symbol).  Each such rule merely adds its list
X# of alternatives to that symbol.
X
Xcomment = '#' { anything } newline
X
X# Comments, as you might have guessed, begin with a pound
X# sign, and end with a newline.
END-of-gram.gr
exit