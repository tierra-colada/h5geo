#include "../../include/h5geo/misc/h5surfcontainerimpl.h"
#include "../../include/h5geo/h5surf.h"
#include "../../include/h5geo/misc/h5surfimpl.h"

H5SurfContainerImpl::H5SurfContainerImpl(const h5gt::File &h5File) :
  H5BaseContainerImpl(h5File){}

H5Surf* H5SurfContainerImpl::getSurf(const std::string &name)
{
  if (!h5File.hasObject(name, h5gt::ObjectType::Group))
    return nullptr;

  h5gt::Group group = h5File.getGroup(name);
  return getSurf(group);
}

H5Surf* H5SurfContainerImpl::getSurf(
    h5gt::Group group)
{
  if (!isGeoObject(group, h5geo::ObjectType::SURFACE))
    return nullptr;

  return new H5SurfImpl(group);
}

H5Surf* H5SurfContainerImpl::createSurf(
    std::string& name,
    SurfParam &p,
    h5geo::CreationType createFlag)
{
  auto opt = createObject(
        name, h5File, h5geo::ObjectType::SURFACE, &p, createFlag);

  if (!opt.has_value())
    return nullptr;

  return new H5SurfImpl(opt.value());
}

H5Surf* H5SurfContainerImpl::createSurf(
    h5gt::Group group,
    SurfParam &p,
    h5geo::CreationType createFlag)
{
  auto opt = createObject(
        group, h5geo::ObjectType::SURFACE, &p, createFlag);

  if (!opt.has_value())
    return nullptr;

  return new H5SurfImpl(opt.value());
}

std::vector<H5Surf*>
H5SurfContainerImpl::getSurfList() {
  h5gt::Group group = h5File.getGroup("/");
  std::vector<h5gt::Group> childGroupList =
      getChildList(group, h5geo::ObjectType::SURFACE);

  std::vector<H5Surf*> childList;
  childList.reserve(childGroupList.size());
  for (size_t i = 0; i < childGroupList.size(); i++){
    H5Surf* surf = getSurf(childGroupList[i]);
    if (surf != nullptr)
      childList.push_back(surf);
  }
  childList.shrink_to_fit();
  return childList;
}

H5SurfContainer*
h5geo::createSurfContainer(
    h5gt::File h5File, h5geo::CreationType createFlag)
{
  auto opt = H5SurfContainerImpl::createContainer(
        h5File, h5geo::ContainerType::SURFACE, createFlag);
  if (!opt.has_value())
    return nullptr;

  return new H5SurfContainerImpl(opt.value());
}

H5SurfContainer*
h5geo::createSurfContainerByName(
    std::string& fileName, h5geo::CreationType createFlag)
{
  auto opt = H5SurfContainerImpl::createContainer(
        fileName, h5geo::ContainerType::SURFACE, createFlag);
  if (!opt.has_value())
    return nullptr;

  return new H5SurfContainerImpl(opt.value());
}

H5SurfContainer*
h5geo::openSurfContainer(
    h5gt::File h5File){
  return createSurfContainer(h5File, h5geo::CreationType::OPEN);
}

H5SurfContainer*
h5geo::openSurfContainerByName(
    std::string& fileName){
  return createSurfContainerByName(fileName, h5geo::CreationType::OPEN);
}
