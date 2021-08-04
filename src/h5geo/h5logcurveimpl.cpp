#include "../../include/h5geo/misc/h5logcurveimpl.h"
#include "../../include/h5geo/h5wellcontainer.h"
#include "../../include/h5geo/h5core.h"
#include "../../include/h5geo/misc/h5wellimpl.h"

H5LogCurveImpl::H5LogCurveImpl(const h5gt::Group &group) :
  H5BaseObjectImpl(group){}

bool H5LogCurveImpl::writeCurve(
    const h5geo::LogDataType& name,
    const Eigen::Ref<const Eigen::VectorXd>& v)
{
  return writeCurve(std::string{magic_enum::enum_name(name)}, v);
}

bool H5LogCurveImpl::writeCurve(
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

Eigen::VectorXd H5LogCurveImpl::getCurve(
    const h5geo::LogDataType& name)
{
  return getCurve(std::string{magic_enum::enum_name(name)});
}

Eigen::VectorXd H5LogCurveImpl::getCurve(
    const std::string& name)
{
  auto opt = getLogCurveD();
  if (!opt.has_value())
    return Eigen::VectorXd();

  return h5geo::getDataFromIndexedDataset<double>(
        opt.value(), name);
}

bool H5LogCurveImpl::setSpatialUnits(const std::string& str){
  return h5geo::setStringFromObj(
        objG,
        std::string{h5geo::detail::spatial_units},
        str);
}

bool H5LogCurveImpl::setDataUnits(const std::string& str){
  return h5geo::setStringFromObj(
        objG,
        std::string{h5geo::detail::data_units},
        str);
}

std::string H5LogCurveImpl::getSpatialUnits(){
  return h5geo::getStringFromObj(
        objG,
        std::string{h5geo::detail::spatial_units});
}

std::string H5LogCurveImpl::getDataUnits(){
  return h5geo::getStringFromObj(
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
