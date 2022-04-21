#ifndef H5BASEOBJECT_PY_H
#define H5BASEOBJECT_PY_H

#include "h5geo_py.h"

#include <h5geo/impl/h5baseobjectimpl.h>

namespace h5geopy {

template <class TBase>
struct H5BaseObject_py
{
  H5BaseObject_py(
      py::class_<
      H5BaseObject,
      H5BaseObjectImpl<TBase>,
      H5Base,
      std::unique_ptr<H5BaseObject, ObjectDeleter>>
      &py_obj)
  {
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
        .def("setLengthUnits", &H5BaseObject::setLengthUnits)
        .def("setTemporalUnits", &H5BaseObject::setTemporalUnits)
        .def("setAngularUnits", &H5BaseObject::setAngularUnits)
        .def("setDataUnits", &H5BaseObject::setDataUnits)
        .def("setNullValue", &H5BaseObject::setNullValue)

        .def("getSpatialReference", &H5BaseObject::getSpatialReference)
        .def("getLengthUnits", &H5BaseObject::getLengthUnits)
        .def("getTemporalUnits", &H5BaseObject::getTemporalUnits)
        .def("getAngularUnits", &H5BaseObject::getAngularUnits)
        .def("getDataUnits", &H5BaseObject::getDataUnits)
        .def("getNullValue", &H5BaseObject::getNullValue)

        .def("getH5File", &H5BaseObject::getH5File)
        .def("getObjG", &H5BaseObject::getObjG)

        .def("getName", &H5BaseObject::getName)
        .def("getFullName", &H5BaseObject::getFullName)

        .def("getObjGroupList", &H5BaseObject::getObjGroupList)
        .def("getObjNameList", &H5BaseObject::getObjNameList)
        .def("getObjCount", &H5BaseObject::getObjCount)

        .def("isEqual", &H5BaseObject::isEqual)

        // operators == for abstract classes https://github.com/pybind/pybind11/issues/1487
        .def("__eq__", [](const H5BaseObject &self, const H5BaseObject &other){ return self == other; })
        .def("__ne__", [](const H5BaseObject &self, const H5BaseObject &other){ return self != other; });
  }
};


} // h5geopy


#endif // H5BASEOBJECT_PY_H
