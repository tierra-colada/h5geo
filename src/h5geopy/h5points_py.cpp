#include "../../include/h5geopy/h5points_py.h"

#include <h5geo/impl/h5point.h>

namespace h5geopy {

void H5Points1_py(
    py::class_<
    H5Points1,
    H5Points1Impl,
    H5BasePoints,
    H5BaseObject,
    H5Base,
    std::unique_ptr<H5Points1, ObjectDeleter>>
    &py_obj){
  py_obj
      .def("writeData", &H5Points1::writeData,
           py::arg("data"),
           py::arg_v("lengthUnits", "", "str()"),
           py::arg_v("temporalUnits", false, "False"))
      .def("getData", &H5Points1::getData,
           py::arg_v("lengthUnits", "", "str()"),
           py::arg_v("temporalUnits", false, "False"));
}

void H5Points2_py(
    py::class_<
    H5Points2,
    H5Points2Impl,
    H5BasePoints,
    H5BaseObject,
    H5Base,
    std::unique_ptr<H5Points2, ObjectDeleter>>
    &py_obj){
  py_obj
      .def("writeData", &H5Points2::writeData,
           py::arg("data"),
           py::arg_v("lengthUnits", "", "str()"),
           py::arg_v("doCoordTransform", false, "False"))
      .def("getData", &H5Points2::getData,
           py::arg_v("lengthUnits", "", "str()"),
           py::arg_v("doCoordTransform", false, "False"));
}

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
}

void H5Points4_py(
    py::class_<
    H5Points4,
    H5Points4Impl,
    H5BasePoints,
    H5BaseObject,
    H5Base,
    std::unique_ptr<H5Points4, ObjectDeleter>>
    &py_obj){
  py_obj
      .def("writeData", &H5Points4::writeData,
           py::arg("data"),
           py::arg_v("lengthUnits", "", "str()"),
           py::arg_v("temporalUnits", "", "str()"),
           py::arg_v("dataUnits", "", "str()"),
           py::arg_v("doCoordTransform", false, "False"))
      .def("getData", &H5Points4::getData,
           py::arg_v("lengthUnits", "", "str()"),
           py::arg_v("temporalUnits", "", "str()"),
           py::arg_v("dataUnits", "", "str()"),
           py::arg_v("doCoordTransform", false, "False"));
}


} // h5geopy
