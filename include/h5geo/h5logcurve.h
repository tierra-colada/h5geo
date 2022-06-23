#ifndef H5LOGCURVE_H
#define H5LOGCURVE_H

#include "h5baseobject.h"

#include <Eigen/Dense>

class H5WellContainer;
class H5Well;

class H5LogCurve : public H5BaseObject
{
protected:
  virtual ~H5LogCurve() = default;

public:
  virtual bool writeCurve(
      const h5geo::LogDataType& name,
      Eigen::Ref<Eigen::VectorXd> v,
      const std::string& units = "") = 0;
  virtual bool writeCurve(
      const std::string& name,
      Eigen::Ref<Eigen::VectorXd> v,
      const std::string& units = "") = 0;

  virtual size_t getNCurves() = 0;
  virtual size_t getNSamp() = 0;
  virtual Eigen::VectorXd getCurve(
      const h5geo::LogDataType& name,
      const std::string& units = "") = 0;
  virtual Eigen::VectorXd getCurve(
      const std::string& name,
      const std::string& units = "") = 0;

  virtual std::string getRelativeName() = 0;

  virtual LogCurveParam getParam() = 0;

  virtual H5WellContainer* openWellContainer() = 0;
  virtual H5Well* openWell() = 0;

  virtual std::optional<h5gt::DataSet> getLogCurveD() = 0;
};

using H5LogCurve_ptr = std::unique_ptr<H5LogCurve, h5geo::ObjectDeleter>;

#endif // H5LOGCURVE_H
