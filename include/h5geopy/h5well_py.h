#ifndef H5WELL_PY_H
#define H5WELL_PY_H

#include "h5geo_py.h"

#include <h5geo/private/h5wellcontainerimpl.h>
#include <h5geo/private/h5wellimpl.h>

namespace h5geopy {

void H5Well_py(
    py::class_<
    H5Well,
    H5WellImpl,
    H5BaseObject,
    H5Base,
    std::unique_ptr<H5Well, ObjectDeleter>>
    &py_obj);

} // h5geopy


#endif // H5WELL_PY_H
