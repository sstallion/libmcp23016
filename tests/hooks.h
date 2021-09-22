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

#ifndef HOOKS_H
#define HOOKS_H

#include <stddef.h>
#include <stdint.h>
#include <gpiod.h>
#include <i2cd.h>

#define hook(func, mock_func) \
    do { \
	    __hook_##func = mock_func; \
    } while(0)

#define unhook(func) \
    do { \
	    __hook_##func = __real_##func; \
    } while(0)

/* libc */
extern void *__hook_calloc;
extern void *__hook_free;

void *__real_calloc(size_t nmemb, size_t size);
void __real_free(void *ptr);

/* libgpiod */
extern void *__hook_gpiod_chip_open_by_number;
extern void *__hook_gpiod_chip_close;
extern void *__hook_gpiod_chip_get_line;
extern void *__hook_gpiod_line_release;
extern void *__hook_gpiod_line_request_input_flags;
extern void *__hook_gpiod_line_get_value;

struct gpiod_chip *__real_gpiod_chip_open_by_number(unsigned int num);
void __real_gpiod_chip_close(struct gpiod_chip *chip);
struct gpiod_line *__real_gpiod_chip_get_line(struct gpiod_chip *chip, unsigned int offset);
void __real_gpiod_line_release(struct gpiod_line *line);
int __real_gpiod_line_request_input_flags(struct gpiod_line *line, const char *consumer, int flags);
int __real_gpiod_line_get_value(struct gpiod_line *line);

/* libi2cd */
extern void *__hook_i2cd_open_by_number;
extern void *__hook_i2cd_close;
extern void *__hook_i2cd_write;
extern void *__hook_i2cd_write_read;

struct i2cd *__real_i2cd_open_by_number(unsigned int num);
void __real_i2cd_close(struct i2cd *dev);
int __real_i2cd_write(struct i2cd *dev, uint16_t addr, const void *buf, size_t len);
int __real_i2cd_write_read(struct i2cd *dev, uint16_t addr,
		const void *write_buf, size_t write_len, void *read_buf, size_t read_len);

#endif /* HOOKS_H */
