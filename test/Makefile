include ../config.mk

LIBS =  $(BLITZLIB) -lm 
CXXFLAGS = -I.. -g  -Wall -Wno-unused -O2 $(BLITZINC)

GENTEST = prem-test contraction-test

VOTEST = vo-energy-test vo-baseline-test 
CDTEST = cd-density-test cd-baseline-test cd-energy-test
EMTEST = em-energy-test em-baseline-test
ANTITEST = anti-energy-test
TESTEXECS = $(ANTITEST) $(EMTEST) $(CDTEST) $(VOTEST)

default: $(TESTEXECS)

%-test: %-test.o ../libnuosc++.so
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

%-test.o: %-test.cc ../nuosc_prob.h
	$(CXX) $(CXXFLAGS) -o $@ -c $<

%-static-test: %-test.o 
	$(CXX) $(LDFLAGS) -o $@ $^ ../*.o $(LIBS)

TESTFILES=$(addsuffix .vec, $(TESTEXECS))

testdata: $(TESTEXECS) $(TESTFILES)

%-test.vec: %-test
	./$< > $@

clean:
	rm -f *-test *.o *~

contraction-test: contraction-test.cc
	$(CXX) -o $@ $^ $(LIBS)
