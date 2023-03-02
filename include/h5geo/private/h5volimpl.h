#ifndef H5VOLIMPL_H
#define H5VOLIMPL_H

#include "../h5vol.h"
#include "h5baseobjectimpl.h"

class H5VolImpl : public H5BaseObjectImpl<H5Vol>
{
protected:
  explicit H5VolImpl(const h5gt::Group &group);
  virtual ~H5VolImpl() = default;

public:

  virtual bool writeData(
      Eigen::Ref<Eigen::MatrixXf> data,
      const size_t& iX0,
      const size_t& iY0,
      const size_t& iZ0,
      const size_t& nX,
      const size_t& nY,
      const size_t& nZ,
      const std::string& dataUnits) override;

  virtual bool setDomain(const h5geo::Domain& domain) override;
  virtual bool setOrigin(
      Eigen::Ref<Eigen::Vector3d> v,
      const std::string& lengthUnits,
      bool doCoordTransform) override;
  virtual bool setSpacings(
      Eigen::Ref<Eigen::Vector3d> v,
      const std::string& lengthUnits) override;
  virtual bool setOrientation(
      double val,
      const std::string& angularUnits) override;

  virtual Eigen::MatrixXf getData(
      const size_t& iX0,
      const size_t& iY0,
      const size_t& iZ0,
      const size_t& nX,
      const size_t& nY,
      const size_t& nZ,
      const std::string& dataUnits) override;

  virtual h5geo::Domain getDomain() override;
  virtual Eigen::VectorXd getOrigin(
      const std::string& lengthUnits,
      bool doCoordTransform) override;
  virtual Eigen::VectorXd getSpacings(
      const std::string& lengthUnits) override;
  virtual double getOrientation(
      const std::string& angularUnits) override;
  virtual size_t getNX() override;
  virtual size_t getNY() override;
  virtual size_t getNZ() override;

  virtual VolParam getParam() override;

  virtual H5VolContainer* openVolContainer() const override;

  virtual std::optional<h5gt::DataSet> getVolD() const override;

  //----------- FRIEND CLASSES -----------
  friend class H5VolContainerImpl;
  friend class H5BaseObjectImpl<H5Vol>;
  friend H5Vol* h5geo::openVol(h5gt::Group group);
};

#endif // H5VOLIMPL_H
