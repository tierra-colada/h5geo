#ifndef H5BASEOBJECT_H
#define H5BASEOBJECT_H

#include "misc/h5base.h"

#ifdef H5GEO_USE_GDAL
class OGRCoordinateTransformation;
#endif

class H5BaseObject : public virtual H5Base
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
  virtual bool setSpatialUnits(const std::string& str) = 0;
  virtual bool setTemporalUnits(const std::string& str) = 0;
  virtual bool setAngularUnits(const std::string& str) = 0;
  virtual bool setDataUnits(const std::string& str) = 0;

  virtual std::string getSpatialReference() = 0;
  virtual std::string getSpatialUnits() = 0;
  virtual std::string getTemporalUnits() = 0;
  virtual std::string getAngularUnits() = 0;
  virtual std::string getDataUnits() = 0;

  virtual h5gt::File getH5File() const = 0;
  virtual h5gt::Group getObjG() const = 0;

  virtual std::optional<h5gt::Group>
  getGroupOpt(
      h5gt::Group& parent,
      const std::string& groupName) const = 0;

  virtual std::optional<h5gt::DataSet>
  getDatasetOpt(
      const h5gt::Group& parent,
      const std::string& datasetName) const = 0;

  virtual std::string getName() const = 0;
  virtual std::string getFullName() const = 0;

  virtual bool operator == (H5BaseObject& other) const = 0;
  virtual bool operator != (H5BaseObject& other) const = 0;
};

namespace h5geo {
  extern "C" {
  H5GEO_EXPORT H5BaseObject* openBaseObject(h5gt::Group group);
  }
}

using H5BaseObject_ptr = std::unique_ptr<H5BaseObject, h5geo::ObjectDeleter>;

#endif // H5BASEOBJECT_H
