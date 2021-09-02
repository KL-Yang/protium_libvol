#!/usr/bin/python3
import os, sys, argparse
import numpy as np

protium_home=os.getenv('PROTIUM_HOME')
sys.path.insert(0, protium_home+'/pylib')
sys.path.insert(0, '../module')
sys.path.insert(0, '../../protium_libsu/module')
import libpysu as su
import libpyvol as vol

def arg_parser_setup():
    parser = argparse.ArgumentParser()
    parser.add_argument("--nx",    help="length of nx dimension, default is the same as ntraces in inp.su")
    parser.add_argument("su_name", help="input su file")
    parser.add_argument("volname", help="output vol file")
    args = parser.parse_args()
    return args

def su2vol_copy(su1, vl1, ny, nx):
    i=0
    for y in range(ny):
        data = su.read(su1, "trace", i, nx)
        vol.psetvol(vl1, data, i, nx)
        i=i+nx
    return

if __name__=="__main__":
    args = arg_parser_setup()
    su_name = args.su_name
    volname = args.volname

    su1   = su.open(su_name, su.READONLY)
    ninst = su.ninst(su1)
    if args.nx is None:
        args.nx = ninst
    args.nx=int(args.nx)
    nsamp = su.nsamp(su1, 0)

    ny = int(ninst/args.nx)
    if ninst%args.nx!=0:
        print("Dimension error!")
        sys.exit(1)

    print("#ninst=",ninst,"nsamp=",nsamp)
    print("#ny=",ny," nx=", args.nx)

    vl1 = vol.open(volname, vol.CREATE)
    vol.setdim(vl1, ny, args.nx, nsamp);

    su2vol_copy(su1, vl1, ny, args.nx)

    su.close(su1)
    vol.close(vl1)
    print("#expected volume size=",ninst*nsamp*4+128)
    print("#Reach success!")
