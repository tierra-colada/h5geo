#include "../../include/h5geopy/h5enum_py.h"

namespace h5geopy {

void ContainerType_py(py::enum_<ContainerType> &py_obj){
  py_obj
      .value("MAP", ContainerType::MAP)
      .value("WELL", ContainerType::WELL)
      .value("SEISMIC", ContainerType::SEISMIC);
}

void ObjectType_py(py::enum_<ObjectType> &py_obj){
  py_obj
      .value("MAP", ObjectType::MAP)
      .value("WELL", ObjectType::WELL)
      .value("LOGCURVE", ObjectType::LOGCURVE)
      .value("DEVCURVE", ObjectType::DEVCURVE)
      .value("SEISMIC", ObjectType::SEISMIC)
      .value("POINTS_1", ObjectType::POINTS_1)
      .value("POINTS_2", ObjectType::POINTS_2)
      .value("POINTS_3", ObjectType::POINTS_3)
      .value("POINTS_4", ObjectType::POINTS_4)
      .value("WELLTOPS", ObjectType::WELLTOPS);
}

void Domain_py(py::enum_<Domain> &py_obj){
  py_obj
      .value("TVD", Domain::TVD)
      .value("TVDSS", Domain::TVDSS)
      .value("TWT", Domain::TWT)
      .value("OWT", Domain::OWT);
}

void SeisDataType_py(py::enum_<SeisDataType> &py_obj){
  py_obj
      .value("STACK", SeisDataType::STACK)
      .value("PRESTACK", SeisDataType::PRESTACK);
}

void SurveyType_py(py::enum_<SurveyType> &py_obj){
  py_obj
      .value("TWO_D", SurveyType::TWO_D)
      .value("THREE_D", SurveyType::THREE_D);
}

void TxtEncoding_py(py::enum_<TextEncoding> &py_obj){
  py_obj
      .value("ASCII", TextEncoding::ASCII)
      .value("EBCDIC", TextEncoding::EBCDIC);
}

void SegyEndian_py(py::enum_<Endian> &py_obj){
  py_obj
      .value("Little", Endian::Little)
      .value("Big", Endian::Big);
}

void SegyFormat_py(py::enum_<SegyFormat> &py_obj){
  py_obj
      .value("FourByte_IBM", SegyFormat::FourByte_IBM)
      .value("FourByte_IEEE", SegyFormat::FourByte_IEEE)
      .value("FourByte_integer", SegyFormat::FourByte_integer);
}

void WellDataType_py(py::enum_<WellDataType> &py_obj){
  py_obj
      .value("WELL", WellDataType::WELL)
      .value("DEV", WellDataType::DEV)
      .value("LOG", WellDataType::LOG)
      .value("INTERVAL_VEL", WellDataType::INTERVAL_VEL)
      .value("AVG_VEL", WellDataType::AVG_VEL)
      .value("RMS_VEL", WellDataType::RMS_VEL)
      .value("FWAL", WellDataType::FWAL)
      .value("CHECKSHOTS", WellDataType::CHECKSHOTS)
      .value("WELL_TIE", WellDataType::WELL_TIE);
}

void WellName_py(py::enum_<WellName> &py_obj){
  py_obj
      .value("FROM_LAS", WellName::FROM_LAS)
      .value("FROM_FILE_NAME", WellName::FROM_FILE_NAME);
}

void LogDataType_py(py::enum_<LogDataType> &py_obj){
  py_obj
      .value("MD", LogDataType::MD)
      .value("VAL", LogDataType::VAL);
}

void DevDataType_py(py::enum_<DevDataType> &py_obj){
  py_obj
      .value("MD", DevDataType::MD)
      .value("X", DevDataType::X)
      .value("Y", DevDataType::Y)
      .value("Z", DevDataType::Z)
      .value("TVD", DevDataType::TVD)
      .value("TVDSS", DevDataType::TVDSS)
      .value("DX", DevDataType::DX)
      .value("DY", DevDataType::DY)
      .value("AZIM", DevDataType::AZIM)
      .value("INCL", DevDataType::INCL)
      .value("OWT", DevDataType::OWT)
      .value("TWT", DevDataType::TWT);
}

void WellType_py(py::enum_<WellType> &py_obj){
  py_obj
      .value("PROPOSED", WellType::PROPOSED)
      .value("DRY", WellType::DRY)
      .value("OIL", WellType::OIL)
      .value("GAS", WellType::GAS)
      .value("WATER", WellType::WATER)
      .value("CONDENSATE", WellType::CONDENSATE);
}

void TrajectoryFormat_py(py::enum_<TrajectoryFormat> &py_obj){
  py_obj
      .value("MD_AZIM_INCL", TrajectoryFormat::MD_AZIM_INCL)
      .value("TVD_X_Y", TrajectoryFormat::TVD_X_Y)
      .value("TVD_DX_DY", TrajectoryFormat::TVD_DX_DY)
      .value("TVDSS_X_Y", TrajectoryFormat::TVDSS_X_Y)
      .value("TVDSS_DX_DY", TrajectoryFormat::TVDSS_DX_DY);
}

void CreationType_py(py::enum_<CreationType> &py_obj){
  py_obj
      .value("OPEN", CreationType::OPEN)
      .value("CREATE", CreationType::CREATE)
      .value("OPEN_OR_CREATE", CreationType::OPEN_OR_CREATE)
      .value("CREATE_OR_OVERWRITE", CreationType::CREATE_OR_OVERWRITE)
      .value("CREATE_UNDER_NEW_NAME", CreationType::CREATE_UNDER_NEW_NAME);
}

void CaseSensitivity_py(py::enum_<CaseSensitivity> &py_obj){
  py_obj
      .value("CASE_SENSITIVE", CaseSensitivity::CASE_SENSITIVE)
      .value("CASE_INSENSITIVE", CaseSensitivity::CASE_INSENSITIVE);
}

void Delimiter_py(py::enum_<Delimiter> &py_obj){
  py_obj
      .value("TABULATION", Delimiter::TABULATION)
      .value("SEMICOLON", Delimiter::SEMICOLON)
      .value("DOT", Delimiter::DOT)
      .value("SPACE", Delimiter::SPACE)
      .value("COMMA", Delimiter::COMMA);
}


} // h5geopy
