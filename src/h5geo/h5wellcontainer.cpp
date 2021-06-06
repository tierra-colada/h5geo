#include "../../include/h5geo/h5wellcontainer.h"

H5WellContainer::H5WellContainer(const h5gt::File &h5File):
  H5BaseContainer(h5File)
{

}

H5WellContainer::~H5WellContainer(){

}

std::optional<H5WellContainer>
H5WellContainer::create(h5gt::File &h5File)
{
  std::optional<h5gt::File> opt = H5BaseContainer::createContainer(
        h5File, h5geo::ContainerType::WELL);
  if (!opt.has_value())
    return std::nullopt;

  return H5WellContainer(opt.value());
}

std::optional<H5WellContainer>
H5WellContainer::create(
    const std::string& fileName,
    const size_t& openFlags)
{
  std::optional<h5gt::File> opt = H5BaseContainer::createContainer(
        fileName, h5geo::ContainerType::WELL, openFlags);
  if (!opt.has_value())
    return std::nullopt;

  return H5WellContainer(opt.value());
}

std::optional<H5Well>
H5WellContainer::getWell(
    const std::string& name)
{
  if (!h5File.hasObject(name, h5gt::ObjectType::Group))
    return std::nullopt;

  h5gt::Group group = h5File.getGroup(name);
  return H5Well::create(group);
}

std::optional<H5Well>
H5WellContainer::createWell(
    h5gt::Group& group,
    WellParam p,
    const h5geo::CreationType &createFlag)
{
  std::optional<h5gt::Group> opt = H5BaseContainer::createObject(
        group, h5geo::ObjectType::WELL,
        &p, createFlag);

  if (opt.has_value())
    return H5Well::create(opt.value());

  return std::nullopt;
}

std::optional<H5Well>
H5WellContainer::createWell(
    std::string& name,
    WellParam p,
    const h5geo::CreationType &createFlag)
{
  std::optional<h5gt::Group> opt = H5Base::createObject(
        name, h5File, h5geo::ObjectType::WELL,
        &p, createFlag);

  if (opt.has_value())
    return H5Well::create(opt.value());

  return std::nullopt;
}

std::vector<H5Well>
H5WellContainer::getWellList(){
  h5gt::Group group = h5File.getGroup("/");
  std::vector<h5gt::Group> childGroupList =
      H5BaseContainer::getChildList(group, h5geo::ObjectType::WELL);;

  std::vector<H5Well> childList;
  childList.reserve(childGroupList.size());
  for (size_t i = 0; i < childGroupList.size(); i++){
    std::optional<H5Well> opt = H5Well::create(childGroupList[i]);
    if (opt.has_value())
      childList.push_back(opt.value());
  }
  childList.shrink_to_fit();
  return childList;
}
