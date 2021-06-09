#include "../../include/h5geopy/h5devcurve_py.h"

#include <h5geo/misc/h5wellcontainerimpl.h>
#include <h5geo/misc/h5wellimpl.h>

namespace h5geopy {

void H5DevCurve_py(
    py::class_<
    H5DevCurveImpl,
    std::unique_ptr<H5DevCurveImpl, py::nodelete>,
    H5BaseObjectImpl> &py_obj){
  py_obj
      .def("writeCurve", py::overload_cast<const DevDataType&, const Eigen::Ref<const Eigen::VectorXd>&>(&H5DevCurveImpl::writeCurve))
      .def("writeCurve", py::overload_cast<const std::string&, const Eigen::Ref<const Eigen::VectorXd>&>(&H5DevCurveImpl::writeCurve))

      .def("setActive", &H5DevCurveImpl::setActive)
      .def("isActive", &H5DevCurveImpl::isActive)

      .def("getNCurves", &H5DevCurveImpl::getNCurves)
      .def("getNSamp", &H5DevCurveImpl::getNSamp)
      .def("getCurve", py::overload_cast<const DevDataType&>(&H5DevCurveImpl::getCurve))
      .def("getCurve", py::overload_cast<const std::string&>(&H5DevCurveImpl::getCurve))

      .def("getRelativeCurveName", &H5DevCurveImpl::getRelativeCurveName)

      .def("getWellContainer", &H5DevCurveImpl::getWellContainer)
      .def("getWell", &H5DevCurveImpl::getWell)

      .def("getDevCurveD", &H5DevCurveImpl::getDevCurveD);
}


} // h5geopy
