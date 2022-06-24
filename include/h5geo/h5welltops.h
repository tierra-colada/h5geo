#ifndef H5WELLTOPS_H
#define H5WELLTOPS_H

#include "h5points1.h"

/// \class H5WellTops
/// \brief Provides API to work with welltops
///
/// Welltops is a simply H5Points1 stored within H5Well
/// and usually named `WELLTOPS`.
class H5WellTops : public H5Points1
{
protected:
  virtual ~H5WellTops() = default;

public:
  /// \brief Write data to DataSet
  virtual bool writeData(
      const std::map<std::string, double>& data,
      const std::string& lengthUnits = "",
      const std::string& temporalUnits = "") = 0;

  /// \brief Get data as std::map<`name`, `value`>
  virtual std::map<std::string, double> getDataAsMap(
      const std::string& lengthUnits = "",
      const std::string& temporalUnits = "") = 0;

  /// \brief Open parent H5Well
  virtual H5Well* openWell() = 0;
};

using H5WellTops_ptr = std::unique_ptr<H5WellTops, h5geo::ObjectDeleter>;

#endif // H5WELLTOPS_H
