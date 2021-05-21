/**
 * Read the 3D volume and return ny*nx
 * */
int vol_getvol(VOLID_t id, void *data)
{
    protium_volid_t *vol = id;
    vol_head_t *h = &vol->header;
    int64_t data_size, data_read, file_seek;
    file_seek = sizeof(vol_head_t);
    data_size = ((int64_t)h->ny)*h->nx*h->nz*sizeof(float);
    while(data_size!=0) {
        data_read = pread(vol->fid, data, data_size, file_seek);
        if(data_read==(-1)) {
            printf("%s: expect to read %ld but get %ld\n", __func__,
                data_size, data_read);
            abort();
        }
        data_size -= data_read;
        file_seek += data_read;
        data = (void*)data+data_read;
    }
    return h->ny*h->nx;
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
