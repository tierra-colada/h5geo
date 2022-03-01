#include "../../include/h5geo/impl/h5logcurveimpl.h"
#include "../../include/h5geo/h5wellcontainer.h"
#include "../../include/h5geo/impl/h5wellimpl.h"
#include "../../include/h5geo/misc/h5core.h"
#include "../../include/h5geo/misc/h5enum_string.h"

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

  bool val;
  if (!units.empty()){
    double coef;
    if (name == h5geo::MD)
      coef = units::convert(
            units::unit_from_string(units),
            units::unit_from_string(getLengthUnits()));
    else
      coef = units::convert(
            units::unit_from_string(units),
            units::unit_from_string(getDataUnits()));

    v *= coef;

    val = h5geo::writeData2IndexedDataset(
          opt.value(),
          name,
          v,
          true);

    objG.flush();
    return val;
  }

  val = h5geo::writeData2IndexedDataset(
        opt.value(),
        name,
        v,
        true);

  objG.flush();
  return val;
}

size_t H5LogCurveImpl::getNCurves(){
  auto opt = getLogCurveD();
  if (!opt.has_value())
    return 0;

  return opt->getDimensions()[0];
}

size_t H5LogCurveImpl::getNSamp(){
  auto opt = getLogCurveD();
  if (!opt.has_value())
    return 0;

  return opt->getDimensions()[1];
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
            units::unit_from_string(getLengthUnits()),
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

std::string H5LogCurveImpl::getRelativeCurveName(){
  auto optWellG = getParentG(h5geo::ObjectType::WELL);
  if (!optWellG.has_value())
    return std::string();

  if (!optWellG->hasObject("LOG", h5gt::ObjectType::Group))
    return std::string();

  h5gt::Group logG = optWellG->getGroup("LOG");
  return h5geo::getRelativePath(
        logG.getTargetPath(), objG.getTargetPath(),
        h5geo::CaseSensitivity::CASE_INSENSITIVE);
}

H5WellContainer* H5LogCurveImpl::openWellContainer(){
  h5gt::File file = getH5File();
  return h5geo::createWellContainer(
        file, h5geo::CreationType::OPEN_OR_CREATE);
}

H5Well* H5LogCurveImpl::openWell()
{
  auto optWellG = getParentG(h5geo::ObjectType::WELL);
  if (!optWellG.has_value())
    return nullptr;

  return new H5WellImpl(optWellG.value());
}

std::optional<h5gt::DataSet>
H5LogCurveImpl::getLogCurveD()
{
  std::string name = std::string{h5geo::detail::log_data};

  return getDatasetOpt(objG, name);
}

H5LogCurve* h5geo::openLogCurve(h5gt::Group group){
  if (isGeoObjectByType(group, h5geo::ObjectType::LOGCURVE))
    return new H5LogCurveImpl(group);

  return nullptr;
}
