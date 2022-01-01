#include "../../include/h5geopy/h5points_py.h"

#include <h5geo/misc/h5core_types.h>

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

      .def("setDomain", &H5Points::setDomain)

      .def("getContainer", &H5Points::getContainer)
      .def("getDomain", &H5Points::getDomain)
      .def("getPointsD", &H5Points::getPointsD);
}


} // h5geopy
