#ifndef H5SEISCONTAINER_PY_H
#define H5SEISCONTAINER_PY_H

#include "h5geo_py.h"

#include <h5geo/misc/h5seiscontainerimpl.h>
#include <h5geo/misc/h5seisimpl.h>

namespace h5geopy {

void H5SeisContainer_py(
    py::class_<
    H5SeisContainerImpl,
    std::unique_ptr<H5SeisContainerImpl, py::nodelete>,
    H5BaseContainerImpl> &py_obj);

} // h5geopy


#endif // H5SEISCONTAINER_PY_H
