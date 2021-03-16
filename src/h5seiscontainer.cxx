#include "../include/h5seis/h5seiscontainer.h"

H5SeisContainer::H5SeisContainer(const h5gt::File &h5File) :
  H5BaseContainer(h5File)
{

}

H5SeisContainer::~H5SeisContainer(){

}

std::optional<H5SeisContainer>
H5SeisContainer::create(h5gt::File &h5File)
{
  std::optional<h5gt::File> opt = H5BaseContainer::createContainer(
        h5File, h5geo::ContainerType::SEISMIC);
  if (!opt.has_value())
    return std::nullopt;

  return H5SeisContainer(opt.value());
}

std::optional<H5SeisContainer>
H5SeisContainer::create(
    const std::string& fileName,
    const unsigned& openFlags)
{
  std::optional<h5gt::File> opt = H5BaseContainer::createContainer(
        fileName, h5geo::ContainerType::SEISMIC, openFlags);
  if (!opt.has_value())
    return std::nullopt;

  return H5SeisContainer(opt.value());
}

std::optional<H5Seis> H5SeisContainer::getSeis(
    const std::string& name)
{
  if (!h5File.hasObject(name, h5gt::ObjectType::Group))
    return std::nullopt;

  h5gt::Group group = h5File.getGroup(name);
  return H5Seis::create(group);
}

std::optional<H5Seis>
H5SeisContainer::createSeis(
    h5gt::Group& group,
    SeisParam p,
    const h5geo::CreationType& createFlag)
{
  std::optional<h5gt::Group> opt = H5BaseContainer::createObject(
        group, h5geo::ObjectType::SEISMIC,
        &p, createFlag);

  if (opt.has_value())
    return H5Seis::create(opt.value());

  return std::nullopt;
}

std::optional<H5Seis>
H5SeisContainer::createSeis(
    std::string name,
    SeisParam p,
    const h5geo::CreationType& createFlag)
{
  std::optional<h5gt::Group> opt = H5Base::createObject(
        name, h5File, h5geo::ObjectType::SEISMIC,
        &p, createFlag);

  if (opt.has_value())
    return H5Seis::create(opt.value());

  return std::nullopt;
}

std::vector<H5Seis>
H5SeisContainer::getSeisList(){
  h5gt::Group group = h5File.getGroup("/");
  std::vector<h5gt::Group> childGroupList =
      H5BaseContainer::getChildList(group, h5geo::ObjectType::SEISMIC);;

  std::vector<H5Seis> childList;
  childList.reserve(childGroupList.size());
  for (size_t i = 0; i < childGroupList.size(); i++){
    std::optional<H5Seis> opt = H5Seis::create(childGroupList[i]);
    if (opt.has_value())
      childList.push_back(opt.value());
  }
  childList.shrink_to_fit();
  return childList;
}
