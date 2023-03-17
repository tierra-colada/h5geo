#ifndef H5LOGCURVE_H
#define H5LOGCURVE_H

#include "h5baseobject.h"

#include <Eigen/Dense>

class H5WellContainer;
class H5Well;

/// \class H5LogCurve
/// \brief Provides API to work with well logs
///
/// Log curve stores the following curves: `MD`, `VAL`. \n
/// Each log has its type and it is located in HDF5 container using following pattern: 
/// `/well/LOG/log_type/relative_path_to_log_curve`
class H5LogCurve : public H5BaseObject
{
protected:
  virtual ~H5LogCurve() = default;

public:
  /// \brief Write `MD` or `VAL` curve
  virtual bool writeCurve(
      const h5geo::LogDataType& name,
      Eigen::Ref<Eigen::VectorXd> v,
      const std::string& units = "") = 0;
  /// \brief Write `MD` or `VAL` curve
  virtual bool writeCurve(
      const std::string& name,
      Eigen::Ref<Eigen::VectorXd> v,
      const std::string& units = "") = 0;

  /// \brief Get number of curves
  virtual size_t getNCurves() = 0;
  /// \brief Get number of samples
  virtual size_t getNSamp() = 0;
  /// \brief Get `MD` or `VAL` curve
  virtual Eigen::VectorXd getCurve(
      const h5geo::LogDataType& name,
      const std::string& units = "") = 0;
  /// \brief Get `MD` or `VAL` curve
  virtual Eigen::VectorXd getCurve(
      const std::string& name,
      const std::string& units = "") = 0;

	/// \brief Get current Log curve's name
	///
	/// Returned curve name is relative to `LOG` Group within H5Well.
	/// It is possible to use this name when H5Well::openLogCurve for example.
  virtual std::string getRelativeName() = 0;

  /// \brief Get parameters that were used to create current log curve
  virtual H5LogCurveParam getParam() = 0;

  /// \brief Open H5WellContainer where current log curve resides
  virtual H5WellContainer* openWellContainer() = 0;
  /// \brief Open parent H5Well
  virtual H5Well* openWell() = 0;

  /// \brief Get current Log curve's DataSet
  virtual std::optional<h5gt::DataSet> getLogCurveD() = 0;
};

using H5LogCurve_ptr = std::unique_ptr<H5LogCurve, h5geo::ObjectDeleter>;

#endif // H5LOGCURVE_H
