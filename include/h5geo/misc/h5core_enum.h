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

namespace h5geo {


namespace detail {


enum class ContainerAttributes : unsigned{
  ContainerType = 1
};

constexpr auto& cnt_attrs =
    magic_enum::enum_names<h5geo::detail::ContainerAttributes>();
constexpr auto ContainerType = magic_enum::enum_name<h5geo::detail::ContainerAttributes::ContainerType>();

enum class SeisAttributes : unsigned{
  Domain = 1,
  SeisDataType = 2,
  SurveyType = 3,
  SRD = 4,
  spatial_units = 5,
  temporal_units = 6,
  data_units = 7
};

constexpr auto& seis_attrs =
    magic_enum::enum_names<h5geo::detail::SeisAttributes>();
constexpr auto Domain = magic_enum::enum_name<h5geo::detail::SeisAttributes::Domain>();
constexpr auto SeisDataType = magic_enum::enum_name<h5geo::detail::SeisAttributes::SeisDataType>();
constexpr auto SurveyType = magic_enum::enum_name<h5geo::detail::SeisAttributes::SurveyType>();
constexpr auto SRD = magic_enum::enum_name<h5geo::detail::SeisAttributes::SRD>();
constexpr auto spatial_units = magic_enum::enum_name<h5geo::detail::SeisAttributes::spatial_units>();
constexpr auto temporal_units = magic_enum::enum_name<h5geo::detail::SeisAttributes::temporal_units>();
constexpr auto data_units = magic_enum::enum_name<h5geo::detail::SeisAttributes::data_units>();

enum class SeisDatasets : unsigned{
  text_header = 1,
  bin_header = 2,
  trace_header = 3,
  trace = 4,
  boundary = 5
};

constexpr auto& seis_dsets =
    magic_enum::enum_names<h5geo::detail::SeisDatasets>();
constexpr auto text_header = magic_enum::enum_name<h5geo::detail::SeisDatasets::text_header>();
constexpr auto bin_header = magic_enum::enum_name<h5geo::detail::SeisDatasets::bin_header>();
constexpr auto trace_header = magic_enum::enum_name<h5geo::detail::SeisDatasets::trace_header>();
constexpr auto trace = magic_enum::enum_name<h5geo::detail::SeisDatasets::trace>();
constexpr auto boundary = magic_enum::enum_name<h5geo::detail::SeisDatasets::boundary>();

enum class SeisGroups : unsigned{
  sort = 1,
  indexes = 2,
  unique_values = 3
};

constexpr auto& seis_groups =
    magic_enum::enum_names<h5geo::detail::SeisGroups>();
constexpr auto sort = magic_enum::enum_name<h5geo::detail::SeisGroups::sort>();
constexpr auto indexes = magic_enum::enum_name<h5geo::detail::SeisGroups::indexes>();
constexpr auto unique_values = magic_enum::enum_name<h5geo::detail::SeisGroups::unique_values>();

enum class SurfAttributes : unsigned{
  Domain = 1,
  spatial_units = 2,
  data_units = 3,
  origin = 4,
  spacing = 5
};

constexpr auto& surf_attrs =
    magic_enum::enum_names<h5geo::detail::SurfAttributes>();
constexpr auto origin = magic_enum::enum_name<h5geo::detail::SurfAttributes::origin>();
constexpr auto spacing = magic_enum::enum_name<h5geo::detail::SurfAttributes::spacing>();

enum class SurfDatasets : unsigned{
  surf_data = 1
};

constexpr auto& surf_dsets =
    magic_enum::enum_names<h5geo::detail::SurfDatasets>();
constexpr auto surf_data = magic_enum::enum_name<h5geo::detail::SurfDatasets::surf_data>();

enum class WellAttributes : unsigned{
  spatial_units = 1,
  head_coord = 2,
  KB = 3,
  UWI = 4
};

constexpr auto& well_attrs =
    magic_enum::enum_names<h5geo::detail::WellAttributes>();
constexpr auto head_coord = magic_enum::enum_name<h5geo::detail::WellAttributes::head_coord>();
constexpr auto KB = magic_enum::enum_name<h5geo::detail::WellAttributes::KB>();
constexpr auto UWI = magic_enum::enum_name<h5geo::detail::WellAttributes::UWI>();

enum class WellGroups : unsigned{
  LOG = 1,
  DEV = 2,
  ACTIVE_DEV = 3
};

constexpr auto& well_groups =
    magic_enum::enum_names<h5geo::detail::WellGroups>();
constexpr auto LOG = magic_enum::enum_name<h5geo::detail::WellGroups::LOG>();
constexpr auto DEV = magic_enum::enum_name<h5geo::detail::WellGroups::DEV>();
constexpr auto ACTIVE_DEV = magic_enum::enum_name<h5geo::detail::WellGroups::ACTIVE_DEV>();

enum class DevAttributes : unsigned{
  spatial_units = 1,
  temporal_units = 2,
  angle_units = 3
};

constexpr auto& dev_attrs =
    magic_enum::enum_names<h5geo::detail::DevAttributes>();
constexpr auto angle_units = magic_enum::enum_name<h5geo::detail::DevAttributes::angle_units>();

enum class DevDatasets : unsigned{
  dev_data = 1
};

constexpr auto& dev_dsets =
    magic_enum::enum_names<h5geo::detail::DevDatasets>();
constexpr auto dev_data = magic_enum::enum_name<h5geo::detail::DevDatasets::dev_data>();

enum class LogAttributes : unsigned{
  spatial_units = 1,
  data_units = 2
};

constexpr auto& log_attrs =
    magic_enum::enum_names<h5geo::detail::LogAttributes>();

enum class LogDatasets : unsigned{
  log_data = 1
};

constexpr auto& log_dsets =
    magic_enum::enum_names<h5geo::detail::LogDatasets>();
constexpr auto log_data = magic_enum::enum_name<h5geo::detail::LogDatasets::log_data>();


}


enum class ContainerType : unsigned{
  SURFACE = 1,
  WELL = 2,
  SEISMIC = 3
};
//ENABLE_BITMASK_OPERATORS(ContainerType);

constexpr auto& cnt_types =
    magic_enum::enum_names<h5geo::ContainerType>();
constexpr auto SURFACE = magic_enum::enum_name<h5geo::ContainerType::SURFACE>();
constexpr auto WELL = magic_enum::enum_name<h5geo::ContainerType::WELL>();
constexpr auto SEISMIC = magic_enum::enum_name<h5geo::ContainerType::SEISMIC>();

enum class ObjectType: unsigned{
  SURFACE = 1,
  WELL = 2,
  LOGCURVE = 3,
  DEVCURVE = 4,
  SEISMIC = 5
};
//ENABLE_BITMASK_OPERATORS(ObjectType);

constexpr auto& obj_types =
    magic_enum::enum_names<h5geo::ObjectType>();
constexpr auto LOGCURVE = magic_enum::enum_name<h5geo::ObjectType::LOGCURVE>();
constexpr auto DEVCURVE = magic_enum::enum_name<h5geo::ObjectType::DEVCURVE>();

enum class Domain : unsigned{
  MD = 1,
  TVD = 2,
  TVDSS = 4,
  TVDSD = 8,
  TWT = 16,
  OWT = 32
};

constexpr auto& domains =
    magic_enum::enum_names<h5geo::Domain>();
constexpr auto MD = magic_enum::enum_name<h5geo::Domain::MD>();
constexpr auto TVD = magic_enum::enum_name<h5geo::Domain::TVD>();
constexpr auto TVDSS = magic_enum::enum_name<h5geo::Domain::TVDSS>();
constexpr auto TVDSD = magic_enum::enum_name<h5geo::Domain::TVDSD>();
constexpr auto TWT = magic_enum::enum_name<h5geo::Domain::TWT>();
constexpr auto OWT = magic_enum::enum_name<h5geo::Domain::OWT>();

enum class SeisDataType : unsigned{
  STACK = 1,
  PRESTACK = 2,
  ATTRIBUTE = 3
};

constexpr auto& seis_dtypes =
    magic_enum::enum_names<h5geo::SeisDataType>();
constexpr auto STACK = magic_enum::enum_name<h5geo::SeisDataType::STACK>();
constexpr auto PRESTACK = magic_enum::enum_name<h5geo::SeisDataType::PRESTACK>();
constexpr auto ATTRIBUTE = magic_enum::enum_name<h5geo::SeisDataType::ATTRIBUTE>();

enum class SurveyType : unsigned{
  TWO_D = 1,
  THREE_D = 2
};

constexpr auto& survey_types =
    magic_enum::enum_names<h5geo::SurveyType>();
constexpr auto TWO_D = magic_enum::enum_name<h5geo::SurveyType::TWO_D>();
constexpr auto THREE_D = magic_enum::enum_name<h5geo::SurveyType::THREE_D>();

enum class TxtEncoding : unsigned{
  ASCII = 0,
  EBCDIC = 1
};

constexpr auto& txt_encodings =
    magic_enum::enum_names<h5geo::TxtEncoding>();
constexpr auto ASCII = magic_enum::enum_name<h5geo::TxtEncoding::ASCII>();
constexpr auto EBCDIC = magic_enum::enum_name<h5geo::TxtEncoding::EBCDIC>();

enum class SegyEndian : unsigned{
  Little = 0,
  Big = 1
};

constexpr auto& segy_endians =
    magic_enum::enum_names<h5geo::SegyEndian>();
constexpr auto Little = magic_enum::enum_name<h5geo::SegyEndian::Little>();
constexpr auto Big = magic_enum::enum_name<h5geo::SegyEndian::Big>();

enum class SegyFormat : unsigned{
  FourByte_IBM = 0,
  FourByte_IEEE = 1,
  FourByte_integer = 2
};

constexpr auto& segy_formats =
    magic_enum::enum_names<h5geo::SegyFormat>();
constexpr auto FourByte_IBM = magic_enum::enum_name<h5geo::SegyFormat::FourByte_IBM>();
constexpr auto FourByte_IEEE = magic_enum::enum_name<h5geo::SegyFormat::FourByte_IEEE>();
constexpr auto FourByte_integer = magic_enum::enum_name<h5geo::SegyFormat::FourByte_integer>();

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

constexpr auto& well_dtypes =
    magic_enum::enum_names<h5geo::WellDataType>();
constexpr auto DEV = magic_enum::enum_name<h5geo::WellDataType::DEV>();
constexpr auto LOG = magic_enum::enum_name<h5geo::WellDataType::LOG>();
constexpr auto INTERVAL_VEL = magic_enum::enum_name<h5geo::WellDataType::INTERVAL_VEL>();
constexpr auto AVG_VEL = magic_enum::enum_name<h5geo::WellDataType::AVG_VEL>();
constexpr auto RMS_VEL = magic_enum::enum_name<h5geo::WellDataType::RMS_VEL>();
constexpr auto FWAL = magic_enum::enum_name<h5geo::WellDataType::FWAL>();
constexpr auto CHECKSHOTS = magic_enum::enum_name<h5geo::WellDataType::CHECKSHOTS>();
constexpr auto WELL_TIE = magic_enum::enum_name<h5geo::WellDataType::WELL_TIE>();

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
