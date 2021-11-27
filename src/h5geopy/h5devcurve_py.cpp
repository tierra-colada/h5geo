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
      .def("writeCurve", py::overload_cast<
           const DevDataType&,
           Eigen::Ref<Eigen::VectorXd>,
           const std::string&>(
             &H5DevCurve::writeCurve),
           py::arg("name"),
           py::arg("data"),
           py::arg_v("units", "", "str()"))
      .def("writeCurve", py::overload_cast<
           const std::string&,
           Eigen::Ref<Eigen::VectorXd>,
           const std::string&>(
             &H5DevCurve::writeCurve),
           py::arg("name"),
           py::arg("data"),
           py::arg_v("units", "", "str()"))

      .def("setActive", &H5DevCurve::setActive)
      .def("isActive", &H5DevCurve::isActive)

      .def("getNCurves", &H5DevCurve::getNCurves)
      .def("getNSamp", &H5DevCurve::getNSamp)
      .def("getCurve", py::overload_cast<
           const DevDataType&,
           const std::string&>(
             &H5DevCurve::getCurve),
           py::arg("name"),
           py::arg_v("units", "", "str()"))
      .def("getCurve", py::overload_cast<
           const std::string&,
           const std::string&>(
             &H5DevCurve::getCurve),
           py::arg("name"),
           py::arg_v("units", "", "str()"))

      .def("getRelativeCurveName", &H5DevCurve::getRelativeCurveName)

      .def("getWellContainer", &H5DevCurve::getWellContainer)
      .def("getWell", &H5DevCurve::getWell)

      .def("getDevCurveD", &H5DevCurve::getDevCurveD);
}


} // h5geopy
