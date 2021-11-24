#include "../../include/h5geo/misc/h5devcurveimpl.h"
#include "../../include/h5geo/h5wellcontainer.h"
#include "../../include/h5geo/h5core.h"
#include "../../include/h5geo/misc/h5wellimpl.h"
#include "../../include/h5geo/misc/h5core_enum_string.h"

#include <units/units.hpp>

H5DevCurveImpl::H5DevCurveImpl(const h5gt::Group &group) :
  H5BaseObjectImpl(group){}

bool H5DevCurveImpl::writeCurve(
    const h5geo::DevDataType& name,
    Eigen::Ref<Eigen::VectorXd> v,
    const std::string& units)
{
  return writeCurve(
        std::string{magic_enum::enum_name(name)}, v, units);
}

bool H5DevCurveImpl::writeCurve(
    const std::string& name,
    Eigen::Ref<Eigen::VectorXd> v,
    const std::string& units)
{
  auto opt = getDevCurveD();
  if (!opt.has_value())
    return false;

  if (!units.empty()){
    double coef;
    if (name == h5geo::OWT)
      coef = units::convert(
            units::unit_from_string(units),
            units::unit_from_string(getTemporalUnits()));
    else
      coef = units::convert(
            units::unit_from_string(units),
            units::unit_from_string(getSpatialUnits()));

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

bool H5DevCurveImpl::setActive(){
  auto optWellG = getParentG(h5geo::ObjectType::WELL);
  if (!optWellG.has_value())
    return false;

  H5WellImpl well(optWellG.value());

  return well.setActiveDevCurve(*this);
}

bool H5DevCurveImpl::isActive(){
  auto optWellG = getParentG(h5geo::ObjectType::WELL);
  if (!optWellG.has_value())
    return false;

  H5WellImpl well(optWellG.value());

  auto optActiveDevG = well.getActiveDevG();
  if (!optActiveDevG.has_value())
    return false;

  H5DevCurveImpl activeDev(optActiveDevG.value());
  return *this == activeDev;
}

size_t H5DevCurveImpl::getNCurves(){
  auto opt = getDevCurveD();
  if (!opt.has_value())
    return 0;

  return opt->getDimensions()[0];
}

size_t H5DevCurveImpl::getNSamp(){
  auto opt = getDevCurveD();
  if (!opt.has_value())
    return 0;

  return opt->getDimensions()[1];
}

Eigen::VectorXd H5DevCurveImpl::getCurve(
    const h5geo::DevDataType& name,
    const std::string& units)
{
  return getCurve(std::string{magic_enum::enum_name(name)}, units);
}

Eigen::VectorXd H5DevCurveImpl::getCurve(
    const std::string& name,
    const std::string& units)
{
  auto opt = getDevCurveD();
  if (!opt.has_value())
    return Eigen::VectorXd();

  Eigen::VectorXd curve = h5geo::getDataFromIndexedDataset<double>(
        opt.value(), name);

  if (!units.empty()){
    double coef;
    if (name == h5geo::OWT)
      coef = units::convert(
            units::unit_from_string(getTemporalUnits()),
            units::unit_from_string(units));
    else
      coef = units::convert(
            units::unit_from_string(getSpatialUnits()),
            units::unit_from_string(units));

    if (!isnan(coef))
      return curve*coef;

    return Eigen::VectorXd();
  }

  return curve;
}

std::string H5DevCurveImpl::getRelativeCurveName(){
  auto optWellG = getParentG(h5geo::ObjectType::WELL);
  if (!optWellG.has_value())
    return std::string();

  H5WellImpl well(optWellG.value());
  auto optDevG = well.getDevG();
  if (!optDevG.has_value())
    return std::string();

  return h5geo::getRelativePath(
        optDevG->getPath(), objG.getPath(),
        h5geo::CaseSensitivity::CASE_INSENSITIVE);
}

H5WellContainer* H5DevCurveImpl::getWellContainer(){
  h5gt::File file = getH5File();
  return h5geo::createWellContainer(
        file, h5geo::CreationType::OPEN_OR_CREATE);
}

H5Well* H5DevCurveImpl::getWell()
{
  auto optWellG = getParentG(h5geo::ObjectType::WELL);
  if (!optWellG.has_value())
    return nullptr;

  return new H5WellImpl(optWellG.value());
}

std::optional<h5gt::DataSet>
H5DevCurveImpl::getDevCurveD()
{
  std::string name = std::string{magic_enum::enum_name(
        h5geo::detail::DevDatasets::dev_data)};

  return getDatasetOpt(objG, name);
}

H5DevCurve* h5geo::openDevCurve(h5gt::Group group){
  if (isGeoObjectByType(group, h5geo::ObjectType::DEVCURVE))
    return new H5DevCurveImpl(group);

  return nullptr;
}
