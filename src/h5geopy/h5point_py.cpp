#include "../../include/h5geopy/h5point_py.h"

namespace h5geopy {

void Point_py(
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

} // h5geopy
