#ifndef H5CORE_ENUM_STRING_H
#define H5CORE_ENUM_STRING_H

#include "../impl/h5enum.h"

#include <type_traits> // for enum class operators
#include <string>

#if !defined(MAGIC_ENUM_RANGE_MIN)
#define MAGIC_ENUM_RANGE_MIN 0
#endif

#if !defined(MAGIC_ENUM_RANGE_MAX)
#define MAGIC_ENUM_RANGE_MAX 256
#endif

#include <magic_enum.hpp>


/* DONT use `magic_enum::enum_name<Enum::val>()`!!!
 * Use only `magic_enum::enum_name(Enum::val)` and `magic_enum::enum_names<EnumClass>()`
 * If can't decide look in `magic_enum` examples and stricly follow them!
 * Otherwise you will get very odd test fails with VS 2017 for example
 * (don't know why) */

namespace h5geo {


namespace detail {

inline constexpr auto spatial_reference = "spatial_reference";
inline constexpr auto length_units = "length_units";
inline constexpr auto temporal_units = "temporal_units";
inline constexpr auto data_units = "data_units";
inline constexpr auto angular_units = "angular_units";

inline constexpr auto& cnt_attrs =
    magic_enum::enum_names<h5geo::detail::ContainerAttributes>();
inline constexpr auto ContainerType = magic_enum::enum_name(h5geo::detail::ContainerAttributes::ContainerType);
inline constexpr auto& points_attrs =
    magic_enum::enum_names<h5geo::detail::PointsAttributes>();
inline constexpr auto& points_dsets =
    magic_enum::enum_names<h5geo::detail::PointsDatasets>();
inline constexpr auto points_data = magic_enum::enum_name(h5geo::detail::PointsDatasets::points_data);
inline constexpr auto& seis_attrs =
    magic_enum::enum_names<h5geo::detail::SeisAttributes>();
inline constexpr auto Domain = magic_enum::enum_name(h5geo::detail::SeisAttributes::Domain);
inline constexpr auto SeisDataType = magic_enum::enum_name(h5geo::detail::SeisAttributes::SeisDataType);
inline constexpr auto SurveyType = magic_enum::enum_name(h5geo::detail::SeisAttributes::SurveyType);
inline constexpr auto SRD = magic_enum::enum_name(h5geo::detail::SeisAttributes::SRD);
inline constexpr auto& seis_dsets =
    magic_enum::enum_names<h5geo::detail::SeisDatasets>();
inline constexpr auto text_header = magic_enum::enum_name(h5geo::detail::SeisDatasets::text_header);
inline constexpr auto bin_header = magic_enum::enum_name(h5geo::detail::SeisDatasets::bin_header);
inline constexpr auto trace_header = magic_enum::enum_name(h5geo::detail::SeisDatasets::trace_header);
inline constexpr auto trace = magic_enum::enum_name(h5geo::detail::SeisDatasets::trace);
inline constexpr auto boundary = magic_enum::enum_name(h5geo::detail::SeisDatasets::boundary);
inline constexpr auto& seis_groups =
    magic_enum::enum_names<h5geo::detail::SeisGroups>();
inline constexpr auto sort = magic_enum::enum_name(h5geo::detail::SeisGroups::sort);
inline constexpr auto indexes = magic_enum::enum_name(h5geo::detail::SeisGroups::indexes);
inline constexpr auto unique_values = magic_enum::enum_name(h5geo::detail::SeisGroups::unique_values);
inline constexpr auto& seis_segy_groups =
    magic_enum::enum_names<h5geo::detail::SeisSEGYGroups>();
inline constexpr auto segy = magic_enum::enum_name(h5geo::detail::SeisSEGYGroups::segy);
inline constexpr auto& seis_segy_dsets =
    magic_enum::enum_names<h5geo::detail::SeisSEGYDatasets>();
inline constexpr auto bin_header_2bytes = magic_enum::enum_name(h5geo::detail::SeisSEGYDatasets::bin_header_2bytes);
inline constexpr auto bin_header_4bytes = magic_enum::enum_name(h5geo::detail::SeisSEGYDatasets::bin_header_4bytes);
inline constexpr auto trace_header_2bytes = magic_enum::enum_name(h5geo::detail::SeisSEGYDatasets::trace_header_2bytes);
inline constexpr auto trace_header_4bytes = magic_enum::enum_name(h5geo::detail::SeisSEGYDatasets::trace_header_4bytes);
inline constexpr auto trace_float = magic_enum::enum_name(h5geo::detail::SeisSEGYDatasets::trace_float);
inline constexpr auto& map_attrs =
    magic_enum::enum_names<h5geo::detail::MapAttributes>();
inline constexpr auto origin = magic_enum::enum_name(h5geo::detail::MapAttributes::origin);
inline constexpr auto point1 = magic_enum::enum_name(h5geo::detail::MapAttributes::point1);
inline constexpr auto point2 = magic_enum::enum_name(h5geo::detail::MapAttributes::point2);
inline constexpr auto& map_dsets =
    magic_enum::enum_names<h5geo::detail::MapDatasets>();
inline constexpr auto map_data = magic_enum::enum_name(h5geo::detail::MapDatasets::map_data);
inline constexpr auto& well_attrs =
    magic_enum::enum_names<h5geo::detail::WellAttributes>();
inline constexpr auto head_coord = magic_enum::enum_name(h5geo::detail::WellAttributes::head_coord);
inline constexpr auto KB = magic_enum::enum_name(h5geo::detail::WellAttributes::KB);
inline constexpr auto UWI = magic_enum::enum_name(h5geo::detail::WellAttributes::UWI);
inline constexpr auto& well_groups =
    magic_enum::enum_names<h5geo::detail::WellGroups>();
inline constexpr auto LOG = magic_enum::enum_name(h5geo::detail::WellGroups::LOG);
inline constexpr auto DEV = magic_enum::enum_name(h5geo::detail::WellGroups::DEV);
inline constexpr auto ACTIVE = "ACTIVE";
inline constexpr auto& dev_dsets =
    magic_enum::enum_names<h5geo::detail::DevDatasets>();
inline constexpr auto dev_data = magic_enum::enum_name(h5geo::detail::DevDatasets::dev_data);
inline constexpr auto& log_dsets =
    magic_enum::enum_names<h5geo::detail::LogDatasets>();
inline constexpr auto log_data = magic_enum::enum_name(h5geo::detail::LogDatasets::log_data);



} // detail

inline constexpr auto& cnt_types =
    magic_enum::enum_names<h5geo::ContainerType>();
inline constexpr auto MAP = magic_enum::enum_name(h5geo::ContainerType::MAP);
inline constexpr auto WELL = magic_enum::enum_name(h5geo::ContainerType::WELL);
inline constexpr auto SEISMIC = magic_enum::enum_name(h5geo::ContainerType::SEISMIC);
inline constexpr auto& obj_types =
    magic_enum::enum_names<h5geo::ObjectType>();
inline constexpr auto LOGCURVE = magic_enum::enum_name(h5geo::ObjectType::LOGCURVE);
inline constexpr auto DEVCURVE = magic_enum::enum_name(h5geo::ObjectType::DEVCURVE);
inline constexpr auto WELLTOPS = magic_enum::enum_name(h5geo::ObjectType::WELLTOPS);
inline constexpr auto& domains =
    magic_enum::enum_names<h5geo::Domain>();
inline constexpr auto TVD = magic_enum::enum_name(h5geo::Domain::TVD);
inline constexpr auto TVDSS = magic_enum::enum_name(h5geo::Domain::TVDSS);
inline constexpr auto TWT = magic_enum::enum_name(h5geo::Domain::TWT);
inline constexpr auto OWT = magic_enum::enum_name(h5geo::Domain::OWT);
inline constexpr auto& seis_dtypes =
    magic_enum::enum_names<h5geo::SeisDataType>();
inline constexpr auto STACK = magic_enum::enum_name(h5geo::SeisDataType::STACK);
inline constexpr auto PRESTACK = magic_enum::enum_name(h5geo::SeisDataType::PRESTACK);
inline constexpr auto& survey_types =
    magic_enum::enum_names<h5geo::SurveyType>();
inline constexpr auto TWO_D = magic_enum::enum_name(h5geo::SurveyType::TWO_D);
inline constexpr auto THREE_D = magic_enum::enum_name(h5geo::SurveyType::THREE_D);
inline constexpr auto& text_encodings =
    magic_enum::enum_names<h5geo::TextEncoding>();
inline constexpr auto ASCII = magic_enum::enum_name(h5geo::TextEncoding::ASCII);
inline constexpr auto EBCDIC = magic_enum::enum_name(h5geo::TextEncoding::EBCDIC);
inline constexpr auto& segy_endians =
    magic_enum::enum_names<h5geo::Endian>();
inline constexpr auto Little = magic_enum::enum_name(h5geo::Endian::Little);
inline constexpr auto Big = magic_enum::enum_name(h5geo::Endian::Big);
inline constexpr auto& segy_formats =
    magic_enum::enum_names<h5geo::SegyFormat>();
inline constexpr auto FourByte_IBM = magic_enum::enum_name(h5geo::SegyFormat::FourByte_IBM);
inline constexpr auto FourByte_IEEE = magic_enum::enum_name(h5geo::SegyFormat::FourByte_IEEE);
inline constexpr auto FourByte_integer = magic_enum::enum_name(h5geo::SegyFormat::FourByte_integer);
inline constexpr auto& well_dtypes =
    magic_enum::enum_names<h5geo::WellDataType>();
inline constexpr auto DEV = magic_enum::enum_name(h5geo::WellDataType::DEV);
inline constexpr auto LOG = magic_enum::enum_name(h5geo::WellDataType::LOG);
inline constexpr auto INTERVAL_VEL = magic_enum::enum_name(h5geo::WellDataType::INTERVAL_VEL);
inline constexpr auto AVG_VEL = magic_enum::enum_name(h5geo::WellDataType::AVG_VEL);
inline constexpr auto RMS_VEL = magic_enum::enum_name(h5geo::WellDataType::RMS_VEL);
inline constexpr auto FWAL = magic_enum::enum_name(h5geo::WellDataType::FWAL);
inline constexpr auto CHECKSHOTS = magic_enum::enum_name(h5geo::WellDataType::CHECKSHOTS);
inline constexpr auto WELL_TIE = magic_enum::enum_name(h5geo::WellDataType::WELL_TIE);
inline constexpr auto& dev_dtypes =
    magic_enum::enum_names<h5geo::DevDataType>();
inline constexpr auto MD = magic_enum::enum_name(h5geo::DevDataType::MD);
inline constexpr auto X = magic_enum::enum_name(h5geo::DevDataType::X);
inline constexpr auto Y = magic_enum::enum_name(h5geo::DevDataType::Y);
inline constexpr auto Z = magic_enum::enum_name(h5geo::DevDataType::Z);
inline constexpr auto DX = magic_enum::enum_name(h5geo::DevDataType::DX);
inline constexpr auto DY = magic_enum::enum_name(h5geo::DevDataType::DY);
inline constexpr auto AZIM = magic_enum::enum_name(h5geo::DevDataType::AZIM);
inline constexpr auto INCL = magic_enum::enum_name(h5geo::DevDataType::INCL);


}


#endif // H5CORE_ENUM_STRING_H
