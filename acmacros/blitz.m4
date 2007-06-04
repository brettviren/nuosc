# -*- autoconf -*-

dnl
dnl Autoconf macros to build against Blitz++
dnl

AC_DEFUN([BLITZ_SETUP], [

BLITZ_WITH
BLITZ_WITH_INCLUDE
BLITZ_WITH_LIBDIR
BLITZ_SUBST

])

# macro to set BLITZ base dir
AC_DEFUN([BLITZ_WITH], [

AC_MSG_CHECKING(for Blitz++ installation setting)

AC_ARG_WITH(blitz,
	AC_HELP_STRING([--with-blitz=<path>],
	[Blitz++ installation base [[/usr]] ]),
	[blitz_prefix=$with_blitz],
	[blitz_prefix=/usr])

AC_MSG_RESULT([$blitz_prefix])

UTIL_CHECK_PKG_DIR([$blitz_prefix], [Blitz++])

BLITZ_INCLUDE="-I$blitz_prefix/include"
BLITZ_LIB="-L$blitz_prefix/lib -lblitz"

])

# macro to set BLITZ include dir
AC_DEFUN([BLITZ_WITH_INCLUDE], [

AC_MSG_CHECKING([for Blitz++ include dir setting])

AC_ARG_WITH([blitz-include],
	AC_HELP_STRING([--with-blitz-include=<path>],
	[Blitz++ alternate headers dir]),
	[blitz_include=$with_blitz_include],
	[blitz_include=${blitz_prefix}/include])

AC_MSG_RESULT([$blitz_include])

UTIL_CHECK_PKG_DIR([$blitz_include],[Blitz++],[blitz/array.h])

BLITZ_INCLUDE="-I$blitz_include"

])

AC_MSG_RESULT(yes)

# macro to set BLITZ lib dir
AC_DEFUN([BLITZ_WITH_LIBDIR], [

AC_MSG_CHECKING([for Blitz++ lib dir setting])

AC_ARG_WITH([blitz-libdir],
	AC_HELP_STRING([--with-blitz-libdir=<path>], 
	[Blitz++ alternate library dir]),
	[blitz_libdir=$with_blitz_libdir],
	[blitz_libdir=$blitz_prefix/lib])

AC_MSG_RESULT([$blitz_libdir])

UTIL_CHECK_PKG_DIR([$blitz_libdir], [Blitz++])

BLITZ_LIB="-L$blitz_libdir -lblitz"

])


# macro to substitute BLITZ vars to output
AC_DEFUN([BLITZ_SUBST], [

AC_SUBST(BLITZ_PREFIX)
AC_SUBST(BLITZ_INCLUDE)
AC_SUBST(BLITZ_LIB)

])
