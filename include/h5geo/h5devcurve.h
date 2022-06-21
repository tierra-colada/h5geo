#ifndef H5DEVCURVE_H
#define H5DEVCURVE_H

#include "h5baseobject.h"

#include <Eigen/Dense>

class H5WellContainer;
class H5Well;

class H5DevCurve : public H5BaseObject
{
protected:
  virtual ~H5DevCurve() = default;

public:
  virtual bool writeMD(
      Eigen::Ref<Eigen::VectorXd> v,
      const std::string& units = "") = 0;
  virtual bool writeAZIM(
      Eigen::Ref<Eigen::VectorXd> v,
      const std::string& units = "") = 0;
  virtual bool writeINCL(
      Eigen::Ref<Eigen::VectorXd> v,
      const std::string& units = "") = 0;
  virtual bool writeTVD(
      Eigen::Ref<Eigen::VectorXd> v,
      const std::string& units = "") = 0;
  virtual bool writeDX(
      Eigen::Ref<Eigen::VectorXd> v,
      const std::string& units = "") = 0;
  virtual bool writeDY(
      Eigen::Ref<Eigen::VectorXd> v,
      const std::string& units = "") = 0;
  virtual bool writeOWT(
      Eigen::Ref<Eigen::VectorXd> v,
      const std::string& units = "") = 0;

  virtual bool setActive() = 0;
  virtual bool isActive() = 0;

  /// Tvd, Dx, Dy must be filled
  virtual void updateMdAzimIncl() = 0;
  /// Md, Azim, Incl must be filled
  virtual void updateTvdDxDy() = 0;

  virtual size_t getNCurves() = 0;
  virtual size_t getNSamp() = 0;
  virtual Eigen::VectorXd getCurve(
      const h5geo::DevDataType& name,
      const std::string& units = "",
      bool doCoordTransform = false) = 0;
  virtual Eigen::VectorXd getCurve(
      const std::string& name,
      const std::string& units = "",
      bool doCoordTransform = false) = 0;

  virtual std::string getRelativeCurveName() = 0;

  virtual DevCurveParam getParam() = 0;

  virtual H5WellContainer* openWellContainer() = 0;
  virtual H5Well* openWell() = 0;

  virtual std::optional<h5gt::DataSet> getDevCurveD() = 0;
};

using H5DevCurve_ptr = std::unique_ptr<H5DevCurve, h5geo::ObjectDeleter>;

#endif // H5DEVCURVE_H
