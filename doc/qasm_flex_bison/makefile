all:
	$(MAKE) grammar
	$(MAKE) lex
	gcc -c compiler/grammar.tab.c compiler/lex.yy.c
	mv *.o compiler
	ar rvs compiler/lexgram.a compiler/grammar.tab.o compiler/lex.yy.o
	g++ -std=c++11 -Wall -Wextra -o testCompiler compiler/main.cpp compiler/lexgram.a -DYYDEBUG=0
	mv testCompiler bin

grammar:
	bison -d -t compiler/grammar.y
	mv grammar.tab.* compiler

lex:
	flex compiler/lex.l
	mv lex.yy.* compiler

runTest:
	$(MAKE) all
	./bin/testCompiler < test/code.qasm