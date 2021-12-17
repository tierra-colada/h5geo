#ifndef H5CORE_TYPES_PY_H
#define H5CORE_TYPES_PY_H

#include "h5geo_py.h"

#include <h5geo/misc/h5core_types.h>

namespace h5geopy {

void Point_py(
    py::class_<Point>
    &py_obj);

} // h5geopy

#endif // H5CORE_TYPES_PY_H
