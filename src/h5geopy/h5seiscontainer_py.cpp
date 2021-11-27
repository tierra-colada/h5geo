#include "../../include/h5geopy/h5seiscontainer_py.h"

namespace h5geopy {

void H5SeisContainer_py(
    py::class_<
    H5SeisContainer,
    H5SeisContainerImpl,
    H5BaseContainer,
    std::unique_ptr<H5SeisContainer, ObjectDeleter>>
    &py_obj){
  py_obj
      .def("getSeis", py::overload_cast<
           const std::string&>(
             &H5SeisContainer::getSeis))
      .def("getSeis", py::overload_cast<
           h5gt::Group>(
             &H5SeisContainer::getSeis))
      .def("createSeis", py::overload_cast<
           std::string&, SeisParam&,
           CreationType>(
             &H5SeisContainer::createSeis))
      .def("createSeis", py::overload_cast<
           h5gt::Group,
           SeisParam&,
           CreationType>(
             &H5SeisContainer::createSeis))
      .def("getSeisList", &H5SeisContainer::getSeisList);
}


} // h5geopy
