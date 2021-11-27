#include "../../include/h5geo/misc/h5basecontainerimpl.h"
#include "../../include/h5geo/misc/h5seiscontainerimpl.h"
#include "../../include/h5geo/misc/h5mapcontainerimpl.h"
#include "../../include/h5geo/misc/h5wellcontainerimpl.h"
#include "../../include/h5geo/h5core.h"

#include <filesystem>
namespace fs = std::filesystem;

template <typename TBase>
H5BaseContainerImpl<TBase>::H5BaseContainerImpl(const h5gt::File &h5File) :
  h5File(h5File){}

template <typename TBase>
h5gt::File H5BaseContainerImpl<TBase>::getH5File() const{
  return h5File;
}

template <typename TBase>
bool H5BaseContainerImpl<TBase>::operator == (H5BaseContainer& other) const {
  return h5File == other.getH5File();
}

template <typename TBase>
bool H5BaseContainerImpl<TBase>::operator != (H5BaseContainer& other) const {
  return !(*this == other);
}

H5BaseContainer*
h5geo::openBaseContainer(h5gt::File h5File)
{
  return new H5BaseContainerImpl<H5BaseContainer>(h5File);
}

H5BaseContainer*
h5geo::openBaseContainerByName(const std::string& fileName)
{
  if (fs::exists(fileName) && H5Fis_hdf5(fileName.c_str()) <= 0)
    return nullptr;

  h5gt::File h5File(
        fileName,
        h5gt::File::ReadWrite);

  return openBaseContainer(h5File);
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
