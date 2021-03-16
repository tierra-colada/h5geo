#ifndef H5BASEOBJECT_H
#define H5BASEOBJECT_H

#include "h5base.h"

class H5GEO_EXPORT H5BaseObject : public H5Base
{
public:
  virtual ~H5BaseObject();
  h5gt::File getH5File();
  h5gt::Group getObjG();

  std::optional<h5gt::Group>
  getGroupOpt(
      h5gt::Group& parent,
      const std::string& groupName);

  std::optional<h5gt::DataSet>
  getDatasetOpt(
      h5gt::Group& parent,
      const std::string& datasetName);

  std::string getNameWithinParentGroup();
  std::string getFullName();

  bool operator == (H5BaseObject& other);
  bool operator != (H5BaseObject& other);

protected:
  explicit H5BaseObject(const h5gt::Group& group);

  /*!
   * \brief getParent Search for parent of type `ObjectType`
   *  for the `objG`
   * \param objType
   * \return
   */
  std::optional<h5gt::Group>
  getParent(const h5geo::ObjectType& objType);

protected:
  h5gt::Group objG;
};

#endif // H5BASEOBJECT_H
