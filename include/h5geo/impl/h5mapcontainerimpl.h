#ifndef H5MAPCONTAINERIMPL_H
#define H5MAPCONTAINERIMPL_H

#include "../h5mapcontainer.h"
#include "h5basecontainerimpl.h"

class H5MapContainerImpl : public H5BaseContainerImpl<H5MapContainer>
{
protected:
  explicit H5MapContainerImpl(const h5gt::File &h5File);
  virtual ~H5MapContainerImpl() = default;

public:
  virtual H5Map* openMap(
      const std::string& name) override;
  virtual H5Map* openMap(
      h5gt::Group group) override;
  virtual H5Map* createMap(
      std::string& name,
      MapParam &p,
      h5geo::CreationType createFlag) override;
  virtual H5Map* createMap(
      h5gt::Group group,
      MapParam &p,
      h5geo::CreationType createFlag) override;

  //----------- FRIEND CLASSES -----------
  friend class H5BaseContainerImpl<H5MapContainer>;
  friend H5MapContainer* h5geo::createMapContainer(
      h5gt::File h5File, h5geo::CreationType createFlag);
  friend H5MapContainer* h5geo::createMapContainerByName(
      std::string& fileName, h5geo::CreationType createFlag);
  friend H5MapContainer* h5geo::openMapContainer(
      h5gt::File h5File);
};

#endif // H5MAPCONTAINERIMPL_H
