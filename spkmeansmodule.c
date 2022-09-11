#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "utils.h"
#include "kmeans.h"
#include "spkmeans.h"

static PyObject* execute_program(PyObject *self, PyObject *args){
    int k;
    char *goal, *input_filename;
    SPK_INFO *spk_info;
    PyObject *res = PyList_New(2);

    if (!PyArg_ParseTuple(args, "iss", &k, &goal, &input_filename)){
        return NULL;
    }
    /* execute c program */
    spk_info = execute(k, goal, input_filename);

    /* Create the result structure */
    PyList_SetItem(res, 0, Py_BuildValue("i", spk_info->k));
    PyList_SetItem(res, 1, Py_BuildValue("s", spk_info->spk_mat_filename));
    free(spk_info);
    return res;

}

static PyObject* execute_kmeans(PyObject *self, PyObject *args){
    int k, n, d;
    char * datapoints_filename;
    char * centroids_filename;
    if (!PyArg_ParseTuple(args, "iiiss", &k, &n, &d, &datapoints_filename, &centroids_filename)){
        return NULL;
    }
    return Py_BuildValue("s", kmeans_process(k, n, d, datapoints_filename, centroids_filename));
}


static PyMethodDef capiMethods[] = {
    {"execute_program",
    (PyCFunction) execute_program,
    METH_VARARGS,
    PyDoc_STR("Normalized Spectral Clustering Algorithm")},
    {"execute_kmeans",
    (PyCFunction) execute_kmeans,
    METH_VARARGS,
    PyDoc_STR("Kmeans Algorithm")},
    {NULL, NULL, 0, NULL}
    
};

static struct PyModuleDef moduledef = {
    PyModuleDef_HEAD_INIT,
    "spkmeansmodule",
    NULL,
    -1,
    capiMethods
};

PyMODINIT_FUNC PyInit_spkmeansmodule(void) {
    PyObject *m;
    m = PyModule_Create(&moduledef);
    if (!m) {
        return NULL;
    }
    return m;
}