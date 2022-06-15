#ifndef H5CORE_ENUM_PY_H
#define H5CORE_ENUM_PY_H

#include "h5geo_py.h"

#include <h5geo/private/h5enum.h>

namespace h5geopy {

void ContainerType_py(py::enum_<ContainerType> &py_obj);
void ObjectType_py(py::enum_<ObjectType> &py_obj);
void Domain_py(py::enum_<Domain> &py_obj);
void SeisDataType_py(py::enum_<SeisDataType> &py_obj);
void SurveyType_py(py::enum_<SurveyType> &py_obj);
void TxtEncoding_py(py::enum_<TextEncoding> &py_obj);
void SegyEndian_py(py::enum_<Endian> &py_obj);
void SegyFormat_py(py::enum_<SegyFormat> &py_obj);
void WellDataType_py(py::enum_<WellDataType> &py_obj);
void WellName_py(py::enum_<WellName> &py_obj);
void LogDataType_py(py::enum_<LogDataType> &py_obj);
void DevDataType_py(py::enum_<DevDataType> &py_obj);
void WellType_py(py::enum_<WellType> &py_obj);
void TrajectoryFormat_py(py::enum_<TrajectoryFormat> &py_obj);
void CreationType_py(py::enum_<CreationType> &py_obj);
void CaseSensitivity_py(py::enum_<CaseSensitivity> &py_obj);
void Delimiter_py(py::enum_<Delimiter> &py_obj);


} // h5geopy


#endif // H5CORE_ENUM_PY_H
