#include "../../include/h5geo/impl/h5wellimpl.h"
#include "../../include/h5geo/h5wellcontainer.h"
#include "../../include/h5geo/impl/h5devcurveimpl.h"
#include "../../include/h5geo/impl/h5logcurveimpl.h"
#include "../../include/h5geo/misc/h5core.h"
#include "../../include/h5geo/misc/h5enum_string.h"

#include <units/units.hpp>

#ifdef H5GEO_USE_GDAL
#include <gdal.h>
#include <gdal_priv.h>
#endif

H5WellImpl::H5WellImpl(const h5gt::Group &group) :
  H5BaseObjectImpl(group){}

H5LogCurve* H5WellImpl::openLogCurve(
    const std::string &logType,
    const std::string &logName)
{
  auto logG = getLogG();
  if (!logG.has_value())
    return nullptr;

  // if `logName` is full path then we need to be confident
  // that this curve belong to this well
  if (!logType.empty() && logType[0] == '/')
    if (!isSuccessor(logG->getTargetPath(), logType))
      return nullptr;

  if (!logName.empty() && logName[0] == '/' &&
      !logType.empty() && logType[0] == '/')
    if (!isSuccessor(logType, logName))
      return nullptr;

  // logType may be empty that means log is written without type
  if (logType.empty()){
    if (!logG->hasObject(logName, h5gt::ObjectType::Group))
      return nullptr;

    h5gt::Group curveG = logG->getGroup(logName);
    return openLogCurve(curveG);
  } else {
    if (!logG->hasObject(logType, h5gt::ObjectType::Group))
      return nullptr;

    h5gt::Group logTypeG = logG->getGroup(logType);

    if (!logTypeG.hasObject(logName, h5gt::ObjectType::Group))
      return nullptr;

    h5gt::Group curveG = logTypeG.getGroup(logName);
    return openLogCurve(curveG);
  }
}

H5LogCurve* H5WellImpl::openLogCurve(
    h5gt::Group group)
{
  auto logG = getLogG();
  if (!logG.has_value())
    return nullptr;

  if (!isSuccessor(logG.value(), group))
    return nullptr;

  return h5geo::openLogCurve(group);
}

H5DevCurve* H5WellImpl::openDevCurve(
    const std::string &devName)
{
  auto devG = getDevG();
  if (!devG.has_value())
    return nullptr;

  /* if `logName` was full path then we need to be confident
   * that this curve belong to this well */
  if (!devName.empty() && devName[0] == '/')
    if (!isSuccessor(devG->getTargetPath(), devName))
      return nullptr;

  if (!devG->hasObject(devName, h5gt::ObjectType::Group))
    return nullptr;

  h5gt::Group curveG = devG->getGroup(devName);

  return openDevCurve(curveG);
}

H5DevCurve* H5WellImpl::openDevCurve(
    h5gt::Group group)
{
  auto devG = getDevG();
  if (!devG.has_value())
    return nullptr;

  if (!isSuccessor(devG.value(), group))
    return nullptr;

  return h5geo::openDevCurve(group);
}

H5LogCurve* H5WellImpl::createLogCurve(
    std::string& logType,
    std::string& logName,
    LogCurveParam& p,
    h5geo::CreationType createFlag)
{
  auto logG = getLogG();
  if (!logG.has_value())
    return nullptr;

  /* If `logType` is full path then we need to be confident that
   * this `logType` (and `logName`) refers to this well */
  if (!logType.empty() && logType[0] == '/')
    if (!isSuccessor(logG->getTargetPath(), logType))
      return nullptr;

  if (!logName.empty() && logName[0] == '/' &&
      !logType.empty() && logType[0] == '/')
    if (!isSuccessor(logType, logName))
      return nullptr;

  std::optional<h5gt::Group> opt;
  if (logType.empty()){
    opt = createObject(
          logName, logG.value(), h5geo::ObjectType::LOGCURVE, &p, createFlag);
  } else {
    if (!logG->hasObject(logType, h5gt::ObjectType::Group))
      logG->createGroup(logType);

    h5gt::Group logTypeG = logG->getGroup(logType);

    opt = createObject(
          logName, logTypeG, h5geo::ObjectType::LOGCURVE, &p, createFlag);
  }

  if (!opt.has_value())
    return nullptr;

  return new H5LogCurveImpl(opt.value());
}

H5LogCurve* H5WellImpl::createLogCurve(
    h5gt::Group group,
    LogCurveParam& p,
    h5geo::CreationType createFlag)
{
  auto logG = getLogG();
  if (!logG.has_value())
    return nullptr;

  /* We need to be confident that `group` is a successor of `logG` */
  if (!isSuccessor(logG.value(), group))
    return nullptr;

  auto opt = createObject(
        group, h5geo::ObjectType::LOGCURVE, &p, createFlag);

  if (!opt.has_value())
    return nullptr;

  return new H5LogCurveImpl(opt.value());
}

H5DevCurve* H5WellImpl::createDevCurve(
    std::string& devName,
    DevCurveParam& p,
    h5geo::CreationType createFlag)
{
  auto devG = getDevG();
  if (!devG.has_value())
    return nullptr;

  /* If `devName` is full path then we need to be confident that
   * this `devName` refers to this well */
  if (!devName.empty() && devName[0] == '/')
    if (!isSuccessor(devG->getTargetPath(), devName))
      return nullptr;

  std::optional<h5gt::Group> opt = createObject(
        devName, devG.value(), h5geo::ObjectType::DEVCURVE,
        &p, createFlag);

  if (!opt.has_value())
    return nullptr;

  return new H5DevCurveImpl(opt.value());
}

H5DevCurve* H5WellImpl::createDevCurve(
    h5gt::Group group,
    DevCurveParam& p,
    h5geo::CreationType createFlag)
{
  auto devG = getDevG();
  if (!devG.has_value())
    return nullptr;

  /* We need to be confident that `group` is a successor of `devG` */
  if (!isSuccessor(devG.value(), group))
    return nullptr;

  std::optional<h5gt::Group> opt = createObject(
        group, h5geo::ObjectType::DEVCURVE, &p, createFlag);

  if (!opt.has_value())
    return nullptr;

  return new H5DevCurveImpl(opt.value());
}

bool H5WellImpl::setHeadCoord(
    Eigen::Ref<Eigen::Vector2d> v,
    const std::string& lengthUnits,
    bool doCoordTransform)
{
#ifdef H5GEO_USE_GDAL
  if (doCoordTransform){
    OGRCT_ptr coordTrans(createCoordinateTransformationToWriteData(lengthUnits));
    if (!coordTrans)
      return false;

    coordTrans->Transform(1, &v(0), &v(1));
    return h5geo::overwriteAttribute(
          objG,
          std::string{h5geo::detail::head_coord},
          v);
  }
#endif

  return h5geo::overwriteAttribute(
      objG,
      std::string{h5geo::detail::head_coord},
      v, lengthUnits, getLengthUnits());
}

bool H5WellImpl::setKB(
    double& val,
    const std::string& lengthUnits)
{
  return h5geo::overwriteAttribute(
      objG,
      std::string{h5geo::detail::KB},
      val, lengthUnits, getLengthUnits());
}

bool H5WellImpl::setUWI(const std::string& str)
{
  return h5geo::overwriteAttribute(
        objG,
        std::string{h5geo::detail::UWI},
        str);
}

bool H5WellImpl::setActiveDevCurve(H5DevCurve* curve){
  auto opt = getActiveDevG();
  if (opt.has_value())
    objG.unlink(opt->getPath()); // must be getPath (not getTargetPath)

  std::string devName;
  h5geo::splitPathToParentAndObj(curve->getObjG().getPath(), devName);
  if (devName == std::string{h5geo::detail::ACTIVE})
    return false;

  objG.createLink(curve->getObjG(),
                  std::string{h5geo::detail::DEV} + "/" +
                  std::string{h5geo::detail::ACTIVE},
                  h5gt::LinkType::Soft);

  return true;
}

Eigen::VectorXd H5WellImpl::getHeadCoord(
    const std::string& lengthUnits,
    bool doCoordTransform)
{
#ifdef H5GEO_USE_GDAL
  if (doCoordTransform){
    OGRCT_ptr coordTrans(createCoordinateTransformationToReadData(lengthUnits));
    if (!coordTrans)
      return Eigen::VectorXd();

    Eigen::VectorXd v = h5geo::readDoubleEigenVecAttribute(
          objG,
          std::string{h5geo::detail::head_coord});

    if (v.size() != 2)
      return Eigen::VectorXd();

    coordTrans->Transform(1, &v(0), &v(1));
    return v;
  }
#endif

  return h5geo::readDoubleEigenVecAttribute(
      objG,
      std::string{h5geo::detail::head_coord},
      getLengthUnits(), lengthUnits);
}

double H5WellImpl::getKB(
    const std::string& lengthUnits)
{
  return h5geo::readDoubleAttribute(
      objG,
      std::string{h5geo::detail::KB},
      getLengthUnits(), lengthUnits);
}

std::string H5WellImpl::getUWI(){
  return h5geo::readStringAttribute(
        objG,
        std::string{h5geo::detail::UWI});
}

H5DevCurve* H5WellImpl::openActiveDevCurve(){
  auto opt = getActiveDevG();
  if (!opt.has_value())
    return nullptr;

  std::string path = opt->getTargetPath();
  opt->hasObject(path, h5gt::ObjectType::Group);
  return openDevCurve(opt->getTargetPath());
}

std::vector<h5gt::Group>
H5WellImpl::getDevCurveGroupList(){
  auto devG = getDevG();
  if (!devG.has_value())
    return std::vector<h5gt::Group>();

  return getChildGroupList(devG.value(), h5geo::ObjectType::DEVCURVE, true);
}

std::vector<h5gt::Group>
H5WellImpl::getLogCurveGroupList(){
  auto logG = getLogG();
  if (!logG.has_value())
    return std::vector<h5gt::Group>();

  return getChildGroupList(logG.value(), h5geo::ObjectType::LOGCURVE, true);
}

std::vector<std::string>
H5WellImpl::getDevCurveNameList(){
  auto devG = getDevG();
  if (!devG.has_value())
    return std::vector<std::string>();

  return getChildNameList(devG.value(), h5geo::ObjectType::DEVCURVE, devG->getTargetPath(), true);
}

std::vector<std::string>
H5WellImpl::getLogCurveNameList(){
  auto logG = getLogG();
  if (!logG.has_value())
    return std::vector<std::string>();

  return getChildNameList(logG.value(), h5geo::ObjectType::LOGCURVE, logG->getTargetPath(), true);
}

std::vector<std::string>
H5WellImpl::getLogTypeList(){
  auto logG = getLogG();
  if (!logG.has_value())
    return std::vector<std::string>();

  std::vector<std::string> logTypeNameList;
  std::vector<std::string> childNameList = logG->listObjectNames();
  logTypeNameList.reserve(childNameList.size());
  for (const auto& name : childNameList){
    if (logG->getObjectType(name) != h5gt::ObjectType::Group)
      continue;

    h5gt::Group logTypeG = logG->getGroup(name);
    // log curve may be created without type, so log curve is not a type
    if (h5geo::isLogCurve(logTypeG))
      continue;

    logTypeNameList.push_back(name);
  }
  logTypeNameList.shrink_to_fit();
  return logTypeNameList;
}

size_t H5WellImpl::getDevCurveCount(){
  auto devG = getDevG();
  if (!devG.has_value())
    return 0;

  return getChildCount(devG.value(), h5geo::ObjectType::DEVCURVE, true);
}

size_t H5WellImpl::getLogCurveCount(){
  auto logG = getLogG();
  if (!logG.has_value())
    return 0;

  return getChildCount(logG.value(), h5geo::ObjectType::LOGCURVE, true);
}

H5WellContainer* H5WellImpl::openWellContainer(){
  h5gt::File file = getH5File();
  return h5geo::createWellContainer(
        file, h5geo::CreationType::OPEN_OR_CREATE);
}

std::optional<h5gt::Group>
H5WellImpl::getDevG()
{
  std::string name = std::string{h5geo::detail::DEV};

  if (!objG.hasObject(name, h5gt::ObjectType::Group))
    return std::nullopt;

  return objG.getGroup(name);
}

std::optional<h5gt::Group>
H5WellImpl::getActiveDevG()
{
  std::string name = std::string{h5geo::detail::DEV} + "/" +
      std::string{h5geo::detail::ACTIVE};

  return getGroupOpt(objG, name);
}

std::optional<h5gt::Group>
H5WellImpl::getLogG()
{
  std::string name = std::string{h5geo::detail::LOG};

  return getGroupOpt(objG, name);
}

std::optional<h5gt::Group>
H5WellImpl::getLogTypeG(const std::string& logType)
{
  auto logG = getLogG();
  if (!logG.has_value())
    return std::nullopt;

  if (!logG->hasObject(logType, h5gt::ObjectType::Group))
    return std::nullopt;

  return logG->getGroup(logType);
}

H5Well* h5geo::openWell(h5gt::Group group){
  if (isGeoObjectByType(group, h5geo::ObjectType::WELL))
    return new H5WellImpl(group);

  return nullptr;
}
