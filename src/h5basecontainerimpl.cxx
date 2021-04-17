#include "../include/h5geo/misc/h5basecontainerimpl.h"
#include "../include/h5geo/h5core.h"

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
