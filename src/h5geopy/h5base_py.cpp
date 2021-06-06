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
        SpatialUnits,
        TemporalUnits,
        std::string>())
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

//void WellParam_py(py::class_<WellParam> &py_obj);
//void DevCurveParam_py(py::class_<DevCurveParam> &py_obj);
//void LogCurveParam_py(py::class_<LogCurveParam> &py_obj);
//void SeisParam_py(py::class_<SeisParam> &py_obj);

void H5Base_py(
    py::class_<H5Base,
    std::unique_ptr<H5Base, py::nodelete>> &py_obj){
  py_obj
      .def("Delete", &H5Base::Delete)
      .def("getChildList", &H5Base::getChildList);

}

//void ObjectDeleter_py(py::class_<ObjectDeleter> &py_obj){

//}

} // h5geopy
