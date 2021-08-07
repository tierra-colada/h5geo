#include "../../include/h5geopy/h5devcurve_py.h"

#include <h5geo/misc/h5wellcontainerimpl.h>
#include <h5geo/misc/h5wellimpl.h>

namespace h5geopy {

void H5DevCurve_py(
    py::class_<
    H5DevCurveImpl,
    std::unique_ptr<H5DevCurveImpl, ObjectDeleter>,
    H5BaseObjectImpl,
    H5DevCurve> &py_obj){
  py_obj
      .def("writeCurve", py::overload_cast<const DevDataType&,
           const Eigen::Ref<const Eigen::VectorXd>&, const std::string&>(
             &H5DevCurveImpl::writeCurve),
           py::arg("name"), py::arg("data"), py::arg_v("units", "", "str()"))
      .def("writeCurve", py::overload_cast<const std::string&,
           const Eigen::Ref<const Eigen::VectorXd>&, const std::string&>(
             &H5DevCurveImpl::writeCurve),
           py::arg("name"), py::arg("data"), py::arg_v("units", "", "str()"))

      .def("setActive", &H5DevCurveImpl::setActive)
      .def("isActive", &H5DevCurveImpl::isActive)

      .def("getNCurves", &H5DevCurveImpl::getNCurves)
      .def("getNSamp", &H5DevCurveImpl::getNSamp)
      .def("getCurve", py::overload_cast<const DevDataType&, const std::string&>(&H5DevCurveImpl::getCurve),
           py::arg("name"), py::arg_v("units", "", "str()"))
      .def("getCurve", py::overload_cast<const std::string&, const std::string&>(&H5DevCurveImpl::getCurve),
           py::arg("name"), py::arg_v("units", "", "str()"))

      .def("setSpatialUnits", &H5DevCurve::setSpatialUnits)
      .def("setTemporalUnits", &H5DevCurve::setTemporalUnits)
      .def("setAngleUnits", &H5DevCurve::setAngleUnits)

      .def("getSpatialUnits", &H5DevCurve::getSpatialUnits)
      .def("getTemporalUnits", &H5DevCurve::getTemporalUnits)
      .def("getAngleUnits", &H5DevCurve::getAngleUnits)

      .def("getRelativeCurveName", &H5DevCurveImpl::getRelativeCurveName)

      .def("getWellContainer", &H5DevCurveImpl::getWellContainer)
      .def("getWell", &H5DevCurveImpl::getWell)

      .def("getDevCurveD", &H5DevCurveImpl::getDevCurveD);
}


} // h5geopy
