#ifndef H5MAP_PY_H
#define H5MAP_PY_H

#include "h5geo_py.h"

#include <h5geo/impl/h5mapcontainerimpl.h>
#include <h5geo/impl/h5mapimpl.h>

namespace h5geopy {

void H5Map_py(
    py::class_<
    H5Map,
    H5MapImpl,
    H5BaseObject,
    H5Base,
    std::unique_ptr<H5Map, ObjectDeleter>>
    &py_obj);

} // h5geopy


#endif // H5MAP_PY_H
