#!/usr/bin/bash

flags="--std=gnu99 -ggdb -O0 -I/home/protium/include/ -L/home/protium/lib"
links="../build/libvol.a -lsu -lptcomm"
gcc $flags test0.c $links -o test0.exe
gcc $flags test1.c $links -o test1.exe
gcc $flags su2vol.c $links -o su2vol
gcc $flags volinfo.c $links -o volinfo
