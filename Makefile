
SRCS = $(wildcard nuosc*.cc)
OBJS = $(addsuffix .o, $(basename $(SRCS)))

default: nuosc.oct

nuosc.oct: $(SRCS)
	mkoctfile -I. -o $@ $^

clean:
	rm -f *~ $(OBJS) *.oct

