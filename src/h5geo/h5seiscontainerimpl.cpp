#include "../../include/h5geo/misc/h5seiscontainerimpl.h"
#include "../../include/h5geo/h5seis.h"
#include "../../include/h5geo/misc/h5seisimpl.h"

H5SeisContainerImpl::H5SeisContainerImpl(const h5gt::File &h5File) :
  H5BaseContainerImpl(h5File){}

H5Seis* H5SeisContainerImpl::getSeis(const std::string &name)
{
  if (!h5File.hasObject(name, h5gt::ObjectType::Group))
    return nullptr;

  h5gt::Group group = h5File.getGroup(name);
  return getSeis(group);
}

H5Seis* H5SeisContainerImpl::getSeis(
    h5gt::Group group)
{
  if (!isGeoObject(group, h5geo::ObjectType::SEISMIC))
    return nullptr;

  return new H5SeisImpl(group);
}

H5Seis* H5SeisContainerImpl::createSeis(
    std::string& name,
    SeisParam& p,
    h5geo::CreationType createFlag)
{
  auto opt = createObject(
        name, h5File, h5geo::ObjectType::SEISMIC, &p, createFlag);

  if (!opt.has_value())
    return nullptr;

  return new H5SeisImpl(opt.value());
}

H5Seis* H5SeisContainerImpl::createSeis(
    h5gt::Group group,
    SeisParam& p,
    h5geo::CreationType createFlag)
{
  auto opt = createObject(
        group, h5geo::ObjectType::SEISMIC, &p, createFlag);

  if (!opt.has_value())
    return nullptr;

  return new H5SeisImpl(opt.value());
}

std::vector<H5Seis*>
H5SeisContainerImpl::getSeisList(){
  h5gt::Group group = h5File.getGroup("/");
  std::vector<h5gt::Group> childGroupList =
      getChildList(group, h5geo::ObjectType::SEISMIC);

  std::vector<H5Seis*> childList;
  childList.reserve(childGroupList.size());
  for (size_t i = 0; i < childGroupList.size(); i++){
    H5Seis* seis = getSeis(childGroupList[i]);
    if (seis != nullptr)
      childList.push_back(seis);
  }
  childList.shrink_to_fit();
  return childList;
}

H5SeisContainer*
h5geo::createSeisContainer(
    h5gt::File h5File, h5geo::CreationType createFlag)
{
  auto opt = H5SeisContainerImpl::createContainer(
        h5File, h5geo::ContainerType::SEISMIC, createFlag);
  if (!opt.has_value())
    return nullptr;

  return new H5SeisContainerImpl(opt.value());
}

H5SeisContainer*
h5geo::createSeisContainerByName(
    std::string& fileName, h5geo::CreationType createFlag)
{
  auto opt = H5SeisContainerImpl::createContainer(
        fileName, h5geo::ContainerType::SEISMIC, createFlag);
  if (!opt.has_value())
    return nullptr;

  return new H5SeisContainerImpl(opt.value());
}

H5SeisContainer* h5geo::openSeisContainer(
    h5gt::File h5File){
  return createSeisContainer(h5File, h5geo::CreationType::OPEN);
}

H5SeisContainer* h5geo::openSeisContainerByName(
    std::string& fileName){
  return createSeisContainerByName(fileName, h5geo::CreationType::OPEN);
}

