#ifndef H5BASECONTAINER_H
#define H5BASECONTAINER_H

#include "h5base.h"

/// \class H5BaseContainer
/// \brief Base class for geo-containers
class H5BaseContainer : public H5Base
{
protected:
  virtual ~H5BaseContainer() = default;

public:
  /// \brief Open geo-object
  ///
  /// Check if HDF5 object belongs to any H5BaseObject derived type
  /// and create appropriate object. \n
  /// Use dynamic_cast<> to cast it to the correct type.
  virtual H5BaseObject* openObject(
      const std::string& name) = 0;
  /// \brief Open geo-object
  ///
  /// Check if HDF5 object belongs to any H5BaseObject derived type
  /// and create appropriate object. \n
  /// Use dynamic_cast<> to cast it to the correct type.
  virtual H5BaseObject* openObject(
      h5gt::Group group) = 0;

	/// \brief Open H5BasePoints derived points
  virtual H5BasePoints* openPoints(
      const std::string& name) = 0;
	/// \brief Open H5BasePoints derived points
  virtual H5BasePoints* openPoints(
      h5gt::Group group) = 0;

  virtual H5Horizon* openHorizon(
      const std::string& name) = 0;
  virtual H5Horizon* openHorizon(
      h5gt::Group group) = 0;

  virtual H5Points1* createPoints1(
      std::string& name,
      PointsParam& p,
      h5geo::CreationType createFlag) = 0;
  virtual H5Points1* createPoints1(
      h5gt::Group group,
      PointsParam& p,
      h5geo::CreationType createFlag) = 0;
  virtual H5Points2* createPoints2(
      std::string& name,
      PointsParam& p,
      h5geo::CreationType createFlag) = 0;
  virtual H5Points2* createPoints2(
      h5gt::Group group,
      PointsParam& p,
      h5geo::CreationType createFlag) = 0;
  virtual H5Points3* createPoints3(
      std::string& name,
      PointsParam& p,
      h5geo::CreationType createFlag) = 0;
  virtual H5Points3* createPoints3(
      h5gt::Group group,
      PointsParam& p,
      h5geo::CreationType createFlag) = 0;
  virtual H5Points4* createPoints4(
      std::string& name,
      PointsParam& p,
      h5geo::CreationType createFlag) = 0;
  virtual H5Points4* createPoints4(
      h5gt::Group group,
      PointsParam& p,
      h5geo::CreationType createFlag) = 0;

  virtual H5Horizon* createHorizon(
      std::string& name,
      HorizonParam& p,
      h5geo::CreationType createFlag) = 0;
  virtual H5Horizon* createHorizon(
      h5gt::Group group,
      HorizonParam& p,
      h5geo::CreationType createFlag) = 0;

	/// \brief Get HDF5 file
  virtual h5gt::File getH5File() const = 0;

	/// \brief Find all geo-objects of specified type within current container and return them as vector of Groups
  virtual std::vector<h5gt::Group> getObjGroupList(const h5geo::ObjectType& objType, bool recursive) = 0;
	/// \brief Find all geo-objects of specified type within current container and return them as vector of names
  virtual std::vector<std::string> getObjNameList(const h5geo::ObjectType& objType, bool recursive) = 0;
	/// \brief Get number of geo-objects of specified type within current container
  virtual size_t getObjCount(const h5geo::ObjectType& objType, bool recursive) = 0;
	/// \brief Get current container type
  virtual h5geo::ContainerType getContainerType() = 0;

	/// \brief Check if containers are the same
	///
  /// Same as operator `==`
  virtual bool isEqual(H5BaseContainer* other) const = 0;

	/// \brief Check if containers are the same (compares HDF5 files)
  virtual bool operator == (const H5BaseContainer& other) const = 0;
	/// \brief Check if containers are not the same (compares HDF5 files)
  virtual bool operator != (const H5BaseContainer& other) const = 0;
};

using H5BaseCnt_ptr = std::unique_ptr<H5BaseContainer, h5geo::ObjectDeleter>;

#endif // H5BASECONTAINER_H
