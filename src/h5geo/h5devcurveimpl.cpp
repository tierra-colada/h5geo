#include "../../include/h5geo/private/h5devcurveimpl.h"
#include "../../include/h5geo/h5wellcontainer.h"
#include "../../include/h5geo/h5core.h"
#include "../../include/h5geo/private/h5wellimpl.h"
#include "../../include/h5geo/private/h5enum_string.h"
#include "../../include/h5geo/private/h5deviation.h"

#include <units/units.hpp>

H5DevCurveImpl::H5DevCurveImpl(const h5gt::Group &group) :
  H5BaseObjectImpl(group){}

bool H5DevCurveImpl::writeMD(
    Eigen::Ref<Eigen::VectorXd> v,
    const std::string& units)
{
  return writeCurve(std::string{h5geo::MD}, v, units);
}

bool H5DevCurveImpl::writeAZIM(
    Eigen::Ref<Eigen::VectorXd> v,
    const std::string& units)
{
  return writeCurve(std::string{h5geo::AZIM}, v, units);
}

bool H5DevCurveImpl::writeINCL(
    Eigen::Ref<Eigen::VectorXd> v,
    const std::string& units)
{
  return writeCurve(std::string{h5geo::INCL}, v, units);
}

bool H5DevCurveImpl::writeTVD(
    Eigen::Ref<Eigen::VectorXd> v,
    const std::string& units)
{
  return writeCurve(std::string{h5geo::TVD}, v, units);
}

bool H5DevCurveImpl::writeDX(
    Eigen::Ref<Eigen::VectorXd> v,
    const std::string& units)
{
  return writeCurve(std::string{h5geo::DX}, v, units);
}

bool H5DevCurveImpl::writeDY(
    Eigen::Ref<Eigen::VectorXd> v,
    const std::string& units)
{
  return writeCurve(std::string{h5geo::DY}, v, units);
}

bool H5DevCurveImpl::writeOWT(
    Eigen::Ref<Eigen::VectorXd> v,
    const std::string& units)
{
  return writeCurve(std::string{h5geo::OWT}, v, units);
}

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

  bool val;
  if (!units.empty()){
    double coef;
    if (name == h5geo::OWT){
      coef = units::convert(
            units::unit_from_string(units),
            units::unit_from_string(getTemporalUnits()));
    } else if (name == h5geo::AZIM || name == h5geo::INCL){
      coef = units::convert(
            units::unit_from_string(getAngularUnits()),
            units::unit_from_string(units));
    } else {
      coef = units::convert(
            units::unit_from_string(getLengthUnits()),
            units::unit_from_string(units));
    }

    v *= coef;

    val = h5geo::writeDataToIndexedDataset(
          opt.value(),
          name,
          v,
          true);

    objG.flush();
    return val;
  }

  val = h5geo::writeDataToIndexedDataset(
        opt.value(),
        name,
        v,
        true);

  objG.flush();
  return val;
}

bool H5DevCurveImpl::setActive(){
  auto optWellG = getParentG(h5geo::ObjectType::WELL);
  if (!optWellG.has_value())
    return false;

  H5WellImpl well(optWellG.value());

  return well.setActiveDevCurve(this);
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

void H5DevCurveImpl::updateMdAzimIncl(){
  Eigen::MatrixXd M;
  M.resize(getNSamp(), 3);
  M.col(0) = this->getCurve(std::string{h5geo::TVD});
  M.col(1) = this->getCurve(std::string{h5geo::DX});
  M.col(2) = this->getCurve(std::string{h5geo::DY});

  Eigen::MatrixXd MM = h5geo::TvdDxDy2MdAzIncl(M, getAngularUnits(), false);

  writeMD(MM.col(0));
  writeAZIM(MM.col(1));
  writeINCL(MM.col(2));
}

void H5DevCurveImpl::updateTvdDxDy(){
  Eigen::MatrixXd M;
  M.resize(getNSamp(), 3);
  M.col(0) = this->getCurve(std::string{h5geo::MD});
  M.col(1) = this->getCurve(std::string{h5geo::AZIM});
  M.col(2) = this->getCurve(std::string{h5geo::INCL});

  // passing x0 = 0.0, y0 = 0.0 makes DX, DY output instead of XY (must be float or it won't work)
  Eigen::MatrixXd MM = h5geo::MdAzIncl2MdXYTvd(M, 0.f, 0.f, getAngularUnits(), false);

  writeTVD(MM.col(3));
  writeDX(MM.col(1));
  writeDY(MM.col(2));
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
    const std::string& units,
    bool doCoordTransform)
{
  return getCurve(
        std::string{magic_enum::enum_name(name)},
        units, doCoordTransform);
}

Eigen::VectorXd H5DevCurveImpl::getCurve(
    const std::string& name,
    const std::string& units,
    bool doCoordTransform)
{
  auto opt = getDevCurveD();
  if (!opt.has_value())
    return Eigen::VectorXd();

  double coef = 1;
  if (!units.empty()){
    if (name == h5geo::AZIM ||
        name == h5geo::INCL){
      coef = units::convert(
            units::unit_from_string(getAngularUnits()),
            units::unit_from_string(units));
    } else if (name == h5geo::OWT ||
               name == h5geo::TWT){
      coef = units::convert(
            units::unit_from_string(getTemporalUnits()),
            units::unit_from_string(units));
    } else {
      coef = units::convert(
            units::unit_from_string(getLengthUnits()),
            units::unit_from_string(units));
    }

    if (isnan(coef))
      return Eigen::VectorXd();
  }

  if (name == h5geo::MD ||
      name == h5geo::AZIM ||
      name == h5geo::INCL ||
      name == h5geo::TVD ||
      name == h5geo::DX ||
      name == h5geo::DY ||
      name == h5geo::OWT){
    Eigen::VectorXd curve = h5geo::getDataFromIndexedDataset<double>(
          opt.value(), name);

    return curve*coef;
  } else if (name == h5geo::TWT){
    Eigen::VectorXd curve = h5geo::getDataFromIndexedDataset<double>(
          opt.value(), std::string{h5geo::OWT});

    return curve*coef*2;
  }

  // if the requested data needs to be calculated then we continue
  // and if we go there then units may only have type LENGTH
  H5Well_ptr well(openWell());
  if (!well)
    Eigen::VectorXd();

  double kb = well->getKB(units);

  Eigen::VectorXd headXY;
  if (name == h5geo::X || name == h5geo::Y){
    headXY = well->getHeadCoord(units, doCoordTransform);
    if (headXY.size() != 2)
      Eigen::VectorXd();
  }

  if (name == h5geo::X){
    Eigen::VectorXd curve = h5geo::getDataFromIndexedDataset<double>(
          opt.value(), std::string{h5geo::DX});
    return (curve*coef).array() + headXY(0);
  } else if (name == h5geo::Y){
    Eigen::VectorXd curve = h5geo::getDataFromIndexedDataset<double>(
          opt.value(), std::string{h5geo::DY});
    return (curve*coef).array() + headXY(1);
  } else if (name == h5geo::TVDSS){
    Eigen::VectorXd curve = h5geo::getDataFromIndexedDataset<double>(
          opt.value(), std::string{h5geo::TVD});
    return (curve*coef).array() - kb;
  } else if (name == h5geo::Z){
    Eigen::VectorXd curve = h5geo::getDataFromIndexedDataset<double>(
          opt.value(), std::string{h5geo::TVD});
    return ((curve*coef).array() - kb)*(-1);
  }

  return Eigen::VectorXd();
}

std::string H5DevCurveImpl::getRelativeName(){
  auto optWellG = getParentG(h5geo::ObjectType::WELL);
  if (!optWellG.has_value())
    return std::string();

  if (!optWellG->hasObject(std::string{h5geo::detail::DEV}, h5gt::ObjectType::Group))
    return std::string();

  h5gt::Group devG = optWellG->getGroup(std::string{h5geo::detail::DEV});
  return h5geo::getRelativePath(
        devG.getPath(), objG.getPath(),
        h5geo::CaseSensitivity::CASE_INSENSITIVE);
}

H5DevCurveParam H5DevCurveImpl::getParam()
{
  H5DevCurveParam p;
  // H5BaseObjectParam
  p.spatialReference = getSpatialReference();
  p.lengthUnits = getLengthUnits();
  p.temporalUnits = getTemporalUnits();
  p.angularUnits = getAngularUnits();
  p.dataUnits = getDataUnits();
  return p;
}

H5WellContainer* H5DevCurveImpl::openWellContainer(){
  h5gt::File file = getH5File();
  return h5geo::createWellContainer(
        file, h5geo::CreationType::OPEN_OR_CREATE);
}

H5Well* H5DevCurveImpl::openWell()
{
  auto optWellG = getParentG(h5geo::ObjectType::WELL);
  if (!optWellG.has_value())
    return nullptr;

  return new H5WellImpl(optWellG.value());
}

std::optional<h5gt::DataSet>
H5DevCurveImpl::getDevCurveD()
{
  std::string name = std::string{h5geo::detail::dev_data};

  return getDatasetOpt(objG, name);
}
