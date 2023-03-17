#include "../../include/h5geo/private/h5mapcontainerimpl.h"
#include "../../include/h5geo/h5map.h"
#include "../../include/h5geo/private/h5mapimpl.h"

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
    H5MapParam &p,
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
    H5MapParam &p,
    h5geo::CreationType createFlag)
{
  auto opt = createObject(
        group, h5geo::ObjectType::MAP, &p, createFlag);

  if (!opt.has_value())
    return nullptr;

  return new H5MapImpl(opt.value());
}
