#include "../../include/h5geopy/h5surf_py.h"

namespace h5geopy {

void H5Surf_py(
    py::class_<
    H5Surf,
    std::unique_ptr<H5Surf, py::nodelete>,
    H5BaseObject> &py_obj){
  py_obj
      .def("writeData", &H5Surf::writeData)
      .def("getData", &H5Surf::getData)
      .def("getSurfContainer", &H5Surf::getSurfContainer)
      .def("getSurfD", &H5Surf::getSurfD);
}


} // h5geopy
