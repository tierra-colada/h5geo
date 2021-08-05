#ifndef H5LOGCURVE_H
#define H5LOGCURVE_H

#include "h5baseobject.h"

#include <Eigen/Dense>

class H5WellContainer;
class H5Well;

class H5LogCurve : public virtual H5BaseObject
{
protected:
  virtual ~H5LogCurve() = default;

public:
  virtual bool writeCurve(
      const h5geo::LogDataType& name,
      const Eigen::Ref<const Eigen::VectorXd>& v) = 0;
  virtual bool writeCurve(
      const std::string& name,
      const Eigen::Ref<const Eigen::VectorXd>& v) = 0;

  virtual Eigen::VectorXd getCurve(
      const h5geo::LogDataType& name,
      const std::string& units = "") = 0;
  virtual Eigen::VectorXd getCurve(
      const std::string& name,
      const std::string& units = "") = 0;

  virtual bool setSpatialUnits(const std::string& str) = 0;
  virtual bool setDataUnits(const std::string& str) = 0;

  virtual std::string getSpatialUnits() = 0;
  virtual std::string getDataUnits() = 0;

  virtual std::string getRelativeCurveName() = 0;

  virtual H5WellContainer* getWellContainer() = 0;
  virtual H5Well* getWell() = 0;

  virtual std::optional<h5gt::DataSet> getLogCurveD() = 0;
};

using H5LogCurve_ptr = std::unique_ptr<H5LogCurve, h5geo::ObjectDeleter>;

#endif // H5LOGCURVE_H
