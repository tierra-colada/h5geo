#include "../../include/h5geopy/h5welltops_py.h"

namespace h5geopy {

void H5WellTops_py(
    py::class_<
    H5WellTops,
    H5WellTopsImpl,
    H5Points1,
    H5BasePoints,
    H5BaseObject,
    H5Base,
    std::unique_ptr<H5WellTops, ObjectDeleter>>
    &py_obj){
  py_obj
      .def("writeWellTops", &H5WellTops::writeWellTops,
           py::arg("data"),
           py::arg_v("lengthUnits", "", "str()"),
           py::arg_v("temporalUnits", false, "False"))
      .def("getWellTops", &H5WellTops::getWellTops,
           py::arg_v("lengthUnits", "", "str()"),
           py::arg_v("temporalUnits", false, "False"));
}

} // h5geopy
