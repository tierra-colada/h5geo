#ifndef H5CORE_ENUM_H
#define H5CORE_ENUM_H

#include "h5geo_export.h"
#include "h5core_enum_operators.h"

#include <type_traits>  // for enum class operators
#include <string>
#include <map>

#define MAGIC_ENUM_RANGE_MIN 0
#define MAGIC_ENUM_RANGE_MAX 256
#include <magic_enum.hpp>

namespace h5gt
{
class File;
class Group;
class DataSet;
class Attribute;
}

namespace h5geo
{

//---------NEEDS_TO_BE_INCLUDED_TO_DETAIL_NAMESPACE---------
enum class ContainerAttributes : unsigned{
  ContainerType = 1
};

enum class SeisAttributes : unsigned{
  Domain = 1,
  SpatialUnits = 2,
  TemporalUnits = 3,
  data_units = 4,
  SeisDataType = 5,
  SurveyType = 6,
  SRD = 7
};

enum class SeisDatasets : unsigned{
  text_header = 1,
  bin_header = 2,
  trace_header = 3,
  trace = 4,
  boundary = 5
};

enum class SeisGroups : unsigned{
  sort = 1,
  indexes = 2,
  unique_values = 3
};

enum class SurfAttributes : unsigned{
  Domain = 1,
  SpatialUnits = 2,
  TemporalUnits = 3,
  data_units = 4,
  origin = 5,
  spacing = 6
};

enum class SurfDatasets : unsigned{
  surf_data = 1
};

enum class WellAttributes : unsigned{
  head_coord = 1,
  KB = 2,
  UWI = 3
};

enum class WellGroups : unsigned{
  LOG = 1,
  DEV = 2,
  ACTIVE_DEV = 3
};

enum class DevAttributes : unsigned{
  SpatialUnits = 1,
  TemporalUnits = 2,
  AngleUnits = 3
};

enum class DevDatasets : unsigned{
  dev_data = 1
};

enum class LogAttributes : unsigned{
  SpatialUnits = 1,
  data_units = 2
};

enum class LogDatasets : unsigned{
  log_data = 1
};

//---------END_OF_DETAIL_NAMESPACE---------

enum class ContainerType : unsigned{
  SURFACE = 1,
  WELL = 2,
  SEISMIC = 3
};
//ENABLE_BITMASK_OPERATORS(ContainerType);

enum class ObjectType: unsigned{
  SURFACE = 1,
  WELL = 2,
  LOGCURVE = 3,
  DEVCURVE = 4,
  SEISMIC = 5
};
//ENABLE_BITMASK_OPERATORS(ObjectType);

enum class Domain : unsigned{
  MD = 1,
  TVD = 2,
  TVDSS = 4,
  TVDSD = 8,
  TWT = 16,
  OWT = 32
};

enum class SeisDataType : unsigned{
  STACK = 1,
  PRESTACK = 2,
  ATTRIBUTE = 3
};

enum class SurveyType : unsigned{
  TWO_D = 1,
  THREE_D = 2
};

enum class TxtEncoding : unsigned{
  ASCII = 0,
  EBCDIC = 1
};

enum class SegyEndian : unsigned{
  Little = 0,
  Big = 1
};

enum class SegyFormat : unsigned{
  FourByte_IBM = 0,
  FourByte_IEEE = 1,
  FourByte_integer = 2
};

enum class WellDataType : unsigned{
  WELL = 1,
  DEV = 2,    // deviation
  LOG = 3,    // well log (from .las)
  INTERVAL_VEL = 4,
  AVG_VEL = 5,    // average velocity curve
  RMS_VEL = 6,
  FWAL = 7,   // full waveform acoustic logging
  CHECKSHOTS = 8,
  WELL_TIE = 9
};

enum class WellName : unsigned{
  FROM_LAS = 0,
  FROM_FILE_NAME = 1
};

enum class LogDataType : unsigned{
  MD = 1,
  VAL = 2
};

enum class DevDataType : unsigned{
  MD = 1,
  X = 2,
  Y = 3,
  TVD = 4,
  OWT = 5
};

enum class WellType : unsigned{
  PROPOSED = 1,
  DRY = 2,
  OIL = 4,
  GAS = 8,
  WATER = 16,
  CONDENSATE = 32
};
//ENABLE_BITMASK_OPERATORS(WellType);

enum class TrajectoryFormat : unsigned{
  MD_AZIM_INCL = 0,
  TVD_X_Y = 1,
  TVD_DX_DY = 2,
  TVDSS_X_Y = 3,
  TVDSS_DX_DY = 4
};

enum class SpatialUnits : unsigned{
  METER = 0,
  CENTIMETER = 1,
  MILLIMETER = 2,
  FOOT = 3
};

enum class TemporalUnits : unsigned{
  SECOND = 0,
  MILLISECOND = 1,
  MICROSECOND = 2
};

enum class AngleUnits : unsigned{
  DEGREE = 0,
  RADIAN = 1
};

/*!
 * \brief The CreationType enum
 */
enum class CreationType : unsigned{
  OPEN_OR_CREATE = 1, /*!< Open if exist or Create New */
  CREATE_OR_OVERWRITE = 2, /*!< Create if not exist or Overwrite (Overwrite == delete existing and create new) */
  CREATE_UNDER_NEW_NAME = 3 /*!< Always create under New Unique Name */
};

enum class CaseSensitivity : unsigned{
  CASE_SENSITIVE = 1,
  CASE_INSENSITIVE = 2
};

enum class Delimiter : unsigned{
  TABULATION = 1,
  SEMICOLON = 2,
  DOT = 4,
  SPACE = 8,
  COMMA = 16
};
ENABLE_BITMASK_OPERATORS(Delimiter);

}


#endif // H5CORE_ENUM_H
