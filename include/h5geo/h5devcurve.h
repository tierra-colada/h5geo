#ifndef H5DEVCURVE_H
#define H5DEVCURVE_H

#include "h5baseobject.h"

#include <Eigen/Dense>

class H5WellContainer;
class H5Well;

/// \class H5DevCurve
/// \brief Provides API to work with well deviations (trajectories)
///
/// H5DevCurve stores the following curves: `MD`, `AZIM`, `INCL`, `DX`, `DY`, `TVD`, `OWT`.
/// All other curves are calculated based on these curves. 
/// That is done to prevent calculation errors i.e. everytime `MD_AZIM_INCL` is transformed to 
/// `X_Y_TVD` an error is accumulating. 
/// The same concerns when doing that in backward order: `X_Y_TVD` to `MD_AZIM_INCL`. \n
/// Dev curve is located in HDF5 container using following pattern: `/well/DEV/relative_path_to_dev_curve`
class H5DevCurve : public H5BaseObject
{
protected:
  virtual ~H5DevCurve() = default;

public:
	/// \brief Write `MD` curve
  virtual bool writeMD(
      Eigen::Ref<Eigen::VectorXd> v,
      const std::string& units = "") = 0;
	/// \brief Write `AZIM` curve
  virtual bool writeAZIM(
      Eigen::Ref<Eigen::VectorXd> v,
      const std::string& units = "") = 0;
	/// \brief Write `INCL` curve
  virtual bool writeINCL(
      Eigen::Ref<Eigen::VectorXd> v,
      const std::string& units = "") = 0;
	/// \brief Write `TVD` curve
  virtual bool writeTVD(
      Eigen::Ref<Eigen::VectorXd> v,
      const std::string& units = "") = 0;
	/// \brief Write `DX` curve
  virtual bool writeDX(
      Eigen::Ref<Eigen::VectorXd> v,
      const std::string& units = "") = 0;
	/// \brief Write `DY` curve
  virtual bool writeDY(
      Eigen::Ref<Eigen::VectorXd> v,
      const std::string& units = "") = 0;
	/// \brief Write `OWT` curve
  virtual bool writeOWT(
      Eigen::Ref<Eigen::VectorXd> v,
      const std::string& units = "") = 0;

	/// \brief Set current dev curve as active for the parent H5Well
  virtual bool setActive() = 0;
	/// \brief Check if current dev curve is active for the parent H5Well
  virtual bool isActive() = 0;

	/// \brief Calculate `MD`, `AZIM`, `INCL` based on `X`, `Y`, `TVD`
	///
	/// `TVD`, `DX`, `DY` must be filled. \n
	/// Call it everytime `TVD`, `DX` or `DY` were changed
  virtual void updateMdAzimIncl() = 0;
	/// \brief Calculate `MD`, `AZIM`, `INCL` based on `X`, `Y`, `TVD`
	///
	/// `MD`, `AZIM`, `INCL` must be filled. \n
	/// Call it everytime `MD`, `AZIM` or `INCL` were changed
  virtual void updateTvdDxDy() = 0;

	/// \brief Get number of curves
  virtual size_t getNCurves() = 0;
	/// \brief Get number of samples
  virtual size_t getNSamp() = 0;
	/// \brief Get curve
  virtual Eigen::VectorXd getCurve(
      const h5geo::DevDataType& name,
      const std::string& units = "",
      bool doCoordTransform = false) = 0;
	/// \brief Get curve
  virtual Eigen::VectorXd getCurve(
      const std::string& name,
      const std::string& units = "",
      bool doCoordTransform = false) = 0;

	/// \brief Get current dev curve's name
	///
	/// Returned curve name is relative to `DEV` Group within H5Well.
	/// It is possible to use this name when H5Well::openDevCurve for example.
  virtual std::string getRelativeName() = 0;

	/// \brief Get parameters that were used to create current dev curve
  virtual H5DevCurveParam getParam() = 0;

	/// \brief Open H5WellContainer where current dev curve resides
  virtual H5WellContainer* openWellContainer() = 0;
	/// \brief Open parent H5Well
  virtual H5Well* openWell() = 0;

	/// \brief Get current dev curve's DataSet
  virtual std::optional<h5gt::DataSet> getDevCurveD() = 0;
};

using H5DevCurve_ptr = std::unique_ptr<H5DevCurve, h5geo::ObjectDeleter>;

#endif // H5DEVCURVE_H
