# Hacking

If you have an idea or feature request, please open an [issue][1] even if you
don't have time to contribute!

## Making Changes

> **Note**: These instructions assume you have a working Linux installation
> (most distributions are supported) with GCC and GNU Autotools installed.

To get started, [fork][2] this repository on GitHub and clone a working copy for
development:

    $ git clone git@github.com:YOUR-USERNAME/libmcp23016.git

Once cloned, change the directory to your working copy and update generated
configuration files by issuing:

    $ autoreconf --install

From here, follow the build instructions documented in [README.md].

Once you are finished making changes, be sure to check the output of `make
check` and `make distcheck`. At a minimum, there should be no test regressions
and additional tests should be added for new functionality.

Finally, commit your changes and create a [pull request][3] against the `master`
branch for review.

## Making New Releases

Making new releases is automated by GitHub Actions. Releases should only be
created from the `master` branch; as such `master` should be passing tests at
all times.

To make a new release, follow these steps:

1. Update the package version in [configure.ac] and check the distribution by
   issuing:

        $ autoreconf && ./configure && make distcheck

2. Add release notes to [NEWS.md] and verify by issuing:

        $ build-aux/release-notes.sh v<version>

3. Commit outstanding changes by issuing:

        $ git commit -a -m 'Release v<version>'

4. Create a release tag by issuing:

        $ git tag -a -m 'Release v<version>' v<version>

5. Push changes to the remote `master` branch by issuing:

        $ git push origin master --tags

6. Verify the results of the [gh-pages][4] and [release][5] workflows.

## License

By contributing to this repository, you agree that your contributions will be
distributed under the terms of the GNU Lesser General Public License as
published by the Free Software Foundation; either version 2.1 of the License, or
any later version. See [COPYING] for more details.

[1]: https://github.com/sstallion/libmcp23016/issues
[2]: https://docs.github.com/en/get-started/quickstart/fork-a-repo
[3]: https://docs.github.com/en/github/collaborating-with-pull-requests
[4]: https://github.com/sstallion/libmcp23016/actions/workflows/gh-pages.yml
[5]: https://github.com/sstallion/libmcp23016/actions/workflows/release.yml

[COPYING]: COPYING
[NEWS.md]: NEWS.md
[README.md]: README.md
[configure.ac]: configure.ac
