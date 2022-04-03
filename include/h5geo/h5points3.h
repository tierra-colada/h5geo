#ifndef H5POINTS3_H
#define H5POINTS3_H

#include "h5basepoints.h"

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
};

namespace h5geo {
  extern "C" {
  H5GEO_EXPORT H5Points3* openPoints3(h5gt::Group group);
  }
}

using H5Points3_ptr = std::unique_ptr<H5Points3, h5geo::ObjectDeleter>;

#endif // H5POINTS3_H
