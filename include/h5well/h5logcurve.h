#ifndef H5LOGCURVE_H
#define H5LOGCURVE_H

#include "../h5base/h5baseobject.h"
#include "../h5core/h5core.h"

#include <Eigen/Dense>
#include <optional>

class H5WellContainer;
class H5Well;

class H5GEO_EXPORT H5LogCurve : public H5BaseObject
{
private:
  explicit H5LogCurve(const h5gt::Group &group);

public:
  virtual ~H5LogCurve() override;

  static std::optional<H5LogCurve>
  create(h5gt::Group &group);

  bool writeCurve(
      const h5geo::LogDataType& name,
      const Eigen::Ref<const Eigen::VectorXd>& v);
  bool writeCurve(
      const std::string& name,
      const Eigen::Ref<const Eigen::VectorXd>& v);

  Eigen::VectorXd getCurve(
      const h5geo::LogDataType& name);
  Eigen::VectorXd getCurve(
      const std::string& name);

  std::string getRelativeCurveName();

  std::optional<H5WellContainer> getWellContainer();
  std::optional<H5Well> getWell();

  std::optional<h5gt::DataSet> getLogCurveD();
};

#endif // H5LOGCURVE_H
