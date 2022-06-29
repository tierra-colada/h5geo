#include "../../include/h5geopy/h5horizon_py.h"

namespace h5geopy {

void H5Horizon_py(
    py::class_<
    H5Horizon,
    H5HorizonImpl,
    H5BaseObject,
    H5Base,
    std::unique_ptr<H5Horizon, ObjectDeleter>>
    &py_obj){
  py_obj
      .def("writeData", &H5Horizon::writeData,
           py::arg("data"),
           py::arg_v("unitsFrom", "", "str()"),
           py::arg_v("unitsTo", "", "str()"))
      .def("getData", &H5Horizon::getData,
           py::arg_v("unitsFrom", "", "str()"),
           py::arg_v("unitsTo", "", "str()"))

      .def("writeComponent", &H5Horizon::writeComponent,
           py::arg("componentName"),
           py::arg("data"),
           py::arg_v("unitsFrom", "", "str()"),
           py::arg_v("unitsTo", "", "str()"))
      .def("getComponent", &H5Horizon::getComponent,
           py::arg("componentName"),
           py::arg_v("unitsFrom", "", "str()"),
           py::arg_v("unitsTo", "", "str()"))

      .def("setNPoints", &H5Horizon::setNPoints)
      .def("setNComponents", &H5Horizon::setNComponents)
      .def("setComponents", &H5Horizon::setComponents)
      .def("setDomain", &H5Horizon::setDomain)

      .def("openContainer", &H5Horizon::openContainer)
      .def("getNPoints", &H5Horizon::getNPoints)
      .def("getNComponents", &H5Horizon::getNComponents)
      .def("getComponents", &H5Horizon::getComponents)
      .def("getDomain", &H5Horizon::getDomain)

      .def("getParam", &H5Horizon::getParam)

      .def("getHorizonD", &H5Horizon::getHorizonD);
}


} // h5geopy
