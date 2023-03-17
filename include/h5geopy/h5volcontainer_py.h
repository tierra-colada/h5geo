#ifndef H5VOLCONTAINER_PY_H
#define H5VOLCONTAINER_PY_H

#include "h5geo_py.h"

#include <h5geo/private/h5volcontainerimpl.h>
#include <h5geo/private/h5volimpl.h>

namespace h5geopy {

void H5VolContainer_py(
    py::class_<
    H5VolContainer,
    H5VolContainerImpl,
    H5BaseContainer,
    H5Base,
    std::unique_ptr<H5VolContainer, ObjectDeleter>>
    &py_obj);

} // h5geopy


#endif // H5VOLCONTAINER_PY_H
