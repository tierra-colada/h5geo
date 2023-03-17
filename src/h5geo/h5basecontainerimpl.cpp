#include "../../include/h5geo/private/h5basecontainerimpl.h"
#include "../../include/h5geo/private/h5seiscontainerimpl.h"
#include "../../include/h5geo/private/h5volcontainerimpl.h"
#include "../../include/h5geo/private/h5mapcontainerimpl.h"
#include "../../include/h5geo/private/h5wellcontainerimpl.h"
#include "../../include/h5geo/private/h5points1impl.h"
#include "../../include/h5geo/private/h5points2impl.h"
#include "../../include/h5geo/private/h5points3impl.h"
#include "../../include/h5geo/private/h5points4impl.h"
#include "../../include/h5geo/private/h5horizonimpl.h"
#include "../../include/h5geo/h5core.h"
#include "../../include/h5geo/private/h5enum_string.h"

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
H5Base* H5BaseContainerImpl<H5VolContainer>::clone()
{
  return new H5VolContainerImpl(h5File);
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
H5Horizon* H5BaseContainerImpl<TBase>::openHorizon(
    const std::string& name)
{
  if (!h5File.hasObject(name, h5gt::ObjectType::Group))
    return nullptr;

  h5gt::Group group = h5File.getGroup(name);
  return openHorizon(group);
}

template <typename TBase>
H5Horizon* H5BaseContainerImpl<TBase>::openHorizon(
    h5gt::Group group)
{
  return openHorizon(group);
}

template <typename TBase>
H5Points1* H5BaseContainerImpl<TBase>::createPoints1(
    std::string& name,
    H5PointsParam& p,
    h5geo::CreationType createFlag)
{
  auto opt = H5BaseImpl<TBase>::createObject(
        name, h5File, h5geo::ObjectType::POINTS_1, &p, createFlag);

  if (!opt.has_value())
    return nullptr;

  return new H5Points1Impl(opt.value());
}

template <typename TBase>
H5Points1* H5BaseContainerImpl<TBase>::createPoints1(
    h5gt::Group group,
    H5PointsParam& p,
    h5geo::CreationType createFlag)
{
  auto opt = H5BaseImpl<TBase>::createObject(
        group, h5geo::ObjectType::POINTS_1, &p, createFlag);

  if (!opt.has_value())
    return nullptr;

  return new H5Points1Impl(opt.value());
}

template <typename TBase>
H5Points2* H5BaseContainerImpl<TBase>::createPoints2(
    std::string& name,
    H5PointsParam& p,
    h5geo::CreationType createFlag)
{
  auto opt = H5BaseImpl<TBase>::createObject(
        name, h5File, h5geo::ObjectType::POINTS_2, &p, createFlag);

  if (!opt.has_value())
    return nullptr;

  return new H5Points2Impl(opt.value());
}

template <typename TBase>
H5Points2* H5BaseContainerImpl<TBase>::createPoints2(
    h5gt::Group group,
    H5PointsParam& p,
    h5geo::CreationType createFlag)
{
  auto opt = H5BaseImpl<TBase>::createObject(
        group, h5geo::ObjectType::POINTS_2, &p, createFlag);

  if (!opt.has_value())
    return nullptr;

  return new H5Points2Impl(opt.value());
}

template <typename TBase>
H5Points3* H5BaseContainerImpl<TBase>::createPoints3(
    std::string& name,
    H5PointsParam& p,
    h5geo::CreationType createFlag)
{
  auto opt = H5BaseImpl<TBase>::createObject(
        name, h5File, h5geo::ObjectType::POINTS_3, &p, createFlag);

  if (!opt.has_value())
    return nullptr;

  return new H5Points3Impl(opt.value());
}

template <typename TBase>
H5Points3* H5BaseContainerImpl<TBase>::createPoints3(
    h5gt::Group group,
    H5PointsParam& p,
    h5geo::CreationType createFlag)
{
  auto opt = H5BaseImpl<TBase>::createObject(
        group, h5geo::ObjectType::POINTS_3, &p, createFlag);

  if (!opt.has_value())
    return nullptr;

  return new H5Points3Impl(opt.value());
}

template <typename TBase>
H5Points4* H5BaseContainerImpl<TBase>::createPoints4(
    std::string& name,
    H5PointsParam& p,
    h5geo::CreationType createFlag)
{
  auto opt = H5BaseImpl<TBase>::createObject(
        name, h5File, h5geo::ObjectType::POINTS_4, &p, createFlag);

  if (!opt.has_value())
    return nullptr;

  return new H5Points4Impl(opt.value());
}

template <typename TBase>
H5Points4* H5BaseContainerImpl<TBase>::createPoints4(
    h5gt::Group group,
    H5PointsParam& p,
    h5geo::CreationType createFlag)
{
  auto opt = H5BaseImpl<TBase>::createObject(
        group, h5geo::ObjectType::POINTS_4, &p, createFlag);

  if (!opt.has_value())
    return nullptr;

  return new H5Points4Impl(opt.value());
}

template <typename TBase>
H5Horizon* H5BaseContainerImpl<TBase>::createHorizon(
    std::string& name,
    H5HorizonParam& p,
    h5geo::CreationType createFlag)
{
  auto opt = H5BaseImpl<TBase>::createObject(
        name, h5File, h5geo::ObjectType::HORIZON, &p, createFlag);

  if (!opt.has_value())
    return nullptr;

  return new H5HorizonImpl(opt.value());
}

template <typename TBase>
H5Horizon* H5BaseContainerImpl<TBase>::createHorizon(
    h5gt::Group group,
    H5HorizonParam& p,
    h5geo::CreationType createFlag)
{
  auto opt = H5BaseImpl<TBase>::createObject(
        group, h5geo::ObjectType::HORIZON, &p, createFlag);

  if (!opt.has_value())
    return nullptr;

  return new H5HorizonImpl(opt.value());
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
  return h5geo::readEnumAttribute<h5gt::File, h5geo::ContainerType>
        (h5File, std::string(h5geo::detail::ContainerType));
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

// explicit instantiation (requires that corresponding headers are included)
template class H5BaseContainerImpl<H5BaseContainer>;
template class H5BaseContainerImpl<H5MapContainer>;
template class H5BaseContainerImpl<H5SeisContainer>;
template class H5BaseContainerImpl<H5VolContainer>;
template class H5BaseContainerImpl<H5WellContainer>;
