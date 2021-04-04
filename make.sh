#!/bin/bash

protium make
cd build; rm -rf build; make all; cd ..
#python3 test.py

#note: only if success, should do this!!!
cp libvol.h "$PROTIUM_HOME/include/protium/"
cp build/libvol.a "$PROTIUM_HOME/lib"

flags="--std=gnu99 -ggdb -O0 -I/home/protium/include/ -L/home/protium/lib"
links="build/libvol.a -lptcomm"
#gcc $flags test1.c $links -o test1.exe
#gcc $flags test2.c $links -o test2.exe
#gcc $flags test3.c $links -o test3.exe 

