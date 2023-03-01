#ifndef H5VOL_H
#define H5VOL_H

#include "h5baseobject.h"

#include <Eigen/Dense>

class H5VolContainer;

/// \class H5Vol
/// \brief Provides API to work with volumes
///
/// Volume is an object that is represented by a regular 3D grid.
class H5Vol : public H5BaseObject
{
protected:
  virtual ~H5Vol() = default;

public:

	/// \brief Get parameters that were used to create current map
  virtual VolParam getParam() = 0;

	/// \brief Open H5VolContainer where current vol resides
  virtual H5VolContainer* openVolContainer() const = 0;

	/// \brief Get current vol's DataSet
  virtual std::optional<h5gt::DataSet> getVolD() const = 0;
};

using H5Vol_ptr = std::unique_ptr<H5Vol, h5geo::ObjectDeleter>;

#endif // H5VOL_H
