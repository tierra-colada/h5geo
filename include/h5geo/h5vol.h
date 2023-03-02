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

	/// \brief Set domain for the map (`TVD`, `TVDSS`, `TWT`, `OWT`)
  virtual bool setDomain(const h5geo::Domain& domain) = 0;
	/// \brief Set coordinates of origin
  virtual bool setOrigin(
      Eigen::Ref<Eigen::Vector3d> v,
      const std::string& lengthUnits = "",
      bool doCoordTransform = false) = 0;
	/// \brief Set X,Y,Z unrotated spacings
  virtual bool setSpacings(
      Eigen::Ref<Eigen::Vector3d> v,
      const std::string& lengthUnits = "") = 0;
	/// \brief Set XY plane orientation
  virtual bool setOrientation(
      double val,
      const std::string& angularUnits = "") = 0;

	/// \brief Get domain (`TVD`, `TVDSS`, `TWT`, `OWT`)
  virtual h5geo::Domain getDomain() = 0;
	/// \brief Get coordinates of origin
  virtual Eigen::VectorXd getOrigin(
      const std::string& lengthUnits = "",
      bool doCoordTransform = false) = 0;
	/// \brief Get X,Y,Z unrotated spacings
  virtual Eigen::VectorXd getSpacings(
      const std::string& lengthUnits = "") = 0;
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
  virtual VolParam getParam() = 0;

	/// \brief Open H5VolContainer where current vol resides
  virtual H5VolContainer* openVolContainer() const = 0;

	/// \brief Get current vol's DataSet
  virtual std::optional<h5gt::DataSet> getVolD() const = 0;
};

using H5Vol_ptr = std::unique_ptr<H5Vol, h5geo::ObjectDeleter>;

#endif // H5VOL_H
