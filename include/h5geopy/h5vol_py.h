#ifndef H5VOL_PY_H
#define H5VOL_PY_H

#include "h5geo_py.h"

#include <h5geo/private/h5volcontainerimpl.h>
#include <h5geo/private/h5volimpl.h>

namespace h5geopy {

void H5Vol_py(
    py::class_<
    H5Vol,
    H5VolImpl,
    H5BaseObject,
    H5Base,
    std::unique_ptr<H5Vol, ObjectDeleter>>
    &py_obj);

} // h5geopy


#endif // H5VOL_PY_H
