#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.57)
AC_INIT(ezcommon, 1.0, bon_ami_@hotmail.com)
AC_CONFIG_SRCDIR([ezcommon.cpp])
AM_INIT_AUTOMAKE
#AC_CONFIG_HEADER([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_LIBTOOL

# Checks for libraries.

# Checks for header files.
#AC_CHECK_HEADERS([termios.h unistd.h])
AC_CHECK_HEADERS([termios.h], [], [])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
##AC_STRUCT_TM

# Checks for library functions.
AC_FUNC_MALLOC
AC_HEADER_STDC

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
