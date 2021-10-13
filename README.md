# libmcp23016

libmcp23016 provides a simple and straightforward API for accessing Microchip
MCP23016 I/O expanders from userspace. It relies on the [libgpiod][1] and
[libi2cd][2] libraries to interface with the Linux kernel.

libmcp23016 documentation is hosted on [GitHub Pages][3]. A [code coverage report][4]
is also available.

## Installation

> **Note**: These instructions assume you are installing from a source
> distribution and have a working Linux installation (most distributions are
> supported) with GCC and GNU Autotools installed. If installing from source
> control, see [HACKING.md].

[libgpiod][1] and [libi2cd][2] are required and should be installed using the
system package manager (eg. `libgpiod-dev` and `libi2cd-dev` on Debian-based
distributions via `apt-get`).

[cmocka][5] is required for building tests and should also be installed using
the system package manager (eg. `libcmocka-dev` on Debian-based distributions
via `apt-get`).

If you wish to build without tests, the `--disable-tests` option may be passed
to `configure`:

    $ ./configure --disable-tests

To build and install, issue the following:

    $ ./configure
    $ make all
    $ make check
    $ make install

By default, `make install` will install files in `/usr/local`, which may require
superuser privileges.

## Hacking

Pull requests are welcome! See [HACKING.md] for more details.

## License

libmcp23016 is distributed under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either version 2.1 of the
License, or any later version. See [COPYING] for more details.

[1]: https://git.kernel.org/pub/scm/libs/libgpiod/libgpiod.git/
[2]: https://github.com/sstallion/libi2cd
[3]: https://sstallion.github.io/libmcp23016/
[4]: https://sstallion.github.io/libmcp23016/coverage/
[5]: https://cmocka.org/

[COPYING]: COPYING
[HACKING.md]: HACKING.md
