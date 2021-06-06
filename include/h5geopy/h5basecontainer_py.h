#ifndef H5BASECONTAINER_PY_H
#define H5BASECONTAINER_PY_H

#include "h5geo_py.h"

#include <h5geo/h5basecontainer.h>

using namespace h5geo;

namespace h5geopy {

void H5BaseContainer_py(
    py::class_<H5BaseContainer,
    std::unique_ptr<H5BaseContainer, py::nodelete>,
    H5Base> &py_obj);

} // h5geopy


#endif // H5BASECONTAINER_PY_H
