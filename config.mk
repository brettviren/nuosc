# Hey, Emacs, this is a -*- makefile -*- .

# point to where blitz++ is installed
BLITZ = $(HOME)/lib/blitz-0.6-gcc-2.95

# Set if for non-system/non-default compiler
CXX = g++-2.95

# Where to install: PREFIX/{lib,bin,include}
PREFIX = $(HOME)


# These can probably be left alone.
BLITZLIB = -L$(BLITZ)/lib -lblitz
BLITZINC = -I$(BLITZ)/include