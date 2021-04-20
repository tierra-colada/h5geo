#ifndef H5DEVCURVE_H
#define H5DEVCURVE_H

#include "h5baseobject.h"

#include <Eigen/Dense>

class H5WellContainer;
class H5Well;

class H5DevCurve : public virtual H5BaseObject
{
protected:
  virtual ~H5DevCurve() = default;

public:
  virtual bool writeCurve(
      const h5geo::DevDataType& name,
      const Eigen::Ref<const Eigen::VectorXd>& v) = 0;
  virtual bool writeCurve(
      const std::string& name,
      const Eigen::Ref<const Eigen::VectorXd>& v) = 0;

  virtual bool setActive() = 0;
  virtual bool isActive() = 0;

  virtual size_t getNCurves() = 0;
  virtual size_t getNSamp() = 0;
  virtual Eigen::VectorXd getCurve(
      const h5geo::DevDataType& name) = 0;
  virtual Eigen::VectorXd getCurve(
      const std::string& name) = 0;

  virtual std::string getRelativeCurveName() = 0;

  virtual H5WellContainer* getWellContainer() = 0;
  virtual H5Well* getWell() = 0;

  virtual std::optional<h5gt::DataSet> getDevCurveD() = 0;
};

using H5DevCurve_ptr = std::unique_ptr<H5DevCurve, h5geo::ObjectDeleter>;

#endif // H5DEVCURVE_H
