CXX = g++
LIBS =  -lblitz -lm
CXXFLAGS = -I.. -g  -Wall -Wno-unused

# vo-energy-test vo-baseline-test vo-test 
default: cd-density-test cd-baseline-test cd-energy-test

%-test: %-test.o ../libnuosc++.so
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

%-test.o: %-test.cc ../nuosc_prob.h
	$(CXX) $(CXXFLAGS) -o $@ -c $<

clean:
	rm -f *-test *.o *~