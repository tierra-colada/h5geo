#ifndef H5WELLCONTAINER_PY_H
#define H5WELLCONTAINER_PY_H

#include "h5geo_py.h"

#include <h5geo/misc/h5wellcontainerimpl.h>
#include <h5geo/misc/h5wellimpl.h>

namespace h5geopy {

void H5WellContainer_py(
    py::class_<
    H5WellContainerImpl,
    std::unique_ptr<H5WellContainerImpl, py::nodelete>,
    H5BaseContainerImpl> &py_obj);

} // h5geopy


#endif // H5WELLCONTAINER_PY_H
