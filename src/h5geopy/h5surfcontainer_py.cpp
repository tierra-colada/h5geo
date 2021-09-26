#include "../../include/h5geopy/h5surfcontainer_py.h"

namespace h5geopy {

void H5SurfContainer_py(
    py::class_<
    H5SurfContainerImpl,
    std::unique_ptr<H5SurfContainerImpl, ObjectDeleter>,
    H5BaseContainerImpl,
    H5SurfContainer> &py_obj){
  py_obj
      .def("getSurf", py::overload_cast<
           const std::string&>(
             &H5SurfContainerImpl::getSurf))
      .def("getSurf", py::overload_cast<
           h5gt::Group>(
             &H5SurfContainerImpl::getSurf))
      .def("createSurf", py::overload_cast<
           std::string&,
           SurfParam&,
           CreationType>(
             &H5SurfContainerImpl::createSurf))
      .def("createSurf", py::overload_cast<
           h5gt::Group,
           SurfParam&,
           CreationType>(
             &H5SurfContainerImpl::createSurf))
      .def("getSurfList", &H5SurfContainerImpl::getSurfList);
}


} // h5geopy
