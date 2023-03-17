#include "../../include/h5geopy/h5base_py.h"

namespace h5geopy {

void BaseObjectParam_py(py::class_<H5BaseObjectParam> &py_obj){
  py_obj
      .def(py::init<>())
      .def_readwrite("spatialReference", &H5MapParam::spatialReference)
      .def_readwrite("lengthUnits", &H5MapParam::lengthUnits)
      .def_readwrite("temporalUnits", &H5SeisParam::temporalUnits)
      .def_readwrite("angularUnits", &H5SeisParam::angularUnits)
      .def_readwrite("dataUnits", &H5SeisParam::dataUnits)
      .def_readwrite("nullValue", &H5SeisParam::nullValue);
}

void PointsParam_py(py::class_<H5PointsParam, H5BaseObjectParam> &py_obj){
  py_obj
      .def(py::init<>())
      .def_readwrite("domain", &H5PointsParam::domain)
      .def_readwrite("nPoints", &H5PointsParam::nPoints)
      .def_readwrite("chunkSize", &H5PointsParam::chunkSize);
}

void WellTopsParam_py(py::class_<H5WellTopsParam, H5PointsParam, H5BaseObjectParam> &py_obj){
  py_obj
      .def(py::init<>());
}

void HorizonParam_py(py::class_<H5HorizonParam, H5BaseObjectParam> &py_obj){
  py_obj
      .def(py::init<>())
      .def_readwrite("domain", &H5HorizonParam::domain)
      .def_readwrite("nPoints", &H5HorizonParam::nPoints)
      .def_readwrite("components", &H5HorizonParam::components)
      .def_readwrite("chunkSize", &H5HorizonParam::pointsChunkSize);
}

void MapParam_py(py::class_<H5MapParam, H5BaseObjectParam> &py_obj){
  py_obj
      .def(py::init<>())
      .def_readwrite("X0", &H5MapParam::X0)
      .def_readwrite("Y0", &H5MapParam::Y0)
      .def_readwrite("X1", &H5MapParam::X1)
      .def_readwrite("Y1", &H5MapParam::Y1)
      .def_readwrite("X2", &H5MapParam::X2)
      .def_readwrite("Y2", &H5MapParam::Y2)
      .def_readwrite("nX", &H5MapParam::nX)
      .def_readwrite("nY", &H5MapParam::nY)
      .def_readwrite("domain", &H5MapParam::domain)
      .def_readwrite("xChunkSize", &H5MapParam::xChunkSize)
      .def_readwrite("yChunkSize", &H5MapParam::yChunkSize);
}

void WellParam_py(py::class_<H5WellParam, H5BaseObjectParam> &py_obj){
  py_obj
      .def(py::init<>())
      .def_readwrite("headX", &H5WellParam::headX)
      .def_readwrite("headY", &H5WellParam::headY)
      .def_readwrite("kb", &H5WellParam::kb)
      .def_readwrite("uwi", &H5WellParam::uwi);
}

void DevCurveParam_py(py::class_<H5DevCurveParam, H5BaseObjectParam> &py_obj){
  py_obj
      .def(py::init<>())
      .def_readwrite("chunkSize", &H5DevCurveParam::chunkSize);
}

void LogCurveParam_py(py::class_<H5LogCurveParam, H5BaseObjectParam> &py_obj){
  py_obj
      .def(py::init<>())
      .def_readwrite("chunkSize", &H5LogCurveParam::chunkSize);
}

void SeisParam_py(py::class_<H5SeisParam, H5BaseObjectParam> &py_obj){
  py_obj
      .def(py::init<>())
      .def_readwrite("domain", &H5SeisParam::domain)
      .def_readwrite("dataType", &H5SeisParam::dataType)
      .def_readwrite("surveyType", &H5SeisParam::surveyType)
      .def_readwrite("nTrc", &H5SeisParam::nTrc)
      .def_readwrite("nSamp", &H5SeisParam::nSamp)
      .def_readwrite("srd", &H5SeisParam::srd)
      .def_readwrite("trcChunk", &H5SeisParam::trcChunk)
      .def_readwrite("stdChunk", &H5SeisParam::stdChunk)
      .def_readwrite("mapSEGY", &H5SeisParam::mapSEGY)
      .def_readwrite("segyFiles", &H5SeisParam::segyFiles);
}

void VolParam_py(py::class_<H5VolParam, H5BaseObjectParam> &py_obj){
  py_obj
      .def(py::init<>())
      .def_readwrite("X0", &H5VolParam::X0)
      .def_readwrite("Y0", &H5VolParam::Y0)
      .def_readwrite("Z0", &H5VolParam::Z0)
      .def_readwrite("dX", &H5VolParam::dX)
      .def_readwrite("dY", &H5VolParam::dY)
      .def_readwrite("dZ", &H5VolParam::dZ)
      .def_readwrite("nX", &H5VolParam::nX)
      .def_readwrite("nY", &H5VolParam::nY)
      .def_readwrite("nZ", &H5VolParam::nZ)
      .def_readwrite("orientation", &H5VolParam::orientation)
      .def_readwrite("domain", &H5VolParam::domain)
      .def_readwrite("xChunkSize", &H5VolParam::xChunkSize)
      .def_readwrite("yChunkSize", &H5VolParam::yChunkSize)
      .def_readwrite("zChunkSize", &H5VolParam::zChunkSize)
      .def_readwrite("compression_level", &H5VolParam::compression_level);
}

void ObjectDeleter_py(py::class_<ObjectDeleter> &py_obj){
  py_obj
      .def("__call__", [](const ObjectDeleter& obj, H5Base * base) { obj(base); });
}

} // h5geopy
