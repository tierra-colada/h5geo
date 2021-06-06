#include "../../include/h5geo/h5well.h"
#include "../../include/h5geo/h5wellcontainer.h"

H5Well::H5Well(const h5gt::Group &group) :
  H5BaseObject(group)
{

}

H5Well::~H5Well(){

}

std::optional<H5Well>
H5Well::create(h5gt::Group &group){
  if (H5BaseContainer::isObject(
        group, h5geo::ObjectType::WELL))
    return H5Well(group);
  else
    return std::nullopt;
}

Eigen::Vector2d H5Well::getHeadCoord(){
  std::string name = std::string{magic_enum::enum_name(
        h5geo::WellAttributes::head_coord)};
  Eigen::Vector2d v(2);

  if (!objG.hasAttribute(name))
    return Eigen::Vector2d();

  objG.getAttribute(name).read(v.data());
  return v;
}

double H5Well::getKB(){
  std::string name = std::string{magic_enum::enum_name(
        h5geo::WellAttributes::KB)};
  double x;

  if (!objG.hasAttribute(name))
    return NAN;

  objG.getAttribute(name).read(x);
  return x;
}

std::string H5Well::getUWI(){
  std::string name = std::string{magic_enum::enum_name(
        h5geo::WellAttributes::UWI)};
  std::string uwi;

  if (!objG.hasAttribute(name))
    return std::string();

  objG.getAttribute(name).read(uwi);
  return uwi;
}

bool H5Well::setHeadCoord(
    const Eigen::Ref<const Eigen::Vector2d>& v)
{
  std::string name = std::string{magic_enum::enum_name(
        h5geo::WellAttributes::head_coord)};

  if (!objG.hasAttribute(name))
    return false;

  objG.getAttribute(name).write_raw(v.data());
  return true;
}

bool H5Well::setKB(const double& kb){
  std::string name = std::string{magic_enum::enum_name(
        h5geo::WellAttributes::KB)};

  if (!objG.hasAttribute(name))
    return false;

  objG.getAttribute(name).write(kb);
  return true;
}

bool H5Well::setUWI(const std::string& uwi){
  std::string name = std::string{magic_enum::enum_name(
        h5geo::WellAttributes::KB)};

  if (!objG.hasAttribute(name))
    return false;

  objG.getAttribute(name).write(uwi);
  return true;
}

bool H5Well::setActiveDevCurve(H5DevCurve& curve){
  auto opt = getActiveDevG();
  if (opt.has_value())
    objG.unlink(opt->getPath());

  objG.createLink(curve.getObjG(),
                  std::string{magic_enum::enum_name(h5geo::WellGroups::ACTIVE_DEV)},
                  h5gt::LinkType::Soft);

  return true;
}

std::optional<H5LogCurve>
H5Well::getLogCurve(
    const std::string &logType,
    const std::string &logName)
{
  auto opt = getLogG();
  if (!opt.has_value())
    return std::nullopt;

  if (!opt->hasObject(logType, h5gt::ObjectType::Group))
    return std::nullopt;

  h5gt::Group logTypeG = opt->getGroup(logType);

  if (!logTypeG.hasObject(logName, h5gt::ObjectType::Group))
    return std::nullopt;

  h5gt::Group curveG = logTypeG.getGroup(logName);

  auto logCurve = H5LogCurve::create(curveG);

  if (!logCurve.has_value())
    return std::nullopt;

  /* if `logName` was full path then we need to be confident
   * that this curve belong to this well */
  auto well = logCurve->getWell();

  if (!well.has_value())
    return std::nullopt;

  if (*this != well.value())
    return std::nullopt;

  return logCurve;
}

std::optional<H5DevCurve>
H5Well::getActiveDevCurve(){
  auto opt = getActiveDevG();
  if (!opt.has_value())
    return std::nullopt;

  std::string path = opt->getTargetPath();
  opt->hasObject(path, h5gt::ObjectType::Group);
  return getDevCurve(opt->getTargetPath());
}

std::optional<H5DevCurve>
H5Well::getDevCurve(const std::string &devName){
  auto opt = getDevG();
  if (!opt.has_value())
    return std::nullopt;

  if (!opt->hasObject(devName, h5gt::ObjectType::Group))
    return std::nullopt;

  h5gt::Group curveG = opt->getGroup(devName);

  auto curve = H5DevCurve::create(curveG);

  if (!curve.has_value())
    return std::nullopt;

  /* if `devName` was full path then we need to be confident
   * that this curve belong to this well */
  auto well = curve->getWell();

  if (!well.has_value())
    return std::nullopt;

  if (*this != well.value())
    return std::nullopt;

  return curve;
}

std::vector<H5DevCurve>
H5Well::getDevCurveList(){
  std::vector<h5gt::Group> childGroupList =
      getChildList(objG, h5geo::ObjectType::DEVCURVE);;

  std::vector<H5DevCurve> childList;
  childList.reserve(childGroupList.size());
  for (size_t i = 0; i < childGroupList.size(); i++){
    std::optional<H5DevCurve> opt = H5DevCurve::create(childGroupList[i]);
    if (opt.has_value())
      childList.push_back(opt.value());
  }
  childList.shrink_to_fit();
  return childList;
}

std::vector<H5LogCurve>
H5Well::getLogCurveList(){
  std::vector<h5gt::Group> childGroupList =
      getChildList(objG, h5geo::ObjectType::LOGCURVE);;

  std::vector<H5LogCurve> childList;
  childList.reserve(childGroupList.size());
  for (size_t i = 0; i < childGroupList.size(); i++){
    std::optional<H5LogCurve> opt = H5LogCurve::create(childGroupList[i]);
    if (opt.has_value())
      childList.push_back(opt.value());
  }
  childList.shrink_to_fit();
  return childList;
}

std::vector<std::string>
H5Well::getDevCurveNameList(){
  std::vector<H5DevCurve> curveList = getDevCurveList();
  std::vector<std::string> curveNameList(curveList.size());

  for (size_t i = 0; i < curveList.size(); i++){
    curveNameList[i] = curveList[i].getRelativeCurveName();
  }
  return curveNameList;
}

std::vector<std::string>
H5Well::getLogCurveNameList(){
  std::vector<H5LogCurve> curveList = getLogCurveList();
  std::vector<std::string> curveNameList(curveList.size());

  for (size_t i = 0; i < curveList.size(); i++){
    curveNameList[i] = curveList[i].getRelativeCurveName();
  }
  return curveNameList;
}

std::vector<std::string>
H5Well::getLogTypeNameList(){
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

std::optional<H5LogCurve>
H5Well::createLogCurve(
    h5gt::Group& group,
    LogCurveParam p,
    const h5geo::CreationType& createFlag)
{
  std::optional<h5gt::Group> opt = H5BaseContainer::createObject(
        group, h5geo::ObjectType::LOGCURVE,
        &p, createFlag);

  if (opt.has_value())
    return H5LogCurve::create(opt.value());

  return std::nullopt;
}

std::optional<H5LogCurve>
H5Well::createLogCurve(
    std::string& logType,
    std::string& logName,
    LogCurveParam p,
    const h5geo::CreationType& createFlag)
{
  auto logG = getLogG();
  if (!logG.has_value())
    return std::nullopt;

  /* If `logType` is full path then we need to be confident that
   * this `logType` (and `logName`) refers to this well */
  if (!logType.empty() && logType[0] == '/'){
    std::string wellPath = logG->getPath();
    if (logType.substr(0, wellPath.size()).find(wellPath) == std::string::npos) {
      return std::nullopt;
    }
  }

  if (!logName.empty() && logName[0] == '/'){
    std::string wellPath = logG->getPath();
    if (logName.substr(0, wellPath.size()).find(wellPath) == std::string::npos) {
      return std::nullopt;
    }
  }

  if (logType.empty())
    logType = "UNKNOWN";

  if (!logG->hasObject(logType, h5gt::ObjectType::Group))
    logG->createGroup(logType);

  h5gt::Group logTypeG = logG->getGroup(logType);

  std::optional<h5gt::Group> opt = H5Base::createObject(
        logName, logTypeG, h5geo::ObjectType::LOGCURVE,
        &p, createFlag);

  if (opt.has_value())
    return H5LogCurve::create(opt.value());

  return std::nullopt;
}

std::optional<H5DevCurve>
H5Well::createDevCurve(
    h5gt::Group& group,
    DevCurveParam p,
    const h5geo::CreationType& createFlag)
{
  std::optional<h5gt::Group> opt = H5BaseContainer::createObject(
        group,
        h5geo::ObjectType::DEVCURVE,
        &p,
        createFlag);

  if (opt.has_value())
    return H5DevCurve::create(opt.value());

  return std::nullopt;
}

std::optional<H5DevCurve>
H5Well::createDevCurve(
    std::string& devName,
    DevCurveParam p,
    const h5geo::CreationType& createFlag)
{
  auto devG = getDevG();
  if (!devG.has_value())
    return std::nullopt;

  /* If `devName` is full path then we need to be confident that
   * this `devName` refers to this well */
  if (!devName.empty() && devName[0] == '/'){
    std::string wellPath = devG->getPath();
    if (devName.substr(0, wellPath.size()).find(wellPath) == std::string::npos) {
      return std::nullopt;
    }
  }

  std::optional<h5gt::Group> opt = H5BaseContainer::createObject(
        devName, devG.value(), h5geo::ObjectType::DEVCURVE,
        &p, createFlag);

  if (opt.has_value())
    return H5DevCurve::create(opt.value());

  return std::nullopt;
}

std::optional<H5WellContainer>
H5Well::getWellContainer(){
  h5gt::File file = getH5File();
  return H5WellContainer::create(file);
}

std::optional<h5gt::Group>
H5Well::getDevG()
{
  std::string name = std::string{magic_enum::enum_name(
        h5geo::WellGroups::DEV)};

  if (!objG.hasObject(name, h5gt::ObjectType::Group))
    return std::nullopt;

  return objG.getGroup(name);
}

std::optional<h5gt::Group>
H5Well::getActiveDevG()
{
  std::string name = std::string{magic_enum::enum_name(
        h5geo::WellGroups::ACTIVE_DEV)};

  return getGroupOpt(objG, name);
}

std::optional<h5gt::Group>
H5Well::getLogG()
{
  std::string name = std::string{magic_enum::enum_name(
        h5geo::WellGroups::LOG)};

  return getGroupOpt(objG, name);
}

std::optional<h5gt::Group>
H5Well::getLogTypeG(const std::string& logType)
{
  auto logG = getLogG();
  if (!logG.has_value())
    return std::nullopt;

  if (!logG->hasObject(logType, h5gt::ObjectType::Group))
    return std::nullopt;

  return logG->getGroup(logType);
}
