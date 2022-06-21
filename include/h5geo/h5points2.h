#ifndef H5POINTS2_H
#define H5POINTS2_H

#include "h5basepoints.h"

class H5Points2 : public H5BasePoints
{
protected:
  virtual ~H5Points2() = default;

public:
  virtual bool writeData(
      h5geo::Point2Array& data,
      const std::string& lengthUnits = "",
      bool doCoordTransform = false) = 0;

  virtual h5geo::Point2Array getData(
      const std::string& lengthUnits = "",
      bool doCoordTransform = false) = 0;
};

using H5Points2_ptr = std::unique_ptr<H5Points2, h5geo::ObjectDeleter>;

#endif // H5POINTS2_H
