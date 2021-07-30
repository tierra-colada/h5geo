#ifndef H5WELL_PY_H
#define H5WELL_PY_H

#include "h5geo_py.h"

#include <h5geo/misc/h5wellcontainerimpl.h>
#include <h5geo/misc/h5wellimpl.h>

namespace h5geopy {

void H5Well_py(
    py::class_<
    H5WellImpl,
    std::unique_ptr<H5WellImpl, ObjectDeleter>,
    H5BaseObjectImpl,
    H5Well> &py_obj);

} // h5geopy


#endif // H5WELL_PY_H
