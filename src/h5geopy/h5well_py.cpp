#include "../../include/h5geopy/h5well_py.h"

#include <h5geo/private/h5devcurveimpl.h>
#include <h5geo/private/h5logcurveimpl.h>
#include <h5geo/private/h5welltopsimpl.h>

namespace h5geopy {

void H5Well_py(
    py::class_<
    H5Well,
    H5WellImpl,
    H5BaseObject,
    H5Base,
    std::unique_ptr<H5Well, ObjectDeleter>>
    &py_obj){
  py_obj
      .def("openLogCurve", py::overload_cast<const std::string&, const std::string&>(&H5Well::openLogCurve))
      .def("openLogCurve", py::overload_cast<h5gt::Group>(&H5Well::openLogCurve))
      .def("openDevCurve", py::overload_cast<const std::string&>(&H5Well::openDevCurve))
      .def("openDevCurve", py::overload_cast<h5gt::Group>(&H5Well::openDevCurve))
      .def("openWellTops", &H5Well::openWellTops)

      .def("createLogCurve", py::overload_cast<std::string&, std::string&, H5LogCurveParam&, CreationType>(&H5Well::createLogCurve))
      .def("createLogCurve", py::overload_cast<h5gt::Group, H5LogCurveParam&, CreationType>(&H5Well::createLogCurve))
      .def("createDevCurve", py::overload_cast<std::string&, H5DevCurveParam&, CreationType>(&H5Well::createDevCurve))
      .def("createDevCurve", py::overload_cast<h5gt::Group, H5DevCurveParam&, CreationType>(&H5Well::createDevCurve))
      .def("createWellTops", &H5Well::createWellTops)

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

      .def("openActiveDevCurve", &H5Well::openActiveDevCurve)
      .def("getDevCurveGroupList", &H5Well::getDevCurveGroupList)
      .def("getLogCurveGroupList", &H5Well::getLogCurveGroupList)
      .def("getDevCurveNameList", &H5Well::getDevCurveNameList)
      .def("getLogCurveNameList", &H5Well::getLogCurveNameList)
      .def("getLogTypeList", &H5Well::getLogTypeList)
      .def("getDevCurveCount", &H5Well::getDevCurveCount)
      .def("getLogCurveCount", &H5Well::getLogCurveCount)
      .def("getParam", &H5Well::getParam)

      .def("openWellContainer", &H5Well::openWellContainer)

      .def("getDevG", &H5Well::getDevG)
      .def("getActiveDevG", &H5Well::getActiveDevG)
      .def("getWellTopsG", &H5Well::getWellTopsG)
      .def("getLogG", &H5Well::getLogG)
      .def("getLogTypeG", &H5Well::getLogTypeG);
}


} // h5geopy
