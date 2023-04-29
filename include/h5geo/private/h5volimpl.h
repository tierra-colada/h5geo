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
      const std::string& dataUnits = "") override;

  virtual bool readSEGYSTACK(
      const std::string& segy,
      const size_t& ilHdrOffset,
      const size_t& ilHdrSize,
      const size_t& xlHdrOffset,
      const size_t& xlHdrSize,
      const size_t& xHdrOffset,
      const size_t& xHdrSize,
      const size_t& yHdrOffset,
      const size_t& yHdrSize,
      double sampRate,
      size_t nSamp = 0,
      size_t nTrc = 0,
      h5geo::SegyFormat format = static_cast<h5geo::SegyFormat>(0),
      h5geo::Endian endian = static_cast<h5geo::Endian>(0),
      std::function<void(double)> progressCallback = nullptr) override;

  virtual bool setDomain(const h5geo::Domain& domain) override;
  virtual bool setOrigin(
      Eigen::Ref<Eigen::Vector3d> v,
      const std::string& lengthUnits = "",
      const std::string& temporalUnits = "",
      bool doCoordTransform = false) override;
  virtual bool setSpacings(
      Eigen::Ref<Eigen::Vector3d> v,
      const std::string& lengthUnits = "",
      const std::string& temporalunits = "") override;
  virtual bool setOrientation(
      double val,
      const std::string& angularUnits = "") override;

  virtual bool resize(
      size_t nx, size_t ny, size_t nz) override;

  virtual Eigen::MatrixXf getData(
      const size_t& iX0,
      const size_t& iY0,
      const size_t& iZ0,
      const size_t& nX,
      const size_t& nY,
      const size_t& nZ,
      const std::string& dataUnits = "") override;

  virtual h5geo::Domain getDomain() override;
  virtual Eigen::VectorXd getOrigin(
      const std::string& lengthUnits = "",
      const std::string& temporalUnits = "",
      bool doCoordTransform = false) override;
  virtual Eigen::VectorXd getSpacings(
      const std::string& lengthUnits = "",
      const std::string& temporalUnits = "") override;
  virtual double getOrientation(
      const std::string& angularUnits = "") override;
  virtual size_t getNX() override;
  virtual size_t getNY() override;
  virtual size_t getNZ() override;

  virtual H5VolParam getParam() override;

  virtual H5VolContainer* openVolContainer() const override;

  virtual std::optional<h5gt::DataSet> getVolD() const override;

  virtual bool exportToSEGY(
    const std::string& segyFile, 
    std::function<void(double)> progressCallback = nullptr) override;

  virtual bool recreateVolD(
      size_t nX, size_t nY, size_t nZ,
      size_t xChunk, size_t yChunk, size_t zChunk,
      size_t compressionLevel) override;

  //----------- FRIEND CLASSES -----------
  friend class H5VolContainerImpl;
  friend class H5BaseObjectImpl<H5Vol>;
  friend H5Vol* h5geo::openVol(h5gt::Group group);
};

#endif // H5VOLIMPL_H
