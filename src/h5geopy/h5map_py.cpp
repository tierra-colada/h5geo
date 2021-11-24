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
      .def("setSpatialUnits", &H5MapImpl::setSpatialUnits)
      .def("setDataUnits", &H5MapImpl::setDataUnits)
      .def("setOrigin", py::overload_cast<
           std::vector<double>&,
           const std::string&>(
             &H5MapImpl::setOrigin),
           py::arg("xy"),
           py::arg_v("spatialUnits", "", "str()"))
      .def("setOrigin", py::overload_cast<
           Eigen::Ref<Eigen::Vector2d>,
           const std::string&>(
             &H5MapImpl::setOrigin),
           py::arg("xy"),
           py::arg_v("spatialUnits", "", "str()"))
      .def("setPoint1", py::overload_cast<
           std::vector<double>&,
           const std::string&>(
             &H5MapImpl::setPoint1),
           py::arg("xy"),
           py::arg_v("spatialUnits", "", "str()"))
      .def("setPoint1", py::overload_cast<
           Eigen::Ref<Eigen::Vector2d>,
           const std::string&>(
             &H5MapImpl::setPoint1),
           py::arg("xy"),
           py::arg_v("spatialUnits", "", "str()"))
      .def("setPoint2", py::overload_cast<
           std::vector<double>&,
           const std::string&>(
             &H5MapImpl::setPoint2),
           py::arg("xy"),
           py::arg_v("spatialUnits", "", "str()"))
      .def("setPoint2", py::overload_cast<
           Eigen::Ref<Eigen::Vector2d>,
           const std::string&>(
             &H5MapImpl::setPoint2),
           py::arg("xy"),
           py::arg_v("spatialUnits", "", "str()"))

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
      .def("getSpatialUnits", &H5MapImpl::getSpatialUnits)
      .def("getDataUnits", &H5MapImpl::getDataUnits)
      .def("getOrigin", &H5MapImpl::getOrigin,
           py::arg_v("spatialUnits", "", "str()"))
      .def("getPoint1", &H5MapImpl::getPoint1,
           py::arg_v("spatialUnits", "", "str()"))
      .def("getPoint2", &H5MapImpl::getPoint2,
           py::arg_v("spatialUnits", "", "str()"))

      .def("getMapContainer", &H5MapImpl::getMapContainer)

      .def("getMapD", &H5MapImpl::getMapD);
}


} // h5geopy
