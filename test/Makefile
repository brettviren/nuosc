CXX = g++
LIBS =  -lblitz -lm
CXXFLAGS = -I.. -g  -Wall -Wno-unused

GENTEST = prem-test

VOTEST = vo-energy-test vo-baseline-test 
CDTEST = cd-density-test cd-baseline-test cd-energy-test
EMTEST = em-energy-test em-baseline-test
TESTEXECS = $(EMTEST) $(CDTEST) $(VOTEST)

default: $(TESTEXECS)

%-test: %-test.o ../libnuosc++.so
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

%-test.o: %-test.cc ../nuosc_prob.h
	$(CXX) $(CXXFLAGS) -o $@ -c $<

TESTFILES=$(addsuffix .vec, $(TESTEXECS))

testdata: $(TESTEXECS) $(TESTFILES)

%-test.vec: %-test
	./$< > $@

clean:
	rm -f *-test *.o *~