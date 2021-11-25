#include "../../include/h5geopy/h5baseobject_py.h"

#ifdef H5GEO_USE_GDAL
#include <gdal/gdal.h>
#include <gdal/gdal_priv.h>
#endif

namespace h5geopy {

void H5BaseObject_py(
    py::class_<
    H5BaseObjectImpl,
    std::unique_ptr<H5BaseObjectImpl, ObjectDeleter>,
    H5BaseImpl,
    H5BaseObject> &py_obj){
  py_obj

    #ifdef H5GEO_USE_GDAL
      .def("createCoordinateTransformationToReadData",
             &H5BaseObjectImpl::createCoordinateTransformationToReadData)
      .def("createCoordinateTransformationToWriteData",
             &H5BaseObjectImpl::createCoordinateTransformationToWriteData)
    #endif

      .def("setSpatialReference", py::overload_cast<const std::string&>(
             &H5BaseObjectImpl::setSpatialReference))
      .def("setSpatialReference", py::overload_cast<const std::string&, const std::string&>(
             &H5BaseObjectImpl::setSpatialReference))
      .def("setSpatialUnits", &H5BaseObjectImpl::setSpatialUnits)
      .def("setTemporalUnits", &H5BaseObjectImpl::setTemporalUnits)
      .def("setAngularUnits", &H5BaseObjectImpl::setAngularUnits)
      .def("setDataUnits", &H5BaseObjectImpl::setDataUnits)

      .def("getSpatialReference", &H5BaseObjectImpl::getSpatialReference)
      .def("getSpatialUnits", &H5BaseObjectImpl::getSpatialUnits)
      .def("getTemporalUnits", &H5BaseObjectImpl::getTemporalUnits)
      .def("getAngularUnits", &H5BaseObjectImpl::getAngularUnits)
      .def("getDataUnits", &H5BaseObjectImpl::getDataUnits)

      .def("getH5File", &H5BaseObjectImpl::getH5File)
      .def("getObjG", &H5BaseObjectImpl::getObjG)

      .def("getGroupOpt", &H5BaseObjectImpl::getGroupOpt)
      .def("getDatasetOpt", &H5BaseObjectImpl::getDatasetOpt)

      .def("getName", &H5BaseObjectImpl::getName)
      .def("getFullName", &H5BaseObjectImpl::getFullName);
//      .def(py::self == py::self)
//      .def(py::self != py::self);
}


//  py_obj
//      .def("getH5File", [](H5BaseObject* self) {
//    return dynamic_cast<H5BaseObjectImpl*>(self)->getH5File();});
//      .def("getObjG", &H5BaseObjectImpl::getObjG)
//      .def("getGroupOpt", &H5BaseObjectImpl::getGroupOpt)
//      .def("getDatasetOpt", &H5BaseObjectImpl::getDatasetOpt)
//      .def("getName", &H5BaseObjectImpl::getName)
//      .def("getFullName", &H5BaseObjectImpl::getFullName);
//      .def(py::self == py::self)
//      .def(py::self != py::self);

} // h5geo
