#include "../../include/h5geopy/h5well_py.h"

#include <h5geo/misc/h5devcurveimpl.h>
#include <h5geo/misc/h5logcurveimpl.h>

namespace h5geopy {

void H5Well_py(
    py::class_<
    H5Well,
    H5WellImpl,
    H5BaseObject,
    std::unique_ptr<H5Well, ObjectDeleter>>
    &py_obj){
  py_obj
      .def("getLogCurve", py::overload_cast<const std::string&, const std::string&>(&H5Well::getLogCurve))
      .def("getLogCurve", py::overload_cast<h5gt::Group>(&H5Well::getLogCurve))
      .def("getDevCurve", py::overload_cast<const std::string&>(&H5Well::getDevCurve))
      .def("getDevCurve", py::overload_cast<h5gt::Group>(&H5Well::getDevCurve))
      .def("createLogCurve", py::overload_cast<std::string&, std::string&, LogCurveParam&, CreationType>(&H5Well::createLogCurve))
      .def("createLogCurve", py::overload_cast<h5gt::Group, LogCurveParam&, CreationType>(&H5Well::createLogCurve))
      .def("createDevCurve", py::overload_cast<std::string&, DevCurveParam&, CreationType>(&H5Well::createDevCurve))
      .def("createDevCurve", py::overload_cast<h5gt::Group, DevCurveParam&, CreationType>(&H5Well::createDevCurve))

      .def("setHeadCoord", &H5Well::setHeadCoord,
           py::arg("xy"), py::arg_v("lengthUnits", "", "str()"),
           py::arg_v("doCoordTransform", false, "False"))
      .def("setKB", &H5Well::setKB,
           py::arg("val"), py::arg_v("lengthUnits", "", "str()"))
      .def("setUWI", &H5Well::setUWI)

      .def("getHeadCoord", &H5Well::getHeadCoord,
           py::arg_v("lengthUnits", "", "str()"),
           py::arg_v("doCoordTransform", false, "False"))
      .def("getKB", &H5Well::getKB,
           py::arg_v("lengthUnits", "", "str()"))
      .def("getUWI", &H5Well::getUWI)

      .def("getActiveDevCurve", &H5Well::getActiveDevCurve)
      .def("getDevCurveList", &H5Well::getDevCurveGroupList)
      .def("getLogCurveList", &H5Well::getLogCurveGroupList)
      .def("getDevCurveNameList", &H5Well::getDevCurveNameList)
      .def("getLogCurveNameList", &H5Well::getLogCurveNameList)
      .def("getLogTypeNameList", &H5Well::getLogTypeNameList)

      .def("getWellContainer", &H5Well::getWellContainer)

      .def("getDevG", &H5Well::getDevG)
      .def("getActiveDevG", &H5Well::getActiveDevG)
      .def("getLogG", &H5Well::getLogG)
      .def("getLogTypeG", &H5Well::getLogTypeG);
}


} // h5geopy
