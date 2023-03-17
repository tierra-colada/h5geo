#ifndef H5VOLCONTAINER_H
#define H5VOLCONTAINER_H

#include "h5geo_export.h"
#include "h5basecontainer.h"

#include <memory>

class H5Vol;

/// \class H5VolContainer
/// \brief A container built around HDF5 file and used 
/// for storing and manipulating H5Vol objects
class H5VolContainer : public H5BaseContainer
{
protected:
  virtual ~H5VolContainer() = default;

public:
	/// \brief Open H5Vol
  virtual H5Vol* openVol(
      const std::string& name) = 0;
	/// \brief Open H5Vol
  virtual H5Vol* openVol(
      h5gt::Group group) = 0;
	/// \brief Create H5Vol
  virtual H5Vol* createVol(
      std::string& name,
      H5VolParam& p,
      h5geo::CreationType createFlag) = 0;
	/// \brief Create H5Vol
  virtual H5Vol* createVol(
      h5gt::Group group,
      H5VolParam& p,
      h5geo::CreationType createFlag) = 0;
};

using H5VolCnt_ptr = std::unique_ptr<H5VolContainer, h5geo::ObjectDeleter>;

#endif // H5VOLCONTAINER_H
