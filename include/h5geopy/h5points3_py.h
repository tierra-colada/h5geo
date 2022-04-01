#ifndef H5POINTS3_PY_H
#define H5POINTS3_PY_H

#include "h5geo_py.h"

#include <h5geo/impl/h5basecontainerimpl.h>
#include <h5geo/impl/h5points3impl.h>

namespace h5geopy {

void H5Points3_py(
    py::class_<
    H5Points3,
    H5Points3Impl,
    H5BasePoints,
    H5BaseObject,
    H5Base,
    std::unique_ptr<H5Points3, ObjectDeleter>>
    &py_obj);

} // h5geopy

#endif // H5POINTS3_PY_H
