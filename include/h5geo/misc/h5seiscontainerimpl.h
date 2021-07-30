#ifndef H5SEISCONTAINERIMPL_H
#define H5SEISCONTAINERIMPL_H

#include "../h5seiscontainer.h"
#include "h5basecontainerimpl.h"

#include <memory>

class H5SeisContainerImpl :
    public virtual H5SeisContainer,
    public virtual H5BaseContainerImpl
{
protected:
  explicit H5SeisContainerImpl(const h5gt::File &h5File);
  virtual ~H5SeisContainerImpl() = default;

public:
  virtual H5Seis* getSeis(
      const std::string& name) override;
  virtual H5Seis* getSeis(
      h5gt::Group group) override;
  virtual H5Seis* createSeis(
      std::string& name,
      SeisParam& p,
      h5geo::CreationType createFlag) override;
  virtual H5Seis* createSeis(
      h5gt::Group group,
      SeisParam& p,
      h5geo::CreationType createFlag) override;

  virtual std::vector<H5Seis*> getSeisList() override;

  //----------- FRIEND CLASSES -----------
  friend H5SeisContainer* h5geo::createSeisContainer(
      h5gt::File h5File, h5geo::CreationType createFlag);
  friend H5SeisContainer* h5geo::createSeisContainerByName(
      std::string& fileName, h5geo::CreationType createFlag);
  friend H5SeisContainer* h5geo::openSeisContainer(
      h5gt::File h5File);
  friend H5SeisContainer* h5geo::openSeisContainerByName(
      std::string& fileName);
};

#endif // H5SEISCONTAINERIMPL_H
