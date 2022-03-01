#include "../../include/h5geopy/h5sr_settings_py.h"

#include <h5geo/impl/h5sr_settings.h>

namespace h5geopy {


void defineSRSettingsFunctions(py::module_& m_sr){
//  m_sr.attr("SpatialReference")= &sr::SpatialReference;

  m_sr.def("setSpatialReference", &sr::setSpatialReference);
  m_sr.def("setSpatialReferenceFromUserInput", py::overload_cast<
           const std::string&>(
             &sr::setSpatialReferenceFromUserInput));
  m_sr.def("setSpatialReferenceFromUserInput", py::overload_cast<
           const std::string&, const std::string&>(
             &sr::setSpatialReferenceFromUserInput));
  m_sr.def("getSpatialReference", &sr::getSpatialReference);
//  m_sr.def("getSpatialReference",[]() {
//    OGRSpatialReference sref = sr::getSpatialReference();
//    py::object osr = py::module::import("osgeo").attr("osr");
//    return py::cast(sref);
//  });
  m_sr.def("setLengthUnits", &sr::setLengthUnits);
  m_sr.def("setAngularUnits", &sr::setAngularUnits);
  m_sr.def("setTemporalUnits", &sr::setTemporalUnits);
  m_sr.def("getLengthUnits", &sr::getLengthUnits);
  m_sr.def("getAngularUnits", &sr::getAngularUnits);
  m_sr.def("getTemporalUnits", &sr::getTemporalUnits);
  m_sr.def("setDomain", py::overload_cast<const std::string&>(&sr::setDomain));
  m_sr.def("setDomain", py::overload_cast<const h5geo::Domain&>(&sr::setDomain));
  m_sr.def("getDomain", &sr::getDomain);
  m_sr.def("getDomainEnum", &sr::getDomainEnum);
}


} // h5geopy
