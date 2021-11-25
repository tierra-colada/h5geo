#include "../../include/h5geo/misc/h5baseobjectimpl.h"
#include "../../include/h5geo/h5core.h"
#include "../../include/h5geo/misc/h5core_enum_string.h"

#ifdef H5GEO_USE_GDAL
#include "../../include/h5geo/misc/h5core_sr_settings.h"
#endif

H5BaseObjectImpl::H5BaseObjectImpl(const h5gt::Group &group) :
  objG(group){}

#ifdef H5GEO_USE_GDAL

OGRCoordinateTransformation*
H5BaseObjectImpl::createCoordinateTransformationToReadData(
    const std::string& unitsTo)
{
  std::string unitsFrom = getSpatialUnits();
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

OGRCoordinateTransformation*
H5BaseObjectImpl::createCoordinateTransformationToWriteData(
    const std::string &unitsFrom)
{
  double coefFrom = units::convert(
        units::unit_from_string(unitsFrom),
        units::meter);

  if (isnan(coefFrom))
    return nullptr;

  std::string unitsTo = getSpatialUnits();
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

bool H5BaseObjectImpl::setSpatialReference(const std::string& str){
  return h5geo::overwriteAttribute(
        objG,
        std::string{h5geo::detail::spatial_reference},
        str);
}

bool H5BaseObjectImpl::setSpatialReference(
    const std::string& authName, const std::string& code){
  return h5geo::overwriteAttribute(
        objG,
        std::string{h5geo::detail::spatial_reference},
        authName + ":" + code);
}

bool H5BaseObjectImpl::setSpatialUnits(const std::string& str){
  return h5geo::overwriteAttribute(
        objG,
        std::string{h5geo::detail::spatial_units},
        str);
}

bool H5BaseObjectImpl::setTemporalUnits(const std::string& str){
  return h5geo::overwriteAttribute(
        objG,
        std::string{h5geo::detail::temporal_units},
        str);
}

bool H5BaseObjectImpl::setAngularUnits(const std::string& str){
  return h5geo::overwriteAttribute(
        objG,
        std::string{h5geo::detail::angular_units},
        str);
}

bool H5BaseObjectImpl::setDataUnits(const std::string& str){
  return h5geo::overwriteAttribute(
        objG,
        std::string{h5geo::detail::data_units},
        str);
}

std::string H5BaseObjectImpl::getSpatialReference(){
  return h5geo::readStringAttribute(
        objG,
        std::string{h5geo::detail::spatial_reference});
}

std::string H5BaseObjectImpl::getSpatialUnits(){
  return h5geo::readStringAttribute(
        objG,
        std::string{h5geo::detail::spatial_units});
}

std::string H5BaseObjectImpl::getTemporalUnits(){
  return h5geo::readStringAttribute(
        objG,
        std::string{h5geo::detail::temporal_units});
}

std::string H5BaseObjectImpl::getAngularUnits(){
  return h5geo::readStringAttribute(
        objG,
        std::string{h5geo::detail::angular_units});
}

std::string H5BaseObjectImpl::getDataUnits(){
  return h5geo::readStringAttribute(
        objG,
        std::string{h5geo::detail::data_units});
}

h5gt::File H5BaseObjectImpl::getH5File() const {
  return h5gt::File::FromId(objG.getFileId(true), false);
}

h5gt::Group H5BaseObjectImpl::getObjG() const {
  return objG;
}

std::optional<h5gt::Group>
H5BaseObjectImpl::getGroupOpt(
    h5gt::Group& parent,
    const std::string& name) const 
{
  if (!parent.hasObject(name, h5gt::ObjectType::Group))
    return std::nullopt;

  return parent.getGroup(name);
}

std::optional<h5gt::DataSet>
H5BaseObjectImpl::getDatasetOpt(
    const h5gt::Group& parent,
    const std::string& name) const 
{
  if (!parent.hasObject(name, h5gt::ObjectType::Dataset))
    return std::nullopt;

  return parent.getDataSet(name);
}

std::string H5BaseObjectImpl::getName() const {
  std::string objName;
  h5geo::splitPathToParentAndObj(objG.getPath(), objName);
  return objName;
}

std::string H5BaseObjectImpl::getFullName() const {
  return objG.getPath();
}

bool H5BaseObjectImpl::operator == (H5BaseObject& other) const {
  return objG == other.getObjG();
}

bool H5BaseObjectImpl::operator != (H5BaseObject& other) const {
  return !(*this == other);
}

std::optional<h5gt::Group>
H5BaseObjectImpl::getParentG(
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

H5BaseObject*
h5geo::openBaseObject(h5gt::Group group)
{
  return new H5BaseObjectImpl(group);
}
