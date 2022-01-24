#ifndef H5MAP_H
#define H5MAP_H

#include "h5baseobject.h"

#include <Eigen/Dense>

class H5MapContainer;

class H5Map : public H5BaseObject
{
protected:
  virtual ~H5Map() = default;

public:
  virtual bool writeData(
      Eigen::Ref<Eigen::MatrixXd> M,
      const std::string& dataUnits = "") = 0;

  virtual Eigen::MatrixXd getData(const std::string& dataUnits = "") = 0;

  virtual bool setDomain(const h5geo::Domain& domain) = 0;
  virtual bool setOrigin(
      Eigen::Ref<Eigen::Vector2d> v,
      const std::string& lengthUnits = "",
      bool doCoordTransform = false) = 0;
  virtual bool setPoint1(
      Eigen::Ref<Eigen::Vector2d> v,
      const std::string& lengthUnits = "",
      bool doCoordTransform = false) = 0;
  virtual bool setPoint2(
      Eigen::Ref<Eigen::Vector2d> v,
      const std::string& lengthUnits = "",
      bool doCoordTransform = false) = 0;

  virtual bool addAttribute(H5Map* map, std::string name = "") = 0;
  virtual bool addExternalAttribute(H5Map* map, std::string name = "") = 0;
  virtual bool removeAttribute(const std::string& name) = 0;
  virtual H5Map* getAttribute(const std::string& name) = 0;

  virtual h5geo::Domain getDomain() = 0;
  virtual Eigen::VectorXd getOrigin(
      const std::string& lengthUnits = "",
      bool doCoordTransform = false) = 0;
  virtual Eigen::VectorXd getPoint1(
      const std::string& lengthUnits = "",
      bool doCoordTransform = false) = 0;
  virtual Eigen::VectorXd getPoint2(
      const std::string& lengthUnits = "",
      bool doCoordTransform = false) = 0;

  virtual H5MapContainer* openMapContainer() const = 0;

  virtual std::optional<h5gt::DataSet> getMapD() const = 0;
};

namespace h5geo {
  extern "C" {
  H5GEO_EXPORT H5Map* openMap(h5gt::Group group);
  }
}

using H5Map_ptr = std::unique_ptr<H5Map, h5geo::ObjectDeleter>;

#endif // H5MAP_H
