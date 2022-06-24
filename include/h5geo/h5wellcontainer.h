#ifndef H5WELLCONTAINER_H
#define H5WELLCONTAINER_H

#include "h5geo_export.h"
#include "h5basecontainer.h"

#include <memory>

class H5Well;

/// \class H5WellContainer
/// \brief A container built around HDF5 file and used 
/// for storing and manipulating H5Well objects
class H5WellContainer : public H5BaseContainer
{
protected:
  virtual ~H5WellContainer() = default;

public:
	/// \brief Open H5Well
  virtual H5Well* openWell(
      const std::string& name) = 0;
	/// \brief Open H5Well
  virtual H5Well* openWell(
      h5gt::Group group) = 0;
	/// \brief Create H5Well
  virtual H5Well* createWell(
      std::string& name,
      WellParam& p,
      h5geo::CreationType createFlag) = 0;
	/// \brief Create H5Well
  virtual H5Well* createWell(
      h5gt::Group group,
      WellParam& p,
      h5geo::CreationType createFlag) = 0;

	/// \brief Find and open H5Well using its `UWI`
	///
	/// Works much slower than using H5WellContainer::openWell()
  virtual H5Well* openWellByUWI(
      const std::string& name) = 0;
};

using H5WellCnt_ptr = std::unique_ptr<H5WellContainer, h5geo::ObjectDeleter>;

#endif // H5WELLCONTAINER_H
