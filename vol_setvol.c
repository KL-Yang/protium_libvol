/**
 * Read the 3D volume and return ny*nx
 * */
int vol_setvol(VOLID_t id, const void *data)
{
    protium_volid_t *vol = id;
    if(!(vol->flag & VOL_FLAG_WRITE))
        vol_write_header(id);

    vol_head_t *h = &vol->header;
    int64_t data_size, data_read, file_seek;
    file_seek = sizeof(vol_head_t);
    data_size = ((int64_t)h->ny)*h->nx*h->nz*sizeof(float);
    while(data_size!=0) {
        data_read = pwrite(vol->fid, data, data_size, file_seek);
        if(data_read==(-1)) {
            printf("%s: expect to write %ld but write %ld\n", __func__,
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
 * for example, vol.setvol(id, arry)
 * @param[in] id opaque object to access the database
 * @param[in] array 3D np array volume to be written
 * @return the numpy array contains the requested data
 * */
static PyObject * pyvol_setvol(PyObject __attribute__((unused)) *self, PyObject *args)
{
    PyObject *db, *data;
    protium_volid_t *vol;
    if(!PyArg_ParseTuple(args, "OO", &db, &data))
        return NULL;
    //vol = PyCapsule_GetPointer(db, NULL);
    vol = pyvol_obj2ptr(db);
    
    int nd;
    npy_intp *dims;
    nd = PyArray_NDIM((PyArrayObject*)data);
    dims = PyArray_DIMS((PyArrayObject*)data);
    vol_head_t *h = &vol->header;
    if(((nd==3) && (dims[0]!=h->ny || dims[1]!=h->nx || dims[2]!=h->nz))
            || ((nd==2) && (dims[0]!=h->ny*h->nx || dims[1]!=h->nz))) {
        printf("%s: array dimension is not correct!\n", __func__);
        abort();
    }
    if(NPY_FLOAT!=PyArray_TYPE((PyArrayObject*)data)) {
        printf("%s: array type is not correct!\n", __func__);
        int type=PyArray_TYPE((PyArrayObject*)data);
        printf("type=%d expect=%d\n", type, NPY_FLOAT);
        abort();
    }
    void *psrc = PyArray_DATA((PyArrayObject*)data);
    vol_setvol(vol, psrc);
    Py_INCREF(Py_True);
    return Py_True;
}
#endif
