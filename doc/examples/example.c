#include <stdio.h>
#include <stdlib.h>
#include <mcp23016.h>

int main(void)
{
	const struct mcp23016_config config = {
		.num = 0,         /* I2C slave address: 0x20 */
		.i2c_num = 1,     /* I2C character device: /dev/i2c-1 */
		.gpio_num = 0,    /* GPIO chip: /dev/gpiochip0 */
		.gpio_offset = 19 /* GPIO line: GPIO19 */
	};
	struct mcp23016 *dev;
	uint16_t port;

	/* Open MCP23016 device */
	dev = mcp23016_open(&config);
	if (dev == NULL) {
		perror(NULL);
		return EXIT_FAILURE;
	}

	/* Reset registers to POR defaults */
	if (mcp23016_reset(dev) < 0) {
		perror(NULL);
		mcp23016_close(dev);
		return EXIT_FAILURE;
	}

	/* Get the port value */
	if (mcp23016_get_port(dev, &port) < 0) {
		perror(NULL);
		mcp23016_close(dev);
		return EXIT_FAILURE;
	}

	printf("port value: %#" PRIx16 "\n", port);

	/* Close MCP23016 device handle */
	mcp23016_close(dev);

	return EXIT_SUCCESS;
}
