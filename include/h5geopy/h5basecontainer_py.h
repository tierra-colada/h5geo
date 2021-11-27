#ifndef H5BASECONTAINER_PY_H
#define H5BASECONTAINER_PY_H

#include "h5geo_py.h"

#include <h5geo/misc/h5basecontainerimpl.h>

namespace h5geopy {

void H5BaseContainer_py(
    py::class_<
    H5BaseContainer,
    H5BaseContainerImpl,
    H5Base,
    std::unique_ptr<H5BaseContainer, ObjectDeleter>>
    &py_obj);

} // h5geopy


#endif // H5BASECONTAINER_PY_H
