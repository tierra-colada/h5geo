#ifndef H5SURFIMPL_H
#define H5SURFIMPL_H

#include "../h5surf.h"
#include "h5baseobjectimpl.h"

class H5SurfImpl :
    public virtual H5Surf,
    public virtual H5BaseObjectImpl
{
protected:
  explicit H5SurfImpl(const h5gt::Group &group);
  virtual ~H5SurfImpl() = default;

public:
  virtual bool writeData(
      Eigen::Ref<Eigen::MatrixXd> M,
      const std::string& dataUnits = "") override;

  virtual Eigen::MatrixXd getData(const std::string& dataUnits = "") override;

  virtual bool setDomain(const h5geo::Domain& domain) override;
  virtual bool setSpatialUnits(const std::string& str) override;
  virtual bool setDataUnits(const std::string& str) override;
  virtual bool setOrientation(double orientation) override;
  virtual bool setOrigin(
      std::vector<double>& v, const std::string& spatialUnits = "") override;
  virtual bool setOrigin(
      Eigen::Ref<Eigen::Vector2d> v, const std::string& spatialUnits = "") override;
  virtual bool setSpacing(
      std::vector<double>& v, const std::string& spatialUnits = "") override;
  virtual bool setSpacing(
      Eigen::Ref<Eigen::Vector2d> v, const std::string& spatialUnits = "") override;

  virtual h5geo::Domain getDomain() override;
  virtual std::string getSpatialUnits() override;
  virtual std::string getDataUnits() override;
  virtual double getOrientation() override;
  virtual Eigen::VectorXd getOrigin(const std::string& spatialUnits = "") override;
  virtual Eigen::VectorXd getSpacing(const std::string& spatialUnits = "") override;

  virtual H5SurfContainer* getSurfContainer() const override;

  virtual std::optional<h5gt::DataSet> getSurfD() const override;

  //----------- FRIEND CLASSES -----------
  friend class H5SurfContainerImpl;
};

#endif // H5SURFIMPL_H
