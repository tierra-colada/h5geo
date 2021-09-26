#include "../../include/h5geo/misc/h5logcurveimpl.h"
#include "../../include/h5geo/h5wellcontainer.h"
#include "../../include/h5geo/h5core.h"
#include "../../include/h5geo/misc/h5wellimpl.h"

#include <units/units.hpp>

H5LogCurveImpl::H5LogCurveImpl(const h5gt::Group &group) :
  H5BaseObjectImpl(group){}

bool H5LogCurveImpl::writeCurve(
    const h5geo::LogDataType& name,
    Eigen::Ref<Eigen::VectorXd> v,
    const std::string& units)
{
  return writeCurve(std::string{magic_enum::enum_name(name)}, v, units);
}

bool H5LogCurveImpl::writeCurve(
    const std::string& name,
    Eigen::Ref<Eigen::VectorXd> v,
    const std::string& units)
{
  auto opt = getLogCurveD();
  if (!opt.has_value())
    return false;

  if (!units.empty()){
    double coef;
    if (name == h5geo::MD)
      coef = units::convert(
            units::unit_from_string(units),
            units::unit_from_string(getSpatialUnits()));
    else
      coef = units::convert(
            units::unit_from_string(units),
            units::unit_from_string(getDataUnits()));

    v *= coef;

    return h5geo::writeData2IndexedDataset(
          opt.value(),
          name,
          v,
          true);
  }

  return h5geo::writeData2IndexedDataset(
        opt.value(),
        name,
        v,
        true);
}

Eigen::VectorXd H5LogCurveImpl::getCurve(
    const h5geo::LogDataType& name,
    const std::string& units)
{
  return getCurve(std::string{magic_enum::enum_name(name)}, units);
}

Eigen::VectorXd H5LogCurveImpl::getCurve(
    const std::string& name,
    const std::string& units)
{
  auto opt = getLogCurveD();
  if (!opt.has_value())
    return Eigen::VectorXd();

  Eigen::VectorXd curve = h5geo::getDataFromIndexedDataset<double>(
        opt.value(), name);

  if (!units.empty()){
    double coef;
    if (name == h5geo::MD)
      coef = units::convert(
            units::unit_from_string(getSpatialUnits()),
            units::unit_from_string(units));
    else
      coef = units::convert(
            units::unit_from_string(getDataUnits()),
            units::unit_from_string(units));

    if (!isnan(coef))
      return curve*coef;

    return Eigen::VectorXd();
  }

  return curve;
}

bool H5LogCurveImpl::setSpatialUnits(const std::string& str){
  return h5geo::overwriteAttribute(
        objG,
        std::string{h5geo::detail::spatial_units},
        str);
}

bool H5LogCurveImpl::setDataUnits(const std::string& str){
  return h5geo::overwriteAttribute(
        objG,
        std::string{h5geo::detail::data_units},
        str);
}

std::string H5LogCurveImpl::getSpatialUnits(){
  return h5geo::readStringAttribute(
        objG,
        std::string{h5geo::detail::spatial_units});
}

std::string H5LogCurveImpl::getDataUnits(){
  return h5geo::readStringAttribute(
        objG,
        std::string{h5geo::detail::data_units});
}

std::string H5LogCurveImpl::getRelativeCurveName(){
  auto optWellG = getParentG(h5geo::ObjectType::WELL);
  if (!optWellG.has_value())
    return std::string();

  H5WellImpl well(optWellG.value());

  auto optLogG = well.getLogG();
  if (!optLogG.has_value())
    return std::string();

  return h5geo::getRelativePath(
        optLogG->getPath(), objG.getPath(),
        h5geo::CaseSensitivity::CASE_INSENSITIVE);
}

H5WellContainer* H5LogCurveImpl::getWellContainer(){
  h5gt::File file = getH5File();
  return h5geo::createWellContainer(
        file, h5geo::CreationType::OPEN_OR_CREATE);
}

H5Well* H5LogCurveImpl::getWell()
{
  auto optWellG = getParentG(h5geo::ObjectType::WELL);
  if (!optWellG.has_value())
    return nullptr;

  return new H5WellImpl(optWellG.value());
}

std::optional<h5gt::DataSet>
H5LogCurveImpl::getLogCurveD()
{
  std::string name = std::string{magic_enum::enum_name(
        h5geo::detail::LogDatasets::log_data)};

  return getDatasetOpt(objG, name);
}
