#ifndef H5POINTS1_H
#define H5POINTS1_H

#include "h5basepoints.h"

class H5Points1 : public H5BasePoints
{
protected:
  virtual ~H5Points1() = default;

public:
  virtual bool writeData(
      h5geo::Point1Array& data,
      const std::string& lengthUnits = "",
      const std::string& temporalUnits = "") = 0;

  virtual h5geo::Point1Array getData(
      const std::string& lengthUnits = "",
      const std::string& temporalUnits = "") = 0;
};

using H5Points1_ptr = std::unique_ptr<H5Points1, h5geo::ObjectDeleter>;

#endif // H5POINTS1_H
