#ifndef H5BASEPOINTS_PY_H
#define H5BASEPOINTS_PY_H

#include "h5geo_py.h"

#include <h5geo/impl/h5basepointsimpl.h>

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

  }
};

} // h5geopy

#endif // H5BASEPOINTS_PY_H
