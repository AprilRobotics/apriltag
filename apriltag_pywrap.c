#define NPY_NO_DEPRECATED_API NPY_API_VERSION

#include <stdbool.h>
#include <Python.h>
#include <structmember.h>
#include <numpy/arrayobject.h>
#include <signal.h>

#include "apriltag.h"
#include "tag36h10.h"
#include "tag36h11.h"
#include "tag25h9.h"
#include "tag16h5.h"
#include "tagCircle21h7.h"
#include "tagCircle49h12.h"
#include "tagCustom48h12.h"
#include "tagStandard41h12.h"
#include "tagStandard52h13.h"


#define SUPPORTED_TAG_FAMILIES(_)           \
    _(tag36h10)                             \
    _(tag36h11)                             \
    _(tag25h9)                              \
    _(tag16h5)                              \
    _(tagCircle21h7)                        \
    _(tagCircle49h12)                       \
    _(tagStandard41h12)                     \
    _(tagStandard52h13)                     \
    _(tagCustom48h12)

#define TAG_CREATE_FAMILY(name) \
    else if (0 == strcmp(family, #name)) self->tf = name ## _create();
#define TAG_SET_DESTROY_FUNC(name) \
    else if (0 == strcmp(family, #name)) self->destroy_func = name ## _destroy;
#define FAMILY_STRING(name) "  " #name "\n"


// Python is silly. There's some nuance about signal handling where it sets a
// SIGINT (ctrl-c) handler to just set a flag, and the python layer then reads
// this flag and does the thing. Here I'm running C code, so SIGINT would set a
// flag, but not quit, so I can't interrupt the solver. Thus I reset the SIGINT
// handler to the default, and put it back to the python-specific version when
// I'm done
#define SET_SIGINT() struct sigaction sigaction_old;                    \
do {                                                                    \
    if( 0 != sigaction(SIGINT,                                          \
                       &(struct sigaction){ .sa_handler = SIG_DFL },    \
                       &sigaction_old) )                                \
    {                                                                   \
        PyErr_SetString(PyExc_RuntimeError, "sigaction() failed");      \
        goto done;                                                      \
    }                                                                   \
} while(0)
#define RESET_SIGINT() do {                                             \
    if( 0 != sigaction(SIGINT,                                          \
                       &sigaction_old, NULL ))                          \
        PyErr_SetString(PyExc_RuntimeError, "sigaction-restore failed"); \
} while(0)

#define PYMETHODDEF_ENTRY(function_prefix, name, args) {#name,          \
                                                        (PyCFunction)function_prefix ## name, \
                                                        args,           \
                                                        function_prefix ## name ## _docstring}

typedef struct {
    PyObject_HEAD

    apriltag_family_t*   tf;
    apriltag_detector_t* td;
    void (*destroy_func)(apriltag_family_t *tf);
} apriltag_py_t;


static PyObject *
apriltag_new(PyTypeObject *type, PyObject *args, PyObject *kwargs)
{
    bool success = false;

    apriltag_py_t* self = (apriltag_py_t*)type->tp_alloc(type, 0);
    if(self == NULL) goto done;

    self->tf = NULL;
    self->td = NULL;

    const char* family          = NULL;
    int         Nthreads        = 1;
    int         maxhamming      = 1;
    float       decimate        = 1.0;
    float       blur            = 0.0;
    bool        refine_edges    = true;
    bool        debug           = false;
    PyObject*   py_refine_edges = NULL;
    PyObject*   py_debug        = NULL;

    char* keywords[] = {"family",
                        "threads",
                        "maxhamming",
                        "decimate",
                        "blur",
                        "refine-edges",
                        "debug",
                        NULL };

    if(!PyArg_ParseTupleAndKeywords( args, kwargs, "s|iiffOO",
                                     keywords,
                                     &family,
                                     &Nthreads,
                                     &maxhamming,
                                     &decimate,
                                     &blur,
                                     &py_refine_edges,
                                     &py_debug ))
    {
        goto done;
    }

    if(py_refine_edges != NULL)
        refine_edges = PyObject_IsTrue(py_refine_edges);
    if(py_debug        != NULL)
        debug        = PyObject_IsTrue(py_debug);


    if(0) ; SUPPORTED_TAG_FAMILIES(TAG_SET_DESTROY_FUNC)
    else
    {
        PyErr_Format(PyExc_RuntimeError, "Unrecognized tag family name: '%s'. Families I know about:\n%s",
                     family, SUPPORTED_TAG_FAMILIES(FAMILY_STRING));
        goto done;
    }

    if(0) ; SUPPORTED_TAG_FAMILIES(TAG_CREATE_FAMILY);

    self->td = apriltag_detector_create();
    if(self->td == NULL)
    {
        PyErr_SetString(PyExc_RuntimeError, "apriltag_detector_create() failed!");
        goto done;
    }

    apriltag_detector_add_family_bits(self->td, self->tf, maxhamming);
    self->td->quad_decimate       = decimate;
    self->td->quad_sigma          = blur;
    self->td->nthreads            = Nthreads;
    self->td->refine_edges        = refine_edges;
    self->td->debug               = debug;

    success = true;

 done:
    if(!success)
    {
        if(self != NULL)
        {
            if(self->td != NULL)
            {
                apriltag_detector_destroy(self->td);
                self->td = NULL;
            }
            if(self->tf != NULL)
            {
                self->destroy_func(self->tf);
                self->tf = NULL;
            }
            Py_DECREF(self);
        }
        return NULL;
    }

    return (PyObject*)self;
}

static void apriltag_dealloc(apriltag_py_t* self)
{
    if(self == NULL)
        return;
    if(self->td != NULL)
    {
        apriltag_detector_destroy(self->td);
        self->td = NULL;
    }
    if(self->tf != NULL)
    {
        self->destroy_func(self->tf);
        self->tf = NULL;
    }

    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* apriltag_detect(apriltag_py_t* self,
                                 PyObject* args)
{
    PyObject*      result           = NULL;
    PyArrayObject* xy_c             = NULL;
    PyArrayObject* xy_lb_rb_rt_lt   = NULL;
    PyArrayObject* image            = NULL;
    PyObject*      detections_tuple = NULL;

    SET_SIGINT();
    if(!PyArg_ParseTuple( args, "O&",
                          PyArray_Converter, &image ))
        goto done;

    npy_intp* dims    = PyArray_DIMS   (image);
    npy_intp* strides = PyArray_STRIDES(image);
    int       ndims   = PyArray_NDIM   (image);
    if( ndims != 2 )
    {
        PyErr_Format(PyExc_RuntimeError, "The input image array must have exactly 2 dims; got %d",
                     ndims);
        goto done;
    }
    if( PyArray_TYPE(image) != NPY_UINT8 )
    {
        PyErr_SetString(PyExc_RuntimeError, "The input image array must contain 8-bit unsigned data");
        goto done;
    }
    if( strides[ndims-1] != 1 )
    {
        PyErr_SetString(PyExc_RuntimeError, "Image rows must live in contiguous memory");
        goto done;
    }


    image_u8_t im = {.width  = dims[1],
                     .height = dims[0],
                     .stride = strides[0],
                     .buf    = PyArray_DATA(image)};

    zarray_t* detections = apriltag_detector_detect(self->td, &im);
    int N = zarray_size(detections);

    detections_tuple = PyTuple_New(N);
    if(detections_tuple == NULL)
    {
        PyErr_Format(PyExc_RuntimeError, "Error creating output tuple of size %d", N);
        goto done;
    }
    
    for (int i=0; i < N; i++)
    {
        xy_c = (PyArrayObject*)PyArray_SimpleNew(1, ((npy_intp[]){2}), NPY_FLOAT64);
        if(xy_c == NULL)
        {
            PyErr_SetString(PyExc_RuntimeError, "Could not allocate xy_c array");
            goto done;
        }
        xy_lb_rb_rt_lt = (PyArrayObject*)PyArray_SimpleNew(2, ((npy_intp[]){4,2}), NPY_FLOAT64);
        if(xy_lb_rb_rt_lt == NULL)
        {
            PyErr_SetString(PyExc_RuntimeError, "Could not allocate xy_lb_rb_rt_lt array");
            goto done;
        }

        apriltag_detection_t* det;
        zarray_get(detections, i, &det);

        *(double*)PyArray_GETPTR1(xy_c, 0) = det->c[0];
        *(double*)PyArray_GETPTR1(xy_c, 1) = det->c[1];

        for(int j=0; j<4; j++)
        {
            *(double*)PyArray_GETPTR2(xy_lb_rb_rt_lt, j, 0) = det->p[j][0];
            *(double*)PyArray_GETPTR2(xy_lb_rb_rt_lt, j, 1) = det->p[j][1];
        }

        PyTuple_SET_ITEM(detections_tuple, i,
                         Py_BuildValue("{s:i,s:f,s:i,s:N,s:N}",
                                       "hamming", det->hamming,
                                       "margin",  det->decision_margin,
                                       "id",      det->id,
                                       "center",  xy_c,
                                       "lb-rb-rt-lt", xy_lb_rb_rt_lt));
        xy_c           = NULL;
        xy_lb_rb_rt_lt = NULL;
    }
    apriltag_detections_destroy(detections);

    result = detections_tuple;
    detections_tuple = NULL;

  done:
    Py_XDECREF(xy_c);
    Py_XDECREF(xy_lb_rb_rt_lt);
    Py_XDECREF(image);
    Py_XDECREF(detections_tuple);

    RESET_SIGINT();
    return result;
}


static const char apriltag_detect_docstring[] =
#include "apriltag_detect.docstring.h"
    ;
static const char apriltag_type_docstring[] =
#include "apriltag_py_type.docstring.h"
    ;

static PyMethodDef apriltag_methods[] =
    { PYMETHODDEF_ENTRY(apriltag_, detect, METH_VARARGS),
      {}
    };

static PyTypeObject apriltagType =
{
     PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name      = "apriltag",
    .tp_basicsize = sizeof(apriltag_py_t),
    .tp_new       = apriltag_new,
    .tp_dealloc   = (destructor)apriltag_dealloc,
    .tp_methods   = apriltag_methods,
    .tp_flags     = Py_TPFLAGS_DEFAULT,
    .tp_doc       = apriltag_type_docstring
};

static PyMethodDef methods[] =
    { {}
    };


#if PY_MAJOR_VERSION == 2

PyMODINIT_FUNC initapriltag(void)
{
    if (PyType_Ready(&apriltagType) < 0)
        return;

    PyObject* module = Py_InitModule3("apriltag", methods,
                                      "AprilTags visual fiducial system detector");

    Py_INCREF(&apriltagType);
    PyModule_AddObject(module, "apriltag", (PyObject *)&apriltagType);

    import_array();
}

#else

static struct PyModuleDef module_def =
    {
     PyModuleDef_HEAD_INIT,
     "apriltag",
     "AprilTags visual fiducial system detector",
     -1,
     methods
    };

PyMODINIT_FUNC PyInit_apriltag(void)
{
    if (PyType_Ready(&apriltagType) < 0)
        return NULL;

    PyObject* module =
        PyModule_Create(&module_def);

    Py_INCREF(&apriltagType);
    PyModule_AddObject(module, "apriltag", (PyObject *)&apriltagType);

    import_array();

    return module;
}

#endif

