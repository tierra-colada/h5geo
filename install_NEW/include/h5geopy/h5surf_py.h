#ifndef H5SURF_PY_H
#define H5SURF_PY_H

#include "h5geo_py.h"

#include <h5geo/misc/h5surfcontainerimpl.h>
#include <h5geo/misc/h5surfimpl.h>

namespace h5geopy {

void H5Surf_py(
    py::class_<
    H5SurfImpl,
    std::unique_ptr<H5SurfImpl, py::nodelete>,
    H5BaseObjectImpl,
    H5Surf> &py_obj);

} // h5geopy


#endif // H5SURF_PY_H
