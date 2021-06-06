#ifndef H5SURF_PY_H
#define H5SURF_PY_H

#include "h5geo_py.h"

#include <h5geo/h5surfcontainer.h>
#include <h5geo/h5surf.h>

using namespace h5geo;

namespace h5geopy {

void H5Surf_py(
    py::class_<
    H5Surf,
    std::unique_ptr<H5Surf, py::nodelete>,
    H5BaseObject> &py_obj);

} // h5geopy


#endif // H5SURF_PY_H
