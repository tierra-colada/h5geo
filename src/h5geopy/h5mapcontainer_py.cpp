#include "../../include/h5geopy/h5mapcontainer_py.h"

namespace h5geopy {

void H5MapContainer_py(
    py::class_<
    H5MapContainer,
    H5MapContainerImpl,
    H5BaseContainer,
    H5Base,
    std::unique_ptr<H5MapContainer, ObjectDeleter>>
    &py_obj){
  py_obj
      .def("openMap", py::overload_cast<
           const std::string&>(
             &H5MapContainer::openMap))
      .def("openMap", py::overload_cast<
           h5gt::Group>(
             &H5MapContainer::openMap))
      .def("createMap", py::overload_cast<
           std::string&,
           MapParam&,
           CreationType>(
             &H5MapContainer::createMap))
      .def("createMap", py::overload_cast<
           h5gt::Group,
           MapParam&,
           CreationType>(
             &H5MapContainer::createMap))
      .def("openMapList", &H5MapContainer::openMapList);
}

} // h5geopy
