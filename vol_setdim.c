int vol_setdim(VOLID_t id, int ny, int nx, int nz)
{
    protium_volid_t *vol = id;
    vol->flag &= (~VOL_FLAG_WRITE);
    vol->header.ny = ny;
    vol->header.nx = nx;
    vol->header.nz = nz;
    return 0;
}

int vol_sety(VOLID_t id, float y0, float dy, int iy0)
{
    protium_volid_t *vol = id;
    vol->flag &= (~VOL_FLAG_WRITE);
    vol->header.y0  = y0;
    vol->header.dy  = dy;
    vol->header.iy0 = iy0;
    return 0;
}

#ifdef LIBVOL_PYTHON
/**
 * @brief Get dimension size of the volume
 *   vol.setdim(id, ny, nx, nz)
 * @param[in] id opaque object to access the volume
 * */
static PyObject * pyvol_setdim(PyObject __attribute__((unused)) *self, PyObject *args)
{
    PyObject *db;
    int ny, nx, nz;
    protium_volid_t *id;
    PyArg_ParseTuple(args, "Oiii", &db, &ny, &nx, &nz);
    //id = PyCapsule_GetPointer(db, NULL);
    id = pyvol_obj2ptr(db);
    
    vol_setdim(id, ny, nx, nz);
    Py_INCREF(Py_True);
    return Py_True;
}

/**
 * @brief Get dimension size of the volume
 *   vol.sety(id, y0, dy, fy)
 * @param[in] id opaque object to access the volume
 * */
static PyObject * pyvol_sety(PyObject __attribute__((unused)) *self, PyObject *args)
{
    PyObject *db;
    protium_volid_t *id;
    int fy; float y0, dy;
    PyArg_ParseTuple(args, "Offi", &db, &y0, &dy, &fy);
    //id = PyCapsule_GetPointer(db, NULL);
    id = pyvol_obj2ptr(db);
    
    vol_sety(id, y0, dy, fy);
    Py_INCREF(Py_True);
    return Py_True;
}

#endif
