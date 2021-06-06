#ifndef H5GEOFUNCTIONS_PY_H
#define H5GEOFUNCTIONS_PY_H

#include "h5geo_py.h"

#include <h5geo/misc/h5base.h>

using namespace h5geo;

namespace h5geopy {

void defineAllFunctions(py::module_& m);

} // h5geopy


#endif // H5GEOFUNCTIONS_PY_H
