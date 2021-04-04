int vol_getdim(VOLID_t id, int *ny, int *nx, int *nz)
{
    protium_volid_t *vol = id;
    *ny = vol->header.ny;
    *nx = vol->header.nx;
    *nz = vol->header.nz;
    return 0;
}

int vol_gety(VOLID_t id, float *y0, float *dy, int *fy)
{
    protium_volid_t *vol = id;
    *y0 = vol->header.y0;
    *dy = vol->header.dy;
    *fy = vol->header.yline0;
    return 0;
}

#ifdef LIBVOL_PYTHON
/**
 * @brief Get dimension size of the volume
 *   (ny,nx,nz) = vol.getdim(id)
 * @param[in] id opaque object to access the volume
 * */
static PyObject * pyvol_getdim(PyObject __attribute__((unused)) *self, PyObject *args)
{
    PyObject *db;
    protium_suid_t *id;
    PyArg_ParseTuple(args, "O", &db);
    id = PyCapsule_GetPointer(db, NULL);
    
    int ny, nx, nz;
    vol_getdim(id, &ny, &nx, &nz);
    PyObject *ony, *onx, *onz;
    ony = PyLong_FromLong(ny);
    onx = PyLong_FromLong(nx);
    onz = PyLong_FromLong(nz);
    return PyTuple_Pack(3, ony, onx, onz);
}

/**
 * @brief Get dimension size of the volume
 *   (y0,dy,fy) = vol.gety(id)
 * @param[in] id opaque object to access the volume
 * */
static PyObject * pyvol_gety(PyObject __attribute__((unused)) *self, PyObject *args)
{
    PyObject *db;
    protium_suid_t *id;
    PyArg_ParseTuple(args, "O", &db);
    id = PyCapsule_GetPointer(db, NULL);
    
    int fy; float y0, dy;
    vol_gety(id, &y0, &dy, &fy);
    PyObject *oy0, *ody, *ofy;
    oy0 = PyFloat_FromDouble(y0);
    ody = PyFloat_FromDouble(dy);
    ofy = PyLong_FromLong(fy);
    return PyTuple_Pack(3, oy0, ody, ofy);
}

#endif