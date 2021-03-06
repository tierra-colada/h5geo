#include "../../include/h5geopy/h5devcurve_py.h"

#include <h5geo/private/h5wellcontainerimpl.h>
#include <h5geo/private/h5wellimpl.h>

namespace h5geopy {

void H5DevCurve_py(
    py::class_<
    H5DevCurve,
    H5DevCurveImpl,
    H5BaseObject,
    H5Base,
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
      .def("writeTVD", &H5DevCurve::writeTVD,
           py::arg("v"),
           py::arg_v("units", "", "str()"))
      .def("writeDX", &H5DevCurve::writeDX,
           py::arg("v"),
           py::arg_v("units", "", "str()"))
      .def("writeDY", &H5DevCurve::writeDY,
           py::arg("v"),
           py::arg_v("units", "", "str()"))
      .def("writeOWT", &H5DevCurve::writeOWT,
           py::arg("v"),
           py::arg_v("units", "", "str()"))

      .def("setActive", &H5DevCurve::setActive)
      .def("isActive", &H5DevCurve::isActive)

      .def("updateMdAzimIncl", &H5DevCurve::updateMdAzimIncl)
      .def("updateTvdDxDy", &H5DevCurve::updateTvdDxDy)

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

      .def("getRelativeName", &H5DevCurve::getRelativeName)
      .def("getParam", &H5DevCurve::getParam)

      .def("openWellContainer", &H5DevCurve::openWellContainer)
      .def("openWell", &H5DevCurve::openWell)

      .def("getDevCurveD", &H5DevCurve::getDevCurveD);
}


} // h5geopy
