#include "../../include/h5geopy/h5base_py.h"

namespace h5geopy {

void BaseObjectParam_py(py::class_<BaseObjectParam> &py_obj){
  py_obj
      .def(
        py::init<>())
      .def_readwrite("spatialReference", &MapParam::spatialReference)
      .def_readwrite("lengthUnits", &MapParam::lengthUnits)
      .def_readwrite("temporalUnits", &SeisParam::temporalUnits)
      .def_readwrite("angularUnits", &SeisParam::angularUnits)
      .def_readwrite("dataUnits", &SeisParam::dataUnits);
}

void PointsParam_py(py::class_<PointsParam, BaseObjectParam> &py_obj){
  py_obj
      .def(
        py::init<>())
      .def_readwrite("domain", &PointsParam::domain)
      .def_readwrite("nPoints", &PointsParam::nPoints)
      .def_readwrite("chunkSize", &PointsParam::chunkSize);
}

void WellTopsParam_py(py::class_<WellTopsParam, PointsParam, BaseObjectParam> &py_obj){
  py_obj
      .def(
        py::init<>());
}

void MapParam_py(py::class_<MapParam, BaseObjectParam> &py_obj){
  py_obj
      .def(
        py::init<>())
      .def_readwrite("X0", &MapParam::X0)
      .def_readwrite("Y0", &MapParam::Y0)
      .def_readwrite("X1", &MapParam::X1)
      .def_readwrite("Y1", &MapParam::Y1)
      .def_readwrite("X2", &MapParam::X2)
      .def_readwrite("Y2", &MapParam::Y2)
      .def_readwrite("nX", &MapParam::nX)
      .def_readwrite("nY", &MapParam::nY)
      .def_readwrite("domain", &MapParam::domain)
      .def_readwrite("xChunkSize", &MapParam::xChunkSize)
      .def_readwrite("yChunkSize", &MapParam::yChunkSize);
}

void WellParam_py(py::class_<WellParam, BaseObjectParam> &py_obj){
  py_obj
      .def(
        py::init<>())
      .def_readwrite("headX", &WellParam::headX)
      .def_readwrite("headY", &WellParam::headY)
      .def_readwrite("kb", &WellParam::kb)
      .def_readwrite("uwi", &WellParam::uwi);
}

void DevCurveParam_py(py::class_<DevCurveParam, BaseObjectParam> &py_obj){
  py_obj
      .def(
        py::init<>())
      .def_readwrite("chunkSize", &DevCurveParam::chunkSize);
}

void LogCurveParam_py(py::class_<LogCurveParam, BaseObjectParam> &py_obj){
  py_obj
      .def(
        py::init<>())
      .def_readwrite("chunkSize", &LogCurveParam::chunkSize);
}

void SeisParam_py(py::class_<SeisParam, BaseObjectParam> &py_obj){
  py_obj
      .def(
        py::init<>())
      .def_readwrite("domain", &SeisParam::domain)
      .def_readwrite("dataType", &SeisParam::dataType)
      .def_readwrite("surveyType", &SeisParam::surveyType)
      .def_readwrite("nTrc", &SeisParam::nTrc)
      .def_readwrite("nSamp", &SeisParam::nSamp)
      .def_readwrite("srd", &SeisParam::srd)
      .def_readwrite("trcChunk", &SeisParam::trcChunk)
      .def_readwrite("stdChunk", &SeisParam::stdChunk)
      .def_readwrite("mapSEGY", &SeisParam::mapSEGY)
      .def_readwrite("segyFiles", &SeisParam::segyFiles);
}

void ObjectDeleter_py(py::class_<ObjectDeleter> &py_obj){
  py_obj
      .def("__call__", [](const ObjectDeleter& obj, H5Base * base) { obj(base); });
}

} // h5geopy
