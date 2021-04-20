#ifndef H5WELL_H
#define H5WELL_H

#include "h5baseobject.h"

#include <Eigen/Dense>

class H5WellContainer;
class H5DevCurve;
class H5LogCurve;

class H5Well : public virtual H5BaseObject
{
protected:
  virtual ~H5Well() = default;

public:
  virtual H5LogCurve* getLogCurve(
      const std::string &logType,
      const std::string &logName) = 0;
  virtual H5LogCurve* getLogCurve(
      h5gt::Group& group) = 0;
  virtual H5DevCurve* getDevCurve(
      const std::string &devName) = 0;
  virtual H5DevCurve* getDevCurve(
      h5gt::Group& group) = 0;

  virtual H5LogCurve* createLogCurve(
      std::string& logType,
      std::string& logName,
      LogCurveParam& p,
      h5geo::CreationType createFlag) = 0;
  virtual H5LogCurve* createLogCurve(
      h5gt::Group& group,
      LogCurveParam& p,
      h5geo::CreationType createFlag) = 0;
  virtual H5DevCurve* createDevCurve(
      std::string& devName,
      DevCurveParam& p,
      h5geo::CreationType createFlag) = 0;
  virtual H5DevCurve* createDevCurve(
      h5gt::Group& group,
      DevCurveParam& p,
      h5geo::CreationType createFlag) = 0;

  virtual Eigen::Vector2d getHeadCoord() = 0;
  virtual double getKB() = 0;
  virtual std::string getUWI() = 0;

  virtual bool setHeadCoord(const Eigen::Ref<const Eigen::Vector2d>& v) = 0;
  virtual bool setKB(const double& kb) = 0;
  virtual bool setUWI(const std::string& uwi) = 0;

  virtual H5DevCurve* getActiveDevCurve() = 0;
  virtual std::vector<H5DevCurve*> getDevCurveList() = 0;
  virtual std::vector<H5LogCurve*> getLogCurveList() = 0;
  virtual std::vector<std::string> getDevCurveNameList() = 0;
  virtual std::vector<std::string> getLogCurveNameList() = 0;
  virtual std::vector<std::string> getLogTypeNameList() = 0;

  virtual H5WellContainer* getWellContainer() = 0;

  virtual std::optional<h5gt::Group> getDevG() = 0;
  virtual std::optional<h5gt::Group> getActiveDevG() = 0;
  virtual std::optional<h5gt::Group> getLogG() = 0;
  virtual std::optional<h5gt::Group> getLogTypeG(const std::string& logType) = 0;
};

using H5Well_ptr = std::unique_ptr<H5Well, h5geo::ObjectDeleter>;

#endif // H5WELL_H
