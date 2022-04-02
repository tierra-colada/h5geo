#ifndef H5POINT_PY_H
#define H5POINT_PY_H

#include "h5geo_py.h"

#include <h5geo/impl/h5point.h>

namespace h5geopy {

void Point1_py(
    py::class_<Point1>
    &py_obj);

void Point2_py(
    py::class_<Point2>
    &py_obj);

void Point3_py(
    py::class_<Point3>
    &py_obj);

void Point4_py(
    py::class_<Point4>
    &py_obj);

} // h5geopy

#endif // H5POINT_PY_H
