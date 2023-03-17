#ifndef H5VOL_H
#define H5VOL_H

#include "h5baseobject.h"

#include <Eigen/Dense>

class H5VolContainer;

/// \class H5Vol
/// \brief Provides API to work with volumes
///
/// Volume is an object that is represented by a regular 3D grid.
class H5Vol : public H5BaseObject
{
protected:
  virtual ~H5Vol() = default;

public:

  /// \brief Write subvolume starting from iX0, iY0, iZ0 indices.
  /// `data` matrix is of size: nRows=nX, nCols=nY*nZ
  virtual bool writeData(
      Eigen::Ref<Eigen::MatrixXf> data,
      const size_t& iX0,
      const size_t& iY0,
      const size_t& iZ0,
      const size_t& nX,
      const size_t& nY,
      const size_t& nZ,
      const std::string& dataUnits = "") = 0;

  /// \brief Read SEGY STACK data, i.e. nTrc should be equal to nil*nxl.
  /// After reading origin, spacings, orientation, and angular units will be set.
  /// \param segy 
  /// \param ilHdrOffset INLINE offset in bytes
  /// \param ilHdrSize INLINE size in bytes
  /// \param xlHdrOffset XLINE offset in bytes
  /// \param xlHdrSize XLINE size in bytes
  /// \param xHdrOffset X-coord offset in bytes
  /// \param xHdrSize X-coord size in bytes
  /// \param yHdrOffset Y-coord offset in bytes
  /// \param yHdrSize Y-coord size in bytes
  /// \param sampRate sampling rate of SEGY file (must know the sign)
  /// \param nSamp number of samples in SEGY (if 0 then try automatically detect)
  /// \param nTrc number of traces in SEGY (if 0 then try automatically detect)
  /// \param format SEGY format (ibm32, ieee32 or int4)
  /// \param endian Big or Little
  /// \param progressCallback 
  /// \return 
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
      std::function<void(double)> progressCallback = nullptr) = 0;

	/// \brief Set domain for the map (`TVD`, `TVDSS`, `TWT`, `OWT`)
  virtual bool setDomain(const h5geo::Domain& domain) = 0;
	/// \brief Set coordinates of origin
  virtual bool setOrigin(
      Eigen::Ref<Eigen::Vector3d> v,
      const std::string& lengthUnits = "",
      const std::string& temporalUnits = "",
      bool doCoordTransform = false) = 0;
	/// \brief Set X,Y,Z unrotated spacings
  virtual bool setSpacings(
      Eigen::Ref<Eigen::Vector3d> v,
      const std::string& lengthUnits = "",
      const std::string& temporalUnits = "") = 0;
	/// \brief Set XY plane orientation
  virtual bool setOrientation(
      double val,
      const std::string& angularUnits = "") = 0;

  /// \brief Resize volume
  virtual bool resize(
      size_t nx, size_t ny, size_t nz) = 0;

  /// \brief Read subvolume starting from iX0, iY0, iZ0 indices.
  /// `data` matrix is of size: nRows=nX, nCols=nY*nZ
  virtual Eigen::MatrixXf getData(
      const size_t& iX0,
      const size_t& iY0,
      const size_t& iZ0,
      const size_t& nX,
      const size_t& nY,
      const size_t& nZ,
      const std::string& dataUnits = "") = 0;

	/// \brief Get domain (`TVD`, `TVDSS`, `TWT`, `OWT`)
  virtual h5geo::Domain getDomain() = 0;
	/// \brief Get coordinates of origin
  virtual Eigen::VectorXd getOrigin(
      const std::string& lengthUnits = "",
      const std::string& temporalUnits = "",
      bool doCoordTransform = false) = 0;
	/// \brief Get X,Y,Z unrotated spacings
  virtual Eigen::VectorXd getSpacings(
      const std::string& lengthUnits = "",
      const std::string& temporalUnits = "") = 0;
	/// \brief Get XY plane orientation
  virtual double getOrientation(
      const std::string& angularUnits = "") = 0;
	/// \brief Get number of X values
  virtual size_t getNX() = 0;
	/// \brief Get number of Y values
  virtual size_t getNY() = 0;
	/// \brief Get number of Z values
  virtual size_t getNZ() = 0;

	/// \brief Get parameters that were used to create current map
  virtual H5VolParam getParam() = 0;

	/// \brief Open H5VolContainer where current vol resides
  virtual H5VolContainer* openVolContainer() const = 0;

	/// \brief Get current vol's DataSet
  virtual std::optional<h5gt::DataSet> getVolD() const = 0;

  virtual bool exportToSEGY(
    const std::string& segyFile, 
    std::function<void(double)> progressCallback = nullptr) = 0;
};

using H5Vol_ptr = std::unique_ptr<H5Vol, h5geo::ObjectDeleter>;

#endif // H5VOL_H
