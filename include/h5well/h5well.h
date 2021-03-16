#ifndef H5WELL_H
#define H5WELL_H

#include "h5logcurve.h"
#include "h5devcurve.h"

class H5WellContainer;

class H5GEO_EXPORT H5Well : public H5BaseObject
{
private:
  explicit H5Well(const h5gt::Group &group);
  bool setActiveDevCurve(H5DevCurve& curve);

public:
  virtual ~H5Well() override;

  static std::optional<H5Well>
  create(h5gt::Group &group);

  Eigen::Vector2d getHeadCoord();
  double getKB();
  std::string getUWI();

  bool setHeadCoord(const Eigen::Ref<const Eigen::Vector2d>& v);
  bool setKB(const double& kb);
  bool setUWI(const std::string& uwi);

  std::optional<H5LogCurve> getLogCurve(
      const std::string &logType,
      const std::string &logName);
  std::optional<H5DevCurve> getDevCurve(
      const std::string &devName);

  std::optional<H5DevCurve> getActiveDevCurve();
  std::vector<H5DevCurve> getDevCurveList();
  std::vector<H5LogCurve> getLogCurveList();
  std::vector<std::string> getDevCurveNameList();
  std::vector<std::string> getLogCurveNameList();
  std::vector<std::string> getLogTypeNameList();

  std::optional<H5LogCurve> createLogCurve(
      h5gt::Group& group,
      LogCurveParam p,
      const h5geo::CreationType& createFlag =
      h5geo::CreationType::CREATE_OR_OVERWRITE);

  std::optional<H5LogCurve> createLogCurve(
      std::string& logType,
      std::string& logName,
      LogCurveParam p,
      const h5geo::CreationType& createFlag =
      h5geo::CreationType::CREATE_OR_OVERWRITE);

  std::optional<H5DevCurve> createDevCurve(
      h5gt::Group& group,
      DevCurveParam p,
      const h5geo::CreationType& createFlag =
      h5geo::CreationType::CREATE_OR_OVERWRITE);
  std::optional<H5DevCurve> createDevCurve(
      std::string& devName,
      DevCurveParam p,
      const h5geo::CreationType& createFlag =
      h5geo::CreationType::CREATE_OR_OVERWRITE);

  std::optional<H5WellContainer> getWellContainer();

  std::optional<h5gt::Group> getDevG();
  std::optional<h5gt::Group> getActiveDevG();
  std::optional<h5gt::Group> getLogG();
  std::optional<h5gt::Group> getLogTypeG(const std::string& logType);

  friend class H5DevCurve;
};

#endif // H5WELL_H
