#ifndef H_LIBVOL_I_SRC_LIBVOL_PROTIUM
#define H_LIBVOL_I_SRC_LIBVOL_PROTIUM
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "libvol.h"

#define VOL_API_LEVEL       0
#define VOL_HEAD_SIZE       128     //FOR DEBUG SAFTY

//in memory flag
#define VOL_FLAG_WRITE      (1<<0)
/**
 * Currently level=0, struct is fixed to 128byte
 * */
#pragma pack(push)
#pragma pack(1)
typedef struct vol_head_struct {
    char        tag[3];
    char        level;
    int32_t     ny,nx,nz;
    float       y0,x0,z0;
    float       dy,dx,dz;
    int32_t     yline0;
    int32_t     xline0;
    int32_t    _place_hold[20];
} vol_head_t;
#pragma pack(pop)

typedef struct protium_volid_struct {
    int         fid;
    int         flag;
    vol_head_t  header;
} protium_volid_t;

#endif
