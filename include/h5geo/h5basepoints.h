#ifndef H5BASEPOINTS_H
#define H5BASEPOINTS_H

#include "h5baseobject.h"

class H5BaseContainer;

class H5BasePoints : public H5BaseObject
{
protected:
  virtual ~H5BasePoints() = default;

public:
  virtual bool setNPoints(size_t n) = 0;
  virtual bool setDomain(const h5geo::Domain& domain) = 0;

  virtual H5BaseContainer* openContainer() const = 0;
  virtual size_t getNPoints() = 0;
  virtual h5geo::Domain getDomain() = 0;

  virtual std::optional<h5gt::DataSet> getPointsD() const = 0;
};

namespace h5geo {
  extern "C" {
  H5GEO_EXPORT H5BasePoints* openPoints(h5gt::Group group);
  }
}

using H5BasePoints_ptr = std::unique_ptr<H5BasePoints, h5geo::ObjectDeleter>;

#endif // H5BASEPOINTS_H
