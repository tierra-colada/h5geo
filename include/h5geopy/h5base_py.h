#ifndef H5BASE_PY_H
#define H5BASE_PY_H

#include "h5geo_py.h"

#include <h5geo/misc/h5baseimpl.h>

namespace h5geopy {

void MapParam_py(py::class_<MapParam> &py_obj);
void WellParam_py(py::class_<WellParam> &py_obj);
void DevCurveParam_py(py::class_<DevCurveParam> &py_obj);
void LogCurveParam_py(py::class_<LogCurveParam> &py_obj);
void SeisParam_py(py::class_<SeisParam> &py_obj);

void H5Base_py(py::class_<
               H5BaseImpl,
               std::unique_ptr<H5BaseImpl, ObjectDeleter>,
               H5Base> &py_obj);

void ObjectDeleter_py(py::class_<ObjectDeleter> &py_obj);

} // h5geopy


#endif // H5BASE_PY_H
