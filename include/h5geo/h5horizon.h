#ifndef H5HORIZON_H
#define H5HORIZON_H

#include "h5baseobject.h"

#include <Eigen/Dense>

class H5BaseContainer;

/// \class H5Horizon
/// \brief Provides API to work with horizons
///
/// Horizon is an object represented by a dataset with named
/// rows within. Thus it can be `XYZ` and `VAL` - 4 component
/// data. A single `X0 Y0 Z0 VAL0` would be called `point`.
class H5Horizon : public H5BaseObject
{
protected:
  virtual ~H5Horizon() = default;

public:
  /// \brief Write data to DataSet
  virtual bool writeData(
      Eigen::Ref<Eigen::MatrixXd> M,
      const std::string& unitsFrom = "",
      const std::string& unitsTo = "") = 0;

  /// \brief Read data from DataSet
  virtual Eigen::MatrixXd getData(
      const std::string& unitsFrom = "",
      const std::string& unitsTo = "") = 0;

  /// \brief Write single component to DataSet
  virtual bool writeComponent(
      const std::string& componentName,
      Eigen::Ref<Eigen::VectorXd> v,
      const std::string& unitsFrom = "",
      const std::string& unitsTo = "") = 0;

  /// \brief Read single component from DataSet
  virtual Eigen::VectorXd getComponent(
      const std::string& componentName,
      const std::string& unitsFrom = "",
      const std::string& unitsTo = "") = 0;

  /// \brief Resize rows of HDF5 DataSet
  virtual bool setNPoints(size_t n) = 0;
  /// \brief Resize columns of  HDF5 DataSet
  virtual bool setNComponents(size_t n) = 0;
  /// \brief Set components
  ///
  /// Delete all attributes from horizon DataSet and create new
  virtual bool setComponents(const std::map<std::string, size_t>& components) = 0;
  /// \brief Set domain for the points (`TVD`, `TVDSS`, `TWT`, `OWT`)
  virtual bool setDomain(const h5geo::Domain& domain) = 0;

  /// \brief Open container where current points resides
  virtual H5BaseContainer* openContainer() const = 0;
  /// \brief Return number of points for the horizon
  virtual size_t getNPoints() = 0;
  /// \brief Return number of dimension for the horizon
  virtual size_t getNComponents() = 0;
  /// \brief Return component names and corresponding HDF5 rows
  virtual std::map<std::string, size_t> getComponents() = 0;
  /// \brief Get domain (`TVD`, `TVDSS`, `TWT`, `OWT`)
  virtual h5geo::Domain getDomain() = 0;

  /// \brief Get parameters that were used to create current points
  virtual HorizonParam getParam() = 0;

  /// \brief Get current points DataSet
  virtual std::optional<h5gt::DataSet> getHorizonD() const = 0;
};

using H5Horizon_ptr = std::unique_ptr<H5Horizon, h5geo::ObjectDeleter>;

#endif // H5HORIZON_H
