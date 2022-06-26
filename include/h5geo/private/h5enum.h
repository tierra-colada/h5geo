#ifndef H5NUM_H
#define H5NUM_H

#include <type_traits>

#include <h5gt/H5DataType.hpp>

#include "h5enum_operators.h"

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

typedef std::underlying_type<ContainerAttributes>::type ContainerAttributesUType;
inline h5gt::EnumType<ContainerAttributesUType> create_enum_ContainerAttributes() {
  return {{"ContainerType", static_cast<ContainerAttributesUType>(ContainerAttributes::ContainerType)}};
}

enum class PointsAttributes : unsigned{
  Domain = 1
};

typedef std::underlying_type<PointsAttributes>::type PointsAttributesUType;
inline h5gt::EnumType<PointsAttributesUType> create_enum_PointsAttributes() {
  return {{"Domain", static_cast<PointsAttributesUType>(PointsAttributes::Domain)}};
}

enum class PointsDatasets : unsigned{
  points_data = 1
};

typedef std::underlying_type<PointsDatasets>::type PointsDatasetsUType;
inline h5gt::EnumType<PointsDatasetsUType> create_enum_PointsDatasets() {
  return {{"points_data", static_cast<PointsDatasetsUType>(PointsDatasets::points_data)}};
}

enum class SeisAttributes : unsigned{
  Domain = 1,
  SeisDataType = 2,
  SurveyType = 3,
  SRD = 4
};

typedef std::underlying_type<SeisAttributes>::type SeisAttributesUType;
inline h5gt::EnumType<SeisAttributesUType> create_enum_SeisAttributes() {
  return {{"Domain", static_cast<SeisAttributesUType>(SeisAttributes::Domain)},
          {"SeisDataType", static_cast<SeisAttributesUType>(SeisAttributes::SeisDataType)},
          {"SurveyType", static_cast<SeisAttributesUType>(SeisAttributes::SurveyType)},
          {"SRD", static_cast<SeisAttributesUType>(SeisAttributes::SRD)}};
}

enum class SeisDatasets : unsigned{
  text_header = 1,
  bin_header = 2,
  trace_header = 3,
  trace = 4,
  boundary = 5
};

typedef std::underlying_type<SeisDatasets>::type SeisDatasetsUType;
inline h5gt::EnumType<SeisDatasetsUType> create_enum_SeisDatasets() {
  return {{"text_header", static_cast<SeisDatasetsUType>(SeisDatasets::text_header)},
          {"bin_header", static_cast<SeisDatasetsUType>(SeisDatasets::bin_header)},
          {"trace_header", static_cast<SeisDatasetsUType>(SeisDatasets::trace_header)},
          {"trace", static_cast<SeisDatasetsUType>(SeisDatasets::trace)},
          {"boundary", static_cast<SeisDatasetsUType>(SeisDatasets::boundary)}};
}

enum class SeisGroups : unsigned{
  sort = 1,
  indexes = 2,
  unique_values = 3
};

typedef std::underlying_type<SeisGroups>::type SeisGroupsUType;
inline h5gt::EnumType<SeisGroupsUType> create_enum_SeisGroups() {
  return {{"sort", static_cast<SeisGroupsUType>(SeisGroups::sort)},
          {"indexes", static_cast<SeisGroupsUType>(SeisGroups::indexes)},
          {"unique_values", static_cast<SeisGroupsUType>(SeisGroups::unique_values)}};
}

enum class SeisSEGYGroups : unsigned{
  segy = 1
};

typedef std::underlying_type<SeisSEGYGroups>::type SeisSEGYGroupsUType;
inline h5gt::EnumType<SeisSEGYGroupsUType> create_enum_SeisSEGYGroups() {
  return {{"segy", static_cast<SeisSEGYGroupsUType>(SeisSEGYGroups::segy)}};
}

enum class SeisSEGYDatasets : unsigned{
  text_header = 1,
  bin_header_2bytes = 2,
  bin_header_4bytes = 3,
  trace_header_2bytes = 4,
  trace_header_4bytes = 5,
  trace_float = 6
};

typedef std::underlying_type<SeisSEGYDatasets>::type SeisSEGYDatasetsUType;
inline h5gt::EnumType<SeisSEGYDatasetsUType> create_enum_SeisSEGYDatasets() {
  return {{"text_header", static_cast<SeisSEGYDatasetsUType>(SeisSEGYDatasets::text_header)},
          {"bin_header_2bytes", static_cast<SeisSEGYDatasetsUType>(SeisSEGYDatasets::bin_header_2bytes)},
          {"bin_header_4bytes", static_cast<SeisSEGYDatasetsUType>(SeisSEGYDatasets::bin_header_4bytes)},
          {"trace_header_2bytes", static_cast<SeisSEGYDatasetsUType>(SeisSEGYDatasets::trace_header_2bytes)},
          {"trace_header_4bytes", static_cast<SeisSEGYDatasetsUType>(SeisSEGYDatasets::trace_header_4bytes)},
          {"trace_float", static_cast<SeisSEGYDatasetsUType>(SeisSEGYDatasets::trace_float)}};
}

enum class MapAttributes : unsigned{
  Domain = 1,
  origin = 2,
  point1 = 3,
  point2 = 4
};

typedef std::underlying_type<MapAttributes>::type MapAttributesUType;
inline h5gt::EnumType<MapAttributesUType> create_enum_MapAttributes() {
  return {{"Domain", static_cast<MapAttributesUType>(MapAttributes::Domain)},
          {"origin", static_cast<MapAttributesUType>(MapAttributes::origin)},
          {"point1", static_cast<MapAttributesUType>(MapAttributes::point1)},
          {"point2", static_cast<MapAttributesUType>(MapAttributes::point2)}};
}

enum class MapDatasets : unsigned{
  map_data = 1
};

typedef std::underlying_type<MapDatasets>::type MapDatasetsUType;
inline h5gt::EnumType<MapDatasetsUType> create_enum_MapDatasets() {
  return {{"map_data", static_cast<MapDatasetsUType>(MapDatasets::map_data)}};
}

enum class WellAttributes : unsigned{
  head_coord = 1,
  KB = 2,
  UWI = 3
};

typedef std::underlying_type<WellAttributes>::type WellAttributesUType;
inline h5gt::EnumType<WellAttributesUType> create_enum_WellAttributes() {
  return {{"head_coord", static_cast<WellAttributesUType>(WellAttributes::head_coord)},
          {"KB", static_cast<WellAttributesUType>(WellAttributes::KB)},
          {"UWI", static_cast<WellAttributesUType>(WellAttributes::UWI)}};
}

enum class WellGroups : unsigned{
  LOG = 1,
  DEV = 2
};

typedef std::underlying_type<WellGroups>::type WellGroupsUType;
inline h5gt::EnumType<WellGroupsUType> create_enum_WellGroups() {
  return {{"LOG", static_cast<WellGroupsUType>(WellGroups::LOG)},
          {"DEV", static_cast<WellGroupsUType>(WellGroups::DEV)}};
}

enum class DevDatasets : unsigned{
  dev_data = 1
};

typedef std::underlying_type<DevDatasets>::type DevDatasetsUType;
inline h5gt::EnumType<DevDatasetsUType> create_enum_DevDatasets() {
  return {{"dev_data", static_cast<DevDatasetsUType>(DevDatasets::dev_data)}};
}

enum class LogDatasets : unsigned{
  log_data = 1
};

typedef std::underlying_type<LogDatasets>::type LogDatasetsUType;
inline h5gt::EnumType<LogDatasetsUType> create_enum_LogDatasets() {
  return {{"log_data", static_cast<LogDatasetsUType>(LogDatasets::log_data)}};
}

} // detail


enum class ContainerType : unsigned{
  MAP = 1,
  WELL = 2,
  SEISMIC = 3
};
//ENABLE_BITMASK_OPERATORS(ContainerType);

typedef std::underlying_type<ContainerType>::type ContainerTypeUType;
inline h5gt::EnumType<ContainerTypeUType> create_enum_ContainerType() {
  return {{"MAP", static_cast<ContainerTypeUType>(ContainerType::MAP)},
          {"WELL", static_cast<ContainerTypeUType>(ContainerType::WELL)},
          {"SEISMIC", static_cast<ContainerTypeUType>(ContainerType::SEISMIC)}};
}

enum class ObjectType: unsigned{
  MAP = 1,
  WELL = 2,
  LOGCURVE = 3,
  DEVCURVE = 4,
  SEISMIC = 5,
  POINTS_1 = 6,
  POINTS_2 = 7,
  POINTS_3 = 8,
  POINTS_4 = 9,
  WELLTOPS = 10
};
//ENABLE_BITMASK_OPERATORS(ObjectType);

typedef std::underlying_type<ObjectType>::type ObjectTypeUType;
inline h5gt::EnumType<ObjectTypeUType> create_enum_ObjectType() {
  return {{"MAP", static_cast<ObjectTypeUType>(ObjectType::MAP)},
          {"WELL", static_cast<ObjectTypeUType>(ObjectType::WELL)},
          {"LOGCURVE", static_cast<ObjectTypeUType>(ObjectType::LOGCURVE)},
          {"DEVCURVE", static_cast<ObjectTypeUType>(ObjectType::DEVCURVE)},
          {"SEISMIC", static_cast<ObjectTypeUType>(ObjectType::SEISMIC)},
          {"POINTS_1", static_cast<ObjectTypeUType>(ObjectType::POINTS_1)},
          {"POINTS_2", static_cast<ObjectTypeUType>(ObjectType::POINTS_2)},
          {"POINTS_3", static_cast<ObjectTypeUType>(ObjectType::POINTS_3)},
          {"POINTS_4", static_cast<ObjectTypeUType>(ObjectType::POINTS_4)},
          {"WELLTOPS", static_cast<ObjectTypeUType>(ObjectType::WELLTOPS)}};
}

enum class Domain : unsigned{
  TVD = 1,
  TVDSS = 2,
  TWT = 4,
  OWT = 8
};

typedef std::underlying_type<Domain>::type DomainUType;
inline h5gt::EnumType<DomainUType> create_enum_Domain() {
  return {{"OWT", static_cast<DomainUType>(Domain::OWT)},
          {"TWT", static_cast<DomainUType>(Domain::TWT)},
          {"TVD", static_cast<DomainUType>(Domain::TVD)},
          {"TVDSS", static_cast<DomainUType>(Domain::TVDSS)}};
}

enum class SeisDataType : unsigned{
  STACK = 1,
  PRESTACK = 2
};

typedef std::underlying_type<SeisDataType>::type SeisDataTypeUType;
inline h5gt::EnumType<SeisDataTypeUType> create_enum_SeisDataType() {
  return {{"STACK", static_cast<SeisDataTypeUType>(SeisDataType::STACK)},
          {"PRESTACK", static_cast<SeisDataTypeUType>(SeisDataType::PRESTACK)}};
}

enum class SurveyType : unsigned{
  TWO_D = 1,
  THREE_D = 2
};

typedef std::underlying_type<SurveyType>::type SurveyTypeUType;
inline h5gt::EnumType<SurveyTypeUType> create_enum_SurveyType() {
  return {{"TWO_D", static_cast<SurveyTypeUType>(SurveyType::TWO_D)},
          {"THREE_D", static_cast<SurveyTypeUType>(SurveyType::THREE_D)}};
}

enum class TextEncoding : unsigned{
  ASCII = 1,
  EBCDIC = 2
};

typedef std::underlying_type<TextEncoding>::type TextEncodingUType;
inline h5gt::EnumType<TextEncodingUType> create_enum_TextEncoding() {
  return {{"ASCII", static_cast<TextEncodingUType>(TextEncoding::ASCII)},
          {"EBCDIC", static_cast<TextEncodingUType>(TextEncoding::EBCDIC)}};
}

enum class Endian : unsigned{
  Little = 1,
  Big = 2
};

typedef std::underlying_type<Endian>::type EndianUType;
inline h5gt::EnumType<EndianUType> create_enum_Endian() {
  return {{"Little", static_cast<EndianUType>(Endian::Little)},
          {"Big", static_cast<EndianUType>(Endian::Big)}};
}

enum class SegyFormat : unsigned{
  FourByte_IBM = 1,
  FourByte_IEEE = 2,
  FourByte_integer = 3
};

typedef std::underlying_type<SegyFormat>::type SegyFormatUType;
inline h5gt::EnumType<SegyFormatUType> create_enum_SegyFormat() {
  return {{"FourByte_IBM", static_cast<SegyFormatUType>(SegyFormat::FourByte_IBM)},
          {"FourByte_IEEE", static_cast<SegyFormatUType>(SegyFormat::FourByte_IEEE)},
          {"FourByte_integer", static_cast<SegyFormatUType>(SegyFormat::FourByte_integer)}};
}

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

typedef std::underlying_type<WellDataType>::type WellDataTypeUType;
inline h5gt::EnumType<WellDataTypeUType> create_enum_WellDataType() {
  return {{"WELL", static_cast<WellDataTypeUType>(WellDataType::WELL)},
          {"DEV", static_cast<WellDataTypeUType>(WellDataType::DEV)},
          {"LOG", static_cast<WellDataTypeUType>(WellDataType::LOG)},
          {"INTERVAL_VEL", static_cast<WellDataTypeUType>(WellDataType::INTERVAL_VEL)},
          {"AVG_VEL", static_cast<WellDataTypeUType>(WellDataType::AVG_VEL)},
          {"RMS_VEL", static_cast<WellDataTypeUType>(WellDataType::RMS_VEL)},
          {"FWAL", static_cast<WellDataTypeUType>(WellDataType::FWAL)},
          {"CHECKSHOTS", static_cast<WellDataTypeUType>(WellDataType::CHECKSHOTS)},
          {"WELL_TIE", static_cast<WellDataTypeUType>(WellDataType::WELL_TIE)}};
}

enum class WellName : unsigned{
  FROM_LAS = 0,
  FROM_FILE_NAME = 1
};

typedef std::underlying_type<WellName>::type WellNameUType;
inline h5gt::EnumType<WellNameUType> create_enum_WellName() {
  return {{"FROM_LAS", static_cast<WellNameUType>(WellName::FROM_LAS)},
          {"FROM_FILE_NAME", static_cast<WellNameUType>(WellName::FROM_FILE_NAME)}};
}

enum class LogDataType : unsigned{
  MD = 1,
  VAL = 2
};

typedef std::underlying_type<LogDataType>::type LogDataTypeUType;
inline h5gt::EnumType<LogDataTypeUType> create_enum_LogDataType() {
  return {{"MD", static_cast<LogDataTypeUType>(LogDataType::MD)},
          {"VAL", static_cast<LogDataTypeUType>(LogDataType::VAL)}};
}

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

typedef std::underlying_type<DevDataType>::type DevDataTypeUType;
inline h5gt::EnumType<DevDataTypeUType> create_enum_DevDataType() {
  return {{"MD", static_cast<DevDataTypeUType>(DevDataType::MD)},
          {"X", static_cast<DevDataTypeUType>(DevDataType::X)},
          {"Y", static_cast<DevDataTypeUType>(DevDataType::Y)},
          {"Z", static_cast<DevDataTypeUType>(DevDataType::Z)},
          {"TVD", static_cast<DevDataTypeUType>(DevDataType::TVD)},
          {"TVDSS", static_cast<DevDataTypeUType>(DevDataType::TVDSS)},
          {"DX", static_cast<DevDataTypeUType>(DevDataType::DX)},
          {"DY", static_cast<DevDataTypeUType>(DevDataType::DY)},
          {"AZIM", static_cast<DevDataTypeUType>(DevDataType::AZIM)},
          {"INCL", static_cast<DevDataTypeUType>(DevDataType::INCL)},
          {"OWT", static_cast<DevDataTypeUType>(DevDataType::OWT)},
          {"TWT", static_cast<DevDataTypeUType>(DevDataType::TWT)}};
}

enum class WellType : unsigned{
  PROPOSED = 1,
  DRY = 2,
  OIL = 4,
  GAS = 8,
  WATER = 16,
  CONDENSATE = 32
};
//ENABLE_BITMASK_OPERATORS(WellType);

typedef std::underlying_type<WellType>::type WellTypeUType;
inline h5gt::EnumType<WellTypeUType> create_enum_WellType() {
  return {{"PROPOSED", static_cast<WellTypeUType>(WellType::PROPOSED)},
          {"DRY", static_cast<WellTypeUType>(WellType::DRY)},
          {"OIL", static_cast<WellTypeUType>(WellType::OIL)},
          {"GAS", static_cast<WellTypeUType>(WellType::GAS)},
          {"WATER", static_cast<WellTypeUType>(WellType::WATER)},
          {"CONDENSATE", static_cast<WellTypeUType>(WellType::CONDENSATE)}};
}

enum class TrajectoryFormat : unsigned{
  MD_AZIM_INCL = 1,
  TVD_X_Y = 2,
  TVD_DX_DY = 3,
  TVDSS_X_Y = 4,
  TVDSS_DX_DY = 5
};

typedef std::underlying_type<TrajectoryFormat>::type TrajectoryFormatUType;
inline h5gt::EnumType<TrajectoryFormatUType> create_enum_TrajectoryFormat() {
  return {{"MD_AZIM_INCL", static_cast<TrajectoryFormatUType>(TrajectoryFormat::MD_AZIM_INCL)},
          {"TVD_X_Y", static_cast<TrajectoryFormatUType>(TrajectoryFormat::TVD_X_Y)},
          {"TVD_DX_DY", static_cast<TrajectoryFormatUType>(TrajectoryFormat::TVD_DX_DY)},
          {"TVDSS_X_Y", static_cast<TrajectoryFormatUType>(TrajectoryFormat::TVDSS_X_Y)},
          {"TVDSS_DX_DY", static_cast<TrajectoryFormatUType>(TrajectoryFormat::TVDSS_DX_DY)}};
}

enum class CreationType : unsigned{
  OPEN = 1, /*!< Open if exist */
  CREATE = 2, /*!< Create if not exist */
  OPEN_OR_CREATE = 3, /*!< Open if exist or Create New */
  CREATE_OR_OVERWRITE = 4, /*!< Create if not exist or Overwrite (Overwrite == delete existing and create new) */
  CREATE_UNDER_NEW_NAME = 5 /*!< Always create under New Unique Name */
};

typedef std::underlying_type<CreationType>::type CreationTypeUType;
inline h5gt::EnumType<CreationTypeUType> create_enum_CreationType() {
  return {{"OPEN", static_cast<CreationTypeUType>(CreationType::OPEN)},
          {"CREATE", static_cast<CreationTypeUType>(CreationType::CREATE)},
          {"OPEN_OR_CREATE", static_cast<CreationTypeUType>(CreationType::OPEN_OR_CREATE)},
          {"CREATE_OR_OVERWRITE", static_cast<CreationTypeUType>(CreationType::CREATE_OR_OVERWRITE)},
          {"CREATE_UNDER_NEW_NAME", static_cast<CreationTypeUType>(CreationType::CREATE_UNDER_NEW_NAME)}};
}

enum class CaseSensitivity : unsigned{
  CASE_SENSITIVE = 1,
  CASE_INSENSITIVE = 2
};

typedef std::underlying_type<CaseSensitivity>::type CaseSensitivityUType;
inline h5gt::EnumType<CaseSensitivityUType> create_enum_CaseSensitivity() {
  return {{"CASE_SENSITIVE", static_cast<CaseSensitivityUType>(CaseSensitivity::CASE_SENSITIVE)},
          {"CASE_INSENSITIVE", static_cast<CaseSensitivityUType>(CaseSensitivity::CASE_INSENSITIVE)}};
}

enum class Delimiter : unsigned{
  TABULATION = 1,
  SEMICOLON = 2,
  DOT = 4,
  SPACE = 8,
  COMMA = 16
};
ENABLE_BITMASK_OPERATORS(Delimiter);

typedef std::underlying_type<Delimiter>::type DelimiterUType;
inline h5gt::EnumType<DelimiterUType> create_enum_Delimiter() {
  return {{"TABULATION", static_cast<DelimiterUType>(Delimiter::TABULATION)},
          {"SEMICOLON", static_cast<DelimiterUType>(Delimiter::SEMICOLON)},
          {"DOT", static_cast<DelimiterUType>(Delimiter::DOT)},
          {"SPACE", static_cast<DelimiterUType>(Delimiter::SPACE)},
          {"COMMA", static_cast<DelimiterUType>(Delimiter::COMMA)}};
}


} // h5geo


H5GT_REGISTER_TYPE(h5geo::detail::ContainerAttributes, h5geo::detail::create_enum_ContainerAttributes)
H5GT_REGISTER_TYPE(h5geo::detail::PointsAttributes, h5geo::detail::create_enum_PointsAttributes)
H5GT_REGISTER_TYPE(h5geo::detail::PointsDatasets, h5geo::detail::create_enum_PointsDatasets)
H5GT_REGISTER_TYPE(h5geo::detail::SeisAttributes, h5geo::detail::create_enum_SeisAttributes)
H5GT_REGISTER_TYPE(h5geo::detail::SeisDatasets, h5geo::detail::create_enum_SeisDatasets)
H5GT_REGISTER_TYPE(h5geo::detail::SeisGroups, h5geo::detail::create_enum_SeisGroups)
H5GT_REGISTER_TYPE(h5geo::detail::SeisSEGYGroups, h5geo::detail::create_enum_SeisSEGYGroups)
H5GT_REGISTER_TYPE(h5geo::detail::SeisSEGYDatasets, h5geo::detail::create_enum_SeisSEGYDatasets)
H5GT_REGISTER_TYPE(h5geo::detail::MapAttributes, h5geo::detail::create_enum_MapAttributes)
H5GT_REGISTER_TYPE(h5geo::detail::MapDatasets, h5geo::detail::create_enum_MapDatasets)
H5GT_REGISTER_TYPE(h5geo::detail::WellAttributes, h5geo::detail::create_enum_WellAttributes)
H5GT_REGISTER_TYPE(h5geo::detail::WellGroups, h5geo::detail::create_enum_WellGroups)
H5GT_REGISTER_TYPE(h5geo::detail::DevDatasets, h5geo::detail::create_enum_DevDatasets)
H5GT_REGISTER_TYPE(h5geo::detail::LogDatasets, h5geo::detail::create_enum_LogDatasets)

H5GT_REGISTER_TYPE(h5geo::ContainerType, h5geo::create_enum_ContainerType)
H5GT_REGISTER_TYPE(h5geo::ObjectType, h5geo::create_enum_ObjectType)
H5GT_REGISTER_TYPE(h5geo::Domain, h5geo::create_enum_Domain)
H5GT_REGISTER_TYPE(h5geo::SeisDataType, h5geo::create_enum_SeisDataType)
H5GT_REGISTER_TYPE(h5geo::SurveyType, h5geo::create_enum_SurveyType)
H5GT_REGISTER_TYPE(h5geo::TextEncoding, h5geo::create_enum_TextEncoding)
H5GT_REGISTER_TYPE(h5geo::Endian, h5geo::create_enum_Endian)
H5GT_REGISTER_TYPE(h5geo::SegyFormat, h5geo::create_enum_SegyFormat)
H5GT_REGISTER_TYPE(h5geo::WellDataType, h5geo::create_enum_WellDataType)
H5GT_REGISTER_TYPE(h5geo::WellName, h5geo::create_enum_WellName)
H5GT_REGISTER_TYPE(h5geo::LogDataType, h5geo::create_enum_LogDataType)
H5GT_REGISTER_TYPE(h5geo::DevDataType, h5geo::create_enum_DevDataType)
H5GT_REGISTER_TYPE(h5geo::WellType, h5geo::create_enum_WellType)
H5GT_REGISTER_TYPE(h5geo::TrajectoryFormat, h5geo::create_enum_TrajectoryFormat)
H5GT_REGISTER_TYPE(h5geo::CreationType, h5geo::create_enum_CreationType)
H5GT_REGISTER_TYPE(h5geo::CaseSensitivity, h5geo::create_enum_CaseSensitivity)
H5GT_REGISTER_TYPE(h5geo::Delimiter, h5geo::create_enum_Delimiter)


#endif // H5CORE_ENUM_H
