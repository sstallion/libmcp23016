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

#ifndef MCP23016_PRIVATE_H
#define MCP23016_PRIVATE_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <mcp23016.h>

#include <stdint.h>
#include <gpiod.h>
#include <i2cd.h>

#define LOW(x)		(((x) >> 0) & 0xff)
#define HIGH(x)		(((x) >> 8) & 0xff)

#ifdef __GLIBC__
extern const char *__progname;
#define CONSUMER	__progname
#else
#define CONSUMER	"libmcp23016"
#endif

/* Device Addresses */
#define BASE_ADDR	0x20
#define END_ADDR	0x27

/* Register Addresses */
#define REG_GP0		0x00	/* General Purpose I/O Port Register 0 */
#define REG_GP1		0x01	/* General Purpose I/O Port Register 1 */
#define REG_OLAT0	0x02	/* Output Latch Register 0 */
#define REG_OLAT1	0x03	/* Output Latch Register 1 */
#define REG_IPOL0	0x04	/* Input Polarity Port Register 0 */
#define REG_IPOL1	0x05	/* Input Polarity Port Register 1 */
#define REG_IODIR0	0x06	/* I/O Direction Register 0 */
#define REG_IODIR1	0x07	/* I/O Direction Register 1 */
#define REG_INTCAP0	0x08	/* Interrupt Captured Value for Port Register 0 */
#define REG_INTCAP1	0x09	/* Interrupt Captured Value for Port Register 1 */
#define REG_IOCON0	0x0a	/* I/O Expander Control Register 0 */
#define REG_IOCON1	0x0b	/* I/O Expander Control Register 1 */

struct mcp23016_device {
	uint16_t i2c_addr;		/**< I2C slave address. */
	struct i2cd *i2c_dev;		/**< Pointer to an I2C character device handle. */
};

struct mcp23016_interrupt {
	struct gpiod_chip *gpio_chip;	/**< Pointer to a GPIO chip object. */
	struct gpiod_line *gpio_line;	/**< Pointer to a GPIO line object. */
};

int mcp23016_register_read(struct mcp23016_device *dev, uint8_t reg, uint16_t *val);
int mcp23016_register_write(struct mcp23016_device *dev, uint8_t reg, uint16_t val);

#endif /* MCP23016_PRIVATE_H */
