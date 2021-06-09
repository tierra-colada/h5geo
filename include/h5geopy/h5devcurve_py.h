#ifndef H5DEVCURVE_PY_H
#define H5DEVCURVE_PY_H

#include "h5geo_py.h"

#include <h5geo/misc/h5devcurveimpl.h>

namespace h5geopy {

void H5DevCurve_py(
    py::class_<
    H5DevCurveImpl,
    std::unique_ptr<H5DevCurveImpl, py::nodelete>,
    H5BaseObjectImpl> &py_obj);

} // h5geopy


#endif // H5DEVCURVE_PY_H
