#include "../../include/h5geopy/h5base_py.h"

namespace h5geopy {

void SurfParam_py(py::class_<SurfParam> &py_obj){
  py_obj
      .def(
        py::init<>())
      .def(
        py::init<
        size_t,
        size_t,
        double,
        double,
        double,
        double,
        Domain,
        std::string,
        std::string,
        std::string>(),
        py::arg("nX"), py::arg("nY"),
        py::arg("X0"), py::arg("Y0"),
        py::arg("dX"), py::arg("dY"),
        py::arg("domain"), py::arg("spatialUnits"), py::arg("temporalUnits"),
        py::arg("dataUnits"))
      .def_readwrite("nX", &SurfParam::nX)
      .def_readwrite("nY", &SurfParam::nY)
      .def_readwrite("X0", &SurfParam::X0)
      .def_readwrite("Y0", &SurfParam::Y0)
      .def_readwrite("dX", &SurfParam::dX)
      .def_readwrite("dY", &SurfParam::dY)
      .def_readwrite("domain", &SurfParam::domain)
      .def_readwrite("spatialUnits", &SurfParam::spatialUnits)
      .def_readwrite("temporalUnits", &SurfParam::temporalUnits)
      .def_readwrite("dataUnits", &SurfParam::dataUnits);
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
        py::arg("headX"), py::arg("headY"),
        py::arg("kb"), py::arg("uwi"),
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
        py::arg("angleUnits"), py::arg_v("setActive", false),
        py::arg_v("chunkSize", 1000))
      .def_readwrite("spatialUnits", &DevCurveParam::spatialUnits)
      .def_readwrite("temporalUnits", &DevCurveParam::temporalUnits)
      .def_readwrite("angleUnits", &DevCurveParam::angleUnits)
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
        size_t,
        size_t,
        double,
        hsize_t,
        hsize_t>(),
        py::arg("domain"),
        py::arg("dataType"),
        py::arg("surveyType"),
        py::arg("spatialUnits"),
        py::arg("temporalUnits"),
        py::arg("dataUnits"),
        py::arg("nTrc"),
        py::arg("nSamp"),
        py::arg_v("srd", 0),
        py::arg_v("trcChunk", 20000),
        py::arg_v("stdChunk", 1000))
      .def_readwrite("domain", &SeisParam::domain)
      .def_readwrite("spatialUnits", &SeisParam::spatialUnits)
      .def_readwrite("temporalUnits", &SeisParam::temporalUnits)
      .def_readwrite("dataUnits", &SeisParam::dataUnits)
      .def_readwrite("dataType", &SeisParam::dataType)
      .def_readwrite("surveyType", &SeisParam::surveyType)
      .def_readwrite("nTrc", &SeisParam::nTrc)
      .def_readwrite("nSamp", &SeisParam::nSamp)
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
