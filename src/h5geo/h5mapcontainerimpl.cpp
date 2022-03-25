#include "../../include/h5geo/impl/h5mapcontainerimpl.h"
#include "../../include/h5geo/h5map.h"
#include "../../include/h5geo/impl/h5mapimpl.h"

#include <optional>
#include <filesystem>
namespace fs = std::filesystem;

H5MapContainerImpl::H5MapContainerImpl(const h5gt::File &h5File) :
  H5BaseContainerImpl(h5File){}

H5Map* H5MapContainerImpl::openMap(const std::string &name)
{
  if (!h5File.hasObject(name, h5gt::ObjectType::Group))
    return nullptr;

  h5gt::Group group = h5File.getGroup(name);
  return openMap(group);
}

H5Map* H5MapContainerImpl::openMap(
    h5gt::Group group)
{
  return h5geo::openMap(group);
}

H5Map* H5MapContainerImpl::createMap(
    std::string& name,
    MapParam &p,
    h5geo::CreationType createFlag)
{
  auto opt = createObject(
        name, h5File, h5geo::ObjectType::MAP, &p, createFlag);

  if (!opt.has_value())
    return nullptr;

  return new H5MapImpl(opt.value());
}

H5Map* H5MapContainerImpl::createMap(
    h5gt::Group group,
    MapParam &p,
    h5geo::CreationType createFlag)
{
  auto opt = createObject(
        group, h5geo::ObjectType::MAP, &p, createFlag);

  if (!opt.has_value())
    return nullptr;

  return new H5MapImpl(opt.value());
}

H5MapContainer*
h5geo::createMapContainer(
    h5gt::File h5File, h5geo::CreationType createFlag)
{
  auto opt = H5MapContainerImpl::createContainer(
        h5File, h5geo::ContainerType::MAP, createFlag);
  if (!opt.has_value())
    return nullptr;

  return new H5MapContainerImpl(opt.value());
}

H5MapContainer*
h5geo::createMapContainerByName(
    std::string& fileName, h5geo::CreationType createFlag)
{
  auto opt = H5MapContainerImpl::createContainer(
        fileName, h5geo::ContainerType::MAP, createFlag);
  if (!opt.has_value())
    return nullptr;

  return new H5MapContainerImpl(opt.value());
}

H5MapContainer*
h5geo::openMapContainer(
    h5gt::File h5File){
  return createMapContainer(h5File, h5geo::CreationType::OPEN);
}

H5MapContainer*
h5geo::openMapContainerByName(
    const std::string& fileName){
  if (!fs::exists(fileName) || H5Fis_hdf5(fileName.c_str()) < 1)
    return nullptr;

  try {
    h5gt::File h5File(
          fileName,
          h5gt::File::ReadWrite);
    return h5geo::openMapContainer(h5File);
  } catch (h5gt::Exception& err) {
    return nullptr;
  }
}
