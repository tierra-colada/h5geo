#include "../../include/h5geopy/h5surf_py.h"

namespace h5geopy {

void H5Surf_py(
    py::class_<
    H5SurfImpl,
    std::unique_ptr<H5SurfImpl, ObjectDeleter>,
    H5BaseObjectImpl,
    H5Surf> &py_obj){
  py_obj
      .def("writeData", &H5SurfImpl::writeData,
           py::arg("data"),
           py::arg_v("dataUnits", "", "str()"))
      .def("getData", &H5SurfImpl::getData,
           py::arg_v("dataUnits", "", "str()"))

      .def("setDomain", &H5SurfImpl::setDomain)
      .def("setSpatialUnits", &H5SurfImpl::setSpatialUnits)
      .def("setDataUnits", &H5SurfImpl::setDataUnits)
      .def("setOrientation", &H5SurfImpl::setOrientation)
      .def("setOrigin", py::overload_cast<
           std::vector<double>&,
           const std::string&>(
             &H5SurfImpl::setOrigin),
           py::arg("xy"),
           py::arg_v("spatialUnits", "", "str()"))
      .def("setOrigin", py::overload_cast<
           Eigen::Ref<Eigen::Vector2d>,
           const std::string&>(
             &H5SurfImpl::setOrigin),
           py::arg("xy"),
           py::arg_v("spatialUnits", "", "str()"))
      .def("setSpacing", py::overload_cast<
           std::vector<double>&,
           const std::string&>(
             &H5SurfImpl::setSpacing),
           py::arg("xy"),
           py::arg_v("spatialUnits", "", "str()"))
      .def("setSpacing", py::overload_cast<
           Eigen::Ref<Eigen::Vector2d>,
           const std::string&>(
             &H5SurfImpl::setSpacing),
           py::arg("xy"),
           py::arg_v("spatialUnits", "", "str()"))

      .def("getDomain", &H5SurfImpl::getDomain)
      .def("getSpatialUnits", &H5SurfImpl::getSpatialUnits)
      .def("getDataUnits", &H5SurfImpl::getDataUnits)
      .def("getOrientation", &H5SurfImpl::getOrientation)
      .def("getOrigin", &H5SurfImpl::getOrigin,
           py::arg_v("spatialUnits", "", "str()"))
      .def("getSpacing", &H5SurfImpl::getSpacing,
           py::arg_v("spatialUnits", "", "str()"))

      .def("getSurfContainer", &H5SurfImpl::getSurfContainer)

      .def("getSurfD", &H5SurfImpl::getSurfD);
}


} // h5geopy
