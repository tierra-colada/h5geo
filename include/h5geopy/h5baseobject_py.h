#ifndef H5BASEOBJECT_PY_H
#define H5BASEOBJECT_PY_H

#include "h5geo_py.h"

#include <h5geo/misc/h5baseobjectimpl.h>

namespace h5geopy {

void H5BaseObject_py(
    py::class_<
    H5BaseObjectImpl,
    std::unique_ptr<H5BaseObjectImpl, py::nodelete>,
    H5BaseImpl> &py_obj);

} // h5geopy


#endif // H5BASEOBJECT_PY_H
