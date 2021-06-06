#include "../../include/h5geopy/h5base_py.h"
#include "../../include/h5geopy/h5basecontainer_py.h"
#include "../../include/h5geopy/h5baseobject_py.h"
#include "../../include/h5geopy/h5core_enum_py.h"
#include "../../include/h5geopy/h5geofunctions_py.h"
#include "../../include/h5geopy/h5surf_py.h"
#include "../../include/h5geopy/h5surfcontainer_py.h"

namespace h5geopy {

PYBIND11_MODULE(_h5geo, m) {
//  py::module::import("h5gtpy._h5gt").attr("h5gt");
//  py::object g = (py::object)py::module_::import("h5gtpy._h5gt").attr("Group");
  py::module_::import("h5gtpy").attr("h5gt");
  py::module_::import("h5gtpy").attr("h5gt").attr("File");
  py::module_::import("h5gtpy").attr("h5gt").attr("Group");

  m.doc() =
      "API to work with geo-data (seismic, wells, surfaces, other in process) based on HDF5 and originally written in C++: "
  "https://github.com/tierra-colada/h5geo";


  // Simple exception class

  /*-------------------------------------------------------------*/
  /*-------------------------DECLARATION-------------------------*/
  /*-------------------------------------------------------------*/

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

  // BASE
  auto pySurfParam = py::class_<SurfParam>(m, "SurfParam");
  auto pyBase = py::class_<H5Base, std::unique_ptr<H5Base, py::nodelete>>(m, "H5Base");

  // BASECONTAINER
  auto pyBaseContainer = py::class_<H5BaseContainer,
      std::unique_ptr<H5BaseContainer, py::nodelete>,
      H5Base>(m, "H5BaseContainer");

  // BASEOBJECT
  auto pyBaseObject = py::class_<H5BaseObject,
      std::unique_ptr<H5BaseObject, py::nodelete>,
      H5Base>(m, "H5BaseObject");

  // SURFCONTAINER
  auto pySurfContainer =
      py::class_<
      H5SurfContainer,
      std::unique_ptr<H5SurfContainer, py::nodelete>,
      H5BaseContainer>(m, "H5SurfContainer");

  // SURF
  auto pySurf =
      py::class_<
      H5Surf,
      std::unique_ptr<H5Surf, py::nodelete>,
      H5BaseObject>(m, "H5Surf");


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

  // BASE
  SurfParam_py(pySurfParam);
  H5Base_py(pyBase);

  // BASECONTAINER
  H5BaseContainer_py(pyBaseContainer);

  // BASEOBJECT
  H5BaseObject_py(pyBaseObject);

  // SURFCONTAINER
  H5SurfContainer_py(pySurfContainer);

  // SURF
  H5Surf_py(pySurf);

  // DEFINE ALL FUNCTIONS (MUST BE CALLED AFTER EVETYTHING IS ALREADY DECLARED)
  defineAllFunctions(m);

}

} // h5geopy
