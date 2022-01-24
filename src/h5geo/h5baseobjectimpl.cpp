#include "../../include/h5geo/misc/h5baseobjectimpl.h"
#include "../../include/h5geo/misc/h5mapimpl.h"
#include "../../include/h5geo/misc/h5seisimpl.h"
#include "../../include/h5geo/misc/h5wellimpl.h"
#include "../../include/h5geo/misc/h5devcurveimpl.h"
#include "../../include/h5geo/misc/h5logcurveimpl.h"
#include "../../include/h5geo/misc/h5pointsimpl.h"
#include "../../include/h5geo/h5core.h"
#include "../../include/h5geo/misc/h5core_enum_string.h"

#ifdef H5GEO_USE_GDAL
#include <gdal/gdal.h>
#include <gdal/gdal_priv.h>
#include "../../include/h5geo/misc/h5core_sr_settings.h"
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
H5Base* H5BaseObjectImpl<H5Points>::clone()
{
  return new H5PointsImpl(objG);
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

  double coeffTo = units::convert(
        units::unit_from_string(unitsTo),
        units::meter);

  if (isnan(coeffTo))
    return nullptr;

  std::string srAuthAndCodeFrom = getSpatialReference();
  OGRSpatialReference srFrom;
  if (srFrom.SetFromUserInput(srAuthAndCodeFrom.c_str()) != OGRERR_NONE)
    return nullptr;

  OGRSpatialReference srTo = h5geo::sr::getSpatialReference();

  srFrom.SetLinearUnitsAndUpdateParameters(unitsFrom.c_str(), coefFrom);
  srTo.SetLinearUnitsAndUpdateParameters(unitsTo.c_str(), coeffTo);
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

  srFrom.SetLinearUnitsAndUpdateParameters(unitsFrom.c_str(), coefFrom);
  srTo.SetLinearUnitsAndUpdateParameters(unitsTo.c_str(), coefTo);
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
h5gt::File H5BaseObjectImpl<TBase>::getH5File() const {
  return h5gt::File::FromId(objG.getFileId(true), false);
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
  h5geo::splitPathToParentAndObj(objG.getTargetPath(), objName);
  return objName;
}

template <typename TBase>
std::string H5BaseObjectImpl<TBase>::getFullName() const {
  return objG.getTargetPath();
}

template <typename TBase>
std::vector<h5gt::Group>
H5BaseObjectImpl<TBase>::getObjGroupList(const h5geo::ObjectType& objType){
  return H5BaseImpl<TBase>::getChildGroupList(objG, objType);
}

template <typename TBase>
std::vector<std::string>
H5BaseObjectImpl<TBase>::getObjNameList(const h5geo::ObjectType& objType){
  return H5BaseImpl<TBase>::getChildNameList(objG, objType);
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
          parentGroup.getTargetPath(), objName);
    if (!parentGroup.hasObject(path, h5gt::ObjectType::Group))
      return std::nullopt;

    parentGroup = parentGroup.getGroup(path);
    if (isGeoObjectByType(parentGroup, objType))
      return parentGroup;

  } while (!objName.empty());

  return std::nullopt;
}

H5BaseObject*
h5geo::openBaseObject(h5gt::Group group)
{
  return new H5BaseObjectImpl<H5BaseObject>(group);
}

// explicit instantiation (requires that corresponding headers are included)
template class H5BaseObjectImpl<H5BaseObject>;
template class H5BaseObjectImpl<H5Map>;
template class H5BaseObjectImpl<H5Seis>;
template class H5BaseObjectImpl<H5Well>;
template class H5BaseObjectImpl<H5DevCurve>;
template class H5BaseObjectImpl<H5LogCurve>;
template class H5BaseObjectImpl<H5Points>;
