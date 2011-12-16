#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_INIT([libnuosc++], [1.4], [bv@bnl.gov])
AC_PREREQ(2.61)
AM_INIT_AUTOMAKE(libnuosc++, 1.4)
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_SRCDIR([source/nuosc_prob.cc])
# AM_CONFIG_HEADER(config.h)
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LIBTOOL

BLITZ_SETUP


# Checks for libraries.
# FIXME: Replace `main' with a function in `-lm':
AC_CHECK_LIB([m], [main])

# Checks for header files.
AC_CHECK_HEADERS([fenv.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_HEADER_STDC
AC_CHECK_FUNCS([pow sqrt strchr strdup strpbrk strrchr])

# need to add main and test
AC_CONFIG_FILES([Makefile
                 source/Makefile
                 include/Makefile
                 main/Makefile])

AC_OUTPUT
