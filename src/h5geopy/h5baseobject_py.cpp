#include "../../include/h5geopy/h5baseobject_py.h"

#ifdef H5GEO_USE_GDAL
#include <gdal/gdal.h>
#include <gdal/gdal_priv.h>
#endif

namespace h5geopy {

//template <typename TBase>
//void H5BaseObject_py(
//    py::class_<
//    H5BaseObject,
//    H5BaseObjectImpl<TBase>,
//    std::unique_ptr<H5BaseObject, ObjectDeleter>>
//    &py_obj){

//}


//  py_obj
//      .def("getH5File", [](H5BaseObject* self) {
//    return dynamic_cast<H5BaseObjectImpl*>(self)->getH5File();});
//      .def("getObjG", &H5BaseObjectImpl<TBase>::getObjG)
//      .def("getGroupOpt", &H5BaseObjectImpl<TBase>::getGroupOpt)
//      .def("getDatasetOpt", &H5BaseObjectImpl<TBase>::getDatasetOpt)
//      .def("getName", &H5BaseObjectImpl<TBase>::getName)
//      .def("getFullName", &H5BaseObjectImpl<TBase>::getFullName);
//      .def(py::self == py::self)
//      .def(py::self != py::self);

} // h5geo
