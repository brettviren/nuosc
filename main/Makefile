CXX = g++ # -pg  -fprofile-arcs 
LIBS = -L.. -lnuosc++ -lblitz -lm 
CXXFLAGS = -I.. -g  -Wall -Wno-unused # -O2 

OBJS = options.o
MAINS = genprob

default: $(MAINS)

genprob: genprob.o $(OBJS) 
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)
%.o: %.cc
	$(CXX) $(CXXFLAGS) -o $@ -c $<

install: $(MAINS)
	cp $(MAINS) $(PREFIX)/bin

clean:
	rm -f $(MAINS) *.o *~