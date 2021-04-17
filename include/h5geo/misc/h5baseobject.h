#ifndef H5BASEOBJECT_H
#define H5BASEOBJECT_H

#include "h5base.h"

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

#endif // H5BASEOBJECT_H
