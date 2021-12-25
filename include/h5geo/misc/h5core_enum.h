#ifndef H5CORE_ENUM_H
#define H5CORE_ENUM_H

#include "h5core_enum_operators.h"

namespace h5gt
{
class File;
class Group;
class DataSet;
class Attribute;
}

namespace h5geo {


namespace detail {


enum class ContainerAttributes : unsigned{
  ContainerType = 1
};

enum class PointsAttributes : unsigned{
  Domain = 1
};

enum class PointsDatasets : unsigned{
  points_data = 1
};

enum class SeisAttributes : unsigned{
  Domain = 1,
  SeisDataType = 2,
  SurveyType = 3,
  SRD = 4
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

enum class MapAttributes : unsigned{
  Domain = 1,
  origin = 2,
  point1 = 3,
  point2 = 4
};

enum class MapDatasets : unsigned{
  map_data = 1
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

enum class DevDatasets : unsigned{
  dev_data = 1
};

enum class LogDatasets : unsigned{
  log_data = 1
};

} // detail


enum class ContainerType : unsigned{
  MAP = 1,
  WELL = 2,
  SEISMIC = 3
};
//ENABLE_BITMASK_OPERATORS(ContainerType);

enum class ObjectType: unsigned{
  MAP = 1,
  WELL = 2,
  LOGCURVE = 3,
  DEVCURVE = 4,
  SEISMIC = 5,
  POINTS = 6
};
//ENABLE_BITMASK_OPERATORS(ObjectType);

enum class Domain : unsigned{
  TVD = 1,
  TVDSS = 2,
  TWT = 4,
  OWT = 8
};

enum class SeisDataType : unsigned{
  STACK = 1,
  PRESTACK = 2
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
  Z = 4,
  TVD = 5,
  TVDSS = 6,
  DX = 7,
  DY = 8,
  AZIM = 9,
  INCL = 10,
  OWT = 11,
  TWT = 12
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

/*!
 * \brief The CreationType enum
 */
enum class CreationType : unsigned{
  OPEN = 1, /*!< Open if exist */
  CREATE = 2, /*!< Create if not exist */
  OPEN_OR_CREATE = 3, /*!< Open if exist or Create New */
  CREATE_OR_OVERWRITE = 4, /*!< Create if not exist or Overwrite (Overwrite == delete existing and create new) */
  CREATE_UNDER_NEW_NAME = 5 /*!< Always create under New Unique Name */
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

} // h5geo


#endif // H5CORE_ENUM_H
