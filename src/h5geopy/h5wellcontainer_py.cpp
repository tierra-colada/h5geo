#include "../../include/h5geopy/h5wellcontainer_py.h"

namespace h5geopy {

void H5WellContainer_py(
    py::class_<
    H5WellContainer,
    H5WellContainerImpl,
    H5BaseContainer,
    H5Base,
    std::unique_ptr<H5WellContainer, ObjectDeleter>>
    &py_obj){
  py_obj
      .def("openWell", py::overload_cast<
           const std::string&>(
             &H5WellContainer::openWell))
      .def("openWell", py::overload_cast<
           h5gt::Group>(
             &H5WellContainer::openWell))
      .def("createWell", py::overload_cast<
           std::string&,
           WellParam&,
           CreationType>(
             &H5WellContainer::createWell))
      .def("openWellByUWI", &H5WellContainer::openWellByUWI)
      .def("createWell", py::overload_cast<
           h5gt::Group,
           WellParam&,
           CreationType>(
             &H5WellContainer::createWell));
}


} // h5geopy
