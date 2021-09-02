#!/bin/bash

rm -rf build/lib.linux-x86_64-2.6/
gcc -c libvol.c -fPIC
ar cr libvol.a libvol.o
CFLAGS="-DPYTHON_RHEL6" python setup.py build
