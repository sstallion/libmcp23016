#include <stdio.h>
#include <stdlib.h>
#include <mcp23016.h>

int main(void)
{
	struct mcp23016_device *dev;
	uint16_t port;

	/* Open MCP23016 device */
	dev = mcp23016_open("/dev/i2c-1", 0);
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
