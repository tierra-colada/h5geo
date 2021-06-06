#include "../../include/h5geopy/h5geofunctions_py.h"

#include <h5geo/h5surfcontainer.h>
#include <h5geo/h5surf.h>

namespace h5geopy {

void defineAllFunctions(py::module_& m){
  m.def("openBaseContainer", &openBaseContainer);
  m.def("openBaseContainerByName", &openBaseContainerByName);
  m.def("openBaseObject", &openBaseObject);
//  m.def("openSeisContainer", &openSeisContainer);
//  m.def("openSeisContainerByName", &openSeisContainerByName);
  m.def("openSurfContainerByName", &openSurfContainerByName);
//  m.def("openWellContainer", &openWellContainer);
//  m.def("openWellContainerByName", &openWellContainerByName);

  m.def("createSurfContainer", &createSurfContainer);
  m.def("createSurfContainerByName", &createSurfContainerByName);
}


} // h5geopy
