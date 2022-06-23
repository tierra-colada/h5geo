#ifndef H5POINTS3_H
#define H5POINTS3_H

#include "h5basepoints.h"

/// \class H5Points3
/// \brief Provides API to work with 3D points
///
/// H5Points3 consists of `X`, `Y`, `VAL` and `name`. \n
/// Maximal length of `name` is defined by `H5GEO_CHAR_ARRAY_SIZE` macro passed to cmake as CACHE var.
class H5Points3 : public H5BasePoints
{
protected:
  virtual ~H5Points3() = default;

public:
  /// \brief Write data to DataSet
  virtual bool writeData(
      h5geo::Point3Array& data,
      const std::string& lengthUnits = "",
      const std::string& temporalUnits = "",
      bool doCoordTransform = false) = 0;

  /// \brief Read data from DataSet
  virtual h5geo::Point3Array getData(
      const std::string& lengthUnits = "",
      const std::string& temporalUnits = "",
      bool doCoordTransform = false) = 0;
};

using H5Points3_ptr = std::unique_ptr<H5Points3, h5geo::ObjectDeleter>;

#endif // H5POINTS3_H
