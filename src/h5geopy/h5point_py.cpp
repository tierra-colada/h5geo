#include "../../include/h5geopy/h5point_py.h"

namespace h5geopy {

void Point_py(
    py::class_<Point>
    &py_obj){
  py_obj
      .def("setX", &Point::setX)
      .def("setY", &Point::setY)
      .def("setZ", &Point::setZ)

      .def("x", &Point::x)
      .def("y", &Point::y)
      .def("z", &Point::z)

      .def("setName", &Point::setName)
      .def("getName", &Point::getName);
}

} // h5geopy
