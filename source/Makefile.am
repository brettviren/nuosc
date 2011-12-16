INCLUDES = -I$(top_srcdir)/include ${BLITZ_INCLUDE}


NUOSC_LIBRARY_VERSION=1:0:0

lib_LTLIBRARIES = libnuosc++-1.0.la
libnuosc___1_0_la_LDFLAGS = -version-info $(NUOSC_LIBRARY_VERSION)

libnuosc___1_0_la_SOURCES = \
	NuEvolver.cc \
	NuEvolverConstant.cc \
	NuEvolverPrem.cc \
	NuEvolverVacuum.cc \
	OdeFunc.cc \
	OscParam.cc \
	constants.cc \
	fp.cc \
	matrix.cc \
	nuosc_earth.cc \
	nuosc_matrix.cc \
	nuosc_prob.cc

