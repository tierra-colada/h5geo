#include "../../include/h5geo/private/h5core_sr_settings.h"
#include "../../include/h5geo/private/h5enum.h"
#include "../../include/h5geo/h5base.h"

#include <units/units.hpp>
#include <magic_enum.hpp>

#include <gdal.h>
#include <gdal_priv.h>

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

bool convertUnits(
    Eigen::Ref<Eigen::MatrixXd> m,
    const std::string& unitsFrom, const std::string& unitsTo){
  if (unitsFrom.empty() || unitsTo.empty())
    return false;

  double coef = units::convert(
        units::unit_from_string(unitsFrom),
        units::unit_from_string(unitsTo));

  if (isnan(coef))
    return false;

  m *= coef;
  return true;
}

bool convertUnits(
    Eigen::Ref<Eigen::MatrixXf> m,
    const std::string& unitsFrom, const std::string& unitsTo){
  if (unitsFrom.empty() || unitsTo.empty())
    return false;

  double coef = units::convert(
        units::unit_from_string(unitsFrom),
        units::unit_from_string(unitsTo));

  if (isnan(coef))
    return false;

  m *= coef;
  return true;
}

// Transform coord FROM
bool transformCoordFrom(
    Eigen::Ref<Eigen::MatrixXd> x,
    Eigen::Ref<Eigen::MatrixXd> y,
    const std::string& unitsFrom,
    const std::string& srAuthAndCodeFrom){
  if (x.size() != y.size() || x.size() < 1)
    return false;

  double coefFrom = units::convert(
        units::unit_from_string(unitsFrom),
        units::meter);

  if (isnan(coefFrom))
    return false;

  OGRSpatialReference srFrom;
  if (srFrom.SetFromUserInput(srAuthAndCodeFrom.c_str()) != OGRERR_NONE)
    return false;

  if (srFrom.SetLinearUnitsAndUpdateParameters(unitsFrom.c_str(), coefFrom) != OGRERR_NONE)
    return false;

  OGRSpatialReference srTo = h5geo::sr::getSpatialReference();
  OGRCT_ptr coordTrans(OGRCreateCoordinateTransformation(&srFrom, &srTo));
  if (!coordTrans)
    return false;

  return coordTrans->Transform(x.size(), x.data(), y.data());
}

bool transformCoordFrom(
    Eigen::Ref<Eigen::MatrixXd> x,
    Eigen::Ref<Eigen::MatrixXd> y,
    const std::string& unitsFrom,
    const std::string& authNameFrom,
    const std::string& codeFrom){
  std::string srAuthAndCodeTo = authNameFrom + ":" + codeFrom;
  return transformCoordTo(x, y, unitsFrom, srAuthAndCodeTo);
}

bool transformCoordFrom(
    Eigen::Ref<Eigen::MatrixXf> x,
    Eigen::Ref<Eigen::MatrixXf> y,
    const std::string& unitsFrom,
    const std::string& srAuthAndCodeFrom){
  if (x.size() != y.size() || x.size() < 1)
    return false;

  double coefFrom = units::convert(
        units::unit_from_string(unitsFrom),
        units::meter);

  if (isnan(coefFrom))
    return false;

  OGRSpatialReference srFrom;
  if (srFrom.SetFromUserInput(srAuthAndCodeFrom.c_str()) != OGRERR_NONE)
    return false;

  if (srFrom.SetLinearUnitsAndUpdateParameters(unitsFrom.c_str(), coefFrom) != OGRERR_NONE)
    return false;

  OGRSpatialReference srTo = h5geo::sr::getSpatialReference();
  OGRCT_ptr coordTrans(OGRCreateCoordinateTransformation(&srFrom, &srTo));
  if (!coordTrans)
    return false;

  Eigen::MatrixXd xx, yy;
  xx = x.cast<double>();
  yy = y.cast<double>();
  if (!coordTrans->Transform(xx.size(), xx.data(), yy.data()))
    return false;

  x = xx.cast<float>();
  y = yy.cast<float>();
  return true;
}

bool transformCoordFrom(
    Eigen::Ref<Eigen::MatrixXf> x,
    Eigen::Ref<Eigen::MatrixXf> y,
    const std::string& unitsFrom,
    const std::string& authNameFrom,
    const std::string& codeFrom){
  std::string srAuthAndCodeTo = authNameFrom + ":" + codeFrom;
  return transformCoordTo(x, y, unitsFrom, srAuthAndCodeTo);
}

// Transform coord TO
bool transformCoordTo(
    Eigen::Ref<Eigen::MatrixXd> x,
    Eigen::Ref<Eigen::MatrixXd> y,
    const std::string& unitsTo,
    const std::string& srAuthAndCodeTo){
  if (x.size() != y.size() || x.size() < 1)
    return false;

  double coefTo = units::convert(
        units::unit_from_string(unitsTo),
        units::meter);

  if (isnan(coefTo))
    return false;

  OGRSpatialReference srTo;
  if (srTo.SetFromUserInput(srAuthAndCodeTo.c_str()) != OGRERR_NONE)
    return false;

  if (srTo.SetLinearUnitsAndUpdateParameters(unitsTo.c_str(), coefTo) != OGRERR_NONE)
    return false;

  OGRSpatialReference srFrom = h5geo::sr::getSpatialReference();
  OGRCT_ptr coordTrans(OGRCreateCoordinateTransformation(&srFrom, &srTo));
  if (!coordTrans)
    return false;

  return coordTrans->Transform(x.size(), x.data(), y.data());
}

bool transformCoordTo(
    Eigen::Ref<Eigen::MatrixXd> x,
    Eigen::Ref<Eigen::MatrixXd> y,
    const std::string& unitsTo,
    const std::string& authNameTo,
    const std::string& codeTo){
  std::string srAuthAndCodeTo = authNameTo + ":" + codeTo;
  return transformCoordTo(x, y, unitsTo, srAuthAndCodeTo);
}

bool transformCoordTo(
    Eigen::Ref<Eigen::MatrixXf> x,
    Eigen::Ref<Eigen::MatrixXf> y,
    const std::string& unitsTo,
    const std::string& srAuthAndCodeTo){
  if (x.size() != y.size() || x.size() < 1)
    return false;

  double coefTo = units::convert(
        units::unit_from_string(unitsTo),
        units::meter);

  if (isnan(coefTo))
    return false;

  OGRSpatialReference srTo;
  if (srTo.SetFromUserInput(srAuthAndCodeTo.c_str()) != OGRERR_NONE)
    return false;

  if (srTo.SetLinearUnitsAndUpdateParameters(unitsTo.c_str(), coefTo) != OGRERR_NONE)
    return false;

  OGRSpatialReference srFrom = h5geo::sr::getSpatialReference();
  OGRCT_ptr coordTrans(OGRCreateCoordinateTransformation(&srFrom, &srTo));
  if (!coordTrans)
    return false;

  Eigen::MatrixXd xx, yy;
  xx = x.cast<double>();
  yy = y.cast<double>();
  if (!coordTrans->Transform(xx.size(), xx.data(), yy.data()))
    return false;

  x = xx.cast<float>();
  y = yy.cast<float>();
  return true;
}

bool transformCoordTo(
    Eigen::Ref<Eigen::MatrixXf> x,
    Eigen::Ref<Eigen::MatrixXf> y,
    const std::string& unitsTo,
    const std::string& authNameTo,
    const std::string& codeTo){
  std::string srAuthAndCodeTo = authNameTo + ":" + codeTo;
  return transformCoordTo(x, y, unitsTo, srAuthAndCodeTo);
}

} // sr


} // h5geo
