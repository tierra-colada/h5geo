#include "../../include/h5geo/private/h5seiscontainerimpl.h"
#include "../../include/h5geo/h5seis.h"
#include "../../include/h5geo/private/h5seisimpl.h"

#include <optional>
#include <filesystem>
namespace fs = std::filesystem;

H5SeisContainerImpl::H5SeisContainerImpl(const h5gt::File &h5File) :
  H5BaseContainerImpl(h5File){}

H5Seis* H5SeisContainerImpl::openSeis(const std::string &name)
{
  if (!h5File.hasObject(name, h5gt::ObjectType::Group))
    return nullptr;

  h5gt::Group group = h5File.getGroup(name);
  return openSeis(group);
}

H5Seis* H5SeisContainerImpl::openSeis(
    h5gt::Group group)
{
  return h5geo::openSeis(group);
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
