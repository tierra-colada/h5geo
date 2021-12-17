#ifndef H5POINTS_PY_H
#define H5POINTS_PY_H

#include "h5geo_py.h"

#include <h5geo/misc/h5basecontainerimpl.h>
#include <h5geo/misc/h5pointsimpl.h>

namespace h5geopy {

void H5Points_py(
    py::class_<
    H5Points,
    H5PointsImpl,
    H5BaseObject,
    std::unique_ptr<H5Points, ObjectDeleter>>
    &py_obj);

} // h5geopy

#endif // H5POINTS_PY_H
