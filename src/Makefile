all:
#	$(MAKE) grammar
#	$(MAKE) lex
#	g++ -c -std=c++11 library/grammar.tab.c library/lex.yy.c
#	mv *.o library
#	ar rvs library/lexgram.a library/grammar.tab.o library/lex.yy.o
#	g++ -std=c++11 -Wall -Werror -Wextra -o testCompiler library/main.cpp library/lexgram.a -DYYDEBUG=0
#	mv testCompiler bin
	$(MAKE) testing

grammar:
	bison -Wall -d -t library/grammar.y -g -r all
	mv grammar.tab.* library

lex:
	flex library/lex.l
	mv lex.yy.* library

testing:
	./bin/testCompiler test/test.qc && ./bin/testCompiler test/bell.qasm && ./bin/testCompiler test/example2.qasm && ./bin/testCompiler test/example3.qasm && ./bin/testCompiler test/grover.qasm && ./bin/testCompiler test/example7.qasm

runTest:
	$(MAKE) all
	$(MAKE) testing

clean:
	rm -rvf build cbuild dist libQasm libQasm.egg-info

