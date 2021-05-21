static void safe_pwrite(int fd, const void *buf, size_t count, off_t offset)
{
    ssize_t io_size;
    while(count>0) {
        io_size = pwrite(fd, buf, count, offset);
        if(io_size==(-1)) {
            printf("%s: expect to write %ld but get %ld\n", 
                    __func__, count, io_size);
            abort();
        }
        buf    += io_size;
        offset += io_size;
        count  -= io_size;
    }
}

/**
 * Read the 3D volume and return ny*nx
 * */
int vol_setvol(VOLID_t id, const void *data)
{
    protium_volid_t *vol = id;
    if(!(vol->flag & VOL_FLAG_WRITE))
        vol_write_header(id);

    int64_t data_size;
    vol_head_t *h = &vol->header;
    data_size = ((int64_t)h->ny)*h->nx*h->nz*sizeof(float);
    safe_pwrite(vol->fid, data, data_size, sizeof(vol_head_t));
    return h->ny*h->nx;
}

int vol_psetvol(VOLID_t id, const void *data, int first, int n)
{
    protium_volid_t *vol = id;
    if(!(vol->flag & VOL_FLAG_WRITE))
        vol_write_header(id);

    int64_t data_size, file_seek;
    vol_head_t *h = &vol->header;
    assert(first+n<=h->ny*h->nx);
    file_seek = sizeof(vol_head_t)+((int64_t)first)*h->nz*sizeof(float);
    data_size = ((int64_t)n)*h->nz*sizeof(float);
    safe_pwrite(vol->fid, data, data_size, file_seek);
    return n;
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

static PyObject * pyvol_psetvol(PyObject __attribute__((unused)) *self, PyObject *args)
{
    PyObject *db, *data;
    protium_volid_t *vol;
    long long first, num;
    if(!PyArg_ParseTuple(args, "OOLL", &db, &data, &first, &num))
        return NULL;
    //vol = PyCapsule_GetPointer(db, NULL);
    vol = pyvol_obj2ptr(db);
    
    int nd;
    npy_intp *dims;
    nd = PyArray_NDIM((PyArrayObject*)data);
    dims = PyArray_DIMS((PyArrayObject*)data);
    vol_head_t *h = &vol->header;
    if(((nd==3) && (dims[0]*dims[1]!=num || dims[2]!=h->nz))
            || ((nd==2) && (dims[0]!=num || dims[1]!=h->nz))) {
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
    vol_psetvol(vol, psrc, first, num);
    Py_INCREF(Py_True);
    return Py_True;
}

#endif
