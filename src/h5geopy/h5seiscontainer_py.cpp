#include "../../include/h5geopy/h5seiscontainer_py.h"

namespace h5geopy {

void H5SeisContainer_py(
    py::class_<
    H5SeisContainerImpl,
    std::unique_ptr<H5SeisContainerImpl, py::nodelete>,
    H5BaseContainerImpl> &py_obj){
  py_obj
      .def("getSeis", py::overload_cast<const std::string&>(&H5SeisContainerImpl::getSeis))
      .def("getSeis", py::overload_cast<h5gt::Group>(&H5SeisContainerImpl::getSeis))
      .def("createSeis", py::overload_cast<std::string&, SeisParam&, CreationType>(&H5SeisContainerImpl::createSeis))
      .def("createSeis", py::overload_cast<h5gt::Group, SeisParam&, CreationType>(&H5SeisContainerImpl::createSeis))
      .def("getSeisList", &H5SeisContainerImpl::getSeisList);
}


} // h5geopy
