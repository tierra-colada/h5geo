#ifndef H5CORE_SETTINGS_H
#define H5CORE_SETTINGS_H

#include <units/units.hpp>

#include <gdal/gdal.h>
#include <gdal/gdal_priv.h>

namespace h5geo
{

inline OGRSpatialReference SR;

inline void setSpatialReference(OGRSpatialReference sr){
  SR = sr;
}

inline void setSpatialReferenceFromUserInput(
    const std::string& authName, const std::string& code){
  SR.SetFromUserInput((authName + ":" + code).c_str());
}

inline OGRSpatialReference& getSpatialReference(){
  return SR;
}

inline void setSpatialUnits(const std::string& units){
  double coef = units::convert(
      units::unit_from_string(units),
      units::unit_from_string("meter"));
  SR.SetLinearUnitsAndUpdateParameters(units.c_str(), coef);
}

inline void setAngularUnits(const std::string& units){
  double coef = units::convert(
      units::unit_from_string(units),
      units::unit_from_string("radian"));
  SR.SetLinearUnitsAndUpdateParameters(units.c_str(), coef);
}

inline std::string getSpatialUnits(){
  const char *units = nullptr;
  SR.GetLinearUnits(&units);
  return std::string(units);
}

inline std::string getAngularUnits(){
  const char *units = nullptr;
  SR.GetAngularUnits(&units);
  return std::string(units);
}


}


#endif // H5CORE_SETTINGS_H
