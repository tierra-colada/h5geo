#ifndef H5DEVCURVE_PY_H
#define H5DEVCURVE_PY_H

#include "h5geo_py.h"

#include <h5geo/misc/h5devcurveimpl.h>

namespace h5geopy {

void H5DevCurve_py(
    py::class_<
    H5DevCurve,
    H5DevCurveImpl,
    H5BaseObject,
    H5Base,
    std::unique_ptr<H5DevCurve, ObjectDeleter>>
    &py_obj);

} // h5geopy


#endif // H5DEVCURVE_PY_H
