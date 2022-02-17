#include "../../include/h5geopy/h5logcurve_py.h"

#include <h5geo/misc/h5wellcontainerimpl.h>
#include <h5geo/misc/h5wellimpl.h>

namespace h5geopy {

void H5LogCurve_py(
    py::class_<
    H5LogCurve,
    H5LogCurveImpl,
    H5BaseObject,
    H5Base,
    std::unique_ptr<H5LogCurve, ObjectDeleter>>
    &py_obj){
  py_obj
      .def("writeCurve", py::overload_cast<
           const LogDataType&,
           Eigen::Ref<Eigen::VectorXd>,
           const std::string&>(
             &H5LogCurve::writeCurve),
           py::arg("name"),
           py::arg("data"),
           py::arg_v("units", "", "str()"))
      .def("writeCurve", py::overload_cast<
           const std::string&,
           Eigen::Ref<Eigen::VectorXd>,
           const std::string&>(
             &H5LogCurve::writeCurve),
           py::arg("name"),
           py::arg("data"),
           py::arg_v("units", "", "str()"))

      .def("getNCurves", &H5LogCurve::getNCurves)
      .def("getNSamp", &H5LogCurve::getNSamp)
      .def("getCurve", py::overload_cast<
           const LogDataType&,
           const std::string&>(
             &H5LogCurve::getCurve),
           py::arg("name"),
           py::arg_v("units", "", "str()"))
      .def("getCurve", py::overload_cast<
           const std::string&,
           const std::string&>(
             &H5LogCurve::getCurve),
           py::arg("name"),
           py::arg_v("units", "", "str()"))

      .def("getRelativeCurveName", &H5LogCurve::getRelativeCurveName)

      .def("openWellContainer", &H5LogCurve::openWellContainer)
      .def("openWell", &H5LogCurve::openWell)

      .def("getLogCurveD", &H5LogCurve::getLogCurveD);
}


} // h5geopy
