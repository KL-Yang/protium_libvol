#!/usr/bin/python3
import os, sys, argparse
import numpy as np

protium_home=os.getenv('PROTIUM_HOME')
sys.path.insert(0, protium_home+'/pylib')
import libpyvol as vol

# Help
# Create a 2D vol slice from 3D vol and save as vol too
# --yslice --xslice --zslice

def arg_parser_setup():
    parser = argparse.ArgumentParser()
    parser.add_argument("--yslice", help="yslice index", type=int)
    parser.add_argument("--xslice", help="xslice index")
    parser.add_argument("--zslice", help="zslice index")
    parser.add_argument("inp_vol",  help="input vol file")
    parser.add_argument("out_vol",  help="output vol file")
    args = parser.parse_args()
    return args

def get_yslice(v1, ny, nx, nz, iy):
    return vol.pgetvol(v1, iy*nx, nx)

def get_xslice(v1, ny, nx, nz, ix):
    data = np.zeros((ny,nz), dtype=np.float32)
    for iy in range(ny):
        ydat = vol.pgetvol(v1, iy*nx, nx)
        data[iy][:] = ydat[ix][:]
    return data

def get_zslice(v1, ny, nx, nz, iz):
    data = np.zeros((ny,nx), dtype=np.float32)
    for iy in range(ny):
        ydat = vol.pgetvol(v1, iy*nx, nx)
        for ix in range(nx):
            data[iy][ix] = ydat[ix][iz]
    return data

if __name__=="__main__":
    args = arg_parser_setup()
    vol_inp = args.inp_vol
    vol_out = args.out_vol

    v1 = vol.open(vol_inp, vol.READONLY)
    v2 = vol.open(vol_out, vol.CREATE)
    ny, nx, nz = vol.getdim(v1)

    if args.yslice is not None:
        yslice = int(args.yslice)
        assert(yslice<ny and yslice>=0)
        data = get_yslice(v1, ny, nx, nz, yslice)
        vol.setdim(v2, 1, nx, nz)
        print("#yslice=", yslice)
    elif args.xslice is not None:
        xslice = int(args.xslice)
        assert(xslice<nx and xslice>=0)
        data = get_xslice(v1, ny, nx, nz, xslice)
        vol.setdim(v2, 1, ny, nz)
        print("#xslice=", xslice)
    elif args.zslice is not None:
        zslice = int(args.zslice)
        assert(zslice<nz and zslice>=0)
        data = get_zslice(v1, ny, nx, nz, zslice)
        vol.setdim(v2, 1, ny, nx)
        print("#zslice=", zslice)
    else:
        print("Error: must provide a slice")
        sys.exit(1)

    vol.close(v1)
    vol.setvol(v2, data)
    vol.close(v2)

    print("#Reach success!")
