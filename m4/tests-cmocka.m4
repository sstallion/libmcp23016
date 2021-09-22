# SPDX-License-Identifier: FSFAP
# Copyright (C) 2021 Steven Stallion <sstallion@gmail.com>
#
# Copying and distribution of this file, with or without modification,
# are permitted in any medium without royalty provided the copyright
# notice and this notice are preserved.  This file is offered as-is,
# without any warranty.

# serial 2 tests-cmocka.m4

# TESTS_LIB_CMOCKA([FORMAT])
# --------------------------
# Test whether the cmocka library is available if tests are enabled.
# If found, the CMOCKA_LIBS output variable will contain -lcmocka.
AC_DEFUN([TESTS_LIB_CMOCKA], [
  AC_REQUIRE([ENABLE_TESTS])

  CMOCKA_LIBS=
  if test "x$enable_tests" != xno; then
    AC_CHECK_LIB([cmocka], [_mock],
                 [AC_SUBST([CMOCKA_LIBS], [-lcmocka])],
                 [AC_MSG_ERROR([cannot link with library cmocka])])

    AC_CHECK_HEADER([cmocka.h], [],
                    [AC_MSG_ERROR([cannot find header file cmocka.h])], [
                      #include <stdarg.h>
                      #include <stddef.h>
                      #include <setjmp.h>
                      #include <cmocka.h>
                    ])

    # cmocka supports several human-readable test output formats, which can be
    # configured using the environment; see <https://api.cmocka.org/>.
    m4_ifnblank([$1],
                [AS_VAR_APPEND([AM_TESTS_ENVIRONMENT],
                               ["CMOCKA_MESSAGE_OUTPUT=$1; export CMOCKA_MESSAGE_OUTPUT;"])
                 AC_SUBST([AM_TESTS_ENVIRONMENT])])
  fi
])
