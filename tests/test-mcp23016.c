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

#include "mcp23016-private.h"

#include <errno.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <cmocka.h>
#include <gpiod.h>
#include <i2cd.h>

#include "hooks.h"
#include "mocks.h"

int setup(void **state)
{
	hook(calloc, mock_calloc);
	hook(free, mock_free);
	hook(gpiod_chip_open_by_number, mock_gpiod_chip_open_by_number);
	hook(gpiod_chip_close, mock_gpiod_chip_close);
	hook(gpiod_chip_get_line, mock_gpiod_chip_get_line);
	hook(gpiod_line_release, mock_gpiod_line_release);
	hook(gpiod_line_request_input_flags, mock_gpiod_line_request_input_flags);
	hook(gpiod_line_get_value, mock_gpiod_line_get_value);
	hook(i2cd_open_by_number, mock_i2cd_open_by_number);
	hook(i2cd_close, mock_i2cd_close);
	hook(i2cd_write, mock_i2cd_write);
	hook(i2cd_write_read, mock_i2cd_write_read);
	return 0;
}

int teardown(void **state)
{
	unhook(calloc);
	unhook(free);
	unhook(gpiod_chip_open_by_number);
	unhook(gpiod_chip_close);
	unhook(gpiod_chip_get_line);
	unhook(gpiod_line_release);
	unhook(gpiod_line_request_input_flags);
	unhook(gpiod_line_get_value);
	unhook(i2cd_open_by_number);
	unhook(i2cd_close);
	unhook(i2cd_write);
	unhook(i2cd_write_read);
	return 0;
}

void test_mcp23016_open(void **state)
{
	struct mcp23016_config mock_config = {0};
	struct mcp23016 mock_dev = {0};
	struct i2cd mock_i2cd;
	struct gpiod_chip mock_gpiod_chip;
	struct gpiod_line mock_gpiod_line;
	struct mcp23016 *dev;

	expect_value(mock_calloc, nmemb, 1);
	expect_value(mock_calloc, size, sizeof(mock_dev));
	will_return(mock_calloc, &mock_dev);

	expect_value(mock_i2cd_open_by_number, num, mock_config.i2c_num);
	will_return(mock_i2cd_open_by_number, &mock_i2cd);

	expect_value(mock_gpiod_chip_open_by_number, num, mock_config.gpio_num);
	will_return(mock_gpiod_chip_open_by_number, &mock_gpiod_chip);

	expect_value(mock_gpiod_chip_get_line, chip, &mock_gpiod_chip);
	expect_value(mock_gpiod_chip_get_line, offset, mock_config.gpio_offset);
	will_return(mock_gpiod_chip_get_line, &mock_gpiod_line);

	expect_value(mock_gpiod_line_request_input_flags, line, &mock_gpiod_line);
	expect_string(mock_gpiod_line_request_input_flags, consumer, CONSUMER);
	expect_value(mock_gpiod_line_request_input_flags, flags, GPIOD_LINE_REQUEST_FLAG_ACTIVE_LOW);
	will_return(mock_gpiod_line_request_input_flags, 0);

	/* Check behavior when function succeeds */
	dev = mcp23016_open(&mock_config);

	assert_non_null(dev);
	assert_int_equal(dev->i2c_addr, BASE_ADDR);
	assert_ptr_equal(dev->i2c_dev, &mock_i2cd);
	assert_ptr_equal(dev->gpio_chip, &mock_gpiod_chip);
	assert_ptr_equal(dev->gpio_line, &mock_gpiod_line);
}

void test_mcp23016_open_fail_malloc(void **state)
{
	struct mcp23016_config mock_config = {0};
	struct mcp23016 *dev;

	expect_any(mock_calloc, nmemb);
	expect_any(mock_calloc, size);
	will_return(mock_calloc, NULL);

	/* Check behavior when calloc() fails */
	dev = mcp23016_open(&mock_config);

	assert_null(dev);
}

void test_mcp23016_open_fail_i2c_addr(void **state)
{
	struct mcp23016_config mock_config = {
		.num = (END_ADDR-BASE_ADDR) + 1
	};
	struct mcp23016 mock_dev = {0};
	struct mcp23016 *dev;

	expect_any(mock_calloc, nmemb);
	expect_any(mock_calloc, size);
	will_return(mock_calloc, &mock_dev);

	expect_value(mock_free, ptr, &mock_dev);

	/* Check behavior when I2C address invalid */
	dev = mcp23016_open(&mock_config);

	assert_int_equal(errno, EINVAL);
	assert_null(dev);
}

void test_mcp23016_open_fail_i2c_dev(void **state)
{
	struct mcp23016_config mock_config = {0};
	struct mcp23016 mock_dev = {0};
	struct mcp23016 *dev;

	expect_any(mock_calloc, nmemb);
	expect_any(mock_calloc, size);
	will_return(mock_calloc, &mock_dev);

	expect_any(mock_i2cd_open_by_number, num);
	will_return(mock_i2cd_open_by_number, NULL);

	expect_value(mock_free, ptr, &mock_dev);

	/* Check behavior when i2cd_open_by_number() fails */
	dev = mcp23016_open(&mock_config);

	assert_null(dev);
}

void test_mcp23016_open_fail_gpio_chip(void **state)
{
	struct mcp23016_config mock_config = {0};
	struct mcp23016 mock_dev = {0};
	struct i2cd mock_i2cd;
	struct mcp23016 *dev;

	expect_any(mock_calloc, nmemb);
	expect_any(mock_calloc, size);
	will_return(mock_calloc, &mock_dev);

	expect_any(mock_i2cd_open_by_number, num);
	will_return(mock_i2cd_open_by_number, &mock_i2cd);

	expect_any(mock_gpiod_chip_open_by_number, num);
	will_return(mock_gpiod_chip_open_by_number, NULL);

	expect_value(mock_i2cd_close, dev, &mock_i2cd);
	expect_value(mock_free, ptr, &mock_dev);

	/* Check behavior when gpiod_chip_open_by_number() fails */
	dev = mcp23016_open(&mock_config);

	assert_null(dev);
}

void test_mcp23016_open_fail_gpio_line(void **state)
{
	struct mcp23016_config mock_config = {0};
	struct mcp23016 mock_dev = {0};
	struct i2cd mock_i2cd;
	struct gpiod_chip mock_gpiod_chip;
	struct mcp23016 *dev;

	expect_any(mock_calloc, nmemb);
	expect_any(mock_calloc, size);
	will_return(mock_calloc, &mock_dev);

	expect_any(mock_i2cd_open_by_number, num);
	will_return(mock_i2cd_open_by_number, &mock_i2cd);

	expect_any(mock_gpiod_chip_open_by_number, num);
	will_return(mock_gpiod_chip_open_by_number, &mock_gpiod_chip);

	expect_any(mock_gpiod_chip_get_line, chip);
	expect_any(mock_gpiod_chip_get_line, offset);
	will_return(mock_gpiod_chip_get_line, NULL);

	expect_value(mock_gpiod_chip_close, chip, &mock_gpiod_chip);
	expect_value(mock_i2cd_close, dev, &mock_i2cd);
	expect_value(mock_free, ptr, &mock_dev);

	/* Check behavior when gpiod_chip_get_line() fails */
	dev = mcp23016_open(&mock_config);

	assert_null(dev);
}

void test_mcp23016_open_fail_gpio_line_flags(void **state)
{
	struct mcp23016_config mock_config = {0};
	struct mcp23016 mock_dev = {0};
	struct i2cd mock_i2cd;
	struct gpiod_chip mock_gpiod_chip;
	struct gpiod_line mock_gpiod_line;
	struct mcp23016 *dev;

	expect_any(mock_calloc, nmemb);
	expect_any(mock_calloc, size);
	will_return(mock_calloc, &mock_dev);

	expect_any(mock_i2cd_open_by_number, num);
	will_return(mock_i2cd_open_by_number, &mock_i2cd);

	expect_any(mock_gpiod_chip_open_by_number, num);
	will_return(mock_gpiod_chip_open_by_number, &mock_gpiod_chip);

	expect_any(mock_gpiod_chip_get_line, chip);
	expect_any(mock_gpiod_chip_get_line, offset);
	will_return(mock_gpiod_chip_get_line, &mock_gpiod_line);

	expect_any(mock_gpiod_line_request_input_flags, line);
	expect_any(mock_gpiod_line_request_input_flags, consumer);
	expect_any(mock_gpiod_line_request_input_flags, flags);
	will_return(mock_gpiod_line_request_input_flags, -1);

	expect_value(mock_gpiod_line_release, line, &mock_gpiod_line);
	expect_value(mock_gpiod_chip_close, chip, &mock_gpiod_chip);
	expect_value(mock_i2cd_close, dev, &mock_i2cd);
	expect_value(mock_free, ptr, &mock_dev);

	/* Check behavior when gpiod_line_request_input_flags() fails */
	dev = mcp23016_open(&mock_config);

	assert_null(dev);
}

void test_mcp23016_close(void **state)
{
	struct mcp23016 mock_dev = {
		.i2c_addr = BASE_ADDR,
		.i2c_dev = &(struct i2cd){0},
		.gpio_chip = &(struct gpiod_chip){0},
		.gpio_line = &(struct gpiod_line){0}
	};

	expect_value(mock_gpiod_line_release, line, mock_dev.gpio_line);
	expect_value(mock_gpiod_chip_close, chip, mock_dev.gpio_chip);
	expect_value(mock_i2cd_close, dev, mock_dev.i2c_dev);
	expect_value(mock_free, ptr, &mock_dev);

	/* Check behavior when function succeeds */
	mcp23016_close(&mock_dev);
}

void test_mcp23016_reset(void **state)
{
	struct mcp23016 mock_dev = {
		.i2c_addr = BASE_ADDR,
		.i2c_dev = &(struct i2cd){0},
		.gpio_chip = &(struct gpiod_chip){0},
		.gpio_line = &(struct gpiod_line){0}
	};
	uint8_t mock_bufs[][3] = {
		{REG_IODIR0,  0xff, 0xff},
		{REG_OLAT0,   0x00, 0x00},
		{REG_IPOL0,   0x00, 0x00},
		{REG_IOCON0,  0x00, 0x00},
		{REG_INTCAP0, 0x00, 0x00}
	};
	int rc;

	/* mcp23016_set_direction() */
	expect_any(mock_i2cd_write, dev);
	expect_any(mock_i2cd_write, addr);
	expect_memory(mock_i2cd_write, buf, mock_bufs[0], sizeof(mock_bufs[0]));
	expect_value(mock_i2cd_write, len, sizeof(mock_bufs[0]));
	will_return(mock_i2cd_write, 0);

	/* mcp23016_set_output() */
	expect_any(mock_i2cd_write, dev);
	expect_any(mock_i2cd_write, addr);
	expect_memory(mock_i2cd_write, buf, mock_bufs[1], sizeof(mock_bufs[1]));
	expect_value(mock_i2cd_write, len, sizeof(mock_bufs[1]));
	will_return(mock_i2cd_write, 0);

	/* mcp23016_set_polarity() */
	expect_any(mock_i2cd_write, dev);
	expect_any(mock_i2cd_write, addr);
	expect_memory(mock_i2cd_write, buf, mock_bufs[2], sizeof(mock_bufs[2]));
	expect_value(mock_i2cd_write, len, sizeof(mock_bufs[2]));
	will_return(mock_i2cd_write, 0);

	/* mcp23016_set_control() */
	expect_any(mock_i2cd_write, dev);
	expect_any(mock_i2cd_write, addr);
	expect_memory(mock_i2cd_write, buf, mock_bufs[3], sizeof(mock_bufs[3]));
	expect_value(mock_i2cd_write, len, sizeof(mock_bufs[3]));
	will_return(mock_i2cd_write, 0);

	/* mcp23016_clear_interrupt() */
	expect_any(mock_i2cd_write_read, dev);
	expect_any(mock_i2cd_write_read, addr);
	expect_memory(mock_i2cd_write_read, write_buf, mock_bufs[4], 1);
	expect_value(mock_i2cd_write_read, write_len, 1);
	will_return(mock_i2cd_write_read, NULL); /* read_buf */
	expect_any(mock_i2cd_write_read, read_len);
	will_return(mock_i2cd_write_read, 0);

	/* Check behavior when function succeeds */
	rc = mcp23016_reset(&mock_dev);

	assert_return_code(rc, 0);
}

void test_mcp23016_get_port(void **state)
{
	struct mcp23016 mock_dev = {
		.i2c_addr = BASE_ADDR,
		.i2c_dev = &(struct i2cd){0},
		.gpio_chip = &(struct gpiod_chip){0},
		.gpio_line = &(struct gpiod_line){0}
	};
	uint8_t mock_write_buf[] = {REG_GP0};
	uint8_t mock_read_buf[] = {0x55, 0xaa};
	uint16_t port;
	int rc;

	expect_value(mock_i2cd_write_read, dev, mock_dev.i2c_dev);
	expect_value(mock_i2cd_write_read, addr, mock_dev.i2c_addr);
	expect_memory(mock_i2cd_write_read, write_buf, mock_write_buf, sizeof(mock_write_buf));
	expect_value(mock_i2cd_write_read, write_len, sizeof(mock_write_buf));
	will_return(mock_i2cd_write_read, mock_read_buf); /* read_buf */
	expect_value(mock_i2cd_write_read, read_len, sizeof(mock_read_buf));
	will_return(mock_i2cd_write_read, 0);

	/* Check behavior when function succeeds */
	rc = mcp23016_get_port(&mock_dev, &port);

	assert_return_code(rc, 0);
	assert_int_equal(port, 0xaa55);
}

void test_mcp23016_set_port(void **state)
{
	struct mcp23016 mock_dev = {
		.i2c_addr = BASE_ADDR,
		.i2c_dev = &(struct i2cd){0},
		.gpio_chip = &(struct gpiod_chip){0},
		.gpio_line = &(struct gpiod_line){0}
	};
	uint8_t mock_buf[] = {REG_GP0, 0x55, 0xaa};
	int rc;

	expect_value(mock_i2cd_write, dev, mock_dev.i2c_dev);
	expect_value(mock_i2cd_write, addr, mock_dev.i2c_addr);
	expect_memory(mock_i2cd_write, buf, mock_buf, sizeof(mock_buf));
	expect_value(mock_i2cd_write, len, sizeof(mock_buf));
	will_return(mock_i2cd_write, 0);

	/* Check behavior when function succeeds */
	rc = mcp23016_set_port(&mock_dev, 0xaa55);

	assert_return_code(rc, 0);
}

void test_mcp23016_get_output(void **state)
{
	struct mcp23016 mock_dev = {
		.i2c_addr = BASE_ADDR,
		.i2c_dev = &(struct i2cd){0},
		.gpio_chip = &(struct gpiod_chip){0},
		.gpio_line = &(struct gpiod_line){0}
	};
	uint8_t mock_write_buf[] = {REG_OLAT0};
	uint8_t mock_read_buf[] = {0x55, 0xaa};
	uint16_t output;
	int rc;

	expect_value(mock_i2cd_write_read, dev, mock_dev.i2c_dev);
	expect_value(mock_i2cd_write_read, addr, mock_dev.i2c_addr);
	expect_memory(mock_i2cd_write_read, write_buf, mock_write_buf, sizeof(mock_write_buf));
	expect_value(mock_i2cd_write_read, write_len, sizeof(mock_write_buf));
	will_return(mock_i2cd_write_read, mock_read_buf); /* read_buf */
	expect_value(mock_i2cd_write_read, read_len, sizeof(mock_read_buf));
	will_return(mock_i2cd_write_read, 0);

	/* Check behavior when function succeeds */
	rc = mcp23016_get_output(&mock_dev, &output);

	assert_return_code(rc, 0);
	assert_int_equal(output, 0xaa55);
}

void test_mcp23016_set_output(void **state)
{
	struct mcp23016 mock_dev = {
		.i2c_addr = BASE_ADDR,
		.i2c_dev = &(struct i2cd){0},
		.gpio_chip = &(struct gpiod_chip){0},
		.gpio_line = &(struct gpiod_line){0}
	};
	uint8_t mock_buf[] = {REG_OLAT0, 0x55, 0xaa};
	int rc;

	expect_value(mock_i2cd_write, dev, mock_dev.i2c_dev);
	expect_value(mock_i2cd_write, addr, mock_dev.i2c_addr);
	expect_memory(mock_i2cd_write, buf, mock_buf, sizeof(mock_buf));
	expect_value(mock_i2cd_write, len, sizeof(mock_buf));
	will_return(mock_i2cd_write, 0);

	/* Check behavior when function succeeds */
	rc = mcp23016_set_output(&mock_dev, 0xaa55);

	assert_return_code(rc, 0);
}

void test_mcp23016_get_polarity(void **state)
{
	struct mcp23016 mock_dev = {
		.i2c_addr = BASE_ADDR,
		.i2c_dev = &(struct i2cd){0},
		.gpio_chip = &(struct gpiod_chip){0},
		.gpio_line = &(struct gpiod_line){0}
	};
	uint8_t mock_write_buf[] = {REG_IPOL0};
	uint8_t mock_read_buf[] = {0x55, 0xaa};
	uint16_t polarity;
	int rc;

	expect_value(mock_i2cd_write_read, dev, mock_dev.i2c_dev);
	expect_value(mock_i2cd_write_read, addr, mock_dev.i2c_addr);
	expect_memory(mock_i2cd_write_read, write_buf, mock_write_buf, sizeof(mock_write_buf));
	expect_value(mock_i2cd_write_read, write_len, sizeof(mock_write_buf));
	will_return(mock_i2cd_write_read, mock_read_buf); /* read_buf */
	expect_value(mock_i2cd_write_read, read_len, sizeof(mock_read_buf));
	will_return(mock_i2cd_write_read, 0);

	/* Check behavior when function succeeds */
	rc = mcp23016_get_polarity(&mock_dev, &polarity);

	assert_return_code(rc, 0);
	assert_int_equal(polarity, 0xaa55);
}

void test_mcp23016_set_polarity(void **state)
{
	struct mcp23016 mock_dev = {
		.i2c_addr = BASE_ADDR,
		.i2c_dev = &(struct i2cd){0},
		.gpio_chip = &(struct gpiod_chip){0},
		.gpio_line = &(struct gpiod_line){0}
	};
	uint8_t mock_buf[] = {REG_IPOL0, 0x55, 0xaa};
	int rc;

	expect_value(mock_i2cd_write, dev, mock_dev.i2c_dev);
	expect_value(mock_i2cd_write, addr, mock_dev.i2c_addr);
	expect_memory(mock_i2cd_write, buf, mock_buf, sizeof(mock_buf));
	expect_value(mock_i2cd_write, len, sizeof(mock_buf));
	will_return(mock_i2cd_write, 0);

	/* Check behavior when function succeeds */
	rc = mcp23016_set_polarity(&mock_dev, 0xaa55);

	assert_return_code(rc, 0);
}

void test_mcp23016_get_direction(void **state)
{
	struct mcp23016 mock_dev = {
		.i2c_addr = BASE_ADDR,
		.i2c_dev = &(struct i2cd){0},
		.gpio_chip = &(struct gpiod_chip){0},
		.gpio_line = &(struct gpiod_line){0}
	};
	uint8_t mock_write_buf[] = {REG_IODIR0};
	uint8_t mock_read_buf[] = {0x55, 0xaa};
	uint16_t direction;
	int rc;

	expect_value(mock_i2cd_write_read, dev, mock_dev.i2c_dev);
	expect_value(mock_i2cd_write_read, addr, mock_dev.i2c_addr);
	expect_memory(mock_i2cd_write_read, write_buf, mock_write_buf, sizeof(mock_write_buf));
	expect_value(mock_i2cd_write_read, write_len, sizeof(mock_write_buf));
	will_return(mock_i2cd_write_read, mock_read_buf); /* read_buf */
	expect_value(mock_i2cd_write_read, read_len, sizeof(mock_read_buf));
	will_return(mock_i2cd_write_read, 0);

	/* Check behavior when function succeeds */
	rc = mcp23016_get_direction(&mock_dev, &direction);

	assert_return_code(rc, 0);
	assert_int_equal(direction, 0xaa55);
}

void test_mcp23016_set_direction(void **state)
{
	struct mcp23016 mock_dev = {
		.i2c_addr = BASE_ADDR,
		.i2c_dev = &(struct i2cd){0},
		.gpio_chip = &(struct gpiod_chip){0},
		.gpio_line = &(struct gpiod_line){0}
	};
	uint8_t mock_buf[] = {REG_IODIR0, 0x55, 0xaa};
	int rc;

	expect_value(mock_i2cd_write, dev, mock_dev.i2c_dev);
	expect_value(mock_i2cd_write, addr, mock_dev.i2c_addr);
	expect_memory(mock_i2cd_write, buf, mock_buf, sizeof(mock_buf));
	expect_value(mock_i2cd_write, len, sizeof(mock_buf));
	will_return(mock_i2cd_write, 0);

	/* Check behavior when function succeeds */
	rc = mcp23016_set_direction(&mock_dev, 0xaa55);

	assert_return_code(rc, 0);
}

void test_mcp23016_has_interrupt(void **state)
{
	struct mcp23016 mock_dev = {
		.i2c_addr = BASE_ADDR,
		.i2c_dev = &(struct i2cd){0},
		.gpio_chip = &(struct gpiod_chip){0},
		.gpio_line = &(struct gpiod_line){0}
	};
	int res;

	expect_value(mock_gpiod_line_get_value, line, mock_dev.gpio_line);
	will_return(mock_gpiod_line_get_value, 0);

	/* Check behavior when function succeeds */
	res = mcp23016_has_interrupt(&mock_dev);

	assert_int_equal(res, 0);
}

void test_mcp23016_get_interrupt(void **state)
{
	struct mcp23016 mock_dev = {
		.i2c_addr = BASE_ADDR,
		.i2c_dev = &(struct i2cd){0},
		.gpio_chip = &(struct gpiod_chip){0},
		.gpio_line = &(struct gpiod_line){0}
	};
	uint8_t mock_write_buf[] = {REG_INTCAP0};
	uint8_t mock_read_buf[] = {0x55, 0xaa};
	uint16_t interrupt;
	int rc;

	expect_value(mock_i2cd_write_read, dev, mock_dev.i2c_dev);
	expect_value(mock_i2cd_write_read, addr, mock_dev.i2c_addr);
	expect_memory(mock_i2cd_write_read, write_buf, mock_write_buf, sizeof(mock_write_buf));
	expect_value(mock_i2cd_write_read, write_len, sizeof(mock_write_buf));
	will_return(mock_i2cd_write_read, mock_read_buf); /* read_buf */
	expect_value(mock_i2cd_write_read, read_len, sizeof(mock_read_buf));
	will_return(mock_i2cd_write_read, 0);

	/* Check behavior when function succeeds */
	rc = mcp23016_get_interrupt(&mock_dev, &interrupt);

	assert_return_code(rc, 0);
	assert_int_equal(interrupt, 0xaa55);
}

void test_mcp23016_get_control(void **state)
{
	struct mcp23016 mock_dev = {
		.i2c_addr = BASE_ADDR,
		.i2c_dev = &(struct i2cd){0},
		.gpio_chip = &(struct gpiod_chip){0},
		.gpio_line = &(struct gpiod_line){0}
	};
	uint8_t mock_write_buf[] = {REG_IOCON0};
	uint8_t mock_read_buf[] = {0x55, 0xaa};
	uint16_t control;
	int rc;

	expect_value(mock_i2cd_write_read, dev, mock_dev.i2c_dev);
	expect_value(mock_i2cd_write_read, addr, mock_dev.i2c_addr);
	expect_memory(mock_i2cd_write_read, write_buf, mock_write_buf, sizeof(mock_write_buf));
	expect_value(mock_i2cd_write_read, write_len, sizeof(mock_write_buf));
	will_return(mock_i2cd_write_read, mock_read_buf); /* read_buf */
	expect_value(mock_i2cd_write_read, read_len, sizeof(mock_read_buf));
	will_return(mock_i2cd_write_read, 0);

	/* Check behavior when function succeeds */
	rc = mcp23016_get_control(&mock_dev, &control);

	assert_return_code(rc, 0);
	assert_int_equal(control, 0xaa55);
}

void test_mcp23016_set_control(void **state)
{
	struct mcp23016 mock_dev = {
		.i2c_addr = BASE_ADDR,
		.i2c_dev = &(struct i2cd){0},
		.gpio_chip = &(struct gpiod_chip){0},
		.gpio_line = &(struct gpiod_line){0}
	};
	uint8_t mock_buf[] = {REG_IOCON0, 0x55, 0xaa};
	int rc;

	expect_value(mock_i2cd_write, dev, mock_dev.i2c_dev);
	expect_value(mock_i2cd_write, addr, mock_dev.i2c_addr);
	expect_memory(mock_i2cd_write, buf, mock_buf, sizeof(mock_buf));
	expect_value(mock_i2cd_write, len, sizeof(mock_buf));
	will_return(mock_i2cd_write, 0);

	/* Check behavior when function succeeds */
	rc = mcp23016_set_control(&mock_dev, 0xaa55);

	assert_return_code(rc, 0);
}

int main(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_mcp23016_open),
		cmocka_unit_test(test_mcp23016_open_fail_malloc),
		cmocka_unit_test(test_mcp23016_open_fail_i2c_addr),
		cmocka_unit_test(test_mcp23016_open_fail_i2c_dev),
		cmocka_unit_test(test_mcp23016_open_fail_gpio_chip),
		cmocka_unit_test(test_mcp23016_open_fail_gpio_line),
		cmocka_unit_test(test_mcp23016_open_fail_gpio_line_flags),
		cmocka_unit_test(test_mcp23016_close),
		cmocka_unit_test(test_mcp23016_reset),
		cmocka_unit_test(test_mcp23016_get_port),
		cmocka_unit_test(test_mcp23016_set_port),
		cmocka_unit_test(test_mcp23016_get_output),
		cmocka_unit_test(test_mcp23016_set_output),
		cmocka_unit_test(test_mcp23016_get_polarity),
		cmocka_unit_test(test_mcp23016_set_polarity),
		cmocka_unit_test(test_mcp23016_get_direction),
		cmocka_unit_test(test_mcp23016_set_direction),
		cmocka_unit_test(test_mcp23016_has_interrupt),
		cmocka_unit_test(test_mcp23016_get_interrupt),
		cmocka_unit_test(test_mcp23016_get_control),
		cmocka_unit_test(test_mcp23016_set_control)
	};

	return cmocka_run_group_tests(tests, setup, teardown);
}
