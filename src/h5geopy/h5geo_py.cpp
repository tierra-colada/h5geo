#include "../../include/h5geopy/h5base_py.h"
#include "../../include/h5geopy/h5basecontainer_py.h"
#include "../../include/h5geopy/h5baseobject_py.h"
#include "../../include/h5geopy/h5enum_py.h"
#include "../../include/h5geopy/h5sr_settings_py.h"
#include "../../include/h5geopy/h5point_py.h"
#include "../../include/h5geopy/h5devcurve_py.h"
#include "../../include/h5geopy/h5deviation_py.h"
#include "../../include/h5geopy/h5easyhull_py.h"
#include "../../include/h5geopy/h5geofunctions_py.h"
#include "../../include/h5geopy/h5interpolation_py.h"
#include "../../include/h5geopy/h5logcurve_py.h"
#include "../../include/h5geopy/h5points_py.h"
#include "../../include/h5geopy/h5seis_py.h"
#include "../../include/h5geopy/h5seiscontainer_py.h"
#include "../../include/h5geopy/h5sort_py.h"
#include "../../include/h5geopy/h5map_py.h"
#include "../../include/h5geopy/h5mapcontainer_py.h"
#include "../../include/h5geopy/h5well_py.h"
#include "../../include/h5geopy/h5wellcontainer_py.h"

PYBIND11_MAKE_OPAQUE(PointArray);


namespace h5geopy {

PYBIND11_MODULE(_h5geo, m) {
  py::module_::import("h5gtpy._h5gt");

#ifdef H5GEO_USE_GDAL
  py::module_ m_gdal = py::module_::import("osgeo._gdal");
  py::module_ m_ogr = py::module_::import("osgeo._ogr");
  py::module_ m_osr = py::module_::import("osgeo._osr");

  // create submodule for Spatial Reference Settings
  py::module m_sr = m.def_submodule("sr");
#endif

  m.doc() =
      "API to work with geo-data (seismic, wells, maps, other in process) based on HDF5 and originally written in C++: "
  "https://github.com/tierra-colada/h5geo";

  /*-------------------------------------------------------------*/
  /*-------------------------DECLARATION-------------------------*/
  /*-------------------------------------------------------------*/

  // ENUM
  auto pyContainerType = py::enum_<ContainerType>(m, "ContainerType", py::arithmetic());
  auto pyObjType = py::enum_<ObjectType>(m, "ObjectType", py::arithmetic());
  auto pyDomain = py::enum_<Domain>(m, "Domain", py::arithmetic());
  auto pySeisDataType = py::enum_<SeisDataType>(m, "SeisDataType", py::arithmetic());
  auto pySurveyType = py::enum_<SurveyType>(m, "SurveyType", py::arithmetic());
  auto pyTxtEncoding = py::enum_<TextEncoding>(m, "TextEncoding", py::arithmetic());
  auto pySegyEndian = py::enum_<Endian>(m, "Endian", py::arithmetic());
  auto pySegyFormat = py::enum_<SegyFormat>(m, "SegyFormat", py::arithmetic());
  auto pyWellDataType = py::enum_<WellDataType>(m, "WellDataType", py::arithmetic());
  auto pyWellName = py::enum_<WellName>(m, "WellName", py::arithmetic());
  auto pyLogDataType = py::enum_<LogDataType>(m, "LogDataType", py::arithmetic());
  auto pyDevDataType = py::enum_<DevDataType>(m, "DevDataType", py::arithmetic());
  auto pyWellType = py::enum_<WellType>(m, "WellType", py::arithmetic());
  auto pyTrajectoryFormat = py::enum_<TrajectoryFormat>(m, "TrajectoryFormat", py::arithmetic());
  auto pyCreationType = py::enum_<CreationType>(m, "CreationType", py::arithmetic());
  auto pyCaseSensitivity = py::enum_<CaseSensitivity>(m, "CaseSensitivity", py::arithmetic());
  auto pyDelimiter = py::enum_<Delimiter>(m, "Delimiter", py::arithmetic());

  // _DELETER
  auto pyObjectDeleter = py::class_<ObjectDeleter>(m, "ObjectDeleter");

  // BASE
  auto pyBaseObjectParam = py::class_<BaseObjectParam>(m, "BaseObjectParam");
  auto pyPointsParam = py::class_<PointsParam, BaseObjectParam>(m, "PointsParam");
  auto pyMapParam = py::class_<MapParam, BaseObjectParam>(m, "MapParam");
  auto pyWellParam = py::class_<WellParam, BaseObjectParam>(m, "WellParam");
  auto pyDevCurveParam = py::class_<DevCurveParam, BaseObjectParam>(m, "DevCurveParam");
  auto pyLogCurveParam = py::class_<LogCurveParam, BaseObjectParam>(m, "LogCurveParam");
  auto pySeisParam = py::class_<SeisParam, BaseObjectParam>(m, "SeisParam");
  auto pyBase = py::class_<
      H5Base,
      H5BaseImpl<H5Base>,
      std::unique_ptr<H5Base, ObjectDeleter>>
      (m, "H5Base");

  // BASECONTAINER
  auto pyBaseContainer = py::class_<
      H5BaseContainer,
      H5BaseContainerImpl<H5BaseContainer>,
      H5Base,
      std::unique_ptr<H5BaseContainer, ObjectDeleter>>
      (m, "H5BaseContainer");

  // BASEOBJECT
  auto pyBaseObject = py::class_<
      H5BaseObject,
      H5BaseObjectImpl<H5BaseObject>,
      H5Base,
      std::unique_ptr<H5BaseObject, ObjectDeleter>>
      (m, "H5BaseObject");

  // H5GEO::POINT
  auto pyPoint =
      py::class_<Point>
      (m, "Point");

  // POINTS
  auto pyPoints =
      py::class_<
      H5Points,
      H5PointsImpl,
      H5BaseObject,
      H5Base,
      std::unique_ptr<H5Points, ObjectDeleter>>
      (m, "H5Points");

  // MAPCONTAINER
  auto pyMapContainer =
      py::class_<
      H5MapContainer,
      H5MapContainerImpl,
      H5BaseContainer,
      H5Base,
      std::unique_ptr<H5MapContainer, ObjectDeleter>>
      (m, "H5MapContainer");

  // MAP
  auto pyMap =
      py::class_<
      H5Map,
      H5MapImpl,
      H5BaseObject,
      H5Base,
      std::unique_ptr<H5Map, ObjectDeleter>>
      (m, "H5Map");

  // SEISCONTAINER
  auto pySeisContainer =
      py::class_<
      H5SeisContainer,
      H5SeisContainerImpl,
      H5BaseContainer,
      H5Base,
      std::unique_ptr<H5SeisContainer, ObjectDeleter>>
      (m, "H5SeisContainer");

  // SEIS
  auto pySeis =
      py::class_<
      H5Seis,
      H5SeisImpl,
      H5BaseObject,
      H5Base,
      std::unique_ptr<H5Seis, ObjectDeleter>>
      (m, "H5Seis");

  // WELLCONTAINER
  auto pyWellContainer =
      py::class_<
      H5WellContainer,
      H5WellContainerImpl,
      H5BaseContainer,
      H5Base,
      std::unique_ptr<H5WellContainer, ObjectDeleter>>
      (m, "H5WellContainer");

  // WELL
  auto pyWell =
      py::class_<
      H5Well,
      H5WellImpl,
      H5BaseObject,
      H5Base,
      std::unique_ptr<H5Well, ObjectDeleter>>
      (m, "H5Well");

  // DEVCURVE
  auto pyDevCurve =
      py::class_<
      H5DevCurve,
      H5DevCurveImpl,
      H5BaseObject,
      H5Base,
      std::unique_ptr<H5DevCurve, ObjectDeleter>>
      (m, "H5DevCurve");

  // LOGCURVE
  auto pyLogCurve =
      py::class_<
      H5LogCurve,
      H5LogCurveImpl,
      H5BaseObject,
      H5Base,
      std::unique_ptr<H5LogCurve, ObjectDeleter>>
      (m, "H5LogCurve");


  /*------------------------------------------------------------*/
  /*-------------------------DEFINITION-------------------------*/
  /*------------------------------------------------------------*/

  // ENUM -> H5Enum_py.cpp
  ContainerType_py(pyContainerType);
  ObjectType_py(pyObjType);
  Domain_py(pyDomain);
  SeisDataType_py(pySeisDataType);
  SurveyType_py(pySurveyType);
  TxtEncoding_py(pyTxtEncoding);
  SegyEndian_py(pySegyEndian);
  SegyFormat_py(pySegyFormat);
  WellDataType_py(pyWellDataType);
  WellName_py(pyWellName);
  LogDataType_py(pyLogDataType);
  DevDataType_py(pyDevDataType);
  WellType_py(pyWellType);
  TrajectoryFormat_py(pyTrajectoryFormat);
  CreationType_py(pyCreationType);
  CaseSensitivity_py(pyCaseSensitivity);
  Delimiter_py(pyDelimiter);

  // DELETER
  ObjectDeleter_py(pyObjectDeleter);

  // BASE
  BaseObjectParam_py(pyBaseObjectParam);
  PointsParam_py(pyPointsParam);
  MapParam_py(pyMapParam);
  WellParam_py(pyWellParam);
  DevCurveParam_py(pyDevCurveParam);
  LogCurveParam_py(pyLogCurveParam);
  SeisParam_py(pySeisParam);
  H5Base_py pyBase_inst(pyBase);

  // BASECONTAINER
  H5BaseContainer_py pyBaseContainer_inst(pyBaseContainer);

  // BASEOBJECT
  H5BaseObject_py pyBaseObject_inst(pyBaseObject);

  // H5GEO::POINT
  Point_py(pyPoint);
  py::bind_vector<PointArray>(m, "PointArray");

  // POINTS
  H5Points_py(pyPoints);

  // MAPCONTAINER
  H5MapContainer_py(pyMapContainer);

  // MAP
  H5Map_py(pyMap);

  // SEISCONTAINER
  H5SeisContainer_py(pySeisContainer);

  // SEIS
  H5Seis_py(pySeis);

  // WELLCONTAINER
  H5WellContainer_py(pyWellContainer);

  // WELL
  H5Well_py(pyWell);

  // DEVCURVE
  H5DevCurve_py(pyDevCurve);

  // LOGCURVE
  H5LogCurve_py(pyLogCurve);

  // DEFINE FUNCTIONS (MUST BE CALLED AFTER EVETYTHING IS ALREADY DECLARED)
  defineGeoFunctions(m);
  defineDeviationFunctions(m);
  defineEasyHullFunctions(m);
  defineSortFunctions(m);
  defineInterpolationFunctions(m);

#ifdef H5GEO_USE_GDAL
  defineSRSettingsFunctions(m_sr);
#endif
}

} // h5geopy


