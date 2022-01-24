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
        .def("openPoints", py::overload_cast<
             const std::string&>(&H5BaseContainer::openPoints))
        .def("openPoints", py::overload_cast<
             h5gt::Group>(&H5BaseContainer::openPoints))
        .def("createPoints", py::overload_cast<
             std::string&,
             PointsParam&,
             h5geo::CreationType>(&H5BaseContainer::createPoints))
        .def("createPoints", py::overload_cast<
             h5gt::Group,
             PointsParam&,
             h5geo::CreationType>(&H5BaseContainer::createPoints))
        .def("getH5File", &H5BaseContainer::getH5File)
        .def("getObjGroupList", &H5BaseContainer::getObjGroupList)
        .def("getObjNameList", &H5BaseContainer::getObjNameList);
  }
};

} // h5geopy


#endif // H5BASECONTAINER_PY_H
