#ifndef H5POINTS2_H
#define H5POINTS2_H

#include "h5basepoints.h"

/// \class H5Points2
/// \brief Provides API to work with 2D points
///
/// H5Points2 consists of `X`, `VAL` and `name`. \n
/// Maximal length of `name` is defined by `H5GEO_CHAR_ARRAY_SIZE` macro passed to cmake as CACHE var.
class H5Points2 : public H5BasePoints
{
protected:
  virtual ~H5Points2() = default;

public:
  /// \brief Write data to DataSet
  virtual bool writeData(
      h5geo::Point2Array& data,
      const std::string& lengthUnits = "",
      bool doCoordTransform = false) = 0;

  /// \brief Read data from DataSet
  virtual h5geo::Point2Array getData(
      const std::string& lengthUnits = "",
      bool doCoordTransform = false) = 0;
};

using H5Points2_ptr = std::unique_ptr<H5Points2, h5geo::ObjectDeleter>;

#endif // H5POINTS2_H
