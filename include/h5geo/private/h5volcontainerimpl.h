#ifndef H5VOLCONTAINERIMPL_H
#define H5VOLCONTAINERIMPL_H

#include "../h5volcontainer.h"
#include "h5basecontainerimpl.h"

class H5VolContainerImpl : public H5BaseContainerImpl<H5VolContainer>
{
protected:
  explicit H5VolContainerImpl(const h5gt::File &h5File);
  virtual ~H5VolContainerImpl() = default;

public:
  virtual H5Vol* openVol(
      const std::string& name) override;
  virtual H5Vol* openVol(
      h5gt::Group group) override;
  virtual H5Vol* createVol(
      std::string& name,
      VolParam& p,
      h5geo::CreationType createFlag) override;
  virtual H5Vol* createVol(
      h5gt::Group group,
      VolParam& p,
      h5geo::CreationType createFlag) override;

  //----------- FRIEND CLASSES -----------
  friend class H5BaseContainerImpl<H5VolContainer>;
  friend H5VolContainer* h5geo::createVolContainer(
      h5gt::File h5File, h5geo::CreationType createFlag);
  friend H5VolContainer* h5geo::createVolContainerByName(
      std::string& fileName, h5geo::CreationType createFlag);
  friend H5VolContainer* h5geo::openVolContainer(
      h5gt::File h5File);
};

#endif // H5VOLCONTAINERIMPL_H
