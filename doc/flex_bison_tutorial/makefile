all:
	$(MAKE) grammar
	$(MAKE) lex
	gcc -c compiler/grammar.tab.c compiler/lex.yy.c
	mv *.o compiler
	ar rvs compiler/lexgram.a compiler/grammar.tab.o compiler/lex.yy.o
	g++ -std=c++11 -Wall -Wextra -o testCompiler compiler/main.cpp compiler/lexgram.a
	mv testCompiler bin

grammar:
	bison -d compiler/grammar.y
	mv grammar.tab.* compiler

lex:
	flex compiler/lex.lex
	mv lex.yy.* compiler

runTest:
	$(MAKE) all
	./bin/testCompiler < test/code.qasm