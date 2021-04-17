#ifndef H5BASEOBJECTIMPL_H
#define H5BASEOBJECTIMPL_H

#include "h5baseobject.h"
#include "h5baseimpl.h"
#include "h5gt/H5Group.hpp"

class H5BaseObjectImpl :
    public virtual H5BaseObject,
    public virtual H5BaseImpl
{
protected:
  explicit H5BaseObjectImpl(const h5gt::Group &group);
  virtual ~H5BaseObjectImpl() = default;

public:
  virtual h5gt::File getH5File() const override;
  virtual h5gt::Group getObjG() const override;

  virtual std::optional<h5gt::Group>
  getGroupOpt(
      h5gt::Group& parent,
      const std::string& groupName) const override;

  virtual std::optional<h5gt::DataSet>
  getDatasetOpt(
      const h5gt::Group& parent,
      const std::string& datasetName) const override;

  virtual std::string getName() const override;
  virtual std::string getFullName() const override;

  virtual bool operator == (H5BaseObject& other) const override;
  virtual bool operator != (H5BaseObject& other) const override;

protected:
  /*!
   * \brief getParent Search for parent of type `ObjectType`
   *  for the `objG`
   * \param objType
   * \return
   */
  std::optional<h5gt::Group>
  getParentG(const h5geo::ObjectType& objType);

protected:
  h5gt::Group objG;
};

#endif // H5BASEOBJECTIMPL_H
