#ifndef H5MAPIMPL_H
#define H5MAPIMPL_H

#include "../h5map.h"
#include "h5baseobjectimpl.h"

class H5MapImpl :
    public virtual H5Map,
    public virtual H5BaseObjectImpl
{
protected:
  explicit H5MapImpl(const h5gt::Group &group);
  virtual ~H5MapImpl() = default;

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

  virtual H5MapContainer* getMapContainer() const override;

  virtual std::optional<h5gt::DataSet> getMapD() const override;

  //----------- FRIEND CLASSES -----------
  friend class H5MapContainerImpl;
};

#endif // H5MAPIMPL_H
