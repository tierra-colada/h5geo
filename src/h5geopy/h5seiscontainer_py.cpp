#include "../../include/h5geopy/h5seiscontainer_py.h"

namespace h5geopy {

void H5SeisContainer_py(
    py::class_<
    H5SeisContainer,
    H5SeisContainerImpl,
    H5BaseContainer,
    H5Base,
    std::unique_ptr<H5SeisContainer, ObjectDeleter>>
    &py_obj){
  py_obj
      .def("openSeis", py::overload_cast<
           const std::string&>(
             &H5SeisContainer::openSeis))
      .def("openSeis", py::overload_cast<
           h5gt::Group>(
             &H5SeisContainer::openSeis))
      .def("createSeis", py::overload_cast<
           std::string&, H5SeisParam&,
           CreationType>(
             &H5SeisContainer::createSeis))
      .def("createSeis", py::overload_cast<
           h5gt::Group,
           H5SeisParam&,
           CreationType>(
             &H5SeisContainer::createSeis));
}


} // h5geopy
