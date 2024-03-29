#include "../../include/h5geo/private/h5wellcontainerimpl.h"
#include "../../include/h5geo/h5well.h"
#include "../../include/h5geo/h5core.h"
#include "../../include/h5geo/private/h5wellimpl.h"
#include "../../include/h5geo/private/h5enum_string.h"

#include <optional>
#include <filesystem>
namespace fs = std::filesystem;

H5WellContainerImpl::H5WellContainerImpl(const h5gt::File &h5File):
  H5BaseContainerImpl(h5File){}

H5Well* H5WellContainerImpl::openWell(
    const std::string& name)
{
  if (!h5File.hasObject(name, h5gt::ObjectType::Group))
    return nullptr;

  h5gt::Group group = h5File.getGroup(name);
  return openWell(group);
}

H5Well* H5WellContainerImpl::openWell(
    h5gt::Group group)
{
  return h5geo::openWell(group);
}

H5Well* H5WellContainerImpl::createWell(
    std::string& name,
    H5WellParam& p,
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
    H5WellParam& p,
    h5geo::CreationType createFlag)
{
  auto opt = createObject(
        group, h5geo::ObjectType::WELL, &p, createFlag);

  if (!opt.has_value())
    return nullptr;

  return new H5WellImpl(opt.value());
}

H5Well* H5WellContainerImpl::openWellByUWI(
    const std::string& name)
{
  h5gt::Group group = h5File.getGroup("/");
  std::vector<h5gt::Group> childGroupList =
      getChildGroupList(group, h5geo::ObjectType::WELL, true);
  for (h5gt::Group& group : childGroupList){
    std::string uwi = h5geo::readStringAttribute(
          group,
          std::string{h5geo::detail::UWI});

    if (uwi == name &&
        h5geo::isWell(group))
      return openWell(group);
  }
  return nullptr;
}
