#include "../../include/h5geopy/h5volcontainer_py.h"

namespace h5geopy {

void H5VolContainer_py(
    py::class_<
    H5VolContainer,
    H5VolContainerImpl,
    H5BaseContainer,
    H5Base,
    std::unique_ptr<H5VolContainer, ObjectDeleter>>
    &py_obj){
  py_obj
      .def("openVol", py::overload_cast<
           const std::string&>(
             &H5VolContainer::openVol))
      .def("openVol", py::overload_cast<
           h5gt::Group>(
             &H5VolContainer::openVol))
      .def("createVol", py::overload_cast<
           std::string&, H5VolParam&,
           CreationType>(
             &H5VolContainer::createVol))
      .def("createVol", py::overload_cast<
           h5gt::Group,
           H5VolParam&,
           CreationType>(
             &H5VolContainer::createVol));
}


} // h5geopy
