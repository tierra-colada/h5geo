#ifndef H5DEVCURVEIMPL_H
#define H5DEVCURVEIMPL_H

#include "../h5devcurve.h"
#include "h5baseobjectimpl.h"

class H5DevCurveImpl :
    public virtual H5DevCurve,
    public virtual H5BaseObjectImpl
{
protected:
  explicit H5DevCurveImpl(const h5gt::Group &group);
  virtual ~H5DevCurveImpl() = default;

public:
  virtual bool writeCurve(
      const h5geo::DevDataType& name,
      const Eigen::Ref<const Eigen::VectorXd>& v) override;
  virtual bool writeCurve(
      const std::string& name,
      const Eigen::Ref<const Eigen::VectorXd>& v) override;

  virtual bool setActive() override;
  virtual bool isActive() override;

  virtual size_t getNCurves() override;
  virtual size_t getNSamp() override;
  virtual Eigen::VectorXd getCurve(
      const h5geo::DevDataType& name) override;
  virtual Eigen::VectorXd getCurve(
      const std::string& name) override;

  virtual std::string getRelativeCurveName() override;

  virtual H5WellContainer* getWellContainer() override;
  virtual H5Well* getWell() override;

  virtual std::optional<h5gt::DataSet> getDevCurveD() override;

  //----------- FRIEND CLASSES -----------
  friend class H5WellImpl;
};

#endif // H5DEVCURVEIMPL_H
