#ifndef H5WELLCONTAINERIMPL_H
#define H5WELLCONTAINERIMPL_H

#include "../h5wellcontainer.h"
#include "h5basecontainerimpl.h"

class H5WellContainerImpl :
    public virtual H5WellContainer,
    public virtual H5BaseContainerImpl
{
protected:
  explicit H5WellContainerImpl(const h5gt::File &h5File);
  virtual ~H5WellContainerImpl() = default;

public:
  virtual H5Well* getWell(
      const std::string& name) override;
  virtual H5Well* getWell(
      h5gt::Group group) override;
  virtual H5Well* createWell(
      std::string& name,
      WellParam& p,
      h5geo::CreationType createFlag) override;
  virtual H5Well* createWell(
      h5gt::Group group,
      WellParam& p,
      h5geo::CreationType createFlag) override;

  virtual std::vector<H5Well*> getWellList() override;

  //----------- FRIEND CLASSES -----------
  friend H5WellContainer* h5geo::createWellContainer(
      h5gt::File h5File, h5geo::CreationType createFlag);
  friend H5WellContainer* h5geo::createWellContainerByName(
      std::string& fileName, h5geo::CreationType createFlag);
  friend H5WellContainer* h5geo::openWellContainer(
      h5gt::File h5File);
};

#endif // H5WELLCONTAINERIMPL_H
