#include "../../include/h5geopy/h5basecontainer_py.h"

namespace h5geopy {

void H5BaseContainer_py(
    py::class_<
    H5BaseContainer,
    std::unique_ptr<H5BaseContainer, pybind11::nodelete>,
    H5Base> &py_obj){
  py_obj
      .def("getH5File", &H5BaseContainer::getH5File);
//      .def(py::self == py::self)
//      .def(py::self != py::self);
}


} // h5geopy
