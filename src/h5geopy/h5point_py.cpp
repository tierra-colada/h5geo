#include "../../include/h5geopy/h5point_py.h"

namespace h5geopy {

void Point1_py(
    py::class_<Point1>
    &py_obj){
  py_obj
      .def("setX", &Point1::setX)

      .def("getX", &Point1::getX)

      .def("setName", &Point1::setName)
      .def("getName", &Point1::getName);
}

void Point2_py(
    py::class_<Point2>
    &py_obj){
  py_obj
      .def("setX", &Point2::setX)
      .def("setY", &Point2::setY)

      .def("getX", &Point2::getX)
      .def("getY", &Point2::getY)

      .def("setName", &Point2::setName)
      .def("getName", &Point2::getName);
}

void Point3_py(
    py::class_<Point3>
    &py_obj){
  py_obj
      .def("setX", &Point3::setX)
      .def("setY", &Point3::setY)
      .def("setZ", &Point3::setZ)

      .def("getX", &Point3::getX)
      .def("getY", &Point3::getY)
      .def("getZ", &Point3::getZ)

      .def("setName", &Point3::setName)
      .def("getName", &Point3::getName);
}

void Point4_py(
    py::class_<Point4>
    &py_obj){
  py_obj
      .def("setX", &Point4::setX)
      .def("setY", &Point4::setY)
      .def("setZ", &Point4::setZ)
      .def("setVal", &Point4::setVal)

      .def("getX", &Point4::getX)
      .def("getY", &Point4::getY)
      .def("getZ", &Point4::getZ)
      .def("getVal", &Point4::getVal)

      .def("setName", &Point4::setName)
      .def("getName", &Point4::getName);
}

} // h5geopy
