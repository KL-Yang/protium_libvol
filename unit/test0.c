#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <protium/libsu.h>
#include "../libvol.h"

int main()
{
    int ny = 1000, nx = 1000, nz = 1000;
    float *data = calloc(ny*nx, nz*sizeof(float));
    float *p = data;

    for(int i=0; i<ny*nx; i++, p+=nz)
        for(int j=0; j<nz; j++)
            p[j] = random();
    //2. write data to a volume
    VOLID_t vol1, vol2;
    vol_open(&vol1, "xvol0c.vol", VOL_CREATE);
    vol_setdim(vol1, ny, nx, nz);
    vol_setvol(vol1, data);
    vol_close(vol1);

    //3. read data from a volume
    vol_open(&vol2, "xvol0c.vol", VOL_READONLY);
    vol_getdim(vol2, &ny, &nx, &nz);
    printf("volume: ny=%d nx=%d nz=%d\n", ny, nx, nz);
    memset(data, 0, ny*nx*nz*sizeof(float));
    vol_getvol(vol2, data);
    vol_close(vol2);

    vol_open(&vol1, "xvol0c2.vol", VOL_CREATE);
    vol_setdim(vol1, ny, nx, nz);
    for(int i=0; i<ny; i++)
        vol_psetvol(vol1, data+i*nx*nz, i*nx, nx);
    vol_close(vol1);

    free(data);
    printf("runs to finish\n");
    return 0;
}
