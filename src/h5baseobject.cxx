#include "../include/h5base/h5baseobject.h"

H5BaseObject::H5BaseObject(const h5gt::Group& group) :
  objG(group)
{

}

H5BaseObject::~H5BaseObject(){

}

h5gt::File H5BaseObject::getH5File(){
  return h5gt::File::FromId(objG.getFileId(true), false);
}

h5gt::Group H5BaseObject::getObjG(){
  return objG;
}

std::optional<h5gt::Group>
H5BaseObject::getGroupOpt(
    h5gt::Group& parent,
    const std::string& name)
{
  if (!parent.hasObject(name, h5gt::ObjectType::Group))
    return std::nullopt;

  return parent.getGroup(name);
}

std::optional<h5gt::DataSet>
H5BaseObject::getDatasetOpt(
    h5gt::Group& parent,
    const std::string& name)
{
  if (!parent.hasObject(name, h5gt::ObjectType::Dataset))
    return std::nullopt;

  return parent.getDataSet(name);
}


std::string H5BaseObject::getNameWithinParentGroup(){
  std::string objName;
  h5geo::splitPathToParentAndObj(objG.getPath(), objName);
  return objName;
}

std::string H5BaseObject::getFullName(){
  return objG.getPath();
}

std::optional<h5gt::Group>
H5BaseObject::getParent(
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
    if (isObject(parentGroup, objType))
      return parentGroup;

  } while (!objName.empty());

  return std::nullopt;
}

bool H5BaseObject::operator == (H5BaseObject& other) {
  if (objG.getObjectInfo().getAddress() != other.getObjG().getObjectInfo().getAddress())
    return false;

  /* I would better implement this block to check file equality
       * but `H5Fget_fileno` was introduced only since hdf5 1.12.0 */

  unsigned long num, num_other;
  herr_t err = H5Fget_fileno(getH5File().getId(false), &num);
  if (err < 0)
    return false;

  err = H5Fget_fileno(other.getH5File().getId(false), &num_other);
  if (err < 0)
    return false;

  return num == num_other;
}

bool H5BaseObject::operator != (H5BaseObject& other) {
  return !(*this == other);
}
