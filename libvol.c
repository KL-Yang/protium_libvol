#ifndef C_LIBVOL_SRC_PROTIUM
#define C_LIBVOL_SRC_PROTIUM

#define _GNU_SOURCE
#include "vol_i.h"
#include "vol_utility.c"
#include "vol_open.c"
#include "vol_getdim.c"
#include "vol_getvol.c"
#include "vol_setdim.c"
#include "vol_setvol.c"
#include "vol_close.c"

void 
vol_export(const char *path, const void *vol, int ny, int nx, int nz)
{
    VOLID_t id;
    vol_open(&id, path, VOL_CREATE);
    vol_setdim(id, ny, nx, nz);
    vol_setvol(id, vol);
    vol_close(id);
}

void
vol_import(const char *path, void *vol, int ny, int nx, int nz)
{
    VOLID_t id; int my, mx, mz;
    vol_open(&id, path, VOL_READONLY);
    vol_getdim(id, &my, &mx, &mz);
    if(ny!=my || nx!=mx || nz!=mz) {
        printf("%s: data dimension on disk(%dx%dx%d mismatch!\n",
            __func__, my, mx, mz);
        abort();
    }
    vol_getvol(id, vol);
    vol_close(id);
}
#endif
