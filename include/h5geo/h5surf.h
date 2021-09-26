#ifndef H5SURF_H
#define H5SURF_H

#include "h5baseobject.h"

#include <Eigen/Dense>

class H5SurfContainer;

class H5Surf : public virtual H5BaseObject
{
protected:
  virtual ~H5Surf() = default;

public:
  virtual bool writeData(
      Eigen::Ref<Eigen::MatrixXd> M,
      const std::string& dataUnits = "") = 0;

  virtual Eigen::MatrixXd getData(const std::string& dataUnits = "") = 0;

  virtual bool setDomain(const h5geo::Domain& domain) = 0;
  virtual bool setSpatialUnits(const std::string& str) = 0;
  virtual bool setDataUnits(const std::string& str) = 0;
  virtual bool setOrientation(double orientation) = 0;
  virtual bool setOrigin(
      std::vector<double>& v, const std::string& spatialUnits = "") = 0;
  virtual bool setOrigin(
      Eigen::Ref<Eigen::Vector2d> v, const std::string& spatialUnits = "") = 0;
  virtual bool setSpacing(
      std::vector<double>& v, const std::string& spatialUnits = "") = 0;
  virtual bool setSpacing(
      Eigen::Ref<Eigen::Vector2d> v, const std::string& spatialUnits = "") = 0;

  virtual h5geo::Domain getDomain() = 0;
  virtual std::string getSpatialUnits() = 0;
  virtual std::string getDataUnits() = 0;
  virtual double getOrientation() = 0;
  virtual Eigen::VectorXd getOrigin(const std::string& spatialUnits = "") = 0;
  virtual Eigen::VectorXd getSpacing(const std::string& spatialUnits = "") = 0;

  virtual H5SurfContainer* getSurfContainer() const = 0;

  virtual std::optional<h5gt::DataSet> getSurfD() const = 0;
};

using H5Surf_ptr = std::unique_ptr<H5Surf, h5geo::ObjectDeleter>;

#endif // H5SURF_H
