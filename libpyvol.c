#define _GNU_SOURCE
#define Py_USING_UNICODE
#include <Python.h>
#define  NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/arrayobject.h>

#define LIBVOL_PYTHON

#include "vol_i.h"
#include "vol_open.c"
#include "vol_close.c"

static PyMethodDef pyvolMethods[] = {
    {"open",      pyvol_open,     METH_VARARGS, "Open a Seismic Unix dataset."},
    {"close",     pyvol_close,    METH_VARARGS, "Close SU dataset."},
    {NULL, NULL, 0, NULL} /* Sentinel */
};

static struct PyModuleDef libpyvol = {
    PyModuleDef_HEAD_INIT,
    "libpyvol",   /* name of module */
    "libpyvol_doc", /* module documentation, may be NULL */
    -1,       /* size of per-interpreter state of the module,
                 or -1 if the module keeps state in global variables. */
    pyvolMethods
};

static PyObject *PydbError;

PyMODINIT_FUNC PyInit_libpyvol(void)
{
    Py_Initialize();
    PyObject *m = PyModule_Create(&libpyvol);
    if (m == NULL)
      return NULL;
    import_array();
    PydbError = PyErr_NewException("db.error", NULL, NULL);
    PyModule_AddObject(m, "error", PydbError);

    PyModule_AddIntConstant(m, "CREATE",   VOL_CREATE);
    PyModule_AddIntConstant(m, "READONLY", VOL_READONLY);
    PyModule_AddIntConstant(m, "UPDATE",   VOL_UPDATE);
    return m;
}
