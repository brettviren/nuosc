CXX = g++ -pg  -fprofile-arcs 
CXXFLAGS = -pg -O2 -Wall -g  -Wno-unused
#CXXFLAGS = -Wall -O2 
BLITZLIB = -lblitz
LIBS = $(BLITZLIB)

SRCS = $(wildcard *.cc)
OBJS = $(addsuffix .o, $(basename $(SRCS)))
LIB = libnuosc++.so
LIBA = libnuosc++.a
LIBSRC = $(SRCS)

default: $(LIB) tests

tests:
	make -C test


clean:
	rm -f *~ $(OBJS) $(LIB) $(LIBA)


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
