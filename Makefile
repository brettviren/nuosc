CXX = g++ # -pg  -fprofile-arcs 
CXXFLAGS =  -Wall -g  -Wno-unused # -O2
BLITZLIB = -lblitz
LIBS = $(BLITZLIB)

SRCS = $(wildcard *.cc)
OBJS = $(addsuffix .o, $(basename $(SRCS)))
LIB = libnuosc++.so
LIBA = libnuosc++.a
LIBSRC = $(SRCS)

default: $(LIB)  mains # tests

tests:
	make -C test

mains:
	make -C main

clean:
	rm -f *~ $(OBJS) $(LIB) $(LIBA) *.d *.da


$(LIB): $(OBJS)
	$(CXX) -shared -o $@ $^

$(LIBA): $(OBJS)
	$(AR) rv $@ $^

%.o: %.cc
	$(CXX) $(CXXFLAGS) -o $@ -c $<

%.d: %.cc
	set -e; $(CXX) -MM $(CXXFLAGS) $< \
                  | sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@; \
			[ -s $@ ] || rm -f $@
include $(LIBSRC:.cc=.d)
