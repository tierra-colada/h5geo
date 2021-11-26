#ifndef H5CORE_SR_SETTINGS_H
#define H5CORE_SR_SETTINGS_H

#include <units/units.hpp>

#include <gdal/gdal.h>
#include <gdal/gdal_priv.h>

#include <iostream>

namespace h5geo
{


namespace sr {


inline OGRSpatialReference SpatialReference;

inline void setSpatialReference(OGRSpatialReference sr){
  SpatialReference = sr;
}

inline void setSpatialReferenceFromUserInput(
    const std::string& authName, const std::string& code){
  SpatialReference.SetFromUserInput((authName + ":" + code).c_str());
}

inline OGRSpatialReference& getSpatialReference(){
  return SpatialReference;
}

inline void setSpatialUnits(const std::string& units){
  double coef = units::convert(
      units::unit_from_string(units),
      units::unit_from_string("meter"));
  SpatialReference.SetLinearUnitsAndUpdateParameters(units.c_str(), coef);
}

inline void setAngularUnits(const std::string& units){
  double coef = units::convert(
      units::unit_from_string(units),
      units::unit_from_string("radian"));
  SpatialReference.SetAngularUnits(units.c_str(), coef);
}

inline std::string getSpatialUnits(){
  const char *units = nullptr;
  SpatialReference.GetLinearUnits(&units);
  return std::string(units);
}

inline std::string getAngularUnits(){
  const char *units;
  SpatialReference.GetAngularUnits(&units);
  return std::string(units);
}

inline std::string getAuthorityName(){
  char *name = nullptr;
  return std::string(SpatialReference.GetAuthorityName(name));
}

inline std::string getAuthorityCode(){
  char *code = nullptr;
  return std::string(SpatialReference.GetAuthorityCode(code));
}


} // sr


} // h5geo


#endif // H5CORE_SR_SETTINGS_H
