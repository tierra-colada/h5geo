#ifndef H5SURFCONTAINER_PY_H
#define H5SURFCONTAINER_PY_H

#include "h5geo_py.h"

#include <h5geo/h5surfcontainer.h>
#include <h5geo/h5surf.h>

using namespace h5geo;

namespace h5geopy {

void H5SurfContainer_py(
    py::class_<
    H5SurfContainer,
    std::unique_ptr<H5SurfContainer, py::nodelete>,
    H5BaseContainer> &py_obj);

} // h5geopy


#endif // H5SURFCONTAINER_PY_H
