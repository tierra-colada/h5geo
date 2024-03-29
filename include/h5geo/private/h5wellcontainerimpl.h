#ifndef H5WELLCONTAINERIMPL_H
#define H5WELLCONTAINERIMPL_H

#include "../h5wellcontainer.h"
#include "h5basecontainerimpl.h"

class H5WellContainerImpl : public H5BaseContainerImpl<H5WellContainer>
{
protected:
  explicit H5WellContainerImpl(const h5gt::File &h5File);
  virtual ~H5WellContainerImpl() = default;

public:
  virtual H5Well* openWell(
      const std::string& name) override;
  virtual H5Well* openWell(
      h5gt::Group group) override;
  virtual H5Well* createWell(
      std::string& name,
      H5WellParam& p,
      h5geo::CreationType createFlag) override;
  virtual H5Well* createWell(
      h5gt::Group group,
      H5WellParam& p,
      h5geo::CreationType createFlag) override;

  virtual H5Well* openWellByUWI(
      const std::string& name) override;

  //----------- FRIEND CLASSES -----------
  friend class H5BaseContainerImpl<H5WellContainer>;
  friend H5WellContainer* h5geo::createWellContainer(
      h5gt::File h5File, h5geo::CreationType createFlag);
  friend H5WellContainer* h5geo::createWellContainerByName(
      std::string& fileName, h5geo::CreationType createFlag);
  friend H5WellContainer* h5geo::openWellContainer(
      h5gt::File h5File);
};

#endif // H5WELLCONTAINERIMPL_H
