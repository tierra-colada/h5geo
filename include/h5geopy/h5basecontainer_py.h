#ifndef H5BASECONTAINER_PY_H
#define H5BASECONTAINER_PY_H

#include "h5geo_py.h"

#include <h5geo/misc/h5basecontainerimpl.h>

namespace h5geopy {

void H5BaseContainer_py(
    py::class_<
    H5BaseContainerImpl,
    std::unique_ptr<H5BaseContainerImpl, ObjectDeleter>,
    H5BaseImpl,
    H5BaseContainer> &py_obj);

} // h5geopy


#endif // H5BASECONTAINER_PY_H
