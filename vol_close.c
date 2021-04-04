int vol_close(VOLID_t id)
{
    protium_volid_t *vol = id;
    close(vol->fid);
    free(vol);
    return 0;
}

#ifdef LIBVOL_PYTHON
/**
 * @brief close the vol handle in python, for example
 *  vol.close(id)
 * @param[in] id opaque object to access the data flow
 * @return True or False
 * */
static PyObject * pyvol_close(PyObject __attribute__((unused)) *self, PyObject *args)
{
    PyObject *db;
    protium_volid_t *id;
    if(!PyArg_ParseTuple(args, "O", &db))
      return NULL;

    id = PyCapsule_GetPointer(db, NULL);
    vol_close(id);
    Py_INCREF(Py_True);
    return Py_True;
}
#endif
