#ifndef H5WELLTOPS_H
#define H5WELLTOPS_H

#include "h5points1.h"

class H5WellTops : public H5Points1
{
protected:
  virtual ~H5WellTops() = default;

public:
  virtual bool writeData(
      const std::map<std::string, double>& data,
      const std::string& lengthUnits = "",
      const std::string& temporalUnits = "") = 0;

  virtual std::map<std::string, double> getDataAsMap(
      const std::string& lengthUnits = "",
      const std::string& temporalUnits = "") = 0;

  virtual H5Well* openWell() = 0;
};

using H5WellTops_ptr = std::unique_ptr<H5WellTops, h5geo::ObjectDeleter>;

#endif // H5WELLTOPS_H
