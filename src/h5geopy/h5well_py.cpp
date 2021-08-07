#include "../../include/h5geopy/h5well_py.h"

#include <h5geo/misc/h5devcurveimpl.h>
#include <h5geo/misc/h5logcurveimpl.h>

namespace h5geopy {

void H5Well_py(
    py::class_<
    H5WellImpl,
    std::unique_ptr<H5WellImpl, ObjectDeleter>,
    H5BaseObjectImpl,
    H5Well> &py_obj){
  py_obj
      .def("getLogCurve", py::overload_cast<const std::string&, const std::string&>(&H5WellImpl::getLogCurve))
      .def("getLogCurve", py::overload_cast<h5gt::Group>(&H5WellImpl::getLogCurve))
      .def("getDevCurve", py::overload_cast<const std::string&>(&H5WellImpl::getDevCurve))
      .def("getDevCurve", py::overload_cast<h5gt::Group>(&H5WellImpl::getDevCurve))
      .def("createLogCurve", py::overload_cast<std::string&, std::string&, LogCurveParam&, CreationType>(&H5WellImpl::createLogCurve))
      .def("createLogCurve", py::overload_cast<h5gt::Group, LogCurveParam&, CreationType>(&H5WellImpl::createLogCurve))
      .def("createDevCurve", py::overload_cast<std::string&, DevCurveParam&, CreationType>(&H5WellImpl::createDevCurve))
      .def("createDevCurve", py::overload_cast<h5gt::Group, DevCurveParam&, CreationType>(&H5WellImpl::createDevCurve))

      .def("setSpatialUnits", &H5WellImpl::setSpatialUnits)
      .def("setHeadCoord", &H5WellImpl::setHeadCoord,
           py::arg("xy"), py::arg_v("spatialUnits", "", "str()"))
      .def("setKB", &H5WellImpl::setKB,
           py::arg("val"), py::arg_v("spatialUnits", "", "str()"))
      .def("setUWI", &H5WellImpl::setUWI)

      .def("getSpatialUnits", &H5WellImpl::getSpatialUnits)
      .def("getHeadCoord", &H5WellImpl::getHeadCoord,
           py::arg_v("spatialUnits", "", "str()"))
      .def("getKB", &H5WellImpl::getKB)
      .def("getUWI", &H5WellImpl::getUWI)

      .def("getActiveDevCurve", &H5WellImpl::getActiveDevCurve)
      .def("getDevCurveList", &H5WellImpl::getDevCurveList)
      .def("getLogCurveList", &H5WellImpl::getLogCurveList)
      .def("getDevCurveNameList", &H5WellImpl::getDevCurveNameList)
      .def("getLogCurveNameList", &H5WellImpl::getLogCurveNameList)
      .def("getLogTypeNameList", &H5WellImpl::getLogTypeNameList)

      .def("getWellContainer", &H5WellImpl::getWellContainer)

      .def("getDevG", &H5WellImpl::getDevG)
      .def("getActiveDevG", &H5WellImpl::getActiveDevG)
      .def("getLogG", &H5WellImpl::getLogG)
      .def("getLogTypeG", &H5WellImpl::getLogTypeG);
}


} // h5geopy
