#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <argp.h>
#include <string.h>
#include <assert.h>
#include <protium/libsu.h>
//--------------test this function!
#include "../libvol.h"

//TODO: add argp parameter parse
//volinfo                   gives dimension information
//volinfo -m percentile     gives certain percentile min/max value

static int flt_cmp(const void *p1, const void *p2)
{
    float f1 = *((const float*)p1);
    float f2 = *((const float*)p2);
    if(f1>f2)
        return 1;
    return (-1);
}

//default volinfo percentile_of_min_max infile.vol
int main(int argc, char *argv[])
{
    if(argc<3) {
        printf("Usage: %s percentile infile.vol\n", argv[0]);
        printf("          percentile in range [0:50]\n");
        exit(1);
    }
    float percent = atof(argv[1]);
    char *volname = argv[2];

    VOLID_t vol;
    int ny, nx, nz;

    //1. read data from a volume
    vol_open(&vol, volname, VOL_READONLY);
    vol_getdim(vol, &ny, &nx, &nz);
    float *data = calloc(ny*nx, nz*sizeof(float));
    vol_getvol(vol, data);
    vol_close(vol);
    float fmin=FLT_MAX, fmax=-FLT_MAX;
    for(int i=0; i<ny*nx*nz; i++) {
        fmin=(data[i]<fmin)?(data[i]):(fmin);
        fmax=(data[i]>fmax)?(data[i]):(fmax);
    }

    //2. sort the float data
    qsort(data, ny*nx*nz, sizeof(float), &flt_cmp);
    int imin = ny*nx*nz*(percent/100.0f);
    int imax = ny*nx*nz-1-imin;
    assert(imax>imin && imin>=0 && imax<ny*nx*nz);

    float pmin=data[imin];
    float pmax=data[imax];

    free(data);

    printf("%s @ %4.1f%% min(%f)/max(%f): ", volname, percent, fmin, fmax);
    printf("  ny=%4d nx=%4d nz=%4d\n", ny, nx, nz);
    printf("  pmin: %14f @ %d\n", pmin, imin);
    printf("  pmax: %14f @ %d\n", pmax, imax);
    return 0;
}
