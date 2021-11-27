#include "../../include/h5geopy/h5baseobject_py.h"

namespace h5geopy {

void H5BaseObject_py(
    py::class_<
    H5BaseObject,
    H5BaseObjectImpl,
    H5Base,
    std::unique_ptr<H5BaseObject, ObjectDeleter>>
    &py_obj){
  py_obj

    #ifdef H5GEO_USE_GDAL
      .def("createCoordinateTransformationToReadData",
             &H5BaseObject::createCoordinateTransformationToReadData)
      .def("createCoordinateTransformationToWriteData",
             &H5BaseObject::createCoordinateTransformationToWriteData)
    #endif

      .def("setSpatialReference", py::overload_cast<const std::string&>(
             &H5BaseObject::setSpatialReference))
      .def("setSpatialReference", py::overload_cast<const std::string&, const std::string&>(
             &H5BaseObject::setSpatialReference))
      .def("setSpatialUnits", &H5BaseObject::setSpatialUnits)
      .def("setTemporalUnits", &H5BaseObject::setTemporalUnits)
      .def("setAngularUnits", &H5BaseObject::setAngularUnits)
      .def("setDataUnits", &H5BaseObject::setDataUnits)

      .def("getSpatialReference", &H5BaseObject::getSpatialReference)
      .def("getSpatialUnits", &H5BaseObject::getSpatialUnits)
      .def("getTemporalUnits", &H5BaseObject::getTemporalUnits)
      .def("getAngularUnits", &H5BaseObject::getAngularUnits)
      .def("getDataUnits", &H5BaseObject::getDataUnits)

      .def("getH5File", &H5BaseObject::getH5File)
      .def("getObjG", &H5BaseObject::getObjG)

      .def("getGroupOpt", &H5BaseObject::getGroupOpt)
      .def("getDatasetOpt", &H5BaseObject::getDatasetOpt)

      .def("getName", &H5BaseObject::getName)
      .def("getFullName", &H5BaseObject::getFullName);
//      .def(py::self == py::self)
//      .def(py::self != py::self);
}


//  py_obj
//      .def("getH5File", [](H5BaseObject* self) {
//    return dynamic_cast<H5BaseObject*>(self)->getH5File();});
//      .def("getObjG", &H5BaseObject::getObjG)
//      .def("getGroupOpt", &H5BaseObject::getGroupOpt)
//      .def("getDatasetOpt", &H5BaseObject::getDatasetOpt)
//      .def("getName", &H5BaseObject::getName)
//      .def("getFullName", &H5BaseObject::getFullName);
//      .def(py::self == py::self)
//      .def(py::self != py::self);

} // h5geo
