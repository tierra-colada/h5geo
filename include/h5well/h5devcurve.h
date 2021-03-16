#ifndef H5DEVCURVE_H
#define H5DEVCURVE_H

#include "../h5base/h5baseobject.h"
#include "../h5core/h5core.h"

#include <Eigen/Dense>
#include <optional>

class H5WellContainer;
class H5Well;

class H5GEO_EXPORT H5DevCurve : public H5BaseObject
{
private:
  explicit H5DevCurve(const h5gt::Group &group);

public:
  virtual ~H5DevCurve() override;

  static std::optional<H5DevCurve>
  create(h5gt::Group &group);

  bool writeCurve(
      const h5geo::DevDataType& name,
      const Eigen::Ref<const Eigen::VectorXd>& v);
  bool writeCurve(
      const std::string& name,
      const Eigen::Ref<const Eigen::VectorXd>& v);

  bool setActive();
  bool isActive();

  size_t getNCurves();
  size_t getNSamp();
  Eigen::VectorXd getCurve(
      const h5geo::DevDataType& name);
  Eigen::VectorXd getCurve(
      const std::string& name);

  std::string getRelativeCurveName();

  std::optional<H5WellContainer> getWellContainer();
  std::optional<H5Well> getWell();

  std::optional<h5gt::DataSet> getDevCurveD();
};

#endif // H5DEVCURVE_H
