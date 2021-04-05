import os, sys
import numpy as np

sys.path.insert(0, './module')
sys.path.insert(0, '../protium_libsu/module')
import libpysu as su
import libpyvol as vol

##########################################################################
# 1. read data from su1
##########################################################################
su1   = su.open("model_cp.su", su.READONLY)
ninst = su.ninst(su1)
nsamp = su.nsamp(su1, 0)
data  = su.read(su1, "trace", 0, ninst)
su.close(su1)
print("#ninst=",ninst,"nsamp=",nsamp)

##########################################################################
# 2. write data to vl1 volume
##########################################################################
vl1   = vol.open("xvol1.vol", vol.CREATE)
vol.setdim(vl1, 1, ninst, nsamp);
vol.setvol(vl1, data)
vol.close(vl1)
print("#expected volume size=",ninst*nsamp*4+128)
del data

##########################################################################
# 3. read data from vl2 volume
##########################################################################
vl2   = vol.open("xvol1.vol", vol.READONLY)
dim   = vol.getdim(vl2)
dvol  = vol.getvol(vl2)
vol.close(vl2)
print("vl2-dim=", dim, " shape=", dvol.shape)

##########################################################################
# 4. copy everything except trace from su1 to su2
##########################################################################
su2   = su.open("model_test1py.su", su.CREATE)
su.nsamp(su2, nsamp)
ns    = nsamp*np.ones(ninst)
su.write(su2, "ns", ns, 0, ninst)
dvol  = dvol.reshape(-1, nsamp)
su.write(su2, "trace", dvol, 0, ninst)
su.close(su2)

print("#Reach success!")
