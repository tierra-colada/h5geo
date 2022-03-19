#ifndef H5BASECONTAINER_H
#define H5BASECONTAINER_H

#include "impl/h5base.h"

class H5BaseContainer : public H5Base
{
protected:
  virtual ~H5BaseContainer() = default;

public:
  virtual H5Points* openPoints(
      const std::string& name) = 0;
  virtual H5Points* openPoints(
      h5gt::Group group) = 0;
  virtual H5Points* createPoints(
      std::string& name,
      PointsParam& p,
      h5geo::CreationType createFlag) = 0;
  virtual H5Points* createPoints(
      h5gt::Group group,
      PointsParam& p,
      h5geo::CreationType createFlag) = 0;

  virtual h5gt::File getH5File() const = 0;

  virtual std::vector<h5gt::Group> getObjGroupList(const h5geo::ObjectType& objType, bool recursive) = 0;
  virtual std::vector<std::string> getObjNameList(const h5geo::ObjectType& objType, bool recursive) = 0;
  virtual size_t getObjCount(const h5geo::ObjectType& objType, bool recursive) = 0;
  virtual h5geo::ContainerType getContainerType() = 0;

  /// Does the same as operator '=='
  virtual bool isEqual(H5BaseContainer* other) const = 0;

  virtual bool operator == (const H5BaseContainer& other) const = 0;
  virtual bool operator != (const H5BaseContainer& other) const = 0;
};

using H5BaseCnt_ptr = std::unique_ptr<H5BaseContainer, h5geo::ObjectDeleter>;

#endif // H5BASECONTAINER_H
