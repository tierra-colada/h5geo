#ifndef H5BASEOBJECT_H
#define H5BASEOBJECT_H

#include "h5base.h"

#ifdef H5GEO_USE_GDAL
class OGRCoordinateTransformation;
#endif

/// \class H5BaseObject
/// \brief Base class for geo-objects
class H5BaseObject : public H5Base
{
protected:
  virtual ~H5BaseObject() = default;

public:
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
      H5PointsParam& p,
      h5geo::CreationType createFlag) = 0;
  virtual H5Points1* createPoints1(
      h5gt::Group group,
      H5PointsParam& p,
      h5geo::CreationType createFlag) = 0;
  virtual H5Points2* createPoints2(
      std::string& name,
      H5PointsParam& p,
      h5geo::CreationType createFlag) = 0;
  virtual H5Points2* createPoints2(
      h5gt::Group group,
      H5PointsParam& p,
      h5geo::CreationType createFlag) = 0;
  virtual H5Points3* createPoints3(
      std::string& name,
      H5PointsParam& p,
      h5geo::CreationType createFlag) = 0;
  virtual H5Points3* createPoints3(
      h5gt::Group group,
      H5PointsParam& p,
      h5geo::CreationType createFlag) = 0;
  virtual H5Points4* createPoints4(
      std::string& name,
      H5PointsParam& p,
      h5geo::CreationType createFlag) = 0;
  virtual H5Points4* createPoints4(
      h5gt::Group group,
      H5PointsParam& p,
      h5geo::CreationType createFlag) = 0;

  virtual H5Horizon* createHorizon(
      std::string& name,
      H5HorizonParam& p,
      h5geo::CreationType createFlag) = 0;
  virtual H5Horizon* createHorizon(
      h5gt::Group group,
      H5HorizonParam& p,
      h5geo::CreationType createFlag) = 0;

#ifdef H5GEO_USE_GDAL
  /// \brief Create GDAL coordinate transformtation object
  ///
  /// Transformation is prepared to transform data from geo-object's CRS 
  /// to h5geo::sr::SpatialReference
  virtual OGRCoordinateTransformation* createCoordinateTransformationToReadData(
      const std::string& unitsTo) = 0;
  /// \brief Create GDAL coordinate transformtation object
  ///
  /// Transformation is prepared to transform data from h5geo::sr::SpatialReference
  /// to geo-object's CRS 
  virtual OGRCoordinateTransformation* createCoordinateTransformationToWriteData(
      const std::string& unitsFrom) = 0;
#endif

  /// \brief Set spatial reference for current geo-object using `authName:code` form
  virtual bool setSpatialReference(const std::string& str) = 0;
  /// \brief Set spatial reference for current geo-object
  virtual bool setSpatialReference(
      const std::string& authName, const std::string& code) = 0;
  /// \brief Set length units for the current geo-object
  virtual bool setLengthUnits(const std::string& str) = 0;
  /// \brief Set temporal units for the current geo-object
  virtual bool setTemporalUnits(const std::string& str) = 0;
  /// \brief Set angular units for the current geo-object
  virtual bool setAngularUnits(const std::string& str) = 0;
  /// \brief Set data units for the current geo-object
  virtual bool setDataUnits(const std::string& str) = 0;
  /// \brief Set NULL value for the current geo-object
  virtual bool setNullValue(double val) = 0;

 /// \brief Get spatial reference for current geo-object
  virtual std::string getSpatialReference() = 0;
  /// \brief Get length units for the current geo-object
  virtual std::string getLengthUnits() = 0;
  /// \brief Get temporal units for the current geo-object
  virtual std::string getTemporalUnits() = 0;
  /// \brief Get angular units for the current geo-object
  virtual std::string getAngularUnits() = 0;
  /// \brief Get data units for the current geo-object
  virtual std::string getDataUnits() = 0;
  /// \brief Get NULL value for the current geo-object
  virtual double getNullValue() = 0;

  /// \brief Get HDF5 file
  virtual h5gt::File getH5File() const = 0;
  /// \brief Get HDF5 Group
  virtual h5gt::Group getObjG() const = 0;

  /// \brief Get geo-object's name without path
  virtual std::string getName() const = 0;
  /// \brief Get geo-object's name with full path to that object
  virtual std::string getFullName() const = 0;

  /// \brief Find all geo-objects of specified type within current geo-object and return them as vector of Groups
  virtual std::vector<h5gt::Group> getObjGroupList(const h5geo::ObjectType& objType, bool recursive) = 0;
  /// \brief Find all geo-objects of specified type within current geo-object and return them as vector of names
  virtual std::vector<std::string> getObjNameList(const h5geo::ObjectType& objType, bool recursive) = 0;
  /// \brief Get number of geo-objects of specified type within current geo-object
  virtual size_t getObjCount(const h5geo::ObjectType& objType, bool recursive) = 0;

  /// \brief Check if geo-objects are the same
	///
  /// Same as operator `==`
  virtual bool isEqual(H5BaseObject* other) const = 0;

  /// \brief Check if geo-objects are the same (compares HDF5 Groups)
  virtual bool operator == (const H5BaseObject& other) const = 0;
  /// \brief Check if geo-objects are not the same (compares HDF5 Groups)
  virtual bool operator != (const H5BaseObject& other) const = 0;
};

using H5BaseObject_ptr = std::unique_ptr<H5BaseObject, h5geo::ObjectDeleter>;

#endif // H5BASEOBJECT_H
