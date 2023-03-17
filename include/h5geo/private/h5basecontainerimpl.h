#ifndef H5BASECONTAINERIMPL_H
#define H5BASECONTAINERIMPL_H

#include "../h5basecontainer.h"
#include "h5baseimpl.h"

#include <h5gt/H5File.hpp>

template <typename TBase = H5BaseContainer>
class H5BaseContainerImpl : public H5BaseImpl<TBase>
{
protected:
  explicit H5BaseContainerImpl(const h5gt::File &h5File);
  virtual ~H5BaseContainerImpl() = default;

public:
  virtual H5Base* clone() override;

  virtual H5BaseObject* openObject(
      const std::string& name) override;
  virtual H5BaseObject* openObject(
      h5gt::Group group) override;

  virtual H5BasePoints* openPoints(
      const std::string& name) override;
  virtual H5BasePoints* openPoints(
      h5gt::Group group) override;

  virtual H5Horizon* openHorizon(
      const std::string& name) override;
  virtual H5Horizon* openHorizon(
      h5gt::Group group) override;

  virtual H5Points1* createPoints1(
      std::string& name,
      H5PointsParam& p,
      h5geo::CreationType createFlag) override;
  virtual H5Points1* createPoints1(
      h5gt::Group group,
      H5PointsParam& p,
      h5geo::CreationType createFlag) override;
  virtual H5Points2* createPoints2(
      std::string& name,
      H5PointsParam& p,
      h5geo::CreationType createFlag) override;
  virtual H5Points2* createPoints2(
      h5gt::Group group,
      H5PointsParam& p,
      h5geo::CreationType createFlag) override;
  virtual H5Points3* createPoints3(
      std::string& name,
      H5PointsParam& p,
      h5geo::CreationType createFlag) override;
  virtual H5Points3* createPoints3(
      h5gt::Group group,
      H5PointsParam& p,
      h5geo::CreationType createFlag) override;
  virtual H5Points4* createPoints4(
      std::string& name,
      H5PointsParam& p,
      h5geo::CreationType createFlag) override;
  virtual H5Points4* createPoints4(
      h5gt::Group group,
      H5PointsParam& p,
      h5geo::CreationType createFlag) override;

  virtual H5Horizon* createHorizon(
      std::string& name,
      H5HorizonParam& p,
      h5geo::CreationType createFlag) override;
  virtual H5Horizon* createHorizon(
      h5gt::Group group,
      H5HorizonParam& p,
      h5geo::CreationType createFlag) override;

  virtual h5gt::File getH5File() const override;

  virtual std::vector<h5gt::Group> getObjGroupList(const h5geo::ObjectType& objType, bool recursive) override;
  virtual std::vector<std::string> getObjNameList(const h5geo::ObjectType& objType, bool recursive) override;
  virtual size_t getObjCount(const h5geo::ObjectType& objType, bool recursive) override;
  virtual h5geo::ContainerType getContainerType() override;

  /// Does the same as operator '=='
  virtual bool isEqual(H5BaseContainer* other) const override;

  virtual bool operator == (const H5BaseContainer& other) const override;
  virtual bool operator != (const H5BaseContainer& other) const override;

  //----------- FRIEND CLASSES -----------
  friend H5BaseContainer* h5geo::openBaseContainer(
      h5gt::File h5File);
  friend H5BaseContainer* h5geo::openBaseContainerByName(
      const std::string& fileName);
  friend H5BaseContainer* h5geo::openContainer(h5gt::File h5File);

protected:
  h5gt::File h5File;
};

#endif // H5BASECONTAINERIMPL_H
