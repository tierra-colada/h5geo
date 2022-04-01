#include "../../include/h5geo/impl/h5basecontainerimpl.h"
#include "../../include/h5geo/impl/h5seiscontainerimpl.h"
#include "../../include/h5geo/impl/h5mapcontainerimpl.h"
#include "../../include/h5geo/impl/h5wellcontainerimpl.h"
#include "../../include/h5geo/impl/h5points3impl.h"
#include "../../include/h5geo/misc/h5core.h"
#include "../../include/h5geo/misc/h5enum_string.h"

#include <optional>
#include <filesystem>
namespace fs = std::filesystem;

template <typename TBase>
H5BaseContainerImpl<TBase>::H5BaseContainerImpl(const h5gt::File &h5File) :
  h5File(h5File){}

template <>
H5Base* H5BaseContainerImpl<H5BaseContainer>::clone()
{
  return new H5BaseContainerImpl<H5BaseContainer>(h5File);
}

template <>
H5Base* H5BaseContainerImpl<H5MapContainer>::clone()
{
  return new H5MapContainerImpl(h5File);
}

template <>
H5Base* H5BaseContainerImpl<H5SeisContainer>::clone()
{
  return new H5SeisContainerImpl(h5File);
}

template <>
H5Base* H5BaseContainerImpl<H5WellContainer>::clone()
{
  return new H5WellContainerImpl(h5File);
}

template <typename TBase>
H5BaseObject* H5BaseContainerImpl<TBase>::openObject(
    const std::string& name)
{
  if (!h5File.hasObject(name, h5gt::ObjectType::Group))
    return nullptr;

  h5gt::Group group = h5File.getGroup(name);
  return openObject(group);
}

template <typename TBase>
H5BaseObject* H5BaseContainerImpl<TBase>::openObject(
    h5gt::Group group)
{
  return h5geo::openObject(group);
}

template <typename TBase>
H5BasePoints* H5BaseContainerImpl<TBase>::openPoints(
    const std::string& name)
{
  if (!h5File.hasObject(name, h5gt::ObjectType::Group))
    return nullptr;

  h5gt::Group group = h5File.getGroup(name);
  return openPoints(group);
}

template <typename TBase>
H5BasePoints* H5BaseContainerImpl<TBase>::openPoints(
    h5gt::Group group)
{
  return h5geo::openPoints(group);
}

template <typename TBase>
H5Points3* H5BaseContainerImpl<TBase>::createPoints3(
    std::string& name,
    PointsParam& p,
    h5geo::CreationType createFlag)
{
  auto opt = H5BaseImpl<TBase>::createObject(
        name, h5File, h5geo::ObjectType::POINTS, &p, createFlag);

  if (!opt.has_value())
    return nullptr;

  return new H5Points3Impl(opt.value());
}

template <typename TBase>
H5Points3* H5BaseContainerImpl<TBase>::createPoints3(
    h5gt::Group group,
    PointsParam& p,
    h5geo::CreationType createFlag)
{
  auto opt = H5BaseImpl<TBase>::createObject(
        group, h5geo::ObjectType::POINTS, &p, createFlag);

  if (!opt.has_value())
    return nullptr;

  return new H5Points3Impl(opt.value());
}

template <typename TBase>
h5gt::File H5BaseContainerImpl<TBase>::getH5File() const{
  return h5File;
}

template <typename TBase>
std::vector<h5gt::Group>
H5BaseContainerImpl<TBase>::getObjGroupList(const h5geo::ObjectType& objType, bool recursive){
  h5gt::Group group = h5File.getGroup("/");
  return H5BaseImpl<TBase>::getChildGroupList(group, objType, recursive);
}

template <typename TBase>
std::vector<std::string>
H5BaseContainerImpl<TBase>::getObjNameList(const h5geo::ObjectType& objType, bool recursive){
  h5gt::Group group = h5File.getGroup("/");
  return H5BaseImpl<TBase>::getChildNameList(group, objType, "/", recursive);
}

template <typename TBase>
size_t
H5BaseContainerImpl<TBase>::getObjCount(const h5geo::ObjectType& objType, bool recursive){
  h5gt::Group group = h5File.getGroup("/");
  return H5BaseImpl<TBase>::getChildCount(group, objType, recursive);
}

template <typename TBase>
h5geo::ContainerType
H5BaseContainerImpl<TBase>::getContainerType()
{
  return static_cast<h5geo::ContainerType>(
        h5geo::readEnumAttribute(h5File, std::string(h5geo::detail::ContainerType)));
}

template <typename TBase>
bool H5BaseContainerImpl<TBase>::isEqual(H5BaseContainer* other) const{
  if (!other)
    return false;

  return h5File == other->getH5File();
}

template <typename TBase>
bool H5BaseContainerImpl<TBase>::operator == (const H5BaseContainer& other) const {
  return h5File == other.getH5File();
}

template <typename TBase>
bool H5BaseContainerImpl<TBase>::operator != (const H5BaseContainer& other) const {
  return !(*this == other);
}

H5BaseContainer*
h5geo::createContainer(
    h5gt::File h5File,
    h5geo::ContainerType cntType,
    h5geo::CreationType createFlag)
{
  switch (static_cast<h5geo::ContainerType>(cntType)) {
  case h5geo::ContainerType::MAP :
    return h5geo::createMapContainer(h5File, createFlag);
  case h5geo::ContainerType::SEISMIC :
    return h5geo::createSeisContainer(h5File, createFlag);
  case h5geo::ContainerType::WELL :
    return h5geo::createWellContainer(h5File, createFlag);
  default :
    return nullptr;
  }
}

H5BaseContainer*
h5geo::createContainerByName(
    std::string& fileName,
    h5geo::ContainerType cntType,
    h5geo::CreationType createFlag)
{
  switch (static_cast<h5geo::ContainerType>(cntType)) {
  case h5geo::ContainerType::MAP :
    return h5geo::createMapContainerByName(fileName, createFlag);
  case h5geo::ContainerType::SEISMIC :
    return h5geo::createSeisContainerByName(fileName, createFlag);
  case h5geo::ContainerType::WELL :
    return h5geo::createWellContainerByName(fileName, createFlag);
  default :
    return nullptr;
  }
}

H5BaseContainer*
h5geo::openBaseContainer(h5gt::File h5File)
{
  return new H5BaseContainerImpl<H5BaseContainer>(h5File);
}

H5BaseContainer*
h5geo::openBaseContainerByName(const std::string& fileName)
{
  if (!fs::exists(fileName) || H5Fis_hdf5(fileName.c_str()) < 1)
    return nullptr;

  try {
    h5gt::File h5File(
          fileName,
          h5gt::File::ReadWrite);
    return h5geo::openBaseContainer(h5File);
  } catch (h5gt::Exception& err) {
    return nullptr;
  }
}

H5BaseContainer*
h5geo::openContainer(h5gt::File h5File)
{
  H5BaseContainer* baseContainer = nullptr;
  baseContainer = h5geo::openMapContainer(h5File);
  if (baseContainer)
    return baseContainer;
  baseContainer = h5geo::openWellContainer(h5File);
  if (baseContainer)
    return baseContainer;
  baseContainer = h5geo::openSeisContainer(h5File);
  if (baseContainer)
    return baseContainer;

  return h5geo::openBaseContainer(h5File);
}

H5BaseContainer*
h5geo::openContainerByName(const std::string& fileName)
{
  H5BaseContainer* baseContainer = nullptr;
  baseContainer = h5geo::openMapContainerByName(fileName);
  if (baseContainer)
    return baseContainer;
  baseContainer = h5geo::openWellContainerByName(fileName);
  if (baseContainer)
    return baseContainer;
  baseContainer = h5geo::openSeisContainerByName(fileName);
  if (baseContainer)
    return baseContainer;

  return h5geo::openBaseContainerByName(fileName);
}

// explicit instantiation (requires that corresponding headers are included)
template class H5BaseContainerImpl<H5BaseContainer>;
template class H5BaseContainerImpl<H5MapContainer>;
template class H5BaseContainerImpl<H5SeisContainer>;
template class H5BaseContainerImpl<H5WellContainer>;
