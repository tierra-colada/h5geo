#include "../../include/h5geo/misc/h5baseobjectimpl.h"
#include "../../include/h5geo/misc/h5mapimpl.h"
#include "../../include/h5geo/misc/h5devcurveimpl.h"
#include "../../include/h5geo/misc/h5logcurveimpl.h"
#include "../../include/h5geo/misc/h5wellimpl.h"
#include "../../include/h5geo/misc/h5seisimpl.h"
#include "../../include/h5geo/h5core.h"

H5BaseObjectImpl::H5BaseObjectImpl(const h5gt::Group &group) :
  objG(group){}

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

H5BaseObject* h5geo::openObject(h5gt::Group group)
{
  if (isGeoObjectByType(group, h5geo::ObjectType::MAP))
      return new H5MapImpl(group);
  if (isGeoObjectByType(group, h5geo::ObjectType::DEVCURVE))
    return new H5DevCurveImpl(group);
  if (isGeoObjectByType(group, h5geo::ObjectType::LOGCURVE))
    return new H5LogCurveImpl(group);
  if (isGeoObjectByType(group, h5geo::ObjectType::WELL))
    return new H5WellImpl(group);
  if (isGeoObjectByType(group, h5geo::ObjectType::SEISMIC))
    return new H5SeisImpl(group);

  return nullptr;
}
