CXX = g++
LIBS = -L.. -lnuosc++ -lblitz -lm
CXXFLAGS = -I.. -g  -Wall


%-test: %-test.o 
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

%-test.o: %-test.cc
	$(CXX) $(CXXFLAGS) -o $@ -c $<

clean:
	rm -f *.test *.o *~