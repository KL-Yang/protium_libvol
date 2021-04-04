#ifndef H_LIBVOL_SRC_LIBVOL_PROTIUM
#define H_LIBVOL_SRC_LIBVOL_PROTIUM

#define VOL_READONLY 0
#define VOL_CREATE   1
#define VOL_UPDATE   2
#define VOL_SUCCESS  0

//NOTE: later may consider have float/double type!

typedef struct protium_volid_struct * VOLID_t;

/**
 * For Debug other program, use this 6 APIs to dump/replace array
 * */
int vol_open(VOLID_t *id, const char *path, int flag);
int vol_getdim(VOLID_t id, int *ny, int *nx, int *nz);
int vol_getvol(VOLID_t id, float *vol);
int vol_setdim(VOLID_t id, int ny, int nx, int nz);
int vol_setvol(VOLID_t id, float *vol);
int vol_close(VOLID_t id);

/**
 * Get/Set geometry information of the volume.
 * */
int vol_get_y(VOLID_t id, int *ny, float *y0, float *dy, int *fy);
int vol_get_x(VOLID_t id, int *nx, float *x0, float *dx, int *fx);
int vol_get_z(VOLID_t id, int *nz, float *z0, float *dz, int *fz);
int vol_set_y(VOLID_t id, int ny, float y0, float dy, int fy);
int vol_set_x(VOLID_t id, int nx, float x0, float dx, int fx);
int vol_set_z(VOLID_t id, int nz, float z0, float dz, int fz);

/**
 * Partial read/write volume data
 * */
int vol_pgetvol(VOLID_t id, float *vol, int first, int n);
int vol_psetvol(VOLID_t id, float *vol, int first, int n);

#endif
