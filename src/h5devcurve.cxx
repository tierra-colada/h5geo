#include "../include/h5geo/h5devcurve.h"
#include "../include/h5geo/h5wellcontainer.h"

H5DevCurve::H5DevCurve(const h5gt::Group &group) :
  H5BaseObject(group)
{

}

H5DevCurve::~H5DevCurve(){

}

std::optional<H5DevCurve>
H5DevCurve::create(h5gt::Group &group){
  if (H5BaseContainer::isObject(
        group, h5geo::ObjectType::DEVCURVE))
    return H5DevCurve(group);
  else
    return std::nullopt;
}

bool H5DevCurve::writeCurve(
    const h5geo::DevDataType& name,
    const Eigen::Ref<const Eigen::VectorXd>& v)
{
  return writeCurve(
        std::string{magic_enum::enum_name(name)}, v);
}

bool H5DevCurve::writeCurve(
    const std::string& name,
    const Eigen::Ref<const Eigen::VectorXd>& v)
{
  auto opt = getDevCurveD();
  if (!opt.has_value())
    return false;

  return h5geo::writeData2IndexedDataset(
        opt.value(),
        name,
        v,
        true);
}

bool H5DevCurve::setActive(){
  auto optWell = getWell();
  if (!optWell.has_value())
    return false;

  return optWell->setActiveDevCurve(*this);
}

bool H5DevCurve::isActive(){
  auto well = getWell();
  if (!well.has_value())
    return false;

  auto activeCurve = well->getActiveDevCurve();
  if (!activeCurve.has_value())
    return false;

  return *this == activeCurve.value();
}

size_t H5DevCurve::getNCurves(){
  auto opt = getDevCurveD();
  if (!opt.has_value())
    return 0;

  return opt->getDimensions()[0];
}

size_t H5DevCurve::getNSamp(){
  auto opt = getDevCurveD();
  if (!opt.has_value())
    return 0;

  return opt->getDimensions()[1];
}

Eigen::VectorXd H5DevCurve::getCurve(
    const h5geo::DevDataType& name)
{
  return getCurve(std::string{magic_enum::enum_name(name)});
}

Eigen::VectorXd H5DevCurve::getCurve(
    const std::string& name)
{
  auto opt = getDevCurveD();
  if (!opt.has_value())
    return Eigen::VectorXd();

  return h5geo::getDataFromIndexedDataset<double>(
        opt.value(), name);
}

std::string H5DevCurve::getRelativeCurveName(){
  auto optWellG = getWell();
  if (!optWellG.has_value())
    return std::string();

  auto optDevG = optWellG->getDevG();
  if (!optDevG.has_value())
    return std::string();

  return h5geo::getRelativePath(
        optDevG->getPath(), objG.getPath(),
        h5geo::CaseSensitivity::CASE_INSENSITIVE);
}

std::optional<H5WellContainer>
H5DevCurve::getWellContainer(){
  h5gt::File file = getH5File();
  return H5WellContainer::create(file);
}

std::optional<H5Well>
H5DevCurve::getWell()
{
  auto optContainer = getWellContainer();
  if (!optContainer.has_value())
    return std::nullopt;

  auto optWell = H5BaseObject::getParent(h5geo::ObjectType::WELL);
  if (!optWell.has_value())
    return std::nullopt;

  return H5Well::create(optWell.value());
}

std::optional<h5gt::DataSet>
H5DevCurve::getDevCurveD()
{
  std::string name = std::string{magic_enum::enum_name(
        h5geo::DevDatasets::dev_data)};

  return getDatasetOpt(objG, name);
}

