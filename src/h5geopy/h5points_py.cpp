#include "../../include/h5geopy/h5points_py.h"

#include <h5geo/impl/h5point.h>

namespace h5geopy {

void H5Points_py(
    py::class_<
    H5Points,
    H5PointsImpl,
    H5BaseObject,
    H5Base,
    std::unique_ptr<H5Points, ObjectDeleter>>
    &py_obj){
  py_obj
      .def("writeData", &H5Points::writeData,
           py::arg("data"),
           py::arg_v("lengthUnits", "", "str()"),
           py::arg_v("temporalUnits", "", "str()"),
           py::arg_v("doCoordTransform", false, "False"))
      .def("getData", &H5Points::getData,
           py::arg_v("lengthUnits", "", "str()"),
           py::arg_v("temporalUnits", "", "str()"),
           py::arg_v("doCoordTransform", false, "False"))

      .def("setNPoints", &H5Points::setNPoints)
      .def("setDomain", &H5Points::setDomain)

      .def("openContainer", &H5Points::openContainer)
      .def("getNPoints", &H5Points::getNPoints)
      .def("getDomain", &H5Points::getDomain)
      .def("getPointsD", &H5Points::getPointsD);
}


} // h5geopy
