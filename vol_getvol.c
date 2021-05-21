static void safe_pread(int fd, void *buf, size_t count, off_t offset)
{
    ssize_t read_size;
    while(count>0) {
        read_size = pread(fd, buf, count, offset);
        if(read_size==(-1)) {
            printf("%s: expect to read %ld but get %ld\n", 
                    __func__, count, read_size);
            abort();
        }
        buf    += read_size;
        offset += read_size;
        count  -= read_size;
    }
}
ssize_t pread(int fd, void *buf, size_t count, off_t offset);

/**
 * Read the 3D volume and return ny*nx
 * */
int vol_getvol(VOLID_t id, void *data)
{
    int64_t data_size;
    protium_volid_t *vol = id;
    vol_head_t *h = &vol->header;
    data_size = ((int64_t)h->ny)*h->nx*h->nz*sizeof(float);
    safe_pread(vol->fid, data, data_size, sizeof(vol_head_t));
    return h->ny*h->nx;
}

int vol_pgetvol(VOLID_t id, void *data, int first, int n)
{
    protium_volid_t *vol = id;
    vol_head_t *h = &vol->header;
    assert(first+n<=h->ny*h->nx);
    int64_t data_size, file_seek;
    file_seek = sizeof(vol_head_t)+((int64_t)first)*h->nz*sizeof(float);
    data_size = ((int64_t)n)*h->nz*sizeof(float);
    safe_pread(vol->fid, data, data_size, file_seek);
    return n;
}

#ifdef LIBVOL_PYTHON
/**
 * @brief Read the whole volume and return as numpy array,
 * for example, nparry = vol.getvol(id)
 * @param[in] id opaque object to access the database
 * @return the numpy array contains the requested data
 * */
static PyObject * pyvol_getvol(PyObject __attribute__((unused)) *self, PyObject *args)
{
    PyObject *db, *data;
    protium_volid_t *vol;
    if(!PyArg_ParseTuple(args, "O", &db))
        return NULL;
    //vol = PyCapsule_GetPointer(db, NULL);
    vol = pyvol_obj2ptr(db);

    npy_intp dims[3];
    dims[0] = vol->header.ny;
    dims[1] = vol->header.nx;
    dims[2] = vol->header.nz;
    data = PyArray_SimpleNew(3, dims, NPY_FLOAT);
    void *pdes = PyArray_DATA((PyArrayObject*)data);
    vol_getvol(vol, pdes);
    return data;
}
#endif
