#ifndef H5MAP_PY_H
#define H5MAP_PY_H

#include "h5geo_py.h"

#include <h5geo/misc/h5mapcontainerimpl.h>
#include <h5geo/misc/h5mapimpl.h>

namespace h5geopy {

void H5Map_py(
    py::class_<
    H5MapImpl,
    std::unique_ptr<H5MapImpl, ObjectDeleter>,
    H5BaseObjectImpl,
    H5Map> &py_obj);

} // h5geopy


#endif // H5MAP_PY_H
