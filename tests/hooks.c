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

#include "hooks.h"

#include <stddef.h>
#include <stdint.h>
#include <gpiod.h>
#include <i2cd.h>

/* libc */
void *__hook_calloc = __real_calloc;
void *__hook_free = __real_free;

void *__wrap_calloc(size_t nmemb, size_t size)
{
	void *(*fn)(size_t nmemb, size_t size) = __hook_calloc;
	return fn(nmemb, size);
}

void __wrap_free(void *ptr)
{
	void (*fn)(void *ptr) = __hook_free;
	fn(ptr);
}

/* libgpiod */
void *__hook_gpiod_chip_open_by_number = __real_gpiod_chip_open_by_number;
void *__hook_gpiod_chip_close = __real_gpiod_chip_close;
void *__hook_gpiod_chip_get_line = __real_gpiod_chip_get_line;
void *__hook_gpiod_line_release = __real_gpiod_line_release;
void *__hook_gpiod_line_request_input_flags = __real_gpiod_line_request_input_flags;
void *__hook_gpiod_line_get_value = __real_gpiod_line_get_value;

struct gpiod_chip *__wrap_gpiod_chip_open_by_number(unsigned int num)
{
	struct gpiod_chip *(*fn)(unsigned int num) = __hook_gpiod_chip_open_by_number;
	return fn(num);
}

void __wrap_gpiod_chip_close(struct gpiod_chip *chip)
{
	void (*fn)(struct gpiod_chip *chip) = __hook_gpiod_chip_close;
	fn(chip);
}

struct gpiod_line *__wrap_gpiod_chip_get_line(struct gpiod_chip *chip, unsigned int offset)
{
	struct gpiod_line *(*fn)(struct gpiod_chip *chip, unsigned int offset) = __hook_gpiod_chip_get_line;
	return fn(chip, offset);
}

void __wrap_gpiod_line_release(struct gpiod_line *line)
{
	void (*fn)(struct gpiod_line *line) = __hook_gpiod_line_release;
	fn(line);
}

int __wrap_gpiod_line_request_input_flags(struct gpiod_line *line, const char *consumer, int flags)
{
	int (*fn)(struct gpiod_line *line, const char *consumer, int flags) = __hook_gpiod_line_request_input_flags;
	return fn(line, consumer, flags);
}

int __wrap_gpiod_line_get_value(struct gpiod_line *line)
{
	int (*fn)(struct gpiod_line *line) = __hook_gpiod_line_get_value;
	return fn(line);
}

/* libi2cd */
void *__hook_i2cd_open_by_number = __real_i2cd_open_by_number;
void *__hook_i2cd_close = __real_i2cd_close;
void *__hook_i2cd_write = __real_i2cd_write;
void *__hook_i2cd_write_read = __real_i2cd_write_read;

struct i2cd *__wrap_i2cd_open_by_number(unsigned int num)
{
	struct i2cd *(*fn)(unsigned int num) = __hook_i2cd_open_by_number;
	return fn(num);
}

void __wrap_i2cd_close(struct i2cd *dev)
{
	void (*fn)(struct i2cd *dev) = __hook_i2cd_close;
	return fn(dev);
}

int __wrap_i2cd_write(struct i2cd *dev, uint16_t addr, const void *buf, size_t len)
{
	int (*fn)(struct i2cd *dev, uint16_t addr, const void *buf, size_t len) = __hook_i2cd_write;
	return fn(dev, addr, buf, len);
}

int __wrap_i2cd_write_read(struct i2cd *dev, uint16_t addr,
		const void *write_buf, size_t write_len, void *read_buf, size_t read_len)
{

	int (*fn)(struct i2cd *dev, uint16_t addr, const void *write_buf, size_t write_len,
			void *read_buf, size_t read_len) = __hook_i2cd_write_read;
	return fn(dev, addr, write_buf, write_len, read_buf, read_len);
}
