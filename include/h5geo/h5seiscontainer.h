#ifndef H5SEISCONTAINER_H
#define H5SEISCONTAINER_H

#include "h5geo_export.h"
#include "h5basecontainer.h"

#include <memory>

class H5Seis;

/// \class H5SeisContainer
/// \brief A container built around HDF5 file and used 
/// for storing and manipulating H5Seis objects
class H5SeisContainer : public H5BaseContainer
{
protected:
  virtual ~H5SeisContainer() = default;

public:
	/// \brief Open H5Seis
  virtual H5Seis* openSeis(
      const std::string& name) = 0;
	/// \brief Open H5Seis
  virtual H5Seis* openSeis(
      h5gt::Group group) = 0;
	/// \brief Create H5Seis
  virtual H5Seis* createSeis(
      std::string& name,
      SeisParam& p,
      h5geo::CreationType createFlag) = 0;
	/// \brief Create H5Seis
  virtual H5Seis* createSeis(
      h5gt::Group group,
      SeisParam& p,
      h5geo::CreationType createFlag) = 0;
};

using H5SeisCnt_ptr = std::unique_ptr<H5SeisContainer, h5geo::ObjectDeleter>;

#endif // H5SEISCONTAINER_H
