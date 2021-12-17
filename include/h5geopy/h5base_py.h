#ifndef H5BASE_PY_H
#define H5BASE_PY_H

#include "h5geo_py.h"

#include <h5geo/misc/h5baseimpl.h>

namespace h5geopy {

void BaseObjectParam_py(py::class_<BaseObjectParam> &py_obj);
void PointsParam_py(py::class_<PointsParam, BaseObjectParam> &py_obj);
void MapParam_py(py::class_<MapParam, BaseObjectParam> &py_obj);
void WellParam_py(py::class_<WellParam, BaseObjectParam> &py_obj);
void DevCurveParam_py(py::class_<DevCurveParam, BaseObjectParam> &py_obj);
void LogCurveParam_py(py::class_<LogCurveParam, BaseObjectParam> &py_obj);
void SeisParam_py(py::class_<SeisParam, BaseObjectParam> &py_obj);

template <class TBase>
struct H5Base_py
{
  H5Base_py(
      py::class_<
      H5Base,
      H5BaseImpl<TBase>,
      std::unique_ptr<H5Base, ObjectDeleter>>
      &py_obj)
  {
    py_obj
        .def("Delete", &H5Base::Delete)
        .def("getChildList", &H5Base::getChildList);
  }
};

void ObjectDeleter_py(py::class_<ObjectDeleter> &py_obj);

} // h5geopy


#endif // H5BASE_PY_H
