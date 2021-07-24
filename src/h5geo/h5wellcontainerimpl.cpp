#include "../../include/h5geo/misc/h5wellcontainerimpl.h"
#include "../../include/h5geo/h5well.h"
#include "../../include/h5geo/misc/h5wellimpl.h"

H5WellContainerImpl::H5WellContainerImpl(const h5gt::File &h5File):
  H5BaseContainerImpl(h5File){}

H5Well* H5WellContainerImpl::getWell(
    const std::string& name)
{
  if (!h5File.hasObject(name, h5gt::ObjectType::Group))
    return nullptr;

  h5gt::Group group = h5File.getGroup(name);
  return getWell(group);
}

H5Well* H5WellContainerImpl::getWell(
    h5gt::Group group)
{
  if (!isGeoObject(group, h5geo::ObjectType::WELL))
    return nullptr;

  return new H5WellImpl(group);
}

H5Well* H5WellContainerImpl::createWell(
    std::string& name,
    WellParam& p,
    h5geo::CreationType createFlag)
{
  auto opt = createObject(
        name, h5File, h5geo::ObjectType::WELL, &p, createFlag);

  if (!opt.has_value())
    return nullptr;

  return new H5WellImpl(opt.value());
}

H5Well* H5WellContainerImpl::createWell(
    h5gt::Group group,
    WellParam& p,
    h5geo::CreationType createFlag)
{
  auto opt = createObject(
        group, h5geo::ObjectType::WELL, &p, createFlag);

  if (!opt.has_value())
    return nullptr;

  return new H5WellImpl(opt.value());
}

std::vector<H5Well*>
H5WellContainerImpl::getWellList(){
  h5gt::Group group = h5File.getGroup("/");
  std::vector<h5gt::Group> childGroupList =
      getChildList(group, h5geo::ObjectType::WELL);;

  std::vector<H5Well*> childList;
  childList.reserve(childGroupList.size());
  for (size_t i = 0; i < childGroupList.size(); i++){
    H5Well* well = getWell(childGroupList[i]);
    if (well != nullptr)
      childList.push_back(well);
  }
  childList.shrink_to_fit();
  return childList;
}

H5WellContainer*
h5geo::createWellContainer(
    h5gt::File h5File, h5geo::CreationType createFlag)
{
  auto opt = H5WellContainerImpl::createContainer(
        h5File, h5geo::ContainerType::WELL, createFlag);
  if (!opt.has_value())
    return nullptr;

  return new H5WellContainerImpl(opt.value());
}

H5WellContainer*
h5geo::createWellContainerByName(
    std::string& fileName, h5geo::CreationType createFlag)
{
  auto opt = H5WellContainerImpl::createContainer(
        fileName, h5geo::ContainerType::WELL, createFlag);
  if (!opt.has_value())
    return nullptr;

  return new H5WellContainerImpl(opt.value());
}

H5WellContainer*
h5geo::openWellContainer(h5gt::File h5File){
  return createWellContainer(h5File, h5geo::CreationType::OPEN);
}

H5WellContainer*
h5geo::openWellContainerByName(std::string& fileName){
  return createWellContainerByName(fileName, h5geo::CreationType::OPEN);
}

