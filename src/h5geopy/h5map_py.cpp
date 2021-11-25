#include "../../include/h5geopy/h5map_py.h"

namespace h5geopy {

void H5Map_py(
    py::class_<
    H5MapImpl,
    std::unique_ptr<H5MapImpl, ObjectDeleter>,
    H5BaseObjectImpl,
    H5Map> &py_obj){
  py_obj
      .def("writeData", &H5MapImpl::writeData,
           py::arg("data"),
           py::arg_v("dataUnits", "", "str()"))
      .def("getData", &H5MapImpl::getData,
           py::arg_v("dataUnits", "", "str()"))

      .def("setDomain", &H5MapImpl::setDomain)
      .def("setOrigin", &H5MapImpl::setOrigin,
           py::arg("xy"),
           py::arg_v("spatialUnits", "", "str()"),
           py::arg_v("doCoordTransform", false, "False"))
      .def("setPoint1", &H5MapImpl::setPoint1,
           py::arg("xy"),
           py::arg_v("spatialUnits", "", "str()"),
           py::arg_v("doCoordTransform", false, "False"))
      .def("setPoint2", &H5MapImpl::setPoint2,
           py::arg("xy"),
           py::arg_v("spatialUnits", "", "str()"),
           py::arg_v("doCoordTransform", false, "False"))

      .def("addAttribute", &H5MapImpl::addAttribute,
           py::arg("map"),
           py::arg_v("name", "", "str()"))
      .def("addExternalAttribute", &H5MapImpl::addExternalAttribute,
           py::arg("map"),
           py::arg_v("name", "", "str()"))
      .def("removeAttribute", &H5MapImpl::removeAttribute,
           py::arg("name"))
      .def("getAttribute", &H5MapImpl::getAttribute,
           py::arg("name"))

      .def("getDomain", &H5MapImpl::getDomain)
      .def("getOrigin", &H5MapImpl::getOrigin,
           py::arg_v("spatialUnits", "", "str()"),
           py::arg_v("doCoordTransform", false, "False"))
      .def("getPoint1", &H5MapImpl::getPoint1,
           py::arg_v("spatialUnits", "", "str()"),
           py::arg_v("doCoordTransform", false, "False"))
      .def("getPoint2", &H5MapImpl::getPoint2,
           py::arg_v("spatialUnits", "", "str()"),
           py::arg_v("doCoordTransform", false, "False"))

      .def("getMapContainer", &H5MapImpl::getMapContainer)

      .def("getMapD", &H5MapImpl::getMapD);
}


} // h5geopy
