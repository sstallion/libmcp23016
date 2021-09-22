# libmcp23016 Documentation                                             {#index}

libmcp23016 provides a simple and straightforward API for accessing Microchip
MCP23016 I/O expanders from userspace. It relies on the [libgpiod][1] and
[libi2cd][2] libraries to interface with the Linux kernel.

libmcp23016 development is hosted on [GitHub][3]. A [code coverage report][4] is
also available.

## Getting Started

libmcp23016's main data structures and functions are documented in the
[Main API](@ref main) module. Most uses of the API are straightforward: a
MCP23016 device handle is first created by calling mcp23016_open(), which is
then passed to supporting functions followed by a call to mcp23016_close() to
close the handle and free associated memory. As the MCP23016 lacks a hardware
reset, it is advised that a software reset be issued by calling mcp23016_reset()
after opening the device handle to ensure the device is in a consistent state.

The following example demonstrates getting the port value from a MCP23016 device
at position 0 (I2C slave address `0x20`):

@include example.c

Care should be taken if the device handle is shared between threads as
libmcp23016 is not inherently thread-safe. Calls using the same handle should be
restricted to a single thread or synchronized using a mutual exclusion
mechanism.

## License

libmcp23016 is distributed under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either version 2.1 of the
License, or any later version. See [COPYING] for more details.

[1]: https://git.kernel.org/pub/scm/libs/libgpiod/libgpiod.git/
[2]: https://github.com/sstallion/libi2cd
[3]: https://github.com/sstallion/libmcp23016
[4]: https://sstallion.github.io/libmcp23016/coverage/

[COPYING]: https://github.com/sstallion/libmcp23016/blob/master/COPYING
