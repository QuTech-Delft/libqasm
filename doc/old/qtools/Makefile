# Makefile

###################
# decoy target gives help message

help:
	# Specify a make target, one of the grammar processors:
	# [gstats, gcopy, gsqueeze, gsample, gstartfollow, gdeempty gdeebnf]
	# make all         -- Makes all of the above.
	# make clean       -- Deletes all intermediate files.
	# make veryclean   -- Deletes all executables and intermediate files.
	# make gtools.shar -- Bundles everything here into a shell archive.

###################
# top level targets

READWRITE = readg.o writetool.o writeg.o

gstats: readg.o reachable.o gramstats.o gstats.o
	cc -o gstats readg.o reachable.o gramstats.o gstats.o

gcopy: $(READWRITE) gcopy.o
	cc -o gcopy $(READWRITE) gcopy.o

gsqueeze: $(READWRITE) samerule.o squeeze.o gsqueeze.o
	cc -o gsqueeze $(READWRITE) samerule.o squeeze.o gsqueeze.o

gsample: readg.o writetool.o sample.o gsample.o
	cc -o gsample readg.o writetool.o sample.o gsample.o

gstartfollow: $(READWRITE) reachable.o startfollow.o gstartfollow.o
	cc -o gstartfollow $(READWRITE) reachable.o startfollow.o gstartfollow.o

gdeempty: $(READWRITE) samerule.o deempty.o gdeempty.o
	cc -o gdeempty $(READWRITE) samerule.o deempty.o gdeempty.o

gdeebnf: $(READWRITE) deebnf.o gdeebnf.o
	cc -o gdeebnf $(READWRITE) deebnf.o gdeebnf.o

###################
# main programs (these just bind together components, without any computing)

gstats.o: gstats.c readg.h gramstats.h grammar.h
	cc -c gstats.c

gcopy.o: gcopy.c readg.h writeg.h grammar.h
	cc -c gcopy.c

gsqueeze.o: gsqueeze.c readg.h squeeze.h writeg.h grammar.h
	cc -c gsqueeze.c

gsample.o: gsample.c readg.h sample.h grammar.h
	cc -c gsample.c

gstartfollow.o: gstartfollow.c readg.h startfollow.h writeg.h grammar.h
	cc -c gstartfollow.c

gdeempty.o: gdeempty.c readg.h deempty.h writeg.h grammar.h
	cc -c gdeempty.c

gdeebnf.o: gdeebnf.c readg.h deebnf.h writeg.h grammar.h
	cc -c gdeebnf.c


###################
# bottom level components

readg.o: readg.c readg.h grammar.h
	cc -c readg.c

reachable.o: reachable.c reachable.h grammar.h
	cc -c reachable.c

writeg.o: writeg.c writetool.h writeg.h grammar.h
	cc -c writeg.c

writetool.o: writetool.c writetool.h grammar.h
	cc -c writetool.c

gramstats.o: gramstats.c gramstats.h grammar.h
	cc -c gramstats.c

samerule.o: samerule.c samerule.h grammar.h
	cc -c samerule.c

squeeze.o: squeeze.c samerule.h squeeze.h grammar.h
	cc -c squeeze.c

sample.o: sample.c writetool.h sample.h grammar.h
	cc -c sample.c

startfollow.o: startfollow.c startfollow.h grammar.h
	cc -c startfollow.c

deempty.o: deempty.c deempty.h readg.h samerule.h grammar.h
	cc -c deempty.c

deebnf.o: deebnf.c deebnf.h readg.h grammar.h
	cc -c deebnf.c

###################
# utility 

all: gstats gcopy gsqueeze gsample gstartfollow gdeempty gdeebnf

clean:
	rm -f -v *.o

veryclean: clean
	rm -f -v gstats gcopy gsqueeze gsample gstartfollow gdeempty gdeebnf

gtools.shar: README Makefile *.c *.h *.gr
	shar README Makefile *.c *.h *.gr > gtools.shar
