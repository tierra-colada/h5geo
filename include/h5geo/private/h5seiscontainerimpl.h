#ifndef H5SEISCONTAINERIMPL_H
#define H5SEISCONTAINERIMPL_H

#include "../h5seiscontainer.h"
#include "h5basecontainerimpl.h"

class H5SeisContainerImpl : public H5BaseContainerImpl<H5SeisContainer>
{
protected:
  explicit H5SeisContainerImpl(const h5gt::File &h5File);
  virtual ~H5SeisContainerImpl() = default;

public:
  virtual H5Seis* openSeis(
      const std::string& name) override;
  virtual H5Seis* openSeis(
      h5gt::Group group) override;
  virtual H5Seis* createSeis(
      std::string& name,
      H5SeisParam& p,
      h5geo::CreationType createFlag) override;
  virtual H5Seis* createSeis(
      h5gt::Group group,
      H5SeisParam& p,
      h5geo::CreationType createFlag) override;

  //----------- FRIEND CLASSES -----------
  friend class H5BaseContainerImpl<H5SeisContainer>;
  friend H5SeisContainer* h5geo::createSeisContainer(
      h5gt::File h5File, h5geo::CreationType createFlag);
  friend H5SeisContainer* h5geo::createSeisContainerByName(
      std::string& fileName, h5geo::CreationType createFlag);
  friend H5SeisContainer* h5geo::openSeisContainer(
      h5gt::File h5File);
};

#endif // H5SEISCONTAINERIMPL_H
