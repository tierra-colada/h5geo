#include "../../include/h5geopy/h5wellcontainer_py.h"

namespace h5geopy {

void H5WellContainer_py(
    py::class_<
    H5WellContainerImpl,
    std::unique_ptr<H5WellContainerImpl, ObjectDeleter>,
    H5BaseContainerImpl,
    H5WellContainer> &py_obj){
  py_obj
      .def("getWell", py::overload_cast<
           const std::string&>(
             &H5WellContainerImpl::getWell))
      .def("getWell", py::overload_cast<
           h5gt::Group>(
             &H5WellContainerImpl::getWell))
      .def("createWell", py::overload_cast<
           std::string&,
           WellParam&,
           CreationType>(
             &H5WellContainerImpl::createWell))
      .def("createWell", py::overload_cast<
           h5gt::Group,
           WellParam&,
           CreationType>(
             &H5WellContainerImpl::createWell))
      .def("getWellList", &H5WellContainerImpl::getWellList);
}


} // h5geopy
