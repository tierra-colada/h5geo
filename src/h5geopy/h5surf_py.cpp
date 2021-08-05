#include "../../include/h5geopy/h5surf_py.h"

namespace h5geopy {

void H5Surf_py(
    py::class_<
    H5SurfImpl,
    std::unique_ptr<H5SurfImpl, ObjectDeleter>,
    H5BaseObjectImpl,
    H5Surf> &py_obj){
  py_obj
      .def("writeData", &H5SurfImpl::writeData)
      .def("getData", &H5SurfImpl::getData,
           py::arg_v("dataUnits", "", "str()"))

      .def("setDomain", &H5SurfImpl::setDomain)
      .def("setSpatialUnits", &H5SurfImpl::setSpatialUnits)
      .def("setDataUnits", &H5SurfImpl::setDataUnits)
      .def("setOrigin", py::overload_cast<const std::vector<double>&>(&H5SurfImpl::setOrigin))
      .def("setOrigin", py::overload_cast<const Eigen::Ref<const Eigen::Vector2d>&>(&H5SurfImpl::setOrigin))
      .def("setSpacing", py::overload_cast<const std::vector<double>&>(&H5SurfImpl::setSpacing))
      .def("setSpacing", py::overload_cast<const Eigen::Ref<const Eigen::Vector2d>&>(&H5SurfImpl::setSpacing))

      .def("getDomain", &H5SurfImpl::getDomain)
      .def("getSpatialUnits", &H5SurfImpl::getSpatialUnits)
      .def("getDataUnits", &H5SurfImpl::getDataUnits)
      .def("getOrigin", &H5SurfImpl::getOrigin,
           py::arg_v("spatialUnits", "", "str()"))
      .def("getSpacing", &H5SurfImpl::getSpacing,
           py::arg_v("spatialUnits", "", "str()"))

      .def("getSurfContainer", &H5SurfImpl::getSurfContainer)
      .def("getSurfD", &H5SurfImpl::getSurfD);
}


} // h5geopy
