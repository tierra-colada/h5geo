#include "../../include/h5geopy/h5map_py.h"

namespace h5geopy {

void H5Map_py(
    py::class_<
    H5Map,
    H5MapImpl,
    H5BaseObject,
    H5Base,
    std::unique_ptr<H5Map, ObjectDeleter>>
    &py_obj){
  py_obj
      .def("writeData", &H5Map::writeData,
           py::arg("data"),
           py::arg_v("dataUnits", "", "str()"))
      .def("getData", &H5Map::getData,
           py::arg_v("dataUnits", "", "str()"))

      .def("setDomain", &H5Map::setDomain)
      .def("setOrigin", &H5Map::setOrigin,
           py::arg("xy"),
           py::arg_v("spatialUnits", "", "str()"),
           py::arg_v("doCoordTransform", false, "False"))
      .def("setPoint1", &H5Map::setPoint1,
           py::arg("xy"),
           py::arg_v("spatialUnits", "", "str()"),
           py::arg_v("doCoordTransform", false, "False"))
      .def("setPoint2", &H5Map::setPoint2,
           py::arg("xy"),
           py::arg_v("spatialUnits", "", "str()"),
           py::arg_v("doCoordTransform", false, "False"))

      .def("addAttribute", &H5Map::addAttribute,
           py::arg("map"),
           py::arg_v("name", "", "str()"))
      .def("addExternalAttribute", &H5Map::addExternalAttribute,
           py::arg("map"),
           py::arg_v("name", "", "str()"))
      .def("removeAttribute", &H5Map::removeAttribute,
           py::arg("name"))
      .def("getAttribute", &H5Map::getAttribute,
           py::arg("name"))

      .def("getDomain", &H5Map::getDomain)
      .def("getOrigin", &H5Map::getOrigin,
           py::arg_v("spatialUnits", "", "str()"),
           py::arg_v("doCoordTransform", false, "False"))
      .def("getPoint1", &H5Map::getPoint1,
           py::arg_v("spatialUnits", "", "str()"),
           py::arg_v("doCoordTransform", false, "False"))
      .def("getPoint2", &H5Map::getPoint2,
           py::arg_v("spatialUnits", "", "str()"),
           py::arg_v("doCoordTransform", false, "False"))

      .def("getMapContainer", &H5Map::getMapContainer)

      .def("getMapD", &H5Map::getMapD);
}


} // h5geopy
