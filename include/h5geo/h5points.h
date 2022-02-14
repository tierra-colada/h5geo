#ifndef H5POINTS_H
#define H5POINTS_H

#include "h5baseobject.h"

#include <Eigen/Dense>

class H5BaseContainer;

class H5Points : public H5BaseObject
{
protected:
  virtual ~H5Points() = default;

public:
  virtual bool writeData(
      h5geo::PointArray& data,
      const std::string& lengthUnits = "",
      const std::string& temporalUnits = "",
      bool doCoordTransform = false) = 0;

  virtual h5geo::PointArray getData(
      const std::string& lengthUnits = "",
      const std::string& temporalUnits = "",
      bool doCoordTransform = false) = 0;

  virtual bool setNPoints(size_t n) = 0;
  virtual bool setDomain(const h5geo::Domain& domain) = 0;

  virtual H5BaseContainer* getContainer() const = 0;
  virtual size_t getNPoints() = 0;
  virtual h5geo::Domain getDomain() = 0;

  virtual std::optional<h5gt::DataSet> getPointsD() const = 0;
};

namespace h5geo {
  extern "C" {
  H5GEO_EXPORT H5Points* openPoints(h5gt::Group group);
  }
}

using H5Points_ptr = std::unique_ptr<H5Points, h5geo::ObjectDeleter>;

#endif // H5POINTS_H
