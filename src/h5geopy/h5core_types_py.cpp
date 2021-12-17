#include "../../include/h5geopy/h5core_types_py.h"

namespace h5geopy {

void Point_py(
    py::class_<Point>
    &py_obj){
  py_obj
      .def(py::init<>())
      .def(py::init<
           const double&,
           const double&,
           const double&,
           const std::string&>())

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
