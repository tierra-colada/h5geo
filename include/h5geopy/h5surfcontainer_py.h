#ifndef H5SURFCONTAINER_PY_H
#define H5SURFCONTAINER_PY_H

#include "h5geo_py.h"

#include <h5geo/misc/h5surfcontainerimpl.h>
#include <h5geo/misc/h5surfimpl.h>

namespace h5geopy {

void H5SurfContainer_py(
    py::class_<
    H5SurfContainerImpl,
    std::unique_ptr<H5SurfContainerImpl, py::nodelete>,
    H5BaseContainerImpl> &py_obj);

} // h5geopy


#endif // H5SURFCONTAINER_PY_H
