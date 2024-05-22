#include "../../include/h5geopy/h5vol_py.h"

namespace h5geopy {

void H5Vol_py(
    py::class_<
    H5Vol,
    H5VolImpl,
    H5BaseObject,
    H5Base,
    std::unique_ptr<H5Vol, ObjectDeleter>>
    &py_obj){
  py_obj
      // WRITERS
      .def("writeData", &H5Vol::writeData,
           py::arg("data"),
           py::arg("iX0"),
           py::arg("iY0"),
           py::arg("iZ0"),
           py::arg("nX"),
           py::arg("nY"),
           py::arg("nZ"),
           py::arg_v("dataUnits", "", "str()"))

      // READER
      .def("readSEGYSTACK", &H5Vol::readSEGYSTACK,
           py::arg("segy"),
           py::arg("ilHdrOffset"),
           py::arg("ilHdrSize"),
           py::arg("xlHdrOffset"),
           py::arg("xlHdrSize"),
           py::arg("xHdrOffset"),
           py::arg("xHdrSize"),
           py::arg("yHdrOffset"),
           py::arg("yHdrSize"),
           py::arg("sampRate"),
           py::arg_v("nSamp", 0, "0"),
           py::arg_v("nTrc", 0, "0"),
           py::arg_v("format", static_cast<h5geo::SegyFormat>(0), "_h5geo.SegyFormat(0)"),
           py::arg_v("endian", static_cast<h5geo::Endian>(0), "_h5geo.Endian(0)"),
           py::arg_v("progressCallback", nullptr, "None"))

      // SETTERS
      .def("setDomain", &H5Vol::setDomain)
      .def("setOrigin", &H5Vol::setOrigin,
           py::arg("v"),
           py::arg_v("lengthUnits", "", "str()"),
           py::arg_v("temporalUnits", "", "str()"),
           py::arg_v("doCoordTransform", false, "False"))
      .def("setSpacings", &H5Vol::setSpacings,
           py::arg("v"),
           py::arg_v("lengthUnits", "", "str()"),
           py::arg_v("temporalUnits", "", "str()"))
      .def("setOrientation", &H5Vol::setOrientation,
           py::arg("v"),
           py::arg_v("temporalUnits", "", "str()"))
      .def("resize", &H5Vol::resize,
           py::arg("nx"),
           py::arg("ny"),
           py::arg("nz"))

      // GETTERS
      .def("getData", &H5Vol::getData,
           py::arg("iX0"),
           py::arg("iY0"),
           py::arg("iZ0"),
           py::arg("nX"),
           py::arg("nY"),
           py::arg("nZ"),
           py::arg_v("dataUnits", "", "str()"))
      .def("getDomain", &H5Vol::getDomain)
      .def("getOrigin", &H5Vol::getOrigin,
           py::arg_v("lengthUnits", "", "str()"),
           py::arg_v("temporalUnits", "", "str()"),
           py::arg_v("doCoordTransform", false, "False"))
      .def("getSpacings", &H5Vol::getSpacings,
           py::arg_v("lengthUnits", "", "str()"),
           py::arg_v("temporalUnits", "", "str()"))
      .def("getOrientation", &H5Vol::getOrientation,
           py::arg_v("angularUnits", "", "str()"))
      .def("getNX", &H5Vol::getNX)
      .def("getNY", &H5Vol::getNY)
      .def("getNZ", &H5Vol::getNZ)
      .def("getParam", &H5Vol::getParam)
      .def("openVolContainer", &H5Vol::openVolContainer)
      .def("getVolD", &H5Vol::getVolD)

      .def("exportToSEGY", &H5Vol::exportToSEGY,
           py::arg("segyFile"),
           py::arg_v("endian", h5geo::Endian::Big, "_h5geo.Endian.Big"),
           py::arg_v("progressCallback", nullptr, "None"),
           "export geo volume to SEGY")

      .def("recreateVolD", &H5Vol::recreateVolD,
           py::arg("nX"), py::arg("nY"), py::arg("nZ"),
           py::arg("xChunk"), py::arg("yChunk"), py::arg("zChunk"),
           py::arg("compressionLevel"),
           "Unlink and create new dataset");
}


} // h5geopy
