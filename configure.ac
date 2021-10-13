# SPDX-License-Identifier: LGPL-2.1-or-later
# Copyright (C) 2021 Steven Stallion <sstallion@gmail.com>
#
# This library is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published
# by the Free Software Foundation; either version 2.1 of the License, or
# (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
# the GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this library; if not, see <http://www.gnu.org/licenses/>.

AC_PREREQ([2.69])

AC_INIT([libmcp23016], [1.0],
        [https://github.com/sstallion/libmcp23016/issues])

AC_SUBST([PACKAGE_DESCRIPTION], ["Linux userspace driver for Microchip MCP23016 I/O expanders"])
AC_SUBST([PACKAGE_LIBS], [-lmcp23016])

# Update package version information with care; see
# <https://www.gnu.org/software/libtool/manual/html_node/Updating-version-info.html>.
AC_SUBST([PACKAGE_VERSION_INFO], [0:1:0])

AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIRS([m4])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_SRCDIR([src/mcp23016.c])

AM_INIT_AUTOMAKE([foreign subdir-objects 1.12])
AM_SILENT_RULES([yes])

# Silence warning: ar: 'u' modifier ignored since 'D' is the default
AC_SUBST(ARFLAGS, [cr])

AM_PROG_AR
AC_PROG_CC
AC_PROG_INSTALL

LT_INIT

ENABLE_TESTS
ENABLE_COVERAGE
ENABLE_DOXYGEN

TESTS_LIB_CMOCKA([TAP])
TESTS_TAP_DRIVER

AC_CHECK_LIB([gpiod], [gpiod_chip_open], [],
             [AC_MSG_ERROR([cannot link with library gpiod])])

AC_CHECK_LIB([i2cd], [i2cd_open], [],
             [AC_MSG_ERROR([cannot link with library i2cd])])

AC_CHECK_HEADER([endian.h], [],
                [AC_MSG_ERROR([cannot find header file endian.h])])

AC_CHECK_HEADER([gpiod.h], [],
                [AC_MSG_ERROR([cannot find header file gpiod.h])])

AC_CHECK_HEADER([i2cd.h], [],
                [AC_MSG_ERROR([cannot find header file i2cd.h])])

AC_CONFIG_FILES([Makefile libmcp23016.pc])

AC_OUTPUT
