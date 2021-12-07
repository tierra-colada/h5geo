#include "../../include/h5geopy/h5devcurve_py.h"

#include <h5geo/misc/h5wellcontainerimpl.h>
#include <h5geo/misc/h5wellimpl.h>

namespace h5geopy {

void H5DevCurve_py(
    py::class_<
    H5DevCurve,
    H5DevCurveImpl,
    H5BaseObject,
    std::unique_ptr<H5DevCurve, ObjectDeleter>>
    &py_obj){
  py_obj
      .def("writeMD", &H5DevCurve::writeMD,
           py::arg("v"),
           py::arg_v("units", "", "str()"))
      .def("writeAZIM", &H5DevCurve::writeAZIM,
           py::arg("v"),
           py::arg_v("units", "", "str()"))
      .def("writeINCL", &H5DevCurve::writeINCL,
           py::arg("v"),
           py::arg_v("units", "", "str()"))
      .def("writeOWT", &H5DevCurve::writeOWT,
           py::arg("v"),
           py::arg_v("units", "", "str()"))

      .def("setActive", &H5DevCurve::setActive)
      .def("isActive", &H5DevCurve::isActive)

      .def("getNCurves", &H5DevCurve::getNCurves)
      .def("getNSamp", &H5DevCurve::getNSamp)
      .def("getCurve", py::overload_cast<
           const DevDataType&,
           const std::string&,
           bool>(
             &H5DevCurve::getCurve),
           py::arg("name"),
           py::arg_v("units", "", "str()"),
           py::arg_v("doCoordTransform", false, "False"))
      .def("getCurve", py::overload_cast<
           const std::string&,
           const std::string&,
           bool>(
             &H5DevCurve::getCurve),
           py::arg("name"),
           py::arg_v("units", "", "str()"),
           py::arg_v("doCoordTransform", false, "False"))

      .def("getRelativeCurveName", &H5DevCurve::getRelativeCurveName)

      .def("getWellContainer", &H5DevCurve::getWellContainer)
      .def("getWell", &H5DevCurve::getWell)

      .def("getDevCurveD", &H5DevCurve::getDevCurveD);
}


} // h5geopy
