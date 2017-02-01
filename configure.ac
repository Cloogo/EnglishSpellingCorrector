#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([SpellCorrector], [0.90], [wang691234@gmail.com])
AC_CONFIG_SRCDIR([src/core/spellcorrector.c])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([foreign])
# Checks for programs.
AC_PROG_CC
# Checks for libraries.
AC_PROG_RANLIB
# Checks for header files.
AC_CHECK_HEADERS([limits.h stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_MALLOC
AC_FUNC_MMAP
AC_CHECK_FUNCS([memmove munmap strchr strncasecmp])

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 src/candidator/Makefile
                 src/core/Makefile
                 src/header/Makefile
                 src/langmod/Makefile
                 src/test/Makefile])
AC_OUTPUT
