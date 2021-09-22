# SPDX-License-Identifier: FSFAP
# Copyright (C) 2021 Steven Stallion <sstallion@gmail.com>
#
# Copying and distribution of this file, with or without modification,
# are permitted in any medium without royalty provided the copyright
# notice and this notice are preserved.  This file is offered as-is,
# without any warranty.

# serial 1 doxygen.m4

# ENABLE_DOXYGEN([DOXYFILE])
# --------------------------
# Include generating doxygen documentation as a feature, which is disabled by
# default. An Automake conditional is also defined, named ENABLE_DOXYGEN.
#
# The following fragment should be included in the top-level Makefile.am to
# add support for generating doxygen documentation:
#
#   include build-aux/doxygen.am
#
AC_DEFUN([ENABLE_DOXYGEN], [
  m4_define([DOXYFILE], [m4_default([$1], [Doxyfile])])

  AC_REQUIRE_AUX_FILE([doxygen.am])

  AC_ARG_VAR([DOXYGEN], [Doxygen command])
  AC_ARG_VAR([DOXYFLAGS], [Doxygen flags])

  AC_MSG_CHECKING([whether to generate doxygen documentation])
  AC_ARG_ENABLE([doxygen],
                [AS_HELP_STRING([--enable-doxygen],
                                [generate doxygen documentation @<:@default=no@:>@])],
                [enable_doxygen=$enableval], [enable_doxygen=no])
  AC_MSG_RESULT([$enable_doxygen])

  if test "x$enable_doxygen" != xno; then
    AC_CHECK_PROGS([DOXYGEN], [doxygen], [no])
    AS_IF([test "x$DOXYGEN" = xno],
          [AC_MSG_ERROR([cannot find doxygen in path])])

    [DOXYFILE]=DOXYFILE
    AC_SUBST([DOXYFILE])
    AM_SUBST_NOTMAKE([DOXYFILE])

    m4_define([DOXYFILE_CMDS], [
      # Update Doxyfile using host installation to quiesce spurious
      # warnings related to incompatible configuration options:
      $DOXYGEN $DOXYFLAGS -u DOXYFILE >/dev/null 2>&1
      test -f "DOXYFILE.bak" && rm -f "DOXYFILE.bak"
    ])

    m4_define([DOXYFILE_INIT_CMDS], [
      DOXYGEN='$DOXYGEN'
      DOXYFLAGS='$DOXYFLAGS'
    ])

    AC_CONFIG_FILES(DOXYFILE, [DOXYFILE_CMDS], [DOXYFILE_INIT_CMDS])
  fi

  AM_CONDITIONAL([ENABLE_DOXYGEN], [test "x$enable_doxygen" != xno])
])

# DOXYGEN_PROG_DOT
# ----------------
# Test whether the dot program is available if doxygen is enabled.
AC_DEFUN([DOXYGEN_PROG_DOT], [
  AC_REQUIRE([ENABLE_DOXYGEN])

  AC_ARG_VAR([DOT], [Filter for drawing directed graphs])

  if test "x$enable_doxygen" != xno; then
    AC_CHECK_PROGS([DOT], [dot], [no])
    AS_IF([test "x$DOT" = xno],
          [AC_MSG_ERROR([cannot find dot in path])])
  fi
])
