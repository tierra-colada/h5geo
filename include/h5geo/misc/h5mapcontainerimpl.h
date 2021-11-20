#ifndef H5MAPCONTAINERIMPL_H
#define H5MAPCONTAINERIMPL_H

#include "../h5mapcontainer.h"
#include "h5basecontainerimpl.h"

class H5MapContainerImpl :
    public virtual H5MapContainer,
    public virtual H5BaseContainerImpl
{
protected:
  explicit H5MapContainerImpl(const h5gt::File &h5File);
  virtual ~H5MapContainerImpl() = default;

public:
  virtual H5Map* getMap(
      const std::string& name) override;
  virtual H5Map* getMap(
      h5gt::Group group) override;
  virtual H5Map* createMap(
      std::string& name,
      MapParam &p,
      h5geo::CreationType createFlag) override;
  virtual H5Map* createMap(
      h5gt::Group group,
      MapParam &p,
      h5geo::CreationType createFlag) override;

  virtual std::vector<H5Map*> getMapList() override;

  //----------- FRIEND CLASSES -----------
  friend H5MapContainer* h5geo::createMapContainer(
      h5gt::File h5File, h5geo::CreationType createFlag);
  friend H5MapContainer* h5geo::createMapContainerByName(
      std::string& fileName, h5geo::CreationType createFlag);
  friend H5MapContainer* h5geo::openMapContainer(
      h5gt::File h5File);
};

#endif // H5MAPCONTAINERIMPL_H
