#ifndef H5WELL_H
#define H5WELL_H

#include "h5baseobject.h"

#include <Eigen/Dense>

class H5WellContainer;
class H5DevCurve;
class H5LogCurve;
class H5WellTops;

/// \class H5Well
/// \brief Provides API to work with wells
///
/// Well is an object that doesn't dispose DataSets but uses only HDF5 
/// Groups and Attributes for storing information about well head and 
/// kelly bushing for example. \n
/// H5DevCurve, H5LogCurve and H5WellTops are separate objects that 
/// reside within well.
class H5Well : public H5BaseObject
{
protected:
  virtual ~H5Well() = default;

public:
  /// \brief Open H5LogCurve
  ///
  /// If `logType` is empty then `logName` is considered as relative or
  /// full path to the log curve.
  virtual H5LogCurve* openLogCurve(
      const std::string &logType,
      const std::string &logName) = 0;
  /// \brief Open H5LogCurve
  virtual H5LogCurve* openLogCurve(
      h5gt::Group group) = 0;
	/// \brief Open H5DevCurve
  virtual H5DevCurve* openDevCurve(
      const std::string &devName) = 0;
	/// \brief Open H5DevCurve
  virtual H5DevCurve* openDevCurve(
      h5gt::Group group) = 0;
	/// \brief Open H5WellTops
	///
	/// Look for H5WellTops with `WELLTOPS` name
  virtual H5WellTops* openWellTops() = 0;

  /// \brief Create H5LogCurve
  ///
  /// If `logType` is empty then `logName` is considered as relative or
  /// full path to the log curve.
  virtual H5LogCurve* createLogCurve(
      std::string& logType,
      std::string& logName,
      H5LogCurveParam& p,
      h5geo::CreationType createFlag) = 0;
	/// \brief Create H5LogCurve
  virtual H5LogCurve* createLogCurve(
      h5gt::Group group,
      H5LogCurveParam& p,
      h5geo::CreationType createFlag) = 0;
	/// \brief Create H5DevCurve
  virtual H5DevCurve* createDevCurve(
      std::string& devName,
      H5DevCurveParam& p,
      h5geo::CreationType createFlag) = 0;
	/// \brief Create H5DevCurve
  virtual H5DevCurve* createDevCurve(
      h5gt::Group group,
      H5DevCurveParam& p,
      h5geo::CreationType createFlag) = 0;
	/// \brief Create H5WellTops
	///
	/// Created object uses `WELLTOPS` as name
  virtual H5WellTops* createWellTops(
      H5WellTopsParam& p,
      h5geo::CreationType createFlag) = 0;

	/// \brief Set `XY` well head coordinates
  virtual bool setHeadCoord(
      Eigen::Ref<Eigen::Vector2d> v,
      const std::string& lengthUnits = "",
      bool doCoordTransform = false) = 0;
	/// \brief Set `XY` kelly bushing
  virtual bool setKB(
      double& val,
      const std::string& lengthUnits = "") = 0;
	/// \brief Set `UWI` Unique Well Identifier
  virtual bool setUWI(const std::string& str) = 0;

	/// \brief Set selected H5DevCurve as active
	///
	/// Active H5DevCurve is simply HDF5 `SOFT` link that
	/// uses `ACTIVE` as name
  virtual bool setActiveDevCurve(H5DevCurve* curve) = 0;

	/// \brief Get `XY` well head coordinates
  virtual Eigen::VectorXd getHeadCoord(
      const std::string& lengthUnits = "",
      bool doCoordTransform = false) = 0;
	/// \brief Set kelly bushing
  virtual double getKB(
      const std::string& lengthUnits = "") = 0;
	/// \brief Get `UWI` Unique Well Identifier
  virtual std::string getUWI() = 0;

	/// \brief Get active H5DevCurve
	///
	/// Active H5DevCurve is simply HDF5 `SOFT` link that
	/// uses `ACTIVE` as name
  virtual H5DevCurve* openActiveDevCurve() = 0;
  /// Omit active dev curve

	/// \brief Find and return vector of H5DevCurve Groups within current well
  virtual std::vector<h5gt::Group> getDevCurveGroupList() = 0;
	/// \brief Find and return vector of H5LogCurve Groups within current well
  virtual std::vector<h5gt::Group> getLogCurveGroupList() = 0;
  /// \brief Find and return vector of H5DevCurve names within current well
  virtual std::vector<std::string> getDevCurveNameList() = 0;
  /// \brief Find and return vector of H5LogCurve names within current well
  virtual std::vector<std::string> getLogCurveNameList() = 0;
  /// \brief Find and return vector of H5LogCurve types within current well
  virtual std::vector<std::string> getLogTypeList() = 0;
	/// \brief Get number of H5DevCurves within current well
  virtual size_t getDevCurveCount() = 0;
	/// \brief Get number of H5LogCurves within current well
  virtual size_t getLogCurveCount() = 0;

	/// \brief Get parameters that were used to create current well
  virtual H5WellParam getParam() = 0;

	/// \brief Open H5WellContainer where current well resides
  virtual H5WellContainer* openWellContainer() = 0;

	/// \brief Get current well's `DEV` Group (where H5DevCurves reside)
  virtual std::optional<h5gt::Group> getDevG() = 0;
	/// \brief Get current well's active H5DevCurve Group
  virtual std::optional<h5gt::Group> getActiveDevG() = 0;
	/// \brief Get current well's H5WellTops Group
  virtual std::optional<h5gt::Group> getWellTopsG() = 0;
	/// \brief Get current well's `LOG` Group (where H5LogCurves reside)
  virtual std::optional<h5gt::Group> getLogG() = 0;
	/// \brief Get current well's `logType` Group
  virtual std::optional<h5gt::Group> getLogTypeG(const std::string& logType) = 0;
};

using H5Well_ptr = std::unique_ptr<H5Well, h5geo::ObjectDeleter>;

#endif // H5WELL_H
