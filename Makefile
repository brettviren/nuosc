include config.mk

CXXFLAGS =  -Wall -g  -Wno-unused $(BLITZINC) # -O2
LIBS = $(BLITZLIB)

SRCS = $(wildcard *.cc)
OBJS = $(addsuffix .o, $(basename $(SRCS)))
LIB = libnuosc++.so
LIBA = libnuosc++.a
LIBSRC = $(SRCS)


default: $(LIB)  mains # tests

tests:
	$(MAKE) -C test

mains:
	$(MAKE) -C main

clean:
	rm -f *~ $(OBJS) $(LIB) $(LIBA) *.d *.da
	$(MAKE) -C main clean

dist:
	cd .. && tar -czf libnuosc++.tar.gz libnuosc++/{README,INSTALL,config.mk,Makefile,*.{cc,h},{test,main}/{Makefile,*.{cc,h}}} > /dev/null 2>&1 || true


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

