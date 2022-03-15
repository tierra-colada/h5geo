#include "../../include/h5geopy/h5point_py.h"

namespace h5geopy {

void Point_py(
    py::class_<Point>
    &py_obj){
  py_obj
      .def("setX", &Point::setX)
      .def("setY", &Point::setY)
      .def("setZ", &Point::setZ)

      .def("getX", &Point::getX)
      .def("getY", &Point::getY)
      .def("getZ", &Point::getZ)

      .def("setName", &Point::setName)
      .def("getName", &Point::getName);
}

} // h5geopy
