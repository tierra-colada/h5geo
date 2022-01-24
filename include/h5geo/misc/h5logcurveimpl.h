#ifndef H5LOGCURVEIMPL_H
#define H5LOGCURVEIMPL_H

#include "../h5logcurve.h"
#include "h5baseobjectimpl.h"

class H5LogCurveImpl : public H5BaseObjectImpl<H5LogCurve>
{
protected:
  explicit H5LogCurveImpl(const h5gt::Group &group);
  virtual ~H5LogCurveImpl() = default;

public:
  virtual bool writeCurve(
      const h5geo::LogDataType& name,
      Eigen::Ref<Eigen::VectorXd> v,
      const std::string& units = "") override;
  virtual bool writeCurve(
      const std::string& name,
      Eigen::Ref<Eigen::VectorXd> v,
      const std::string& units = "") override;

  virtual Eigen::VectorXd getCurve(
      const h5geo::LogDataType& name,
      const std::string& units = "") override;
  virtual Eigen::VectorXd getCurve(
      const std::string& name,
      const std::string& units = "") override;

  virtual std::string getRelativeCurveName() override;

  virtual H5WellContainer* openWellContainer() override;
  virtual H5Well* openWell() override;

  virtual std::optional<h5gt::DataSet> getLogCurveD() override;

  //----------- FRIEND CLASSES -----------
  friend class H5WellImpl;
  friend class H5BaseObjectImpl<H5LogCurve>;
  friend H5LogCurve* h5geo::openLogCurve(h5gt::Group group);
};

#endif // H5LOGCURVEIMPL_H
