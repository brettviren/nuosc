CXX = g++
CXXFLAGS = -Wall -g 
#CXXFLAGS = -Wall -O2 
BLITZLIB = -lblitz
LIBS = $(BLITZLIB)

SRCS = $(wildcard *.cc)
OBJS = $(addsuffix .o, $(basename $(SRCS)))
LIB = libnuosc++.so
LIBSRC = $(SRCS)

default: $(LIB)


clean:
	rm -f *~ $(OBJS)


$(LIB): $(OBJS)
	$(CXX) -shared -o $@ $^

%.o: %.cc
	$(CXX) $(CXXFLAGS) -o $@ -c $<

%.d: %.cc
	set -e; $(CXX) -MM $(CXXFLAGS) $< \
                  | sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@; \
			[ -s $@ ] || rm -f $@
include $(LIBSRC:.cc=.d)
