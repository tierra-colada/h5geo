#ifndef H5BASEOBJECT_PY_H
#define H5BASEOBJECT_PY_H

#include "h5geo_py.h"

#include <h5geo/misc/h5baseobjectimpl.h>

using namespace h5geo;

namespace h5geopy {

void H5BaseObject_py(
    py::class_<
    H5BaseObject,
    std::unique_ptr<H5BaseObject, py::nodelete>,
    H5Base> &py_obj);

} // h5geopy


#endif // H5BASEOBJECT_PY_H
