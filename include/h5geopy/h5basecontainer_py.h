#ifndef H5BASECONTAINER_PY_H
#define H5BASECONTAINER_PY_H

#include "h5geo_py.h"

#include <h5geo/misc/h5basecontainerimpl.h>

namespace h5geopy {

template <class TBase>
struct H5BaseContainer_py
{
  H5BaseContainer_py(
      py::class_<
      H5BaseContainer,
      H5BaseContainerImpl<TBase>,
      H5Base,
      std::unique_ptr<H5BaseContainer, ObjectDeleter>>
      &py_obj)
  {
    py_obj
        .def("getH5File", &H5BaseContainer::getH5File);
  }
};

} // h5geopy


#endif // H5BASECONTAINER_PY_H
