#ifndef H5LOGCURVE_PY_H
#define H5LOGCURVE_PY_H

#include "h5geo_py.h"

#include <h5geo/misc/h5logcurveimpl.h>

namespace h5geopy {

void H5LogCurve_py(
    py::class_<
    H5LogCurveImpl,
    std::unique_ptr<H5LogCurveImpl, py::nodelete>,
    H5BaseObjectImpl> &py_obj);

} // h5geopy


#endif // H5LOGCURVE_PY_H
