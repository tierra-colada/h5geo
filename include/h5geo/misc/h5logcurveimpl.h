#ifndef H5LOGCURVEIMPL_H
#define H5LOGCURVEIMPL_H

#include "../h5logcurve.h"
#include "h5baseobjectimpl.h"

class H5LogCurveImpl :
    public virtual H5LogCurve,
    public virtual H5BaseObjectImpl
{
protected:
  explicit H5LogCurveImpl(const h5gt::Group &group);
  virtual ~H5LogCurveImpl() = default;

public:
  virtual bool writeCurve(
      const h5geo::LogDataType& name,
      const Eigen::Ref<const Eigen::VectorXd>& v) override;
  virtual bool writeCurve(
      const std::string& name,
      const Eigen::Ref<const Eigen::VectorXd>& v) override;

  virtual Eigen::VectorXd getCurve(
      const h5geo::LogDataType& name,
      const std::string& units = "") override;
  virtual Eigen::VectorXd getCurve(
      const std::string& name,
      const std::string& units = "") override;

  virtual bool setSpatialUnits(const std::string& str) override;
  virtual bool setDataUnits(const std::string& str) override;

  virtual std::string getSpatialUnits() override;
  virtual std::string getDataUnits() override;

  virtual std::string getRelativeCurveName() override;

  virtual H5WellContainer* getWellContainer() override;
  virtual H5Well* getWell() override;

  virtual std::optional<h5gt::DataSet> getLogCurveD() override;

  //----------- FRIEND CLASSES -----------
  friend class H5WellImpl;
};

#endif // H5LOGCURVEIMPL_H
