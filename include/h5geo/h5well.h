#ifndef H5WELL_H
#define H5WELL_H

#include "h5baseobject.h"

#include <Eigen/Dense>

class H5WellContainer;
class H5DevCurve;
class H5LogCurve;
class H5WellTops;

class H5Well : public H5BaseObject
{
protected:
  virtual ~H5Well() = default;

public:
  /// logType maybe empty
  virtual H5LogCurve* openLogCurve(
      const std::string &logType,
      const std::string &logName) = 0;
  virtual H5LogCurve* openLogCurve(
      h5gt::Group group) = 0;
  virtual H5DevCurve* openDevCurve(
      const std::string &devName) = 0;
  virtual H5DevCurve* openDevCurve(
      h5gt::Group group) = 0;
  virtual H5WellTops* openWellTops() = 0;

  /// logType maybe empty
  virtual H5LogCurve* createLogCurve(
      std::string& logType,
      std::string& logName,
      LogCurveParam& p,
      h5geo::CreationType createFlag) = 0;
  virtual H5LogCurve* createLogCurve(
      h5gt::Group group,
      LogCurveParam& p,
      h5geo::CreationType createFlag) = 0;
  virtual H5DevCurve* createDevCurve(
      std::string& devName,
      DevCurveParam& p,
      h5geo::CreationType createFlag) = 0;
  virtual H5DevCurve* createDevCurve(
      h5gt::Group group,
      DevCurveParam& p,
      h5geo::CreationType createFlag) = 0;
  /// Created object under WELLTOPS name
  virtual H5WellTops* createWellTops(
      WellTopsParam& p,
      h5geo::CreationType createFlag) = 0;

  virtual bool setHeadCoord(
      Eigen::Ref<Eigen::Vector2d> v,
      const std::string& lengthUnits = "",
      bool doCoordTransform = false) = 0;
  virtual bool setKB(
      double& val,
      const std::string& lengthUnits = "") = 0;
  virtual bool setUWI(const std::string& str) = 0;
  virtual bool setActiveDevCurve(H5DevCurve* curve) = 0;

  virtual Eigen::VectorXd getHeadCoord(
      const std::string& lengthUnits = "",
      bool doCoordTransform = false) = 0;
  virtual double getKB(
      const std::string& lengthUnits = "") = 0;
  virtual std::string getUWI() = 0;

  virtual H5DevCurve* openActiveDevCurve() = 0;
  /// Omit active dev curve
  virtual std::vector<h5gt::Group> getDevCurveGroupList() = 0;
  virtual std::vector<h5gt::Group> getLogCurveGroupList() = 0;
  /// Omit active dev curve
  virtual std::vector<std::string> getDevCurveNameList() = 0;
  /// return all logCurve names in the form 'TYPE/NAME'
  virtual std::vector<std::string> getLogCurveNameList() = 0;
  /// return all objG child group names that are not logCurves
  virtual std::vector<std::string> getLogTypeList() = 0;
  virtual size_t getDevCurveCount() = 0;
  virtual size_t getLogCurveCount() = 0;

  virtual WellParam getParam() = 0;

  virtual H5WellContainer* openWellContainer() = 0;

  virtual std::optional<h5gt::Group> getDevG() = 0;
  virtual std::optional<h5gt::Group> getActiveDevG() = 0;
  virtual std::optional<h5gt::Group> getWellTopsG() = 0;
  virtual std::optional<h5gt::Group> getLogG() = 0;
  virtual std::optional<h5gt::Group> getLogTypeG(const std::string& logType) = 0;
};

using H5Well_ptr = std::unique_ptr<H5Well, h5geo::ObjectDeleter>;

#endif // H5WELL_H
