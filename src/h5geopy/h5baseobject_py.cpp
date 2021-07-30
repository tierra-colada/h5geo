#include "../../include/h5geopy/h5baseobject_py.h"

namespace h5geopy {

void H5BaseObject_py(
    py::class_<
    H5BaseObjectImpl,
    std::unique_ptr<H5BaseObjectImpl, ObjectDeleter>,
    H5BaseImpl,
    H5BaseObject> &py_obj){
  py_obj
      .def("getH5File", &H5BaseObjectImpl::getH5File)
      .def("getObjG", &H5BaseObjectImpl::getObjG)
      .def("getGroupOpt", &H5BaseObjectImpl::getGroupOpt)
      .def("getDatasetOpt", &H5BaseObjectImpl::getDatasetOpt)
      .def("getName", &H5BaseObjectImpl::getName)
      .def("getFullName", &H5BaseObjectImpl::getFullName);
//      .def(py::self == py::self)
//      .def(py::self != py::self);
}


//  py_obj
//      .def("getH5File", [](H5BaseObject* self) {
//    return dynamic_cast<H5BaseObjectImpl*>(self)->getH5File();});
//      .def("getObjG", &H5BaseObjectImpl::getObjG)
//      .def("getGroupOpt", &H5BaseObjectImpl::getGroupOpt)
//      .def("getDatasetOpt", &H5BaseObjectImpl::getDatasetOpt)
//      .def("getName", &H5BaseObjectImpl::getName)
//      .def("getFullName", &H5BaseObjectImpl::getFullName);
//      .def(py::self == py::self)
//      .def(py::self != py::self);

} // h5geo
