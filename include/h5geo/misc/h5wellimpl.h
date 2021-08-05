#ifndef H5WELLIMPL_H
#define H5WELLIMPL_H

#include "../h5well.h"
#include "h5baseobjectimpl.h"

class H5WellImpl :
    public virtual H5Well,
    public virtual H5BaseObjectImpl
{
protected:
  explicit H5WellImpl(const h5gt::Group &group);
  virtual ~H5WellImpl() = default;

public:
  virtual H5LogCurve* getLogCurve(
      const std::string &logType,
      const std::string &logName) override;
  virtual H5LogCurve* getLogCurve(
      h5gt::Group group) override;
  virtual H5DevCurve* getDevCurve(
      const std::string &devName) override;
  virtual H5DevCurve* getDevCurve(
      h5gt::Group group) override;

  virtual H5LogCurve* createLogCurve(
      std::string& logType,
      std::string& logName,
      LogCurveParam& p,
      h5geo::CreationType createFlag) override;
  virtual H5LogCurve* createLogCurve(
      h5gt::Group group,
      LogCurveParam& p,
      h5geo::CreationType createFlag) override;
  virtual H5DevCurve* createDevCurve(
      std::string& devName,
      DevCurveParam& p,
      h5geo::CreationType createFlag) override;
  virtual H5DevCurve* createDevCurve(
      h5gt::Group group,
      DevCurveParam& p,
      h5geo::CreationType createFlag) override;

  virtual bool setSpatialUnits(const std::string& str) override;
  virtual bool setHeadCoord(const Eigen::Ref<const Eigen::Vector2d>& v) override;
  virtual bool setKB(const double& val) override;
  virtual bool setUWI(const std::string& str) override;

  virtual std::string getSpatialUnits() override;
  virtual Eigen::VectorXd getHeadCoord(const std::string& spatialUnits = "") override;
  virtual double getKB(const std::string& spatialUnits = "") override;
  virtual std::string getUWI() override;

  virtual H5DevCurve* getActiveDevCurve() override;
  virtual std::vector<H5DevCurve*> getDevCurveList() override;
  virtual std::vector<H5LogCurve*> getLogCurveList() override;
  virtual std::vector<std::string> getDevCurveNameList() override;
  virtual std::vector<std::string> getLogCurveNameList() override;
  virtual std::vector<std::string> getLogTypeNameList() override;

  virtual H5WellContainer* getWellContainer() override;

  virtual std::optional<h5gt::Group> getDevG() override;
  virtual std::optional<h5gt::Group> getActiveDevG() override;
  virtual std::optional<h5gt::Group> getLogG() override;
  virtual std::optional<h5gt::Group> getLogTypeG(const std::string& logType) override;

private:
  bool setActiveDevCurve(H5DevCurve& curve);

  //----------- FRIEND CLASSES -----------
  friend class H5WellContainerImpl;
  friend class H5LogCurveImpl;
  friend class H5DevCurveImpl;
};

#endif // H5WELLIMPL_H
