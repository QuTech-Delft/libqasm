/* deebnf.c */

/* written by Douglas Jones, July 2013 */

/* convert Wirth style EBNF to pure BNF
     <a> ::= <b> [ <c> ] <d> { <e> } <f>
   is replaced with
     <a> ::= <b> <q01q> <d> <q02q> <f>
     <q01q> ::= <c>
             |  <empty>
     <q02q> ::= <e>
             |  <e> <q02q>
             |  <empty>
   This is a kluge!  The metasymbols ( ) [ ] { } are parsed as terminals
   in the grammar by the readgram routine.
*/

/*
 * The interface
 */

void deebnf(); /* remove Wirth-style EBNF features */
