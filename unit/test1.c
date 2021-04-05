#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <protium/libsu.h>
#include "../libvol.h"

int main()
{
    //1. read data from su1
    SUID_t su1, su2;
    int ninst, nsamp;
    su_open(&su1, "model_cp.su", SU_READONLY);
    ninst = su_ninst(su1);
    nsamp = su_nsamp(su1, 0);

    float *data = calloc(ninst, nsamp*sizeof(float));
    su_read(su1, "trace", data, 0, ninst);
    su_close(su1);

    //2. write data to a volume
    VOLID_t vol1, vol2;
    vol_open(&vol1, "xvol1c.vol", VOL_CREATE);
    vol_setdim(vol1, 1, ninst, nsamp);
    vol_setvol(vol1, data);
    vol_close(vol1);

    //3. read data from a volume
    int ny, nx, nz, *ns;
    vol_open(&vol2, "xvol1c.vol", VOL_READONLY);
    vol_getdim(vol2, &ny, &nx, &nz);
    printf("volume: ny=%d nx=%d nz=%d\n", ny, nx, nz);
    assert(ninst==ny*nx && nsamp==nz);
    memset(data, 0, ny*nx*nz*sizeof(float));
    vol_getvol(vol2, data);
    vol_close(vol2);

    //4.copy from vol2 to su2
    su_open(&su2, "model_test1c.su", SU_CREATE);
    su_nsamp(su2, nsamp);
    ns = calloc(ninst, sizeof(int));
    for(int i=0; i<ninst; i++)
        ns[i] = nsamp;
    su_write(su2, "ns", ns, 0, ninst);
    free(ns);
    su_write(su2, "trace", data, 0, ninst);
    su_close(su2);

    free(data);
    printf("runs to finish\n");
    return 0;
}
