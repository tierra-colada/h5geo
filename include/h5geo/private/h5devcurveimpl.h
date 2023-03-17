#ifndef H5DEVCURVEIMPL_H
#define H5DEVCURVEIMPL_H

#include "../h5devcurve.h"
#include "h5baseobjectimpl.h"

class H5DevCurveImpl : public H5BaseObjectImpl<H5DevCurve>
{
protected:
  explicit H5DevCurveImpl(const h5gt::Group &group);
  virtual ~H5DevCurveImpl() = default;

public:
  virtual bool writeMD(
      Eigen::Ref<Eigen::VectorXd> v,
      const std::string& units = "") override;
  virtual bool writeAZIM(
      Eigen::Ref<Eigen::VectorXd> v,
      const std::string& units = "") override;
  virtual bool writeINCL(
      Eigen::Ref<Eigen::VectorXd> v,
      const std::string& units = "") override;
  virtual bool writeTVD(
      Eigen::Ref<Eigen::VectorXd> v,
      const std::string& units = "") override;
  virtual bool writeDX(
      Eigen::Ref<Eigen::VectorXd> v,
      const std::string& units = "") override;
  virtual bool writeDY(
      Eigen::Ref<Eigen::VectorXd> v,
      const std::string& units = "") override;
  virtual bool writeOWT(
      Eigen::Ref<Eigen::VectorXd> v,
      const std::string& units = "") override;

  virtual bool setActive() override;
  virtual bool isActive() override;

  virtual void updateMdAzimIncl() override;
  virtual void updateTvdDxDy() override;

  virtual size_t getNCurves() override;
  virtual size_t getNSamp() override;
  virtual Eigen::VectorXd getCurve(
      const h5geo::DevDataType& name,
      const std::string& units = "",
      bool doCoordTransform = false) override;
  virtual Eigen::VectorXd getCurve(
      const std::string& name,
      const std::string& units = "",
      bool doCoordTransform = false) override;

  virtual std::string getRelativeName() override;

  virtual H5DevCurveParam getParam() override;

  virtual H5WellContainer* openWellContainer() override;
  virtual H5Well* openWell() override;

  virtual std::optional<h5gt::DataSet> getDevCurveD() override;

  //----------- FRIEND CLASSES -----------
  friend class H5WellImpl;
  friend class H5BaseObjectImpl<H5DevCurve>;
  friend H5DevCurve* h5geo::openDevCurve(h5gt::Group group);

protected:
  bool writeCurve(
      const h5geo::DevDataType& name,
      Eigen::Ref<Eigen::VectorXd> v,
      const std::string& units = "");
  bool writeCurve(
      const std::string& name,
      Eigen::Ref<Eigen::VectorXd> v,
      const std::string& units = "");
};

#endif // H5DEVCURVEIMPL_H
