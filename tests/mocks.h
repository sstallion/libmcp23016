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

#ifndef MOCKS_H
#define MOCKS_H

#include <stddef.h>
#include <stdint.h>
#include <gpiod.h>
#include <i2cd.h>

/* libc */
void *mock_calloc(size_t nmemb, size_t size);
void mock_free(void *ptr);

/* libgpiod */
struct gpiod_chip {
	int dummy;
};

struct gpiod_line {
	int dummy;
};

struct gpiod_chip *mock_gpiod_chip_open(const char *path);
void mock_gpiod_chip_close(struct gpiod_chip *chip);
struct gpiod_line *mock_gpiod_chip_get_line(struct gpiod_chip *chip, unsigned int offset);
void mock_gpiod_line_release(struct gpiod_line *line);
int mock_gpiod_line_request_input_flags(struct gpiod_line *line, const char *consumer, int flags);
int mock_gpiod_line_get_value(struct gpiod_line *line);

/* libi2cd */
struct i2cd {
	int dummy;
};

struct i2cd *mock_i2cd_open(const char *path);
void mock_i2cd_close(struct i2cd *dev);
int mock_i2cd_write(struct i2cd *dev, uint16_t addr, const void *buf, size_t len);
int mock_i2cd_write_read(struct i2cd *dev, uint16_t addr,
		const void *write_buf, size_t write_len, void *read_buf, size_t read_len);

#endif /* MOCKS_H */
