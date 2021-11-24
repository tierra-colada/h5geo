#include "../../include/h5geopy/h5base_py.h"

namespace h5geopy {

void MapParam_py(py::class_<MapParam> &py_obj){
  py_obj
      .def(
        py::init<>())
      .def(
        py::init<
        double,
        double,
        double,
        double,
        double,
        double,
        size_t,
        size_t,
        Domain,
        std::string,
        std::string,
        hsize_t,
        hsize_t>(),
        py::arg("X0"),
        py::arg("Y0"),
        py::arg("X1"),
        py::arg("Y1"),
        py::arg("X2"),
        py::arg("Y2"),
        py::arg("nX"),
        py::arg("nY"),
        py::arg("domain"),
        py::arg("spatialUnits"),
        py::arg("dataUnits"),
        py::arg("xChunkSize"),
        py::arg("yChunkSize"))
      .def_readwrite("X0", &MapParam::X0)
      .def_readwrite("Y0", &MapParam::Y0)
      .def_readwrite("X1", &MapParam::X1)
      .def_readwrite("Y1", &MapParam::Y1)
      .def_readwrite("X2", &MapParam::X2)
      .def_readwrite("Y2", &MapParam::Y2)
      .def_readwrite("nX", &MapParam::nX)
      .def_readwrite("nY", &MapParam::nY)
      .def_readwrite("domain", &MapParam::domain)
      .def_readwrite("spatialUnits", &MapParam::spatialUnits)
      .def_readwrite("dataUnits", &MapParam::dataUnits)
      .def_readwrite("xChunkSize", &MapParam::xChunkSize)
      .def_readwrite("yChunkSize", &MapParam::yChunkSize);
}

void WellParam_py(py::class_<WellParam> &py_obj){
  py_obj
      .def(
        py::init<>())
      .def(
        py::init<
        double,
        double,
        double,
        std::string,
        std::string>(),
        py::arg("headX"),
        py::arg("headY"),
        py::arg_v("kb", 0, "0"),
        py::arg("uwi"),
        py::arg("spatialUnits"))
      .def_readwrite("headX", &WellParam::headX)
      .def_readwrite("headY", &WellParam::headY)
      .def_readwrite("kb", &WellParam::kb)
      .def_readwrite("uwi", &WellParam::uwi)
      .def_readwrite("spatialUnits", &WellParam::spatialUnits);
}

void DevCurveParam_py(py::class_<DevCurveParam> &py_obj){
  py_obj
      .def(
        py::init<>())
      .def(
        py::init<
        std::string,
        std::string,
        std::string,
        bool,
        hsize_t>(),
        py::arg("spatialUnits"), py::arg("temporalUnits"),
        py::arg("angularUnits"), py::arg_v("setActive", false),
        py::arg_v("chunkSize", 1000))
      .def_readwrite("spatialUnits", &DevCurveParam::spatialUnits)
      .def_readwrite("temporalUnits", &DevCurveParam::temporalUnits)
      .def_readwrite("angularUnits", &DevCurveParam::angularUnits)
      .def_readwrite("setActive", &DevCurveParam::setActive)
      .def_readwrite("chunkSize", &DevCurveParam::chunkSize);
}

void LogCurveParam_py(py::class_<LogCurveParam> &py_obj){
  py_obj
      .def(
        py::init<>())
      .def(
        py::init<
        std::string,
        std::string,
        hsize_t>(),
        py::arg("spatialUnits"),
        py::arg("dataUnits"),
        py::arg_v("chunkSize", 1000))
      .def_readwrite("spatialUnits", &LogCurveParam::spatialUnits)
      .def_readwrite("dataUnits", &LogCurveParam::dataUnits)
      .def_readwrite("chunkSize", &LogCurveParam::chunkSize);
}

void SeisParam_py(py::class_<SeisParam> &py_obj){
  py_obj
      .def(
        py::init<>())
      .def(
        py::init<
        Domain,
        SeisDataType,
        SurveyType,
        std::string,
        std::string,
        std::string,
        std::string,
        size_t,
        size_t,
        double,
        double,
        double,
        double,
        double,
        double,
        hsize_t,
        hsize_t>(),
        py::arg("domain"),
        py::arg("dataType"),
        py::arg("surveyType"),
        py::arg("spatialUnits"),
        py::arg("temporalUnits"),
        py::arg("angularUnits"),
        py::arg("dataUnits"),
        py::arg("nTrc"),
        py::arg("nSamp"),
        py::arg("X0"),
        py::arg("Y0"),
        py::arg("dX"),
        py::arg("dY"),
        py::arg_v("srd", 0, "0"),
        py::arg_v("orientation", 0, "0"),
        py::arg_v("trcChunk", 20000, "20000"),
        py::arg_v("stdChunk", 1000, "1000"))
      .def_readwrite("domain", &SeisParam::domain)
      .def_readwrite("spatialUnits", &SeisParam::spatialUnits)
      .def_readwrite("temporalUnits", &SeisParam::temporalUnits)
      .def_readwrite("temporalUnits", &SeisParam::angularUnits)
      .def_readwrite("dataUnits", &SeisParam::dataUnits)
      .def_readwrite("dataType", &SeisParam::dataType)
      .def_readwrite("surveyType", &SeisParam::surveyType)
      .def_readwrite("nTrc", &SeisParam::nTrc)
      .def_readwrite("nSamp", &SeisParam::nSamp)
      .def_readwrite("X0", &SeisParam::X0)
      .def_readwrite("Y0", &SeisParam::Y0)
      .def_readwrite("dX", &SeisParam::dX)
      .def_readwrite("dY", &SeisParam::dY)
      .def_readwrite("srd", &SeisParam::srd)
      .def_readwrite("trcChunk", &SeisParam::trcChunk)
      .def_readwrite("stdChunk", &SeisParam::stdChunk);
}

void H5Base_py(
    py::class_<
    H5BaseImpl,
    std::unique_ptr<H5BaseImpl, ObjectDeleter>,
    H5Base> &py_obj){
  py_obj
      .def("Delete", &H5BaseImpl::Delete)
      .def("getChildList", &H5BaseImpl::getChildList);
}

void ObjectDeleter_py(py::class_<ObjectDeleter> &py_obj){
  py_obj
      .def("__call__", [](const ObjectDeleter& obj, H5Base * base) { obj(base); });
}

} // h5geopy
