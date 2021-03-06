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
      #ifdef H5GEO_USE_GDAL
      .def("readRasterCoordinates", &H5Map::readRasterCoordinates,
           py::arg("file"),
           py::arg_v("lengthUnits", "", "str()"),
           "`h5geopy.GDALAllRegister()` must be called before using GDAL readers")
      .def("readRasterSpatialReference", &H5Map::readRasterSpatialReference,
           py::arg("file"),
           "`h5geopy.GDALAllRegister()` must be called before using GDAL readers")
      .def("readRasterLengthUnits", &H5Map::readRasterLengthUnits,
           py::arg("file"),
           "`h5geopy.GDALAllRegister()` must be called before using GDAL readers")
      .def("readRasterCoordinates", &H5Map::readRasterCoordinates,
           py::arg("file"),
           py::arg_v("dataUnits", "", "str()"),
           "`h5geopy.GDALAllRegister()` must be called before using GDAL readers")
      #endif  // H5GEO_USE_GDAL

      .def("writeData", &H5Map::writeData,
           py::arg("data"),
           py::arg_v("dataUnits", "", "str()"))
      .def("getData", &H5Map::getData,
           py::arg_v("dataUnits", "", "str()"))

      .def("setDomain", &H5Map::setDomain)
      .def("setOrigin", &H5Map::setOrigin,
           py::arg("xy"),
           py::arg_v("lengthUnits", "", "str()"),
           py::arg_v("doCoordTransform", false, "False"))
      .def("setPoint1", &H5Map::setPoint1,
           py::arg("xy"),
           py::arg_v("lengthUnits", "", "str()"),
           py::arg_v("doCoordTransform", false, "False"))
      .def("setPoint2", &H5Map::setPoint2,
           py::arg("xy"),
           py::arg_v("lengthUnits", "", "str()"),
           py::arg_v("doCoordTransform", false, "False"))

      .def("addAttributeMap", &H5Map::addAttributeMap,
           py::arg("map"),
           py::arg_v("name", "", "str()"))
      .def("addInternalAttributeMap", &H5Map::addInternalAttributeMap,
           py::arg("map"),
           py::arg_v("name", "", "str()"))
      .def("addExternalAttributeMap", &H5Map::addExternalAttributeMap,
           py::arg("map"),
           py::arg_v("name", "", "str()"))
      .def("removeAttributeMap", &H5Map::removeAttributeMap,
           py::arg("name"))
      .def("openAttributeMap", &H5Map::openAttributeMap,
           py::arg("name"))
      .def("getAttributeMapGroupList", &H5Map::getAttributeMapGroupList)
      .def("getAttributeMapNameList", &H5Map::getAttributeMapNameList)
      .def("getAttributeMapCount", &H5Map::getAttributeMapCount)

      .def("getDomain", &H5Map::getDomain)
      .def("getOrigin", &H5Map::getOrigin,
           py::arg_v("lengthUnits", "", "str()"),
           py::arg_v("doCoordTransform", false, "False"))
      .def("getPoint1", &H5Map::getPoint1,
           py::arg_v("lengthUnits", "", "str()"),
           py::arg_v("doCoordTransform", false, "False"))
      .def("getPoint2", &H5Map::getPoint2,
           py::arg_v("lengthUnits", "", "str()"),
           py::arg_v("doCoordTransform", false, "False"))
      .def("getNX", &H5Map::getNX)
      .def("getNY", &H5Map::getNY)
      .def("getParam", &H5Map::getParam)

      .def("openMapContainer", &H5Map::openMapContainer)

      .def("getMapD", &H5Map::getMapD);
}


} // h5geopy
