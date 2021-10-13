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

#include <assert.h>
#include <endian.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <gpiod.h>
#include <i2cd.h>

struct mcp23016_device *mcp23016_open(const char *path, unsigned int num)
{
	struct mcp23016_device *dev;

	assert(path != NULL);

	dev = calloc(1, sizeof(*dev));
	if (dev == NULL)
		return NULL;

	dev->i2c_addr = BASE_ADDR + num;
	if (dev->i2c_addr < BASE_ADDR || dev->i2c_addr > END_ADDR) {
		errno = EINVAL;
		goto err;
	}

	dev->i2c_dev = i2cd_open(path);
	if (dev->i2c_dev == NULL)
		goto err;

	return dev;
err:
	free(dev);
	return NULL;
}

void mcp23016_close(struct mcp23016_device *dev)
{
	assert(dev != NULL);

	i2cd_close(dev->i2c_dev);

	free(dev);
}

int mcp23016_reset(struct mcp23016_device *dev)
{
	int res;

	assert(dev != NULL);

	/* The MCP23016 does not provide a hardware reset. The following
	 * sequence resets registers to POR defaults and clears pending
	 * interrupts.
	 */
	res = mcp23016_set_direction(dev, 0xffff);
	if (res < 0)
		return res;

	res = mcp23016_set_output(dev, 0x0000);
	if (res < 0)
		return res;

	res = mcp23016_set_polarity(dev, 0x0000);
	if (res < 0)
		return res;

	res = mcp23016_set_control(dev, 0x0000);
	if (res < 0)
		return res;

	return mcp23016_clear_interrupt(dev);
}

int mcp23016_register_read(struct mcp23016_device *dev, uint8_t reg, uint16_t *val)
{
	int res;

	assert(dev != NULL);
	assert(val != NULL);

	/* 16-bit registers are accessed by reading an additional byte.
	 * Values are encoded in little-endian byte order.
	 */
	res = i2cd_register_read(dev->i2c_dev, dev->i2c_addr, reg, val, sizeof(*val));
	if (res < 0)
		return res;

	*val = le16toh(*val);
	return 0;
}

int mcp23016_register_write(struct mcp23016_device *dev, uint8_t reg, uint16_t val)
{
	const uint8_t buf[] = {reg, LOW(val), HIGH(val)};
	int res;

	assert(dev != NULL);

	/* 16-bit registers are accessed by writing an additional byte.
	 * Values are encoded in little-endian byte order.
	 */
	res = i2cd_write(dev->i2c_dev, dev->i2c_addr, buf, sizeof(buf));
	if (res < 0)
		return res;

	return 0;
}

int mcp23016_get_port(struct mcp23016_device *dev, uint16_t *val)
{
	return mcp23016_register_read(dev, REG_GP0, val);
}

int mcp23016_set_port(struct mcp23016_device *dev, uint16_t val)
{
	return mcp23016_register_write(dev, REG_GP0, val);
}

int mcp23016_get_output(struct mcp23016_device *dev, uint16_t *val)
{
	return mcp23016_register_read(dev, REG_OLAT0, val);
}

int mcp23016_set_output(struct mcp23016_device *dev, uint16_t val)
{
	return mcp23016_register_write(dev, REG_OLAT0, val);
}

int mcp23016_get_polarity(struct mcp23016_device *dev, uint16_t *val)
{
	return mcp23016_register_read(dev, REG_IPOL0, val);
}

int mcp23016_set_polarity(struct mcp23016_device *dev, uint16_t val)
{
	return mcp23016_register_write(dev, REG_IPOL0, val);
}

int mcp23016_get_direction(struct mcp23016_device *dev, uint16_t *val)
{
	return mcp23016_register_read(dev, REG_IODIR0, val);
}

int mcp23016_set_direction(struct mcp23016_device *dev, uint16_t val)
{
	return mcp23016_register_write(dev, REG_IODIR0, val);
}

int mcp23016_get_interrupt(struct mcp23016_device *dev, uint16_t *val)
{
	return mcp23016_register_read(dev, REG_INTCAP0, val);
}

int mcp23016_get_control(struct mcp23016_device *dev, uint16_t *val)
{
	return mcp23016_register_read(dev, REG_IOCON0, val);
}

int mcp23016_set_control(struct mcp23016_device *dev, uint16_t val)
{
	return mcp23016_register_write(dev, REG_IOCON0, val);
}

struct mcp23016_interrupt *mcp23016_interrupt_open(const char *path, unsigned int offset)
{
	struct mcp23016_interrupt *intr;
	int flags, errsv;

	assert(path != NULL);

	intr = calloc(1, sizeof(*intr));
	if (intr == NULL)
		return NULL;

	intr->gpio_chip = gpiod_chip_open(path);
	if (intr->gpio_chip == NULL)
		goto err;

	intr->gpio_line = gpiod_chip_get_line(intr->gpio_chip, offset);
	if (intr->gpio_line == NULL)
		goto err;

	flags = GPIOD_LINE_REQUEST_FLAG_ACTIVE_LOW;
	if (gpiod_line_request_input_flags(intr->gpio_line, CONSUMER, flags) < 0)
		goto err;

	return intr;
err:
	errsv = errno;

	if (intr->gpio_line != NULL)
		gpiod_line_release(intr->gpio_line);

	if (intr->gpio_chip != NULL)
		gpiod_chip_close(intr->gpio_chip);

	free(intr);

	errno = errsv;
	return NULL;
}

void mcp23016_interrupt_close(struct mcp23016_interrupt *intr)
{
	assert(intr != NULL);

	gpiod_line_release(intr->gpio_line);
	gpiod_chip_close(intr->gpio_chip);

	free(intr);
}

int mcp23016_has_interrupt(struct mcp23016_interrupt *intr)
{
	return gpiod_line_get_value(intr->gpio_line);
}
