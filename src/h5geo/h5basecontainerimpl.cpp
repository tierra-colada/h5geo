#include "../../include/h5geo/misc/h5basecontainerimpl.h"
#include "../../include/h5geo/h5core.h"

H5BaseContainerImpl::H5BaseContainerImpl(const h5gt::File &h5File) :
  h5File(h5File){}

h5gt::File H5BaseContainerImpl::getH5File() const{
  return h5File;
}

bool H5BaseContainerImpl::operator == (H5BaseContainer& other) const {
  return h5File == other.getH5File();
}

bool H5BaseContainerImpl::operator != (H5BaseContainer& other) const {
  return !(*this == other);
}

H5BaseContainer*
h5geo::openBaseContainer(h5gt::File h5File)
{
  return new H5BaseContainerImpl(h5File);
}

H5BaseContainer*
h5geo::openBaseContainerByName(const std::string& fileName)
{
  if (H5Fis_hdf5(fileName.c_str()) <= 0)
    return nullptr;

  h5gt::File h5File(
        fileName,
        h5gt::File::ReadWrite);

  return openBaseContainer(h5File);
}
