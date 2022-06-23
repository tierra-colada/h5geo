#ifndef H5MAP_H
#define H5MAP_H

#include "h5baseobject.h"

#include <Eigen/Dense>

class H5MapContainer;

/// \class H5Map
/// \brief Provides API to work with maps
///
/// Map is an object that is represented by a matrix with coordinates 
/// of upper-left matrix corner (origin X0, Y0), 
/// coordinates of upper-right matrix corner (point1 X1, Y1) and 
/// coordinates of lower-left matrix corner (point2 X2, Y2). 
/// Thus only single Z value corresponds for each XY point.
class H5Map : public H5BaseObject
{
protected:
  virtual ~H5Map() = default;

public:
  /// \brief Write data to DataSet
  virtual bool writeData(
      Eigen::Ref<Eigen::MatrixXd> M,
      const std::string& dataUnits = "") = 0;

	/// \brief WrReadite data from DataSet
  virtual Eigen::MatrixXd getData(const std::string& dataUnits = "") = 0;

	/// \brief Set domain for the map (time, depth etc)
  virtual bool setDomain(const h5geo::Domain& domain) = 0;
	/// \brief Set coordinates of upper-left matrix corner
  virtual bool setOrigin(
      Eigen::Ref<Eigen::Vector2d> v,
      const std::string& lengthUnits = "",
      bool doCoordTransform = false) = 0;
	/// \brief Set coordinates of upper-right matrix corner
  virtual bool setPoint1(
      Eigen::Ref<Eigen::Vector2d> v,
      const std::string& lengthUnits = "",
      bool doCoordTransform = false) = 0;
	/// \brief Set coordinates of bottom-left matrix corner
  virtual bool setPoint2(
      Eigen::Ref<Eigen::Vector2d> v,
      const std::string& lengthUnits = "",
      bool doCoordTransform = false) = 0;

  /// \brief Add attribute map
	///
	/// Check if current map and attribute map reside in the
  /// same hdf5 container or not (create 'Internal' or 'External' attribute map). \n
  /// The nX and nY must be equal. \n
	/// Attribute map is a regular H5Map but resides within parent map.
  /// \param map map to be used as attribute
  /// \param name name for an attribute map
  /// \return Group of newly created attribute map
  virtual std::optional<h5gt::Group> addAttributeMap(H5Map* map, std::string name = "") = 0;
	/// \brief Add internal attribute map
	/// 
  /// Both map and attribute map reside in the same HDF5 container. \n
	/// Created attribute map is simply HDF5 `SOFT` link.
	/// \param map map to be used as attribute
  /// \param name name for an attribute map
  /// \return Group of newly created attribute map
  virtual std::optional<h5gt::Group> addInternalAttributeMap(H5Map* map, std::string name = "") = 0;
  /// \brief Add internal attribute map
	/// 
  /// Both map and attribute map reside in different HDF5 containers. \n
	/// Created attribute map is simply HDF5 `EXTERNAL` link.
	/// \param map map to be used as attribute
  /// \param name name for an attribute map
  /// \return Group of newly created attribute map
  virtual std::optional<h5gt::Group> addExternalAttributeMap(H5Map* map, std::string name = "") = 0;
	/// \brief Remove attribute map
  virtual bool removeAttributeMap(const std::string& name) = 0;
	/// \brief Open attribute map
  virtual H5Map* openAttributeMap(const std::string& name) = 0;
	/// \brief Find and return vector of attribute maps Groups within current map
  virtual std::vector<h5gt::Group> getAttributeMapGroupList() = 0;
	/// \brief Find and return vector of attribute maps names within current map
  virtual std::vector<std::string> getAttributeMapNameList() = 0;
	/// \brief Get number of atribute maps within current map
  virtual size_t getAttributeMapCount() = 0;

	/// \brief Get domain (time, depth etc)
  virtual h5geo::Domain getDomain() = 0;
	/// \brief Get coordinates of upper-left matrix corner
  virtual Eigen::VectorXd getOrigin(
      const std::string& lengthUnits = "",
      bool doCoordTransform = false) = 0;
	/// \brief Get coordinates of upper-right matrix corner
  virtual Eigen::VectorXd getPoint1(
      const std::string& lengthUnits = "",
      bool doCoordTransform = false) = 0;
	/// \brief Get coordinates of bottom-left matrix corner
  virtual Eigen::VectorXd getPoint2(
      const std::string& lengthUnits = "",
      bool doCoordTransform = false) = 0;
	/// \brief Get number of Eigen column-major matrix columns
  virtual size_t getNX() = 0;
	/// \brief Get number of Eigen column-major matrix rows
  virtual size_t getNY() = 0;

	/// \brief Get parameters that were used to create current map
  virtual MapParam getParam() = 0;

	/// \brief Open H5MapContainer where current map resides
  virtual H5MapContainer* openMapContainer() const = 0;

	/// \brief Get current map's DataSet
  virtual std::optional<h5gt::DataSet> getMapD() const = 0;
};

using H5Map_ptr = std::unique_ptr<H5Map, h5geo::ObjectDeleter>;

#endif // H5MAP_H
