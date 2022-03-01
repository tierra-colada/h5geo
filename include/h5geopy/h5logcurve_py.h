#ifndef H5LOGCURVE_PY_H
#define H5LOGCURVE_PY_H

#include "h5geo_py.h"

#include <h5geo/impl/h5logcurveimpl.h>

namespace h5geopy {

void H5LogCurve_py(
    py::class_<
    H5LogCurve,
    H5LogCurveImpl,
    H5BaseObject,
    H5Base,
    std::unique_ptr<H5LogCurve, ObjectDeleter>>
    &py_obj);

} // h5geopy


#endif // H5LOGCURVE_PY_H
