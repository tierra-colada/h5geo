#ifndef H5MAPCONTAINER_PY_H
#define H5MAPCONTAINER_PY_H

#include "h5geo_py.h"

#include <h5geo/misc/h5mapcontainerimpl.h>
#include <h5geo/misc/h5mapimpl.h>

namespace h5geopy {

void H5MapContainer_py(
    py::class_<
    H5MapContainerImpl,
    std::unique_ptr<H5MapContainerImpl, ObjectDeleter>,
    H5BaseContainerImpl,
    H5MapContainer> &py_obj);

} // h5geopy


#endif // H5MAPCONTAINER_PY_H
