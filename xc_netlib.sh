#!/bin/bash
#
# This script is designed to cross-compile, on Linux, the Windows network
# library in this repository using the mingw compiler
#


# This will be passed to ./configure in the --host argument
HOST=x86_64-w64-mingw32

# The C compiler
# Change if you are either using a different Windows cross-compiler, or if it
# is installed in a different path
CC="/usr/bin/${HOST}-gcc"

# C pre-processor flags
# Change if your compiler keeps stuff in a different place
CPPFLAGS="-I/usr/${HOST}/include"

# LDFLAGS.
# Contains the path to the cross-compiler Windows libraries. Can also
# contain other stuff
LDFLAGS="-L/usr/${HOST}/lib"

#
# Call ./configure with correct arguments to cross-compile the library.
#
# Swap shared and static switches around to change from building a DLL
# to a static library.
#

./configure \
    --host=${HOST} \
    --disable-static \
    --enable-shared




