#include "../include/h5surf/h5surfcontainer.h"

H5SurfContainer::H5SurfContainer(const h5gt::File &h5File) :
  H5BaseContainer(h5File)
{

}

H5SurfContainer::~H5SurfContainer(){

}

std::optional<H5SurfContainer>
H5SurfContainer::create(h5gt::File &h5File)
{
  std::optional<h5gt::File> opt = H5BaseContainer::createContainer(
        h5File, h5geo::ContainerType::SURFACE);
  if (!opt.has_value())
    return std::nullopt;

  return H5SurfContainer(opt.value());
}

std::optional<H5SurfContainer>
H5SurfContainer::create(
    const std::string& fileName,
    const unsigned& openFlags)
{
  std::optional<h5gt::File> opt = H5BaseContainer::createContainer(
        fileName, h5geo::ContainerType::SURFACE, openFlags);
  if (!opt.has_value())
    return std::nullopt;

  return H5SurfContainer(opt.value());
}

std::optional<H5Surf>
H5SurfContainer::getSurf(
    const std::string& name)
{
  if (!h5File.hasObject(name, h5gt::ObjectType::Group))
    return std::nullopt;

  h5gt::Group group = h5File.getGroup(name);
  return H5Surf::create(group);
}

std::optional<H5Surf>
H5SurfContainer::createSurf(
    h5gt::Group& group,
    SurfParam p,
    const h5geo::CreationType& createFlag)
{
  std::optional<h5gt::Group> opt = H5Base::createObject(
        group, h5geo::ObjectType::SURFACE,
        &p, createFlag);

  if (opt.has_value())
    return H5Surf::create(opt.value());

  return std::nullopt;
}

std::optional<H5Surf>
H5SurfContainer::createSurf(
    std::string& name,
    SurfParam p,
    const h5geo::CreationType& createFlag)
{
  std::optional<h5gt::Group> opt = H5Base::createObject(
        name, h5File, h5geo::ObjectType::SURFACE,
        &p, createFlag);

  if (opt.has_value())
    return H5Surf::create(opt.value());

  return std::nullopt;
}

std::vector<H5Surf>
H5SurfContainer::getSurfList(){
  h5gt::Group group = h5File.getGroup("/");
  std::vector<h5gt::Group> childGroupList =
      getChildList(group, h5geo::ObjectType::SURFACE);

  std::vector<H5Surf> childList;
  childList.reserve(childGroupList.size());
  for (size_t i = 0; i < childGroupList.size(); i++){
    std::optional<H5Surf> opt = H5Surf::create(childGroupList[i]);
    if (opt.has_value())
      childList.push_back(opt.value());
  }
  childList.shrink_to_fit();
  return childList;
}
