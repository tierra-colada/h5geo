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
  virtual h5gt::File getH5File() const override;

  virtual bool operator == (H5BaseContainer& other) const override;
  virtual bool operator != (H5BaseContainer& other) const override;

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
