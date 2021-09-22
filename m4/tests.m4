# SPDX-License-Identifier: FSFAP
# Copyright (C) 2021 Steven Stallion <sstallion@gmail.com>
#
# Copying and distribution of this file, with or without modification,
# are permitted in any medium without royalty provided the copyright
# notice and this notice are preserved.  This file is offered as-is,
# without any warranty.

# serial 2 tests.m4

# ENABLE_TESTS
# ------------
# Include building tests as a feature, which is enabled by default unless
# cross-compiling. An Automake conditional is also defined, named ENABLE_TESTS.
AC_DEFUN([ENABLE_TESTS], [
  AC_MSG_CHECKING([whether to build tests])
  AC_ARG_ENABLE([tests],
                [AS_HELP_STRING([--disable-tests],
                                [do not build tests @<:@default=yes@:>@])],
                [AS_IF(["x$cross_compiling" != xno], [enable_tests=no],
                       [enable_tests=$enableval])],
                [enable_tests=yes])
  AC_MSG_RESULT([$enable_tests])

  AM_CONDITIONAL([ENABLE_TESTS], [test "x$enable_tests" != xno])
])

# TESTS_TAP_DRIVER
# ----------------
# Use TAP with the Automake test harness (requires version 1.12).
AC_DEFUN([TESTS_TAP_DRIVER], [
  AC_REQUIRE([AC_PROG_AWK])

  AC_REQUIRE_AUX_FILE([tap-driver.sh])

  m4_pattern_allow([AM_TAP_AWK])
  m4_define([LOG_DRIVER],
            ["env AM_TAP_AWK='\$(AWK)' \$(SHELL) $ac_aux_dir/tap-driver.sh"])

  [LOG_DRIVER]=LOG_DRIVER
  AC_SUBST([LOG_DRIVER])

  [TEST_LOG_DRIVER]=LOG_DRIVER
  AC_SUBST([TEST_LOG_DRIVER])
])
