include ../config.mk


LIBS = -L.. -lnuosc++ $(BLITZLIB) -lm 
CXXFLAGS = -I.. -g  -Wall -Wno-unused $(BLITZINC) # -O2 

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