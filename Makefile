CXX = g++ # -pg  -fprofile-arcs 
CXXFLAGS =  -Wall -g  -Wno-unused # -O2
BLITZLIB = -lblitz
LIBS = $(BLITZLIB)

SRCS = $(wildcard *.cc)
OBJS = $(addsuffix .o, $(basename $(SRCS)))
LIB = libnuosc++.so
LIBA = libnuosc++.a
LIBSRC = $(SRCS)

# Where to install: PREFIX/{lib,bin,include}
export PREFIX = /home/bviren

default: $(LIB)  mains # tests

tests:
	$(MAKE) -C test

mains:
	$(MAKE) -C main

clean:
	rm -f *~ $(OBJS) $(LIB) $(LIBA) *.d *.da

dist:
	cd .. && tar -czf libnuosc++.tar.gz libnuosc++/{Makefile,*.{cc,h},{test,main}/{Makefile,*.{cc,h}}} > /dev/null 2>&1 || true


install: $(LIB) mains
	mkdir -p $(PREFIX)/lib $(PREFIX)/bin $(PREFIX)/include/nuosc
	cp *.h $(PREFIX)/include/nuosc
	cp libnuosc++.so $(PREFIX)/lib
	$(MAKE) -C main install

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

