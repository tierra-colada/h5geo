#ifndef H5WELLIMPL_H
#define H5WELLIMPL_H

#include "../h5well.h"
#include "h5baseobjectimpl.h"

class H5WellImpl : public H5BaseObjectImpl<H5Well>
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

  virtual bool setHeadCoord(
      Eigen::Ref<Eigen::Vector2d> v,
      const std::string& lengthUnits = "",
      bool doCoordTransform = false) override;
  virtual bool setKB(
      double& val,
      const std::string& lengthUnits = "") override;
  virtual bool setUWI(const std::string& str) override;
  virtual bool setActiveDevCurve(H5DevCurve* curve) override;

  virtual Eigen::VectorXd getHeadCoord(
      const std::string& lengthUnits = "",
      bool doCoordTransform = false) override;
  virtual double getKB(
      const std::string& lengthUnits = "") override;
  virtual std::string getUWI() override;

  virtual H5DevCurve* getActiveDevCurve() override;
  virtual std::vector<h5gt::Group> getDevCurveGroupList() override;
  virtual std::vector<h5gt::Group> getLogCurveGroupList() override;
  virtual std::vector<std::string> getDevCurveNameList() override;
  virtual std::vector<std::string> getLogCurveNameList() override;
  virtual std::vector<std::string> getLogTypeNameList() override;

  virtual H5WellContainer* getWellContainer() override;

  virtual std::optional<h5gt::Group> getDevG() override;
  virtual std::optional<h5gt::Group> getActiveDevG() override;
  virtual std::optional<h5gt::Group> getLogG() override;
  virtual std::optional<h5gt::Group> getLogTypeG(const std::string& logType) override;

private:

  //----------- FRIEND CLASSES -----------
  friend class H5WellContainerImpl;
  friend class H5LogCurveImpl;
  friend class H5DevCurveImpl;
  friend class H5BaseObjectImpl<H5Well>;
  friend H5Well* h5geo::openWell(h5gt::Group group);
};

#endif // H5WELLIMPL_H
