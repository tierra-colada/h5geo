#ifndef H5SEIS_PY_H
#define H5SEIS_PY_H

#include "h5geo_py.h"

#include <h5geo/misc/h5seiscontainerimpl.h>
#include <h5geo/misc/h5seisimpl.h>

namespace h5geopy {

void H5Seis_py(
    py::class_<
    H5Seis,
    H5SeisImpl,
    H5BaseObject,
    std::unique_ptr<H5Seis, ObjectDeleter>>
    &py_obj);

} // h5geopy


#endif // H5SEIS_PY_H
