#include "../../include/h5geopy/h5wellcontainer_py.h"

namespace h5geopy {

void H5WellContainer_py(
    py::class_<
    H5WellContainer,
    H5WellContainerImpl,
    H5BaseContainer,
    std::unique_ptr<H5WellContainer, ObjectDeleter>>
    &py_obj){
  py_obj
      .def("getWell", py::overload_cast<
           const std::string&>(
             &H5WellContainer::getWell))
      .def("getWell", py::overload_cast<
           h5gt::Group>(
             &H5WellContainer::getWell))
      .def("createWell", py::overload_cast<
           std::string&,
           WellParam&,
           CreationType>(
             &H5WellContainer::createWell))
      .def("getWellByUWI", &H5WellContainer::getWellByUWI)
      .def("createWell", py::overload_cast<
           h5gt::Group,
           WellParam&,
           CreationType>(
             &H5WellContainer::createWell))
      .def("getWellList", &H5WellContainer::getWellList);
}


} // h5geopy
