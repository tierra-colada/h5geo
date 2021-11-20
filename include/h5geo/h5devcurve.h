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
      Eigen::Ref<Eigen::VectorXd> v,
      const std::string& units = "") = 0;
  virtual bool writeCurve(
      const std::string& name,
      Eigen::Ref<Eigen::VectorXd> v,
      const std::string& units = "") = 0;

  virtual bool setActive() = 0;
  virtual bool isActive() = 0;

  virtual size_t getNCurves() = 0;
  virtual size_t getNSamp() = 0;
  virtual Eigen::VectorXd getCurve(
      const h5geo::DevDataType& name,
      const std::string& units = "") = 0;
  virtual Eigen::VectorXd getCurve(
      const std::string& name,
      const std::string& units = "") = 0;

  virtual bool setSpatialUnits(const std::string& str) = 0;
  virtual bool setTemporalUnits(const std::string& str) = 0;
  virtual bool setAngleUnits(const std::string& str) = 0;

  virtual std::string getSpatialUnits() = 0;
  virtual std::string getTemporalUnits() = 0;
  virtual std::string getAngleUnits() = 0;

  virtual std::string getRelativeCurveName() = 0;

  virtual H5WellContainer* getWellContainer() = 0;
  virtual H5Well* getWell() = 0;

  virtual std::optional<h5gt::DataSet> getDevCurveD() = 0;
};

namespace h5geo {
  extern "C" {
  H5GEO_EXPORT H5DevCurve* openDevCurve(h5gt::Group group);
  }
}

using H5DevCurve_ptr = std::unique_ptr<H5DevCurve, h5geo::ObjectDeleter>;

#endif // H5DEVCURVE_H
