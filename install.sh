#!/bin/sh -e

# Usage: install.sh TARGET [RELATIVE PATHS ...]
#
# e.g. ./install.sh /usr/local foo/file1 foo/file2 ...
# This creates the files /usr/local/foo/file1 and /usr/local/foo/file2

TARGETDIR=$1
shift

for src in "$@"; do
    dest=$TARGETDIR/$src
    mkdir -p $(dirname $dest)
    cp $src $dest
    echo $dest
done
