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

#ifndef MCP23016_H
#define MCP23016_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup main Main API
 *
 * @brief Main data structures and functions.
 *
 * <tt>\#include <mcp23016.h></tt>
 *
 * @{
 */

/**
 * @enum mcp23016_control
 * @brief Enum that describes I/O control values.
 */
enum mcp23016_control {
	MCP23016_CONTROL_IARES_NORMAL = 0,	/**< Normal interrupt activity resolution (32ms). */
	MCP23016_CONTROL_IARES_FAST = 1		/**< Fast interrupt activity resolution (200us). */
};

/**
 * @struct mcp23016_device
 * @brief Handle to a MCP23016 device.
 */
struct mcp23016_device;

/**
 * @brief Open the MCP23016 device specified by @p path and @p num.
 *
 * @param path Pointer to an I2C character device.
 * @param num  Relative position of device on I2C bus (ie. @c AD0-2).
 *
 * @return Pointer to a MCP23016 device handle, or @c NULL on error with @c
 * errno set appropriately.
 */
struct mcp23016_device *mcp23016_open(const char *path, unsigned int num);

/**
 * @brief Close a MCP23016 device handle and free associated memory.
 *
 * @param dev Pointer to a MCP23016 device handle.
 *
 * Once closed, @p dev is no longer valid for use.
 */
void mcp23016_close(struct mcp23016_device *dev);

/**
 * @brief Issue a software reset, which resets registers to POR defaults and
 * clears pending interrupts.
 *
 * @param dev Pointer to a MCP23016 device handle.
 *
 * @return 0 on success, or -1 on error with @c errno set appropriately.
 */
int mcp23016_reset(struct mcp23016_device *dev);

/**
 * @brief Get the port value.
 *
 * @param dev Pointer to a MCP23016 device handle.
 * @param val Pointer to the value to receive.
 *
 * @return 0 on success, or -1 on error with @c errno set appropriately.
 *
 * This function returns the value of the @c GP0 and @c GP1 registers in the
 * low and high bytes, respectively.
 */
int mcp23016_get_port(struct mcp23016_device *dev, uint16_t *val);

/**
 * @brief Set the port value.
 *
 * @param dev Pointer to a MCP23016 device handle.
 * @param val The value to set.
 *
 * @return 0 on success, or -1 on error with @c errno set appropriately.
 *
 * This function writes the value of the @c GP0 and @c GP1 registers in the
 * low and high bytes, respectively.
 */
int mcp23016_set_port(struct mcp23016_device *dev, uint16_t val);

/**
 * @brief Get the output latch value.
 *
 * @param dev Pointer to a MCP23016 device handle.
 * @param val Pointer to the value to receive.
 *
 * @return 0 on success, or -1 on error with @c errno set appropriately.
 *
 * This function returns the value of the @c OLAT0 and @c OLAT1 registers in
 * the low and high bytes, respectively.
 */
int mcp23016_get_output(struct mcp23016_device *dev, uint16_t *val);

/**
 * @brief Set the output latch value.
 *
 * @param dev Pointer to a MCP23016 device handle.
 * @param val The value to set.
 *
 * @return 0 on success, or -1 on error with @c errno set appropriately.
 *
 * This function writes the value of the @c OLAT0 and @c OLAT1 registers in
 * the low and high bytes, respectively.
 */
int mcp23016_set_output(struct mcp23016_device *dev, uint16_t val);

/**
 * @brief Get the input polarity value.
 *
 * @param dev Pointer to a MCP23016 device handle.
 * @param val Pointer to the value to receive.
 *
 * @return 0 on success, or -1 on error with @c errno set appropriately.
 *
 * This function returns the value of the @c IPOL0 and @c IPOL1 registers in
 * the low and high bytes, respectively.
 */
int mcp23016_get_polarity(struct mcp23016_device *dev, uint16_t *val);

/**
 * @brief Set the input polarity value.
 *
 * @param dev Pointer to a MCP23016 device handle.
 * @param val The value to set.
 *
 * @return 0 on success, or -1 on error with @c errno set appropriately.
 *
 * This function writes the value of the @c IPOL0 and @c IPOL1 registers in
 * the low and high bytes, respectively.
 */
int mcp23016_set_polarity(struct mcp23016_device *dev, uint16_t val);

/**
 * @brief Get the I/O direction value.
 *
 * @param dev Pointer to a MCP23016 device handle.
 * @param val Pointer to the value to receive.
 *
 * @return 0 on success, or -1 on error with @c errno set appropriately.
 *
 * This function returns the value of the @c IODIR0 and @c IODIR1 registers in
 * the low and high bytes, respectively.
 */
int mcp23016_get_direction(struct mcp23016_device *dev, uint16_t *val);

/**
 * @brief Set the I/O direction value.
 *
 * @param dev  Pointer to a MCP23016 device handle.
 * @param val The value to set.
 *
 * @return 0 on success, or -1 on error with @c errno set appropriately.
 *
 * This function writes the value of the @c IODIR0 and @c IODIR1 registers in
 * the low and high bytes, respectively.
 */
int mcp23016_set_direction(struct mcp23016_device *dev, uint16_t val);

/**
 * @brief Get the interrupt capture value.
 *
 * @param dev Pointer to a MCP23016 device handle.
 * @param val Pointer to the value to receive.
 *
 * @return 0 on success, or -1 on error with @c errno set appropriately.
 *
 * This function returns the value of the @c INTCAP0 and @c INTCAP1 registers
 * in the low and high bytes, respectively.
 */
int mcp23016_get_interrupt(struct mcp23016_device *dev, uint16_t *val);

/**
 * @brief Clear interrupt status.
 *
 * @param dev Pointer to a MCP23016 device handle.
 *
 * @return 0 on success, or -1 on error with @c errno set appropriately.
 *
 * This function is equivalent to calling:
 * @code
 * uint16_t dummy;
 * mcp23016_get_interrupt(dev, &dummy);
 * @endcode
 */
static inline int mcp23016_clear_interrupt(struct mcp23016_device *dev)
{
	uint16_t dummy;

	return mcp23016_get_interrupt(dev, &dummy);
}

/**
 * @brief Get the I/O control value.
 *
 * @param dev Pointer to a MCP23016 device handle.
 * @param val Pointer to the value to receive.
 *
 * @return 0 on success, or -1 on error with @c errno set appropriately.
 *
 * This function returns the value of the @c IOCON0 and @c IOCON1 registers in
 * the low and high bytes, respectively.
 */
int mcp23016_get_control(struct mcp23016_device *dev, uint16_t *val);

/**
 * @brief Set the I/O control value.
 *
 * @param dev Pointer to a MCP23016 device handle.
 * @param val The value to set.
 *
 * @return 0 on success, or -1 on error with @c errno set appropriately.
 *
 * This function writes the value of the @c IOCON0 and @c IOCON1 registers in
 * the low and high bytes, respectively.
 */
int mcp23016_set_control(struct mcp23016_device *dev, uint16_t val);

/**
 * @defgroup interrupt Interrupt Output
 *
 * @brief Interrupt output functions.
 *
 * These functions manage the interrupt output functionality of the MCP23016,
 * which can be shared by multiple devices. Use of these functions is
 * considered optional and is only required if the caller wishes to be
 * notified of input state changes.
 *
 * @{
 */

/**
 * @struct mcp23016_interrupt
 * @brief Handle to a MCP23016 interrupt.
 */
struct mcp23016_interrupt;

/**
 * @brief Open the MCP23016 interrupt specified by @p path and @p offset.
 *
 * @param path   Pointer to a GPIO character device.
 * @param offset GPIO line offset.
 *
 * @return Pointer to a MCP23016 interrupt handle, or @c NULL on error with @c
 * errno set appropriately.
 */
struct mcp23016_interrupt *mcp23016_interrupt_open(const char *path, unsigned int offset);

/**
 * @brief Close a MCP23016 interrupt handle and free associated memory.
 *
 * @param intr Pointer to a MCP23016 interrupt handle.
 *
 * Once closed, @p interrupt is no longer valid for use.
 */
void mcp23016_interrupt_close(struct mcp23016_interrupt *intr);

/**
 * @brief Check interrupt output status.
 *
 * @param intr Pointer to a MCP23016 interrupt handle.
 *
 * @return Interrupt status (0 or 1) on success, or -1 on error with @c errno
 * set appropriately.
 */
int mcp23016_has_interrupt(struct mcp23016_interrupt *intr);

/** @} **/
/** @} **/

#ifdef __cplusplus
}
#endif

#endif /* MCP23016_H */
