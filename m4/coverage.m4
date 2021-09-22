# SPDX-License-Identifier: FSFAP
# Copyright (C) 2021 Steven Stallion <sstallion@gmail.com>
#
# Copying and distribution of this file, with or without modification,
# are permitted in any medium without royalty provided the copyright
# notice and this notice are preserved.  This file is offered as-is,
# without any warranty.

# serial 1 coverage.m4

# ENABLE_COVERAGE
# ---------------
# Include building with coverage as a feature, which is disabled by default.
# If found, the COVERAGE_CFLAGS and COVERAGE_LIBS output variables will
# contain "-fprofile-arcs -ftest-coverage" and -lgcov, respectively. An
# Automake conditional is also defined, named ENABLE_COVERAGE.
#
# The following fragment should be included in the top-level Makefile.am to
# add support for building with coverage:
#
#   include build-aux/coverage.am
#
AC_DEFUN([ENABLE_COVERAGE], [
  AC_REQUIRE([ENABLE_TESTS])

  AC_REQUIRE_AUX_FILE([coverage.am])

  AC_ARG_VAR([GCOV], [Coverage testing tool])
  AC_ARG_VAR([LCOV], [Graphical GCOV front-end])
  AC_ARG_VAR([GENHTML], [Generate HTML view from LCOV coverage data])

  AC_MSG_CHECKING([whether to build with coverage])
  AC_ARG_ENABLE([coverage],
                [AS_HELP_STRING([--enable-coverage],
                                [build with coverage (requires tests) @<:@default=no@:>@])],
                [AS_IF([test "x$enable_tests" = xno],
                       [AC_MSG_ERROR([tests required to build with coverage])])
                 enable_coverage=$enableval], [enable_coverage=no])
  AC_MSG_RESULT([$enable_coverage])

  COVERAGE_CFLAGS=
  COVERAGE_LIBS=
  if test "x$enable_coverage" != xno; then
    AC_CHECK_TOOL([GCOV], [gcov], [no])
    AS_IF([test "x$GCOV" = xno],
          [AC_MSG_ERROR([cannot find gcov in path])])

    AC_CHECK_PROGS([LCOV], [lcov], [no])
    AS_IF([test "x$LCOV" = xno],
          [AC_MSG_ERROR([cannot find lcov in path])])

    AC_CHECK_PROGS([GENHTML], [genhtml], [no])
    AS_IF([test "x$GENHTML" = xno],
          [AC_MSG_ERROR([cannot find genhtml in path])])

    AC_CHECK_LIB([gcov], [__gcov_init],
                 [AC_SUBST([COVERAGE_CFLAGS], ["-fprofile-arcs -ftest-coverage"])
                  AC_SUBST([COVERAGE_LIBS], [-lgcov])],
                 [AC_MSG_ERROR([cannot link with library gcov])])
  fi

  AM_CONDITIONAL([ENABLE_COVERAGE], [test "x$enable_coverage" != xno])
])
