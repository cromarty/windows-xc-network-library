#!/bin/bash

#CCPREFIX=/usr/bin/i686-w64-mingw32-
CCPREFIX=/usr/bin/x86_64-w64-mingw32-

${CCPREFIX}gcc -c test1.c -I../src
${CCPREFIX}gcc -o test1.exe \
    test1.o \
    -L../src/.libs \
    -lmynetlib-0

