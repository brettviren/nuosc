include ../config.mk


LIBS = -L. -L.. -lnuosc++ $(BLITZLIB) -lm 
CXXFLAGS = -I.. -g  -Wall -Wno-unused $(BLITZINC) # -O2 

OBJS = options.o util.o Config.o ProbCalc.o
MAINS = nuosc genprob 

default: $(MAINS)

genprob: genprob.o $(OBJS) 
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)

nuosc: nuosc.o $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -o $@ -c $<

install: $(MAINS)
	cp $(MAINS) $(PREFIX)/bin

clean:
	rm -f $(MAINS) *.o *~