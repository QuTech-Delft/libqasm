all:
	$(MAKE) grammar
	$(MAKE) lex
	g++ -c -std=c++11 library/grammar.tab.c library/lex.yy.c
	mv *.o library
	ar rvs library/lexgram.a library/grammar.tab.o library/lex.yy.o
	g++ -std=c++11 -Wall -Wextra -o testCompiler library/main.cpp library/lexgram.a -DYYDEBUG=0
	mv testCompiler bin

grammar:
	bison -d -t library/grammar.y
	mv grammar.tab.* library

lex:
	flex library/lex.l
	mv lex.yy.* library

runTest:
	$(MAKE) all
	./bin/testCompiler < test/code.qasm