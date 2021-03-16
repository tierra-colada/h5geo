#include "../include/h5well/h5logcurve.h"
#include "../include/h5well/h5wellcontainer.h"

H5LogCurve::H5LogCurve(const h5gt::Group &group) :
  H5BaseObject(group)
{

}

H5LogCurve::~H5LogCurve(){

}

std::optional<H5LogCurve>
H5LogCurve::create(h5gt::Group &group){
  if (H5BaseContainer::isObject(
        group, h5geo::ObjectType::LOGCURVE))
    return H5LogCurve(group);
  else
    return std::nullopt;
}

bool H5LogCurve::writeCurve(
    const h5geo::LogDataType& name,
    const Eigen::Ref<const Eigen::VectorXd>& v)
{
  return writeCurve(std::string{magic_enum::enum_name(name)}, v);
}

bool H5LogCurve::writeCurve(
    const std::string& name,
    const Eigen::Ref<const Eigen::VectorXd>& v)
{
  auto opt = getLogCurveD();
  if (!opt.has_value())
    return false;

  return h5geo::writeData2IndexedDataset(
        opt.value(),
        name,
        v,
        true);
}

Eigen::VectorXd H5LogCurve::getCurve(
    const h5geo::LogDataType& name)
{
  return getCurve(std::string{magic_enum::enum_name(name)});
}

Eigen::VectorXd H5LogCurve::getCurve(
    const std::string& name)
{
  auto opt = getLogCurveD();
  if (!opt.has_value())
    return Eigen::VectorXd();

  return h5geo::getDataFromIndexedDataset<double>(
        opt.value(), name);
}

std::string H5LogCurve::getRelativeCurveName(){
  auto optWellG = getWell();
  if (!optWellG.has_value())
    return std::string();

  auto optLogG = optWellG->getLogG();
  if (!optLogG.has_value())
    return std::string();

  return h5geo::getRelativePath(
        optLogG->getPath(), objG.getPath(),
        h5geo::CaseSensitivity::CASE_INSENSITIVE);
}

std::optional<H5WellContainer>
H5LogCurve::getWellContainer(){
  h5gt::File file = getH5File();
  return H5WellContainer::create(file);
}

std::optional<H5Well>
H5LogCurve::getWell()
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
H5LogCurve::getLogCurveD()
{
  std::string name = std::string{magic_enum::enum_name(
        h5geo::LogDatasets::log_data)};

  return getDatasetOpt(objG, name);
}
