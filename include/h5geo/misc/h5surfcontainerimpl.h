#ifndef H5SURFCONTAINERIMPL_H
#define H5SURFCONTAINERIMPL_H

#include "../h5surfcontainer.h"
#include "h5basecontainerimpl.h"

class H5SurfContainerImpl :
    public virtual H5SurfContainer,
    public virtual H5BaseContainerImpl
{
protected:
  explicit H5SurfContainerImpl(const h5gt::File &h5File);
  virtual ~H5SurfContainerImpl() = default;

public:
  virtual H5Surf* getSurf(
      const std::string& name) override;
  virtual H5Surf* getSurf(
      h5gt::Group group) override;
  virtual H5Surf* createSurf(
      std::string& name,
      SurfParam &p,
      h5geo::CreationType createFlag) override;
  virtual H5Surf* createSurf(
      h5gt::Group group,
      SurfParam &p,
      h5geo::CreationType createFlag) override;

  virtual std::vector<H5Surf*> getSurfList() override;

  //----------- FRIEND CLASSES -----------
  friend H5SurfContainer* h5geo::createSurfContainer(
      h5gt::File h5File, h5geo::CreationType createFlag);
  friend H5SurfContainer* h5geo::createSurfContainerByName(
      std::string& fileName, h5geo::CreationType createFlag);
  friend H5SurfContainer* h5geo::openSurfContainer(
      h5gt::File h5File);
  friend H5SurfContainer* h5geo::openSurfContainerByName(
      std::string& fileName);
};

#endif // H5SURFCONTAINERIMPL_H
