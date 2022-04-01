#include "../../include/h5geopy/h5points3_py.h"

#include <h5geo/impl/h5point.h>

namespace h5geopy {

void H5Points3_py(
    py::class_<
    H5Points3,
    H5Points3Impl,
    H5BasePoints,
    H5BaseObject,
    H5Base,
    std::unique_ptr<H5Points3, ObjectDeleter>>
    &py_obj){
  py_obj
      .def("writeData", &H5Points3::writeData,
           py::arg("data"),
           py::arg_v("lengthUnits", "", "str()"),
           py::arg_v("temporalUnits", "", "str()"),
           py::arg_v("doCoordTransform", false, "False"))
      .def("getData", &H5Points3::getData,
           py::arg_v("lengthUnits", "", "str()"),
           py::arg_v("temporalUnits", "", "str()"),
           py::arg_v("doCoordTransform", false, "False"));

//      .def("setNPoints", &H5Points3::setNPoints)
//      .def("setDomain", &H5Points3::setDomain)

//      .def("openContainer", &H5Points3::openContainer)
//      .def("getNPoints", &H5Points3::getNPoints)
//      .def("getDomain", &H5Points3::getDomain)
//      .def("getPointsD", &H5Points3::getPointsD);
}


} // h5geopy
