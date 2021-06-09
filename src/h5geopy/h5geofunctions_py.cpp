#include "../../include/h5geopy/h5geofunctions_py.h"

#include <h5geo/h5surfcontainer.h>
#include <h5geo/h5surf.h>
#include <h5geo/h5wellcontainer.h>
#include <h5geo/h5well.h>
#include <h5geo/h5seiscontainer.h>
#include <h5geo/h5seis.h>

namespace h5geopy {

void defineAllFunctions(py::module_& m){
  m.def("openBaseContainer", &openBaseContainer);
  m.def("openBaseContainerByName", &openBaseContainerByName);
  m.def("openBaseObject", &openBaseObject);
  m.def("openSeisContainer", &openSeisContainer);
  m.def("openSeisContainerByName", &openSeisContainerByName);
  m.def("openSurfContainerByName", &openSurfContainerByName);
  m.def("openWellContainer", &openWellContainer);
  m.def("openWellContainerByName", &openWellContainerByName);

  m.def("createSurfContainer", &createSurfContainer);
  m.def("createSurfContainerByName", &createSurfContainerByName);
  m.def("createWellContainer", &createWellContainer);
  m.def("createWellContainerByName", &createWellContainerByName);
  m.def("createSeisContainer", &createSeisContainer);
  m.def("createSeisContainerByName", &createSeisContainerByName);
}


} // h5geopy
