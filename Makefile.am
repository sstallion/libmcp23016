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

include build-aux/coverage.am
include build-aux/doxygen.am

ACLOCAL_AMFLAGS = -I m4
AM_CFLAGS = -Wall -Wextra -Wno-unused-parameter
AM_CPPFLAGS = -I$(top_srcdir)/include -I$(top_srcdir)/src

EXTRA_DIST = HACKING.md \
	     NEWS.md \
	     README.md \
	     doc/examples/example.c \
	     doc/index.md

include_HEADERS = include/mcp23016.h

lib_LTLIBRARIES = libmcp23016.la

libmcp23016_la_SOURCES = src/mcp23016.c src/mcp23016-private.h
libmcp23016_la_CFLAGS = $(COVERAGE_CFLAGS) $(AM_CFLAGS)
libmcp23016_la_LIBADD = $(COVERAGE_LIBS) $(AM_LIBS)
libmcp23016_la_LDFLAGS = -version-info $(PACKAGE_VERSION_INFO)

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libmcp23016.pc

if ENABLE_TESTS
check_LIBRARIES = tests/libhooks.a tests/libmocks.a
TESTS_LIBS = $(check_LIBRARIES) $(CMOCKA_LIBS)

tests_libhooks_a_SOURCES = tests/hooks.c tests/hooks.h
tests_libmocks_a_SOURCES = tests/mocks.c tests/mocks.h

check_PROGRAMS = tests/test-mcp23016
TESTS = $(check_PROGRAMS)

tests_test_mcp23016_SOURCES = tests/test-mcp23016.c
tests_test_mcp23016_LDADD = libmcp23016.la $(TESTS_LIBS) $(AM_LIBS)
tests_test_mcp23016_LDFLAGS = -static \
			      -Wl,--wrap=calloc \
			      -Wl,--wrap=free \
			      -Wl,--wrap=gpiod_chip_open \
			      -Wl,--wrap=gpiod_chip_close \
			      -Wl,--wrap=gpiod_chip_get_line \
			      -Wl,--wrap=gpiod_line_release \
			      -Wl,--wrap=gpiod_line_request_input_flags \
			      -Wl,--wrap=gpiod_line_get_value \
			      -Wl,--wrap=i2cd_open \
			      -Wl,--wrap=i2cd_close \
			      -Wl,--wrap=i2cd_write \
			      -Wl,--wrap=i2cd_write_read
endif
