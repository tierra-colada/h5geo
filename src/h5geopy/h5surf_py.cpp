#include "../../include/h5geopy/h5surf_py.h"

namespace h5geopy {

void H5Surf_py(
    py::class_<
    H5SurfImpl,
    std::unique_ptr<H5SurfImpl, py::nodelete>,
    H5BaseObjectImpl> &py_obj){
  py_obj
      .def("writeData", &H5SurfImpl::writeData)
      .def("getData", &H5SurfImpl::getData)
      .def("getSurfContainer", &H5SurfImpl::getSurfContainer)
      .def("getSurfD", &H5SurfImpl::getSurfD);
}


} // h5geopy
