#ifndef H5MAPIMPL_H
#define H5MAPIMPL_H

#include "../h5map.h"
#include "h5baseobjectimpl.h"

class H5MapImpl : public H5BaseObjectImpl<H5Map>
{
protected:
  explicit H5MapImpl(const h5gt::Group &group);
  virtual ~H5MapImpl() = default;

public:
  virtual bool writeData(
      Eigen::Ref<Eigen::MatrixXd> M,
      const std::string& dataUnits = "") override;

  virtual Eigen::MatrixXd getData(const std::string& dataUnits = "") override;

  virtual bool setDomain(const h5geo::Domain& domain) override;
  virtual bool setOrigin(
      Eigen::Ref<Eigen::Vector2d> v,
      const std::string& lengthUnits = "",
      bool doCoordTransform = false) override;
  virtual bool setPoint1(
      Eigen::Ref<Eigen::Vector2d> v,
      const std::string& lengthUnits = "",
      bool doCoordTransform = false) override;
  virtual bool setPoint2(
      Eigen::Ref<Eigen::Vector2d> v,
      const std::string& lengthUnits = "",
      bool doCoordTransform = false) override;

  virtual std::optional<h5gt::Group> addAttributeMap(H5Map* map, std::string name = "") override;
  virtual std::optional<h5gt::Group> addInternalAttributeMap(H5Map* map, std::string name = "") override;
  virtual std::optional<h5gt::Group> addExternalAttributeMap(H5Map* map, std::string name = "") override;
  virtual bool removeAttributeMap(const std::string& name) override;
  virtual H5Map* openAttributeMap(const std::string& name) override;
  virtual std::vector<h5gt::Group> getAttributeMapGroupList() override;
  virtual std::vector<std::string> getAttributeMapNameList() override;
  virtual size_t getAttributeMapCount() override;

  virtual h5geo::Domain getDomain() override;
  virtual Eigen::VectorXd getOrigin(
      const std::string& lengthUnits = "",
      bool doCoordTransform = false) override;
  virtual Eigen::VectorXd getPoint1(
      const std::string& lengthUnits = "",
      bool doCoordTransform = false) override;
  virtual Eigen::VectorXd getPoint2(
      const std::string& lengthUnits = "",
      bool doCoordTransform = false) override;
  virtual size_t getNX() override;
  virtual size_t getNY() override;

  virtual MapParam getParam() override;

  virtual H5MapContainer* openMapContainer() const override;

  virtual std::optional<h5gt::DataSet> getMapD() const override;

  //----------- FRIEND CLASSES -----------
  friend class H5MapContainerImpl;
  friend class H5BaseObjectImpl<H5Map>;
  friend H5Map* h5geo::openMap(h5gt::Group group);
};

#endif // H5MAPIMPL_H
