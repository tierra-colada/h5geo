#include "../../include/h5geopy/h5welltops_py.h"

#include <h5geo/impl/h5wellimpl.h>

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
      .def("writeData", &H5WellTops::writeData,
           py::arg("data"),
           py::arg_v("lengthUnits", "", "str()"),
           py::arg_v("temporalUnits", false, "False"))
      .def("getDataAsMap", &H5WellTops::getDataAsMap,
           py::arg_v("lengthUnits", "", "str()"),
           py::arg_v("temporalUnits", false, "False"))

      .def("openWell", &H5WellTops::openWell);
}

} // h5geopy
