#ifndef H5BASEPOINTS_H
#define H5BASEPOINTS_H

#include "h5baseobject.h"

class H5BaseContainer;

/// \class H5BasePoints
/// \brief Base class for Points
///
/// Points are objects that store data in the form: `XYZ`, `VAL` and `name`. \n
/// H5Points1 consists of `X` and `name`. \n
/// H5Points2 consists of `X`, `VAL` and `name` etc. \n
/// Maximal length of `name` is defined by `H5GEO_CHAR_ARRAY_SIZE` macro passed to cmake as CACHE var.
class H5BasePoints : public H5BaseObject
{
protected:
  virtual ~H5BasePoints() = default;

public:
  /// \brief Resize HDF5 DataSet
  virtual bool setNPoints(size_t n) = 0;
  /// \brief Set domain for the points (`TVD`, `TVDSS`, `TWT`, `OWT`)
  virtual bool setDomain(const h5geo::Domain& domain) = 0;

  /// \brief Open container where current points resides
  virtual H5BaseContainer* openContainer() const = 0;
  /// \brief Return number of points
  virtual size_t getNPoints() = 0;
  /// \brief Get domain (`TVD`, `TVDSS`, `TWT`, `OWT`)
  virtual h5geo::Domain getDomain() = 0;

  /// \brief Get parameters that were used to create current points
  virtual H5PointsParam getParam() = 0;

  /// \brief Get current points DataSet
  virtual std::optional<h5gt::DataSet> getPointsD() const = 0;
};

using H5BasePoints_ptr = std::unique_ptr<H5BasePoints, h5geo::ObjectDeleter>;

#endif // H5BASEPOINTS_H
