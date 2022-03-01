#ifndef H5WELLCONTAINER_PY_H
#define H5WELLCONTAINER_PY_H

#include "h5geo_py.h"

#include <h5geo/impl/h5wellcontainerimpl.h>
#include <h5geo/impl/h5wellimpl.h>

namespace h5geopy {

void H5WellContainer_py(
    py::class_<
    H5WellContainer,
    H5WellContainerImpl,
    H5BaseContainer,
    H5Base,
    std::unique_ptr<H5WellContainer, ObjectDeleter>>
    &py_obj);

} // h5geopy


#endif // H5WELLCONTAINER_PY_H
