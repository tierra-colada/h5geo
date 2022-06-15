#ifndef H5WELLTOPS_PY_H
#define H5WELLTOPS_PY_H

#include "h5geo_py.h"

#include <h5geo/private/h5welltopsimpl.h>

namespace h5geopy {

void H5WellTops_py(
    py::class_<
    H5WellTops,
    H5WellTopsImpl,
    H5Points1,
    H5BasePoints,
    H5BaseObject,
    H5Base,
    std::unique_ptr<H5WellTops, ObjectDeleter>>
    &py_obj);

} // h5geopy

#endif // H5WELLTOPS_PY_H
