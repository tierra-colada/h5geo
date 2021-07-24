#include "../../include/h5geo/misc/h5wellimpl.h"
#include "../../include/h5geo/h5wellcontainer.h"
#include "../../include/h5geo/h5core.h"
#include "../../include/h5geo/misc/h5devcurveimpl.h"
#include "../../include/h5geo/misc/h5logcurveimpl.h"

H5WellImpl::H5WellImpl(const h5gt::Group &group) :
  H5BaseObjectImpl(group){}


H5LogCurve* H5WellImpl::getLogCurve(
    const std::string &logType,
    const std::string &logName)
{
  auto logG = getLogG();
  if (!logG.has_value())
    return nullptr;

  /* if `logName` was full path then we need to be confident
   * that this curve belong to this well */
  if (!logType.empty() && logType[0] == '/')
    if (!isSuccessor(logG->getPath(), logType))
      return nullptr;

  if (!logName.empty() && logName[0] == '/' &&
      !logType.empty() && logType[0] == '/')
    if (!isSuccessor(logType, logName))
      return nullptr;

  if (!logG->hasObject(logType, h5gt::ObjectType::Group))
    return nullptr;

  h5gt::Group logTypeG = logG->getGroup(logType);

  if (!logTypeG.hasObject(logName, h5gt::ObjectType::Group))
    return nullptr;

  h5gt::Group curveG = logTypeG.getGroup(logName);

  return getLogCurve(curveG);
}

H5LogCurve* H5WellImpl::getLogCurve(
    h5gt::Group group)
{
  auto logG = getLogG();
  if (!logG.has_value())
    return nullptr;

  if (!isSuccessor(logG.value(), group))
    return nullptr;

  if (!isGeoObject(group, h5geo::ObjectType::LOGCURVE))
    return nullptr;

  return new H5LogCurveImpl(group);
}

H5DevCurve* H5WellImpl::getDevCurve(
    const std::string &devName)
{
  auto devG = getDevG();
  if (!devG.has_value())
    return nullptr;

  /* if `logName` was full path then we need to be confident
   * that this curve belong to this well */
  if (!devName.empty() && devName[0] == '/')
    if (!isSuccessor(devG->getPath(), devName))
      return nullptr;

  if (!devG->hasObject(devName, h5gt::ObjectType::Group))
    return nullptr;

  h5gt::Group curveG = devG->getGroup(devName);

  return getDevCurve(curveG);
}

H5DevCurve* H5WellImpl::getDevCurve(
    h5gt::Group group)
{
  auto devG = getDevG();
  if (!devG.has_value())
    return nullptr;

  if (!isSuccessor(devG.value(), group))
    return nullptr;

  if (!isGeoObject(group, h5geo::ObjectType::DEVCURVE))
    return nullptr;

  return new H5DevCurveImpl(group);
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
    if (!isSuccessor(logG->getPath(), logType))
      return nullptr;

  if (!logName.empty() && logName[0] == '/' &&
      !logType.empty() && logType[0] == '/')
    if (!isSuccessor(logType, logName))
      return nullptr;

  if (logType.empty())
    logType = "UNKNOWN";

  if (!logG->hasObject(logType, h5gt::ObjectType::Group))
    logG->createGroup(logType);

  h5gt::Group logTypeG = logG->getGroup(logType);

  auto opt = createObject(
        logName, logTypeG, h5geo::ObjectType::LOGCURVE, &p, createFlag);

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
    if (!isSuccessor(devG->getPath(), devName))
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

Eigen::Vector2d H5WellImpl::getHeadCoord(){
  std::string name = std::string{magic_enum::enum_name(
        h5geo::detail::WellAttributes::head_coord)};
  Eigen::Vector2d v(2);

  if (!objG.hasAttribute(name))
    return Eigen::Vector2d();

  objG.getAttribute(name).read(v.data());
  return v;
}

double H5WellImpl::getKB(){
  std::string name = std::string{magic_enum::enum_name(
        h5geo::detail::WellAttributes::KB)};
  double x;

  if (!objG.hasAttribute(name))
    return NAN;

  objG.getAttribute(name).read(x);
  return x;
}

std::string H5WellImpl::getUWI(){
  std::string name = std::string{magic_enum::enum_name(
        h5geo::detail::WellAttributes::UWI)};
  std::string uwi;

  if (!objG.hasAttribute(name))
    return std::string();

  objG.getAttribute(name).read(uwi);
  return uwi;
}

bool H5WellImpl::setHeadCoord(
    const Eigen::Ref<const Eigen::Vector2d>& v)
{
  std::string name = std::string{magic_enum::enum_name(
        h5geo::detail::WellAttributes::head_coord)};

  if (!objG.hasAttribute(name))
    return false;

  objG.getAttribute(name).write_raw(v.data());
  return true;
}

bool H5WellImpl::setKB(const double& kb){
  std::string name = std::string{magic_enum::enum_name(
        h5geo::detail::WellAttributes::KB)};

  if (!objG.hasAttribute(name))
    return false;

  objG.getAttribute(name).write(kb);
  return true;
}

bool H5WellImpl::setUWI(const std::string& uwi){
  std::string name = std::string{magic_enum::enum_name(
        h5geo::detail::WellAttributes::KB)};

  if (!objG.hasAttribute(name))
    return false;

  objG.getAttribute(name).write(uwi);
  return true;
}

H5DevCurve* H5WellImpl::getActiveDevCurve(){
  auto opt = getActiveDevG();
  if (!opt.has_value())
    return nullptr;

  std::string path = opt->getTargetPath();
  opt->hasObject(path, h5gt::ObjectType::Group);
  return getDevCurve(opt->getTargetPath());
}

std::vector<H5DevCurve*>
H5WellImpl::getDevCurveList(){
  std::vector<h5gt::Group> childGroupList =
      getChildList(objG, h5geo::ObjectType::DEVCURVE);;

  std::vector<H5DevCurve*> childList;
  childList.reserve(childGroupList.size());
  for (size_t i = 0; i < childGroupList.size(); i++){
    H5DevCurve* curve = getDevCurve(childGroupList[i]);
    if (curve != nullptr)
      childList.push_back(curve);
  }
  childList.shrink_to_fit();
  return childList;
}

std::vector<H5LogCurve*>
H5WellImpl::getLogCurveList(){
  std::vector<h5gt::Group> childGroupList =
      getChildList(objG, h5geo::ObjectType::LOGCURVE);;

  std::vector<H5LogCurve*> childList;
  childList.reserve(childGroupList.size());
  for (size_t i = 0; i < childGroupList.size(); i++){
    H5LogCurve* curve = getLogCurve(childGroupList[i]);
    if (curve != nullptr)
      childList.push_back(curve);
  }
  childList.shrink_to_fit();
  return childList;
}

std::vector<std::string>
H5WellImpl::getDevCurveNameList(){
  std::vector<H5DevCurve*> curveList = getDevCurveList();
  std::vector<std::string> curveNameList(curveList.size());

  for (size_t i = 0; i < curveList.size(); i++){
    curveNameList[i] = curveList[i]->getRelativeCurveName();
    curveList[i]->Delete();
  }
  return curveNameList;
}

std::vector<std::string>
H5WellImpl::getLogCurveNameList(){
  std::vector<H5LogCurve*> curveList = getLogCurveList();
  std::vector<std::string> curveNameList(curveList.size());

  for (size_t i = 0; i < curveList.size(); i++){
    curveNameList[i] = curveList[i]->getRelativeCurveName();
    curveList[i]->Delete();
  }
  return curveNameList;
}

std::vector<std::string>
H5WellImpl::getLogTypeNameList(){
  auto logG = getLogG();
  if (!logG.has_value())
    return std::vector<std::string>();

  std::vector<std::string> logTypeNameList;
  std::vector<std::string> childNameList = logG->listObjectNames();
  logTypeNameList.reserve(childNameList.size());
  for (const auto& name : childNameList){
    if (logG->getObjectType(name) == h5gt::ObjectType::Group)
      logTypeNameList.push_back(name);
  }
  logTypeNameList.shrink_to_fit();
  return logTypeNameList;
}

H5WellContainer* H5WellImpl::getWellContainer(){
  h5gt::File file = getH5File();
  return h5geo::createWellContainer(
        file, h5geo::CreationType::OPEN_OR_CREATE);
}

std::optional<h5gt::Group>
H5WellImpl::getDevG()
{
  std::string name = std::string{magic_enum::enum_name(
        h5geo::detail::WellGroups::DEV)};

  if (!objG.hasObject(name, h5gt::ObjectType::Group))
    return std::nullopt;

  return objG.getGroup(name);
}

std::optional<h5gt::Group>
H5WellImpl::getActiveDevG()
{
  std::string name = std::string{magic_enum::enum_name(
        h5geo::detail::WellGroups::ACTIVE_DEV)};

  return getGroupOpt(objG, name);
}

std::optional<h5gt::Group>
H5WellImpl::getLogG()
{
  std::string name = std::string{magic_enum::enum_name(
        h5geo::detail::WellGroups::LOG)};

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

bool H5WellImpl::setActiveDevCurve(H5DevCurve& curve){
  auto opt = getActiveDevG();
  if (opt.has_value())
    objG.unlink(opt->getPath());

  objG.createLink(curve.getObjG(),
                  std::string{magic_enum::enum_name(h5geo::detail::WellGroups::ACTIVE_DEV)},
                  h5gt::LinkType::Soft);

  return true;
}
