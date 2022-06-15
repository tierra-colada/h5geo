#ifndef H5BASEPOINTS_PY_H
#define H5BASEPOINTS_PY_H

#include "h5geo_py.h"

#include <h5geo/private/h5basepointsimpl.h>

namespace h5geopy {

template <class TBase>
struct H5BasePoints_py
{
  H5BasePoints_py(
      py::class_<
      H5BasePoints,
      H5BasePointsImpl<TBase>,
      H5BaseObject,
      H5Base,
      std::unique_ptr<H5BasePoints, ObjectDeleter>>
      &py_obj)
  {
    py_obj
        .def("setNPoints", &H5BasePoints::setNPoints)
        .def("setDomain", &H5BasePoints::setDomain)

        .def("openContainer", &H5BasePoints::openContainer)
        .def("getNPoints", &H5BasePoints::getNPoints)
        .def("getDomain", &H5BasePoints::getDomain)
        .def("getPointsD", &H5BasePoints::getPointsD)
        .def("getParam", &H5BasePoints::getParam);
  }
};

} // h5geopy

#endif // H5BASEPOINTS_PY_H
