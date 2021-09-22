#!/bin/sh
# SPDX-License-Identifier: FSFAP
# Copyright (C) 2021 Steven Stallion <sstallion@gmail.com>
#
# Copying and distribution of this file, with or without modification,
# are permitted in any medium without royalty provided the copyright
# notice and this notice are preserved.  This file is offered as-is,
# without any warranty.

set -e

usage() {
  cat <<EOF
Usage: $0 [OPTION]... [VERSION]

Generate release notes from a Markdown formatted file using ATX-style headers.
If VERSION is not specified, the most recent tag in source control that is
reachable from the current commit will be used.

Options:
  -h, --help          display this help and exit
  -f, --file=FILE     search FILE for release notes
  -l, --level=LEVEL   header LEVEL to match

Report bugs to <https://github.com/sstallion/automagic/issues>.
EOF
  exit 1
}

for file in CHANGELOG.md HISTORY.md NEWS.md RELEASES.md; do
  test -f "$file" && break
done

level=2
while test $# -gt 0; do
  case $1 in
    --)
      shift
      break
      ;;
    -h | --help)
      usage
      ;;
    -f | --file)
      file=$2
      shift
      ;;
    -l | --level)
      level=$2
      shift
      ;;
    -*)
      echo "$0: invalid option: $1"
      usage
      ;;
    *)
      break
      ;;
  esac
  shift
done

if test $# -gt 1; then
  echo "$0: invalid number of arguments"
  usage
fi

if test $# = 1; then
  version=$1
else
  # Match lightweight tags to work around missing tag history in shallow
  # clones; see <https://stackoverflow.com/a/51727114>.
  version=`git describe --tags --abbrev=0`
fi

# Limit matches to headers at a specific level. This allows use of advanced
# formats like Keep a Changelog <https://keepachangelog.com/en/1.0.0/>.
h=`eval "printf '#%.0s' {1..$level}"`

# Escape version text before matching.
v=`echo $version | sed 's/[.^$[|+*?(){}\\/]/\\\\&/g'`

# Print lines within an ATX-style header that matches version text, stripping
# empty lines at the beginning and end. The sed(1) script fragments below were
# adapted from <http://sed.sourceforge.net/sed1line.txt>.
gawk "/^$h\s.*$v([^0-9.-]|$)/{h=1; next} /^($h| {,3}\[[^]]+]:)\s/{h=0} h" $file | \
  sed -e '/./,$!d' -e :a -e '/^\n*$/{$d;N;ba' -e '}'

exit 0
