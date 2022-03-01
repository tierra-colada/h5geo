#ifndef H5MAPCONTAINER_H
#define H5MAPCONTAINER_H

#include "impl/h5geo_export.h"
#include "h5basecontainer.h"

#include <memory>

class H5Map;

class H5MapContainer : public H5BaseContainer
{
protected:
  virtual ~H5MapContainer() = default;

public:
  virtual H5Map* openMap(
      const std::string& name) = 0;
  virtual H5Map* openMap(
      h5gt::Group group) = 0;
  virtual H5Map* createMap(
      std::string& name,
      MapParam &p,
      h5geo::CreationType createFlag) = 0;
  virtual H5Map* createMap(
      h5gt::Group group,
      MapParam &p,
      h5geo::CreationType createFlag) = 0;
};

namespace h5geo {
  extern "C" {
  H5GEO_EXPORT H5MapContainer* createMapContainer(
    h5gt::File h5File, h5geo::CreationType createFlag);
  H5GEO_EXPORT H5MapContainer* createMapContainerByName(
    std::string& fileName, h5geo::CreationType createFlag);
  }
}

using H5MapCnt_ptr = std::unique_ptr<H5MapContainer, h5geo::ObjectDeleter>;

#endif // H5MAPCONTAINER_H
