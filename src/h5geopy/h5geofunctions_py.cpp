#include "../../include/h5geopy/h5geofunctions_py.h"

#include <h5geo/h5surfcontainer.h>
#include <h5geo/h5surf.h>
#include <h5geo/h5wellcontainer.h>
#include <h5geo/h5well.h>
#include <h5geo/h5seiscontainer.h>
#include <h5geo/h5seis.h>

namespace h5geopy {


void defineGeoFunctions(py::module_& m){
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

  m.def("isGeoContainer", &isGeoContainer);
  m.def("isGeoContainerByType", &isGeoContainerByType);
  m.def("isGeoObject", &isGeoObject);
  m.def("isGeoObjectByType", &isGeoObjectByType);
  m.def("isSurf", &isSurf);
  m.def("isWell", &isWell);
  m.def("isLogCurve", &isLogCurve);
  m.def("isDevCurve", &isDevCurve);
  m.def("isSeis", &isSeis);
}


} // h5geopy
