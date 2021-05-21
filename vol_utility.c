static void vol_write_header(VOLID_t id)
{
    protium_volid_t *vol = id;
    vol->flag &= (~VOL_FLAG_WRITE);
    if(pwrite(vol->fid, &vol->header, sizeof(vol_head_t), 0)
        != sizeof(vol_head_t)) {
        printf("%s: write header failed!\n", __func__);
        abort();
    }
}
/*
static void
safe_pread(int fd, void *buf, size_t count, off_t offset)
{
} */

#ifdef LIBVOL_PYTHON

#if PY_MAJOR_VERSION >=3 
static PyObject *pyvol_ptr2obj(void *ptr)
{ return PyCapsule_New(ptr, NULL, NULL); }
static void *pyvol_obj2ptr(PyObject *obj)
{ return PyCapsule_GetPointer(obj, NULL); }
#else	//python2.6 in RHEL6
static PyObject *pyvol_ptr2obj(void *ptr)
{ return PyCObject_FromVoidPtr(ptr, NULL); }
static void *pyvol_obj2ptr(PyObject *obj)
{ return PyCObject_AsVoidPtr(obj); }
#endif
#endif
