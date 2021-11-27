#ifndef H5SEISCONTAINER_PY_H
#define H5SEISCONTAINER_PY_H

#include "h5geo_py.h"

#include <h5geo/misc/h5seiscontainerimpl.h>
#include <h5geo/misc/h5seisimpl.h>

namespace h5geopy {

void H5SeisContainer_py(
    py::class_<
    H5SeisContainer,
    H5SeisContainerImpl,
    H5BaseContainer,
    H5Base,
    std::unique_ptr<H5SeisContainer, ObjectDeleter>>
    &py_obj);

} // h5geopy


#endif // H5SEISCONTAINER_PY_H
