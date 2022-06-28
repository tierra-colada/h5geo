#ifndef H5Horizon_PY_H
#define H5Horizon_PY_H

#include "h5geo_py.h"

#include <h5geo/private/h5horizonimpl.h>
#include <h5geo/private/h5basecontainerimpl.h>

namespace h5geopy {

void H5Horizon_py(
    py::class_<
    H5Horizon,
    H5HorizonImpl,
    H5BaseObject,
    H5Base,
    std::unique_ptr<H5Horizon, ObjectDeleter>>
    &py_obj);

} // h5geopy


#endif // H5Horizon_PY_H
