#ifndef H5BASEOBJECT_H
#define H5BASEOBJECT_H

#include "h5base.h"

#ifdef H5GEO_USE_GDAL
class OGRCoordinateTransformation;
#endif

class H5BaseObject : public H5Base
{
protected:
  virtual ~H5BaseObject() = default;

public:
#ifdef H5GEO_USE_GDAL
  virtual OGRCoordinateTransformation* createCoordinateTransformationToReadData(
      const std::string& unitsTo)  = 0;
  virtual OGRCoordinateTransformation* createCoordinateTransformationToWriteData(
      const std::string& unitsFrom)  = 0;
#endif

  virtual bool setSpatialReference(const std::string& str) = 0;
  virtual bool setSpatialReference(
      const std::string& authName, const std::string& code) = 0;
  virtual bool setLengthUnits(const std::string& str) = 0;
  virtual bool setTemporalUnits(const std::string& str) = 0;
  virtual bool setAngularUnits(const std::string& str) = 0;
  virtual bool setDataUnits(const std::string& str) = 0;
  virtual bool setNullValue(double val) = 0;

  virtual std::string getSpatialReference() = 0;
  virtual std::string getLengthUnits() = 0;
  virtual std::string getTemporalUnits() = 0;
  virtual std::string getAngularUnits() = 0;
  virtual std::string getDataUnits() = 0;
  virtual double getNullValue() = 0;

  virtual h5gt::File getH5File() const = 0;
  virtual h5gt::Group getObjG() const = 0;

  virtual std::string getName() const = 0;
  virtual std::string getFullName() const = 0;

  virtual std::vector<h5gt::Group> getObjGroupList(const h5geo::ObjectType& objType, bool recursive) = 0;
  virtual std::vector<std::string> getObjNameList(const h5geo::ObjectType& objType, bool recursive) = 0;
  virtual size_t getObjCount(const h5geo::ObjectType& objType, bool recursive) = 0;

  /// Does the same as operator '=='
  virtual bool isEqual(H5BaseObject* other) const = 0;

  virtual bool operator == (const H5BaseObject& other) const = 0;
  virtual bool operator != (const H5BaseObject& other) const = 0;
};

namespace h5geo {
  extern "C" {
  H5GEO_EXPORT H5BaseObject* openBaseObject(h5gt::Group group);
  }
}

using H5BaseObject_ptr = std::unique_ptr<H5BaseObject, h5geo::ObjectDeleter>;

#endif // H5BASEOBJECT_H
