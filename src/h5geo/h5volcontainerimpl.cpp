#include "../../include/h5geo/private/h5volcontainerimpl.h"
#include "../../include/h5geo/h5vol.h"
#include "../../include/h5geo/private/h5volimpl.h"

#include <optional>
#include <filesystem>
namespace fs = std::filesystem;

H5VolContainerImpl::H5VolContainerImpl(const h5gt::File &h5File) :
  H5BaseContainerImpl(h5File){}

H5Vol* H5VolContainerImpl::openVol(const std::string &name)
{
  if (!h5File.hasObject(name, h5gt::ObjectType::Group))
    return nullptr;

  h5gt::Group group = h5File.getGroup(name);
  return openVol(group);
}

H5Vol* H5VolContainerImpl::openVol(
    h5gt::Group group)
{
  return h5geo::openVol(group);
}

H5Vol* H5VolContainerImpl::createVol(
    std::string& name,
    H5VolParam& p,
    h5geo::CreationType createFlag)
{
  auto opt = createObject(
        name, h5File, h5geo::ObjectType::VOLUME, &p, createFlag);

  if (!opt.has_value())
    return nullptr;

  return new H5VolImpl(opt.value());
}

H5Vol* H5VolContainerImpl::createVol(
    h5gt::Group group,
    H5VolParam& p,
    h5geo::CreationType createFlag)
{
  auto opt = createObject(
        group, h5geo::ObjectType::VOLUME, &p, createFlag);

  if (!opt.has_value())
    return nullptr;

  return new H5VolImpl(opt.value());
}
