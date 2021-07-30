#include "../../include/h5geopy/h5base_py.h"
#include "../../include/h5geopy/h5basecontainer_py.h"
#include "../../include/h5geopy/h5baseobject_py.h"
#include "../../include/h5geopy/h5core_enum_py.h"
#include "../../include/h5geopy/h5devcurve_py.h"
#include "../../include/h5geopy/h5deviation_py.h"
#include "../../include/h5geopy/h5easyhull_py.h"
#include "../../include/h5geopy/h5geofunctions_py.h"
#include "../../include/h5geopy/h5logcurve_py.h"
#include "../../include/h5geopy/h5seis_py.h"
#include "../../include/h5geopy/h5seiscontainer_py.h"
#include "../../include/h5geopy/h5sort_py.h"
#include "../../include/h5geopy/h5surf_py.h"
#include "../../include/h5geopy/h5surfcontainer_py.h"
#include "../../include/h5geopy/h5well_py.h"
#include "../../include/h5geopy/h5wellcontainer_py.h"


namespace h5geopy {

PYBIND11_MODULE(_h5geo, m) {
  py::module_::import("h5gtpy._h5gt");

  m.doc() =
      "API to work with geo-data (seismic, wells, surfaces, other in process) based on HDF5 and originally written in C++: "
  "https://github.com/tierra-colada/h5geo";


  // Simple exception class

  /*-------------------------------------------------------------*/
  /*-------------------------DECLARATION-------------------------*/
  /*-------------------------------------------------------------*/


  /* `nullptr` problem: I use interface classes (base classes are abstract).
   * Factory functions (and methods) may create `nullptr` instead of `H5Base *`
   * pointer for example. If this happens then python doesn't know how to treat
   * `H5Base *` casted from `nullptr` as I bind `H5BaseImpl` classes everywhere.
   * So to bind this `nullptr` to some python class I need declare it as I do below
   * (their names start with `_` as they are not intended to be explicetly used).
   * Secondly I decided to include these abstract classes to bind class hierarchy
   * (you can see that every `py_class_<SomeClassImpl, ..., SomeClass>` has a corresponding
   * abstract class as parent. I do this to keep polymorphism (though this may be excessive) */


  /*----------EMPTY DECLARATION OF ABSTRACT CLASSES (NEEDED TO OVERCOME `nullptr` PROBLEM---------*/

  // _BASE
  py::class_<
      H5Base,
      std::unique_ptr<H5Base,
      ObjectDeleter>>(m, "_H5Base");
  py::class_<
      H5BaseContainer,
      std::unique_ptr<H5BaseContainer, ObjectDeleter>,
      H5Base>(m, "_H5BaseContainer");
  py::class_<
      H5BaseObject,
      std::unique_ptr<H5BaseObject, ObjectDeleter>,
      H5Base>(m, "_H5BaseObject");

  // _SURF
  py::class_<
      H5SurfContainer,
      std::unique_ptr<H5SurfContainer, ObjectDeleter>,
      H5BaseContainer>(m, "_H5SurfContainer");
  py::class_<
      H5Surf,
      std::unique_ptr<H5Surf, ObjectDeleter>,
      H5BaseObject>(m, "_H5Surf");

  // _SEIS
  py::class_<
      H5SeisContainer,
      std::unique_ptr<H5SeisContainer, ObjectDeleter>,
      H5BaseContainer>(m, "_H5SeisContainer");
  py::class_<
      H5Seis,
      std::unique_ptr<H5Seis, ObjectDeleter>,
      H5BaseObject>(m, "_H5Seis");

  // _WELL
  py::class_<
      H5WellContainer,
      std::unique_ptr<H5WellContainer, ObjectDeleter>,
      H5BaseContainer>(m, "_H5WellContainer");
  py::class_<
      H5Well,
      std::unique_ptr<H5Well, ObjectDeleter>,
      H5BaseObject>(m, "_H5Well");
  py::class_<
      H5DevCurve,
      std::unique_ptr<H5DevCurve, ObjectDeleter>,
      H5BaseObject>(m, "_H5DevCurve");
  py::class_<
      H5LogCurve,
      std::unique_ptr<H5LogCurve, ObjectDeleter>,
      H5BaseObject>(m, "_H5LogCurve");

  /*----------END OF `nullptr` PROBLEM---------*/


  // ENUM
  auto pyContainerType = py::enum_<ContainerType>(m, "ContainerType", py::arithmetic());
  auto pyObjType = py::enum_<ObjectType>(m, "ObjectType", py::arithmetic());
  auto pyDomain = py::enum_<Domain>(m, "Domain", py::arithmetic());
  auto pySeisDataType = py::enum_<SeisDataType>(m, "SeisDataType", py::arithmetic());
  auto pySurveyType = py::enum_<SurveyType>(m, "SurveyType", py::arithmetic());
  auto pyTxtEncoding = py::enum_<TxtEncoding>(m, "TxtEncoding", py::arithmetic());
  auto pySegyEndian = py::enum_<SegyEndian>(m, "SegyEndian", py::arithmetic());
  auto pySegyFormat = py::enum_<SegyFormat>(m, "SegyFormat", py::arithmetic());
  auto pyWellDataType = py::enum_<WellDataType>(m, "WellDataType", py::arithmetic());
  auto pyWellName = py::enum_<WellName>(m, "WellName", py::arithmetic());
  auto pyLogDataType = py::enum_<LogDataType>(m, "LogDataType", py::arithmetic());
  auto pyDevDataType = py::enum_<DevDataType>(m, "DevDataType", py::arithmetic());
  auto pyWellType = py::enum_<WellType>(m, "WellType", py::arithmetic());
  auto pyTrajectoryFormat = py::enum_<TrajectoryFormat>(m, "TrajectoryFormat", py::arithmetic());
  auto pySpatialUnits = py::enum_<SpatialUnits>(m, "SpatialUnits", py::arithmetic());
  auto pyTemporalUnits = py::enum_<TemporalUnits>(m, "TemporalUnits", py::arithmetic());
  auto pyAngleUnits = py::enum_<AngleUnits>(m, "AngleUnits", py::arithmetic());
  auto pyCreationType = py::enum_<CreationType>(m, "CreationType", py::arithmetic());
  auto pyCaseSensitivity = py::enum_<CaseSensitivity>(m, "CaseSensitivity", py::arithmetic());
  auto pyDelimiter = py::enum_<Delimiter>(m, "Delimiter", py::arithmetic());

  // DELETER
  auto pyObjectDeleter = py::class_<ObjectDeleter>(m, "ObjectDeleter");

  // BASE
  auto pySurfParam = py::class_<SurfParam>(m, "SurfParam");
  auto pyWellParam = py::class_<WellParam>(m, "WellParam");
  auto pyDevCurveParam = py::class_<DevCurveParam>(m, "DevCurveParam");
  auto pyLogCurveParam = py::class_<LogCurveParam>(m, "LogCurveParam");
  auto pySeisParam = py::class_<SeisParam>(m, "SeisParam");
  auto pyBase = py::class_<
      H5BaseImpl,
      std::unique_ptr<H5BaseImpl, ObjectDeleter>,
      H5Base>(m, "H5Base");

  // BASECONTAINER
  auto pyBaseContainer = py::class_<
      H5BaseContainerImpl,
      std::unique_ptr<H5BaseContainerImpl, ObjectDeleter>,
      H5BaseImpl,
      H5BaseContainer>(m, "H5BaseContainer");

  // BASEOBJECT
  auto pyBaseObject = py::class_<
      H5BaseObjectImpl,
      std::unique_ptr<H5BaseObjectImpl, ObjectDeleter>,
      H5BaseImpl,
      H5BaseObject>(m, "H5BaseObject");

  // SURFCONTAINER
  auto pySurfContainer =
      py::class_<
      H5SurfContainerImpl,
      std::unique_ptr<H5SurfContainerImpl, ObjectDeleter>,
      H5BaseContainerImpl,
      H5SurfContainer>(m, "H5SurfContainer");

  // SURF
  auto pySurf =
      py::class_<
      H5SurfImpl,
      std::unique_ptr<H5SurfImpl, ObjectDeleter>,
      H5BaseObjectImpl,
      H5Surf>(m, "H5Surf");

  // SEISCONTAINER
  auto pySeisContainer =
      py::class_<
      H5SeisContainerImpl,
      std::unique_ptr<H5SeisContainerImpl, ObjectDeleter>,
      H5BaseContainerImpl,
      H5SeisContainer>(m, "H5SeisContainer");

  // SEIS
  auto pySeis =
      py::class_<
      H5SeisImpl,
      std::unique_ptr<H5SeisImpl, ObjectDeleter>,
      H5BaseObjectImpl,
      H5Seis>(m, "H5Seis");

  // WELLCONTAINER
  auto pyWellContainer =
      py::class_<
      H5WellContainerImpl,
      std::unique_ptr<H5WellContainerImpl, ObjectDeleter>,
      H5BaseContainerImpl,
      H5WellContainer>(m, "H5WellContainer");

  // WELL
  auto pyWell =
      py::class_<
      H5WellImpl,
      std::unique_ptr<H5WellImpl, ObjectDeleter>,
      H5BaseObjectImpl,
      H5Well>(m, "H5Well");

  // DEVCURVE
  auto pyDevCurve =
      py::class_<
      H5DevCurveImpl,
      std::unique_ptr<H5DevCurveImpl, ObjectDeleter>,
      H5BaseObjectImpl,
      H5DevCurve>(m, "H5DevCurve");

  // DEVCURVE
  auto pyLogCurve =
      py::class_<
      H5LogCurveImpl,
      std::unique_ptr<H5LogCurveImpl, ObjectDeleter>,
      H5BaseObjectImpl,
      H5LogCurve>(m, "H5LogCurve");


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
  SpatialUnits_py(pySpatialUnits);
  TemporalUnits_py(pyTemporalUnits);
  AngleUnits_py(pyAngleUnits);
  CreationType_py(pyCreationType);
  CaseSensitivity_py(pyCaseSensitivity);
  Delimiter_py(pyDelimiter);

  // DELETER
  ObjectDeleter_py(pyObjectDeleter);

  // BASE
  SurfParam_py(pySurfParam);
  WellParam_py(pyWellParam);
  DevCurveParam_py(pyDevCurveParam);
  LogCurveParam_py(pyLogCurveParam);
  SeisParam_py(pySeisParam);
  H5Base_py(pyBase);

  // BASECONTAINER
  H5BaseContainer_py(pyBaseContainer);

  // BASEOBJECT
  H5BaseObject_py(pyBaseObject);

  // SURFCONTAINER
  H5SurfContainer_py(pySurfContainer);

  // SURF
  H5Surf_py(pySurf);

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
}


} // h5geopy
