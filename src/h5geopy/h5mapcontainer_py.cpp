#include "../../include/h5geopy/h5mapcontainer_py.h"

namespace h5geopy {

void H5MapContainer_py(
    py::class_<
    H5MapContainerImpl,
    std::unique_ptr<H5MapContainerImpl, ObjectDeleter>,
    H5BaseContainerImpl,
    H5MapContainer> &py_obj){
  py_obj
      .def("getMap", py::overload_cast<
           const std::string&>(
             &H5MapContainerImpl::getMap))
      .def("getMap", py::overload_cast<
           h5gt::Group>(
             &H5MapContainerImpl::getMap))
      .def("createMap", py::overload_cast<
           std::string&,
           MapParam&,
           CreationType>(
             &H5MapContainerImpl::createMap))
      .def("createMap", py::overload_cast<
           h5gt::Group,
           MapParam&,
           CreationType>(
             &H5MapContainerImpl::createMap))
      .def("getMapList", &H5MapContainerImpl::getMapList);
}


} // h5geopy
