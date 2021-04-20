#ifndef H5BASEOBJECT_H
#define H5BASEOBJECT_H

#include "misc/h5base.h"

#include <string>

class H5BaseObject : public virtual H5Base
{
protected:
  virtual ~H5BaseObject() = default;

public:
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
  H5GEO_EXPORT H5BaseObject* createBaseObject(h5gt::Group &group);
  }
}

using H5BaseObject_ptr = std::unique_ptr<H5BaseObject, h5geo::ObjectDeleter>;

#endif // H5BASEOBJECT_H
