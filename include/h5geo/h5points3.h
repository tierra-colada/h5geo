#ifndef H5POINTS3_H
#define H5POINTS3_H

#include "h5basepoints.h"

class H5BaseContainer;

class H5Points3 : public H5BasePoints
{
protected:
  virtual ~H5Points3() = default;

public:
  virtual bool writeData(
      h5geo::Point3Array& data,
      const std::string& lengthUnits = "",
      const std::string& temporalUnits = "",
      bool doCoordTransform = false) = 0;

  virtual h5geo::Point3Array getData(
      const std::string& lengthUnits = "",
      const std::string& temporalUnits = "",
      bool doCoordTransform = false) = 0;

//  virtual bool setNPoints(size_t n) = 0;
//  virtual bool setDomain(const h5geo::Domain& domain) = 0;

//  virtual H5BaseContainer* openContainer() const = 0;
//  virtual size_t getNPoints() = 0;
//  virtual h5geo::Domain getDomain() = 0;

//  virtual std::optional<h5gt::DataSet> getPointsD() const = 0;
};

using H5Points3_ptr = std::unique_ptr<H5Points3, h5geo::ObjectDeleter>;

#endif // H5POINTS3_H
