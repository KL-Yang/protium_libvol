#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <protium/libsu.h>
#include "../libvol.h"

int main(int argc, char *argv[])
{
    if(argc<3) {
        printf("USAGE: su2vol a.su b.vol\n"
               "convert su file to vol file!\n");
        exit(1);
    }
    const char *suname = argv[1];
    const char *volname = argv[2];
    //1. read data from su1
    SUID_t su1;
    int ninst, nsamp;
    su_open(&su1, suname, SU_READONLY);
    ninst = su_ninst(su1);
    nsamp = su_nsamp(su1, 0);
    float *data = calloc(ninst, nsamp*sizeof(float));
    su_read(su1, "trace", data, 0, ninst);
    su_close(su1);

    //2. write data to a volume
    VOLID_t vol1;
    vol_open(&vol1, volname, VOL_CREATE);
    vol_setdim(vol1, 1, ninst, nsamp);
    vol_setvol(vol1, data);
    vol_close(vol1);

    free(data);
    printf("runs to finish\n");
    return 0;
}
