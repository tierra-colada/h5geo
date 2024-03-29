#ifndef H5MAPCONTAINER_H
#define H5MAPCONTAINER_H

#include "h5geo_export.h"
#include "h5basecontainer.h"

#include <memory>

class H5Map;

/// \class H5MapContainer
/// \brief A container built around HDF5 file and used 
/// for storing and manipulating H5Map objects
class H5MapContainer : public H5BaseContainer
{
protected:
  virtual ~H5MapContainer() = default;

public:
  /// \brief Open H5Map
  virtual H5Map* openMap(
      const std::string& name) = 0;
	/// \brief Open H5Map
  virtual H5Map* openMap(
      h5gt::Group group) = 0;
	/// \brief Create H5Map
  virtual H5Map* createMap(
      std::string& name,
      H5MapParam &p,
      h5geo::CreationType createFlag) = 0;
	/// \brief Create H5Map
  virtual H5Map* createMap(
      h5gt::Group group,
      H5MapParam &p,
      h5geo::CreationType createFlag) = 0;
};

using H5MapCnt_ptr = std::unique_ptr<H5MapContainer, h5geo::ObjectDeleter>;

#endif // H5MAPCONTAINER_H
