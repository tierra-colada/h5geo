#ifndef H5POINTS4_H
#define H5POINTS4_H

#include "h5basepoints.h"

/// \class H5Points4
/// \brief Provides API to work with 4D points
///
/// H5Points4 consists of `X`, `Y`, `Z`, `VAL` and `name`. \n
/// Maximal length of `name` is defined by `H5GEO_CHAR_ARRAY_SIZE` macro passed to cmake as CACHE var.
class H5Points4 : public H5BasePoints
{
protected:
  virtual ~H5Points4() = default;

public:
  /// \brief Write data to DataSet
  virtual bool writeData(
      h5geo::Point4Array& data,
      const std::string& lengthUnits = "",
      const std::string& temporalUnits = "",
      const std::string& dataUnits = "",
      bool doCoordTransform = false) = 0;

  /// \brief Read data from DataSet
  virtual h5geo::Point4Array getData(
      const std::string& lengthUnits = "",
      const std::string& temporalUnits = "",
      const std::string& dataUnits = "",
      bool doCoordTransform = false) = 0;
};

using H5Points4_ptr = std::unique_ptr<H5Points4, h5geo::ObjectDeleter>;

#endif // H5POINTS4_H
