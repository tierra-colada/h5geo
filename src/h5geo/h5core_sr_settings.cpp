#include "../../include/h5geo/misc/h5core_sr_settings.h"

#include <units/units.hpp>
#include <magic_enum.hpp>

#include <gdal/gdal.h>
#include <gdal/gdal_priv.h>

#include <iostream>

namespace h5geo {


namespace sr {


namespace {

OGRSpatialReference SpatialReference {};
std::string TemporalUnits {};
std::string Domain;

} // namespace

void setSpatialReference(OGRSpatialReference sr){
  SpatialReference = sr;
}

void setSpatialReferenceFromUserInput(
    const std::string& name){
  SpatialReference.SetFromUserInput(name.c_str());
}

void setSpatialReferenceFromUserInput(
    const std::string& authName, const std::string& code){
  SpatialReference.SetFromUserInput((authName + ":" + code).c_str());
}

OGRSpatialReference getSpatialReference(){
  return SpatialReference;
}

void setLengthUnits(const std::string& units){
  double coef = units::convert(
      units::unit_from_string(units),
      units::unit_from_string("meter"));
  SpatialReference.SetLinearUnitsAndUpdateParameters(units.c_str(), coef);
}

void setAngularUnits(const std::string& units){
  double coef = units::convert(
      units::unit_from_string(units),
      units::unit_from_string("radian"));
  SpatialReference.SetAngularUnits(units.c_str(), coef);
}

void setTemporalUnits(const std::string& units){
  TemporalUnits = units;
}

std::string getLengthUnits(){
  const char *units = nullptr;
  SpatialReference.GetLinearUnits(&units);
  return std::string(units);
}

std::string getAngularUnits(){
  const char *units;
  SpatialReference.GetAngularUnits(&units);
  return std::string(units);
}

std::string getTemporalUnits(){
  return TemporalUnits;
}

void setDomain(const std::string& domain){
  Domain = domain;
}

void setDomain(const h5geo::Domain& domain){
  Domain = std::string{magic_enum::enum_name(domain)};
}

std::string getDomain(){
  return Domain;
}

h5geo::Domain getDomainEnum(){
  auto domainEnum = magic_enum::enum_cast<h5geo::Domain>(Domain);
  if (!domainEnum.has_value())
    return static_cast<h5geo::Domain>(0);

  return domainEnum.value();
}

} // sr


} // h5geo
