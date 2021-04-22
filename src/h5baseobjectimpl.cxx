#include "../include/h5geo/misc/h5baseobjectimpl.h"
#include "../include/h5geo/h5core.h"

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
  h5geo::details::splitPathToParentAndObj(objG.getPath(), objName);
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
    path = h5geo::details::splitPathToParentAndObj(
          parentGroup.getPath(), objName);
    if (!parentGroup.hasObject(path, h5gt::ObjectType::Group))
      return std::nullopt;

    parentGroup = parentGroup.getGroup(path);
    if (isObject(parentGroup, objType))
      return parentGroup;

  } while (!objName.empty());

  return std::nullopt;
}

H5BaseObject*
h5geo::openBaseObject(h5gt::Group group)
{
  return new H5BaseObjectImpl(group);
}
