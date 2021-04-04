int vol_open(VOLID_t *id, const char *path, int flag)
{
    protium_volid_t *vol = *id = calloc(1, sizeof(protium_volid_t));
    if(flag==VOL_CREATE) {
        vol->fid = open(path, O_RDWR|O_CREAT,
                S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
    } else {
        if((vol->fid=open(path, O_RDWR))<0) {
            printf("%s: failed to open(%s)!\n", __func__, path);
            abort();
        }
        if(pread(vol->fid, &vol->header, 0, sizeof(vol_head_t))
            !=sizeof(vol_head_t)) {
            printf("%s: pread unfinished!\n", __func__);
            abort();
        }
        if(vol->header.level>VOL_API_LEVEL) {
            printf("%s: file was created by newer library?\n", 
                __func__); abort();
        }
        int64_t file_length, data_length;
        vol_head_t *h = &vol->header;
        file_length = lseek(vol->fid, 0, SEEK_END);
        data_length = sizeof(vol_head_t)+((int64_t)h->ny)*h->nx*h->nz;
        if(file_length!=data_length) {
            printf("%s: file length was unexpected!\n", __func__);
            abort();
        }
    }
    return 0;
}

#ifdef LIBVOL_PYTHON
/**
 * @brief Open a su file and return a handle!
 *   su.open(name, flag)
 * @param[in] flag VOL_CREATE/VOL_READONLY/VOL_UPDATE
 * */
static PyObject * pyvol_open(PyObject __attribute__((unused)) *self, PyObject *args)
{
    int flag;
    const char *name;
    if(!PyArg_ParseTuple(args, "si", &name, &flag))
      return NULL;

    VOLID_t id;
    PyObject *db;
    if(vol_open(&id, name, flag)!=VOL_SUCCESS) {
        printf("%s: *ERROR* su_open(%s, %d) Failed\n", __func__, name, flag);
        return NULL;
    }
    db = PyCapsule_New(id, NULL, NULL);
    return db;
}
#endif