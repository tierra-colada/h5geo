#include "../include/h5base/h5basecontainer.h"

H5BaseContainer::H5BaseContainer(const h5gt::File &h5File) :
  h5File(h5File)
{

}

H5BaseContainer::~H5BaseContainer(){

}


h5gt::File H5BaseContainer::getH5File(){
  return h5File;
}

bool H5BaseContainer::operator == (H5BaseContainer& other) const {
  return h5File == other.getH5File();
}

bool H5BaseContainer::operator != (H5BaseContainer& other) const {
  return !(*this == other);
}

std::optional<h5gt::File>
H5BaseContainer::createContainer(
    h5gt::File &h5File,
    const h5geo::ContainerType& containerType)
{
  h5geo::ContainerType currentContainerType =
      static_cast<h5geo::ContainerType>(
        h5geo::getEnumFromObj(
          h5File,
          std::string{magic_enum::enum_name(h5geo::ContainerAttributes::ContainerType)}));

  if (containerType == currentContainerType)
    return h5File;
  else
    return std::nullopt;
}

std::optional<h5gt::File>
H5BaseContainer::createContainer(
    const std::string& fileName,
    const h5geo::ContainerType& containerType,
    const unsigned& openFlags,
    const h5gt::FileAccessProps& fileAccessProps)
{
  bool fileExist = h5geo::isFileExist(fileName);

  if (!fileExist ||
      (openFlags & h5gt::File::Truncate)){
    try {
      h5gt::File h5File(fileName, openFlags, fileAccessProps);
      h5File.createAttribute<unsigned>("ContainerType",
                                       h5gt::DataSpace(1)).
          write(static_cast<unsigned>(containerType));
      return createContainer(h5File, containerType);
    } catch (h5gt::Exception e) {
      return std::nullopt;
    }
  } else {
    try {
      h5gt::File h5File(fileName, openFlags, fileAccessProps);
      return createContainer(h5File, containerType);
    } catch (h5gt::Exception e) {
      return std::nullopt;
    }
  }
}
