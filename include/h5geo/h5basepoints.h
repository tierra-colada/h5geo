#ifndef H5BASEPOINTS_H
#define H5BASEPOINTS_H

#include "h5baseobject.h"

class H5BaseContainer;

/// \class H5BasePoints
/// \brief Base class for Points
class H5BasePoints : public H5BaseObject
{
protected:
  virtual ~H5BasePoints() = default;

public:
  /// \brief Resize HDF5 DataSet
  virtual bool setNPoints(size_t n) = 0;
  /// \brief Set domain for the points (time, depth etc)
  virtual bool setDomain(const h5geo::Domain& domain) = 0;

  /// \brief Open container where current points resides
  virtual H5BaseContainer* openContainer() const = 0;
  /// \brief Return number of points
  virtual size_t getNPoints() = 0;
  /// \brief Get domain (time, depth etc)
  virtual h5geo::Domain getDomain() = 0;

  /// \brief Get parameters that were used to create current points
  virtual PointsParam getParam() = 0;

  /// \brief Get current points DataSet
  virtual std::optional<h5gt::DataSet> getPointsD() const = 0;
};

using H5BasePoints_ptr = std::unique_ptr<H5BasePoints, h5geo::ObjectDeleter>;

#endif // H5BASEPOINTS_H
