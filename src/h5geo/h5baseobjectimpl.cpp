#include "../../include/h5geo/private/h5baseobjectimpl.h"
#include "../../include/h5geo/private/h5mapimpl.h"
#include "../../include/h5geo/private/h5seisimpl.h"
#include "../../include/h5geo/private/h5wellimpl.h"
#include "../../include/h5geo/private/h5devcurveimpl.h"
#include "../../include/h5geo/private/h5logcurveimpl.h"
#include "../../include/h5geo/private/h5basepointsimpl.h"
#include "../../include/h5geo/private/h5points1impl.h"
#include "../../include/h5geo/private/h5points2impl.h"
#include "../../include/h5geo/private/h5points3impl.h"
#include "../../include/h5geo/private/h5points4impl.h"
#include "../../include/h5geo/private/h5welltopsimpl.h"
#include "../../include/h5geo/private/h5horizonimpl.h"
#include "../../include/h5geo/h5core.h"
#include "../../include/h5geo/private/h5enum_string.h"

#ifdef H5GEO_USE_GDAL
#include <gdal.h>
#include <gdal_priv.h>
#include "../../include/h5geo/private/h5core_sr_settings.h"
#endif

template <typename TBase>
H5BaseObjectImpl<TBase>::H5BaseObjectImpl(const h5gt::Group &group) :
  objG(group){}

template <>
H5Base* H5BaseObjectImpl<H5BaseObject>::clone()
{
  return new H5BaseObjectImpl<H5BaseObject>(objG);
}

template <>
H5Base* H5BaseObjectImpl<H5Map>::clone()
{
  return new H5MapImpl(objG);
}

template <>
H5Base* H5BaseObjectImpl<H5Seis>::clone()
{
  return new H5SeisImpl(objG);
}

template <>
H5Base* H5BaseObjectImpl<H5Well>::clone()
{
  return new H5WellImpl(objG);
}

template <>
H5Base* H5BaseObjectImpl<H5DevCurve>::clone()
{
  return new H5DevCurveImpl(objG);
}

template <>
H5Base* H5BaseObjectImpl<H5LogCurve>::clone()
{
  return new H5LogCurveImpl(objG);
}

template <>
H5Base* H5BaseObjectImpl<H5BasePoints>::clone()
{
  return new H5BasePointsImpl(objG);
}

template <>
H5Base* H5BaseObjectImpl<H5Points1>::clone()
{
  return new H5Points1Impl(objG);
}

template <>
H5Base* H5BaseObjectImpl<H5Points2>::clone()
{
  return new H5Points2Impl(objG);
}

template <>
H5Base* H5BaseObjectImpl<H5Points3>::clone()
{
  return new H5Points3Impl(objG);
}

template <>
H5Base* H5BaseObjectImpl<H5Points4>::clone()
{
  return new H5Points4Impl(objG);
}

template <>
H5Base* H5BaseObjectImpl<H5WellTops>::clone()
{
  return new H5WellTopsImpl(objG);
}

template <>
H5Base* H5BaseObjectImpl<H5Horizon>::clone()
{
  return new H5HorizonImpl(objG);
}

template <typename TBase>
H5BasePoints* H5BaseObjectImpl<TBase>::openPoints(
    const std::string& name)
{
  if (!objG.hasObject(name, h5gt::ObjectType::Group))
    return nullptr;

  h5gt::Group group = objG.getGroup(name);
  return openPoints(group);
}

template <typename TBase>
H5BasePoints* H5BaseObjectImpl<TBase>::openPoints(
    h5gt::Group group)
{
  return h5geo::openPoints(group);
}

template <typename TBase>
H5Horizon* H5BaseObjectImpl<TBase>::openHorizon(
    const std::string& name)
{
  if (!objG.hasObject(name, h5gt::ObjectType::Group))
    return nullptr;

  h5gt::Group group = objG.getGroup(name);
  return openHorizon(group);
}

template <typename TBase>
H5Horizon* H5BaseObjectImpl<TBase>::openHorizon(
    h5gt::Group group)
{
  return openHorizon(group);
}

template <typename TBase>
H5Points1* H5BaseObjectImpl<TBase>::createPoints1(
    std::string& name,
    PointsParam& p,
    h5geo::CreationType createFlag)
{
  auto opt = H5BaseImpl<TBase>::createObject(
        name, objG, h5geo::ObjectType::POINTS_1, &p, createFlag);

  if (!opt.has_value())
    return nullptr;

  return new H5Points1Impl(opt.value());
}

template <typename TBase>
H5Points1* H5BaseObjectImpl<TBase>::createPoints1(
    h5gt::Group group,
    PointsParam& p,
    h5geo::CreationType createFlag)
{
  auto opt = H5BaseImpl<TBase>::createObject(
        group, h5geo::ObjectType::POINTS_1, &p, createFlag);

  if (!opt.has_value())
    return nullptr;

  return new H5Points1Impl(opt.value());
}

template <typename TBase>
H5Points2* H5BaseObjectImpl<TBase>::createPoints2(
    std::string& name,
    PointsParam& p,
    h5geo::CreationType createFlag)
{
  auto opt = H5BaseImpl<TBase>::createObject(
        name, objG, h5geo::ObjectType::POINTS_2, &p, createFlag);

  if (!opt.has_value())
    return nullptr;

  return new H5Points2Impl(opt.value());
}

template <typename TBase>
H5Points2* H5BaseObjectImpl<TBase>::createPoints2(
    h5gt::Group group,
    PointsParam& p,
    h5geo::CreationType createFlag)
{
  auto opt = H5BaseImpl<TBase>::createObject(
        group, h5geo::ObjectType::POINTS_2, &p, createFlag);

  if (!opt.has_value())
    return nullptr;

  return new H5Points2Impl(opt.value());
}

template <typename TBase>
H5Points3* H5BaseObjectImpl<TBase>::createPoints3(
    std::string& name,
    PointsParam& p,
    h5geo::CreationType createFlag)
{
  auto opt = H5BaseImpl<TBase>::createObject(
        name, objG, h5geo::ObjectType::POINTS_3, &p, createFlag);

  if (!opt.has_value())
    return nullptr;

  return new H5Points3Impl(opt.value());
}

template <typename TBase>
H5Points3* H5BaseObjectImpl<TBase>::createPoints3(
    h5gt::Group group,
    PointsParam& p,
    h5geo::CreationType createFlag)
{
  auto opt = H5BaseImpl<TBase>::createObject(
        group, h5geo::ObjectType::POINTS_3, &p, createFlag);

  if (!opt.has_value())
    return nullptr;

  return new H5Points3Impl(opt.value());
}

template <typename TBase>
H5Points4* H5BaseObjectImpl<TBase>::createPoints4(
    std::string& name,
    PointsParam& p,
    h5geo::CreationType createFlag)
{
  auto opt = H5BaseImpl<TBase>::createObject(
        name, objG, h5geo::ObjectType::POINTS_4, &p, createFlag);

  if (!opt.has_value())
    return nullptr;

  return new H5Points4Impl(opt.value());
}

template <typename TBase>
H5Points4* H5BaseObjectImpl<TBase>::createPoints4(
    h5gt::Group group,
    PointsParam& p,
    h5geo::CreationType createFlag)
{
  auto opt = H5BaseImpl<TBase>::createObject(
        group, h5geo::ObjectType::POINTS_4, &p, createFlag);

  if (!opt.has_value())
    return nullptr;

  return new H5Points4Impl(opt.value());
}

template <typename TBase>
H5Horizon* H5BaseObjectImpl<TBase>::createHorizon(
    std::string& name,
    HorizonParam& p,
    h5geo::CreationType createFlag)
{
  auto opt = H5BaseImpl<TBase>::createObject(
        name, objG, h5geo::ObjectType::HORIZON, &p, createFlag);

  if (!opt.has_value())
    return nullptr;

  return new H5HorizonImpl(opt.value());
}

template <typename TBase>
H5Horizon* H5BaseObjectImpl<TBase>::createHorizon(
    h5gt::Group group,
    HorizonParam& p,
    h5geo::CreationType createFlag)
{
  auto opt = H5BaseImpl<TBase>::createObject(
        group, h5geo::ObjectType::HORIZON, &p, createFlag);

  if (!opt.has_value())
    return nullptr;

  return new H5HorizonImpl(opt.value());
}

#ifdef H5GEO_USE_GDAL

template <typename TBase>
OGRCoordinateTransformation*
H5BaseObjectImpl<TBase>::createCoordinateTransformationToReadData(
    const std::string& unitsTo)
{
  std::string unitsFrom = getLengthUnits();
  double coefFrom = units::convert(
        units::unit_from_string(unitsFrom),
        units::meter);

  if (isnan(coefFrom))
    return nullptr;

  double coefTo = units::convert(
        units::unit_from_string(unitsTo),
        units::meter);

  if (isnan(coefTo))
    return nullptr;

  std::string srAuthAndCodeFrom = getSpatialReference();
  OGRSpatialReference srFrom;
  if (srFrom.SetFromUserInput(srAuthAndCodeFrom.c_str()) != OGRERR_NONE)
    return nullptr;

  OGRSpatialReference srTo = h5geo::sr::getSpatialReference();

  if (srFrom.SetLinearUnitsAndUpdateParameters(unitsFrom.c_str(), coefFrom) != OGRERR_NONE)
    return nullptr;

  if (srTo.SetLinearUnitsAndUpdateParameters(unitsTo.c_str(), coefTo) != OGRERR_NONE)
    return nullptr;

  return OGRCreateCoordinateTransformation(&srFrom, &srTo);
}

template <typename TBase>
OGRCoordinateTransformation*
H5BaseObjectImpl<TBase>::createCoordinateTransformationToWriteData(
    const std::string &unitsFrom)
{
  double coefFrom = units::convert(
        units::unit_from_string(unitsFrom),
        units::meter);

  if (isnan(coefFrom))
    return nullptr;

  std::string unitsTo = getLengthUnits();
  double coefTo = units::convert(
        units::unit_from_string(unitsTo),
        units::meter);

  if (isnan(coefTo))
    return nullptr;

  OGRSpatialReference srFrom = h5geo::sr::getSpatialReference();

  std::string srAuthAndCodeTo = getSpatialReference();
  OGRSpatialReference srTo;
  if (srTo.SetFromUserInput(srAuthAndCodeTo.c_str()) != OGRERR_NONE)
    return nullptr;

  if (srFrom.SetLinearUnitsAndUpdateParameters(unitsFrom.c_str(), coefFrom) != OGRERR_NONE)
    return nullptr;

  if (srTo.SetLinearUnitsAndUpdateParameters(unitsTo.c_str(), coefTo) != OGRERR_NONE)
    return nullptr;

  return OGRCreateCoordinateTransformation(&srFrom, &srTo);
}

#endif

template <typename TBase>
bool H5BaseObjectImpl<TBase>::setSpatialReference(const std::string& str){
  return h5geo::overwriteAttribute(
        objG,
        std::string{h5geo::detail::spatial_reference},
        str);
}

template <typename TBase>
bool H5BaseObjectImpl<TBase>::setSpatialReference(
    const std::string& authName, const std::string& code){
  return h5geo::overwriteAttribute(
        objG,
        std::string{h5geo::detail::spatial_reference},
        authName + ":" + code);
}

template <typename TBase>
bool H5BaseObjectImpl<TBase>::setLengthUnits(const std::string& str){
  return h5geo::overwriteAttribute(
        objG,
        std::string{h5geo::detail::length_units},
        str);
}

template <typename TBase>
bool H5BaseObjectImpl<TBase>::setTemporalUnits(const std::string& str){
  return h5geo::overwriteAttribute(
        objG,
        std::string{h5geo::detail::temporal_units},
        str);
}

template <typename TBase>
bool H5BaseObjectImpl<TBase>::setAngularUnits(const std::string& str){
  return h5geo::overwriteAttribute(
        objG,
        std::string{h5geo::detail::angular_units},
        str);
}

template <typename TBase>
bool H5BaseObjectImpl<TBase>::setDataUnits(const std::string& str){
  return h5geo::overwriteAttribute(
        objG,
        std::string{h5geo::detail::data_units},
        str);
}

template <typename TBase>
bool H5BaseObjectImpl<TBase>::setNullValue(double val){
  return h5geo::overwriteAttribute(
        objG,
        std::string{h5geo::detail::null_value},
        val);
}

template <typename TBase>
std::string H5BaseObjectImpl<TBase>::getSpatialReference(){
  return h5geo::readStringAttribute(
        objG,
        std::string{h5geo::detail::spatial_reference});
}

template <typename TBase>
std::string H5BaseObjectImpl<TBase>::getLengthUnits(){
  return h5geo::readStringAttribute(
        objG,
        std::string{h5geo::detail::length_units});
}

template <typename TBase>
std::string H5BaseObjectImpl<TBase>::getTemporalUnits(){
  return h5geo::readStringAttribute(
        objG,
        std::string{h5geo::detail::temporal_units});
}

template <typename TBase>
std::string H5BaseObjectImpl<TBase>::getAngularUnits(){
  return h5geo::readStringAttribute(
        objG,
        std::string{h5geo::detail::angular_units});
}

template <typename TBase>
std::string H5BaseObjectImpl<TBase>::getDataUnits(){
  return h5geo::readStringAttribute(
        objG,
        std::string{h5geo::detail::data_units});
}

template <typename TBase>
double H5BaseObjectImpl<TBase>::getNullValue(){
  return h5geo::readDoubleAttribute(
        objG,
        std::string{h5geo::detail::null_value});
}

template <typename TBase>
h5gt::File H5BaseObjectImpl<TBase>::getH5File() const {
  return objG.getFile();
}

template <typename TBase>
h5gt::Group H5BaseObjectImpl<TBase>::getObjG() const {
  return objG;
}

template <typename TBase>
std::optional<h5gt::Group>
H5BaseObjectImpl<TBase>::getGroupOpt(
    h5gt::Group& parent,
    const std::string& name) const 
{
  if (!parent.hasObject(name, h5gt::ObjectType::Group))
    return std::nullopt;

  return parent.getGroup(name);
}

template <typename TBase>
std::optional<h5gt::DataSet>
H5BaseObjectImpl<TBase>::getDatasetOpt(
    const h5gt::Group& parent,
    const std::string& name) const 
{
  if (!parent.hasObject(name, h5gt::ObjectType::Dataset))
    return std::nullopt;

  return parent.getDataSet(name);
}

template <typename TBase>
std::string H5BaseObjectImpl<TBase>::getName() const {
  std::string objName;
  h5geo::splitPathToParentAndObj(objG.getPath(), objName);
  return objName;
}

template <typename TBase>
std::string H5BaseObjectImpl<TBase>::getFullName() const {
  return objG.getPath();
}

template <typename TBase>
std::vector<h5gt::Group>
H5BaseObjectImpl<TBase>::getObjGroupList(const h5geo::ObjectType& objType, bool recursive){
  return H5BaseImpl<TBase>::getChildGroupList(objG, objType, recursive);
}

template <typename TBase>
std::vector<std::string>
H5BaseObjectImpl<TBase>::getObjNameList(const h5geo::ObjectType& objType, bool recursive){
  return H5BaseImpl<TBase>::getChildNameList(objG, objType, objG.getPath(), recursive);
}

template <typename TBase>
size_t
H5BaseObjectImpl<TBase>::getObjCount(const h5geo::ObjectType& objType, bool recursive){
  return H5BaseImpl<TBase>::getChildCount(objG, objType, recursive);
}

template <typename TBase>
bool H5BaseObjectImpl<TBase>::isEqual(H5BaseObject* other) const{
  if (!other)
    return false;

  // to compare h5gt objects their files must be open
  h5gt::File file1 = this->getH5File();
  h5gt::File file2 = other->getH5File();
  return objG == other->getObjG();
}

template <typename TBase>
bool H5BaseObjectImpl<TBase>::operator == (const H5BaseObject& other) const {
  // to compare h5gt objects their files must be open
  h5gt::File file1 = this->getH5File();
  h5gt::File file2 = other.getH5File();
  return objG == other.getObjG();
}

template <typename TBase>
bool H5BaseObjectImpl<TBase>::operator != (const H5BaseObject& other) const {
  return !(*this == other);
}

template <typename TBase>
std::optional<h5gt::Group>
H5BaseObjectImpl<TBase>::getParentG(
    const h5geo::ObjectType& objType)
{
  std::string path, objName;
  h5gt::Group parentGroup = objG;

  do {
    path = h5geo::splitPathToParentAndObj(
          parentGroup.getPath(), objName);
    if (!parentGroup.hasObject(path, h5gt::ObjectType::Group))
      return std::nullopt;

    parentGroup = parentGroup.getGroup(path);
    if (isGeoObjectByType(parentGroup, objType))
      return parentGroup;

  } while (!objName.empty());

  return std::nullopt;
}

// explicit instantiation (requires that corresponding headers are included)
template class H5BaseObjectImpl<H5BaseObject>;
template class H5BaseObjectImpl<H5Map>;
template class H5BaseObjectImpl<H5Seis>;
template class H5BaseObjectImpl<H5Well>;
template class H5BaseObjectImpl<H5DevCurve>;
template class H5BaseObjectImpl<H5LogCurve>;
template class H5BaseObjectImpl<H5BasePoints>;
template class H5BaseObjectImpl<H5Points1>;
template class H5BaseObjectImpl<H5Points2>;
template class H5BaseObjectImpl<H5Points3>;
template class H5BaseObjectImpl<H5Points4>;
template class H5BaseObjectImpl<H5WellTops>;
template class H5BaseObjectImpl<H5Horizon>;
