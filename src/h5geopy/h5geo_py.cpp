#include "../../include/h5geopy/h5base_py.h"
#include "../../include/h5geopy/h5basecontainer_py.h"
#include "../../include/h5geopy/h5baseobject_py.h"
#include "../../include/h5geopy/h5enum_py.h"
#include "../../include/h5geopy/h5core_sr_settings_py.h"
#include "../../include/h5geopy/h5point_py.h"
#include "../../include/h5geopy/h5devcurve_py.h"
#include "../../include/h5geopy/h5deviation_py.h"
#include "../../include/h5geopy/h5easyhull_py.h"
#include "../../include/h5geopy/h5geofunctions_py.h"
#include "../../include/h5geopy/h5interpolation_py.h"
#include "../../include/h5geopy/h5core_segy_py.h"
#include "../../include/h5geopy/h5sort_py.h"
#include "../../include/h5geopy/h5logcurve_py.h"
#include "../../include/h5geopy/h5basepoints_py.h"
#include "../../include/h5geopy/h5points_py.h"
#include "../../include/h5geopy/h5seis_py.h"
#include "../../include/h5geopy/h5seiscontainer_py.h"
#include "../../include/h5geopy/h5vol_py.h"
#include "../../include/h5geopy/h5volcontainer_py.h"
#include "../../include/h5geopy/h5map_py.h"
#include "../../include/h5geopy/h5mapcontainer_py.h"
#include "../../include/h5geopy/h5well_py.h"
#include "../../include/h5geopy/h5wellcontainer_py.h"
#include "../../include/h5geopy/h5welltops_py.h"
#include "../../include/h5geopy/h5horizon_py.h"


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
  auto pyBaseObjectParam = py::class_<H5BaseObjectParam>(m, "H5BaseObjectParam");
  auto pyPointsParam = py::class_<H5PointsParam, H5BaseObjectParam>(m, "H5PointsParam");
  auto pyWellTopsParam = py::class_<H5WellTopsParam, H5PointsParam, H5BaseObjectParam>(m, "H5WellTopsParam");
  auto pyHorizonParam = py::class_<H5HorizonParam, H5BaseObjectParam>(m, "H5HorizonParam");
  auto pyMapParam = py::class_<H5MapParam, H5BaseObjectParam>(m, "H5MapParam");
  auto pyWellParam = py::class_<H5WellParam, H5BaseObjectParam>(m, "H5WellParam");
  auto pyDevCurveParam = py::class_<H5DevCurveParam, H5BaseObjectParam>(m, "H5DevCurveParam");
  auto pyLogCurveParam = py::class_<H5LogCurveParam, H5BaseObjectParam>(m, "H5LogCurveParam");
  auto pySeisParam = py::class_<H5SeisParam, H5BaseObjectParam>(m, "H5SeisParam");
  auto pyVolParam = py::class_<H5VolParam, H5BaseObjectParam>(m, "H5VolParam");
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
  auto pyPoint1 =
      py::class_<Point1>
      (m, "Point1");

  auto pyPoint2 =
      py::class_<Point2>
      (m, "Point2");

  auto pyPoint3 =
      py::class_<Point3>
      (m, "Point3");

  auto pyPoint4 =
      py::class_<Point4>
      (m, "Point4");

  // POINTS
  auto pyBasePoints =
      py::class_<
      H5BasePoints,
      H5BasePointsImpl<H5BasePoints>,
      H5BaseObject,
      H5Base,
      std::unique_ptr<H5BasePoints, ObjectDeleter>>
      (m, "H5BasePoints");

  auto pyPoints1 =
      py::class_<
      H5Points1,
      H5Points1Impl<H5Points1>,
      H5BasePoints,
      H5BaseObject,
      H5Base,
      std::unique_ptr<H5Points1, ObjectDeleter>>
      (m, "H5Points1");

  auto pyPoints2 =
      py::class_<
      H5Points2,
      H5Points2Impl<H5Points2>,
      H5BasePoints,
      H5BaseObject,
      H5Base,
      std::unique_ptr<H5Points2, ObjectDeleter>>
      (m, "H5Points2");

  auto pyPoints3 =
      py::class_<
      H5Points3,
      H5Points3Impl<H5Points3>,
      H5BasePoints,
      H5BaseObject,
      H5Base,
      std::unique_ptr<H5Points3, ObjectDeleter>>
      (m, "H5Points3");

  auto pyPoints4 =
      py::class_<
      H5Points4,
      H5Points4Impl<H5Points4>,
      H5BasePoints,
      H5BaseObject,
      H5Base,
      std::unique_ptr<H5Points4, ObjectDeleter>>
      (m, "H5Points4");

  // WELLTOPS
  auto pyWellTops =
      py::class_<
      H5WellTops,
      H5WellTopsImpl,
      H5Points1,
      H5BasePoints,
      H5BaseObject,
      H5Base,
      std::unique_ptr<H5WellTops, ObjectDeleter>>
      (m, "H5WellTops");

  // HORIZON
  auto pyHorizon =
      py::class_<
      H5Horizon,
      H5HorizonImpl,
      H5BaseObject,
      H5Base,
      std::unique_ptr<H5Horizon, ObjectDeleter>>
      (m, "H5Horizon");

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

  // VOLCONTAINER
  auto pyVolContainer =
      py::class_<
      H5VolContainer,
      H5VolContainerImpl,
      H5BaseContainer,
      H5Base,
      std::unique_ptr<H5VolContainer, ObjectDeleter>>
      (m, "H5VolContainer");

  // VOL
  auto pyVol =
      py::class_<
      H5Vol,
      H5VolImpl,
      H5BaseObject,
      H5Base,
      std::unique_ptr<H5Vol, ObjectDeleter>>
      (m, "H5Vol");

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
  WellTopsParam_py(pyWellTopsParam);
  HorizonParam_py(pyHorizonParam);
  MapParam_py(pyMapParam);
  WellParam_py(pyWellParam);
  DevCurveParam_py(pyDevCurveParam);
  LogCurveParam_py(pyLogCurveParam);
  SeisParam_py(pySeisParam);
  VolParam_py(pyVolParam);
  H5Base_py pyBase_inst(pyBase);

  // BASECONTAINER
  H5BaseContainer_py pyBaseContainer_inst(pyBaseContainer);

  // BASEOBJECT
  H5BaseObject_py pyBaseObject_inst(pyBaseObject);

  // H5GEO::POINT
  Point1_py(pyPoint1);
  py::bind_vector<Point1Array>(m, "Point1Array");
  Point2_py(pyPoint2);
  py::bind_vector<Point2Array>(m, "Point2Array");
  Point3_py(pyPoint3);
  py::bind_vector<Point3Array>(m, "Point3Array");
  Point4_py(pyPoint4);
  py::bind_vector<Point4Array>(m, "Point4Array");

  // POINTS
  H5BasePoints_py pyBasePoints_inst(pyBasePoints);
  H5Points1_py(pyPoints1);
  H5Points2_py(pyPoints2);
  H5Points3_py(pyPoints3);
  H5Points4_py(pyPoints4);

  // WELLTOPS
  H5WellTops_py(pyWellTops);

  // HORIZON
  H5Horizon_py(pyHorizon);

  // MAPCONTAINER
  H5MapContainer_py(pyMapContainer);

  // MAP
  H5Map_py(pyMap);

  // SEISCONTAINER
  H5SeisContainer_py(pySeisContainer);

  // SEIS
  H5Seis_py(pySeis);

// VOLCONTAINER
  H5VolContainer_py(pyVolContainer);

  // VOL
  H5Vol_py(pyVol);

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
  defineSEGYFunctions(m);
  defineInterpolationFunctions(m);

#ifdef H5GEO_USE_GDAL
  defineSRSettingsFunctions(m_sr);
#endif
}

} // h5geopy


