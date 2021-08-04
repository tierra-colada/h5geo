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
      const Eigen::Ref<const Eigen::MatrixXd>& M) override;

  virtual Eigen::MatrixXd getData() const override;

  virtual bool setDomain(const h5geo::Domain& domain) override;
  virtual bool setSpatialUnits(const std::string& str) override;
  virtual bool setTemporalUnits(const std::string& str) override;
  virtual bool setDataUnits(const std::string& str) override;
  virtual bool setOrigin(const std::vector<double>& v) override;
  virtual bool setOrigin(const Eigen::Ref<const Eigen::Vector2d>& v) override;
  virtual bool setSpacing(const std::vector<double>& v) override;
  virtual bool setSpacing(const Eigen::Ref<const Eigen::Vector2d>& v) override;

  virtual h5geo::Domain getDomain() override;
  virtual std::string getSpatialUnits() override;
  virtual std::string getTemporalUnits() override;
  virtual std::string getDataUnits() override;
  virtual Eigen::Vector2d getOrigin() override;
  virtual Eigen::Vector2d getSpacing() override;

  virtual H5SurfContainer* getSurfContainer() const override;

  virtual std::optional<h5gt::DataSet> getSurfD() const override;

  //----------- FRIEND CLASSES -----------
  friend class H5SurfContainerImpl;
};

#endif // H5SURFIMPL_H
