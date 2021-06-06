#include "../../include/h5geopy/h5surfcontainer_py.h"

namespace h5geopy {

void H5SurfContainer_py(
    py::class_<
    H5SurfContainer,
    std::unique_ptr<H5SurfContainer, py::nodelete>,
    H5BaseContainer> &py_obj){
  py_obj
      .def("getSurf", py::overload_cast<const std::string&>(&H5SurfContainer::getSurf))
      .def("getSurf", py::overload_cast<h5gt::Group>(&H5SurfContainer::getSurf))
      .def("createSurf", py::overload_cast<std::string&, SurfParam&, CreationType>(&H5SurfContainer::createSurf))
      .def("createSurf", py::overload_cast<h5gt::Group, SurfParam&, CreationType>(&H5SurfContainer::createSurf))
      .def("getSurfList", &H5SurfContainer::getSurfList);
}


} // h5geopy
