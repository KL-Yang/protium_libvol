#!/usr/bin/python3
from distutils.core import setup, Extension

#https://docs.python.org/2/distutils/apiref.html#distutils.core.Extension

libpyvol = Extension('libpyvol',
                     include_dirs=["/usr/include/glib-2.0","/usr/include/python2.6"],
                     libraries = ['glib-2.0'],
                     library_dirs = ['/home/protium/lib'],
                     extra_compile_args = ["-g","-std=gnu99"],
                     sources = ["/home/eric/protium_libvol/libpyvol.c"])

setup (name = 'libpyvol',
       version = '',
       description = 'This is a test, hello buddy!',
       author = 'Yang Kunlun',
       author_email = 'yangkunlun@gmail.com',
       ext_modules = [libpyvol])
