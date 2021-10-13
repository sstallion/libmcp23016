/* SPDX-License-Identifier: LGPL-2.1-or-later */
/*
 * Copyright (C) 2021 Steven Stallion <sstallion@gmail.com>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 * the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#include "mocks.h"

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <cmocka.h>
#include <gpiod.h>
#include <i2cd.h>

void *mock_calloc(size_t nmemb, size_t size)
{
	check_expected(nmemb);
	check_expected(size);

	return mock_type(void *);
}

void mock_free(void *ptr)
{
	check_expected_ptr(ptr);
}

struct gpiod_chip *mock_gpiod_chip_open(const char *path)
{
	check_expected(path);

	return mock_type(struct gpiod_chip *);
}

void mock_gpiod_chip_close(struct gpiod_chip *chip)
{
	check_expected_ptr(chip);
}

struct gpiod_line *mock_gpiod_chip_get_line(struct gpiod_chip *chip, unsigned int offset)
{
	check_expected_ptr(chip);
	check_expected(offset);

	return mock_type(struct gpiod_line *);
}

void mock_gpiod_line_release(struct gpiod_line *line)
{
	check_expected_ptr(line);
}

int mock_gpiod_line_request_input_flags(struct gpiod_line *line, const char *consumer, int flags)
{
	check_expected_ptr(line);
	check_expected_ptr(consumer);
	check_expected(flags);

	return mock_type(int);
}

int mock_gpiod_line_get_value(struct gpiod_line *line)
{
	check_expected_ptr(line);

	return mock_type(int);
}

struct i2cd *mock_i2cd_open(const char *path)
{
	check_expected(path);

	return mock_type(struct i2cd *);
}

void mock_i2cd_close(struct i2cd *dev)
{
	check_expected_ptr(dev);
}

int mock_i2cd_write(struct i2cd *dev, uint16_t addr, const void *buf, size_t len)
{
	check_expected_ptr(dev);
	check_expected(addr);
	check_expected(buf);
	check_expected(len);

	return mock_type(int);
}

int mock_i2cd_write_read(struct i2cd *dev, uint16_t addr,
		const void *write_buf, size_t write_len, void *read_buf, size_t read_len)
{
	void *mock_read_buf;

	check_expected_ptr(dev);
	check_expected(addr);
	check_expected(write_buf);
	check_expected(write_len);
	check_expected(read_len);

	mock_read_buf = mock_type(void *);
	if (mock_read_buf != NULL)
		memcpy(read_buf, mock_read_buf, read_len);

	return mock_type(int);
}
