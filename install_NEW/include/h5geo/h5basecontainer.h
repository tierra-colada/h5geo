#ifndef H5BASECONTAINER_H
#define H5BASECONTAINER_H

#include "misc/h5base.h"

class H5BaseContainer : public virtual H5Base
{
protected:
  virtual ~H5BaseContainer() = default;

public:
  virtual h5gt::File getH5File() const = 0;

  virtual bool operator == (H5BaseContainer& other) const = 0;
  virtual bool operator != (H5BaseContainer& other) const = 0;
};

using H5BaseCnt_ptr = std::unique_ptr<H5BaseContainer, h5geo::ObjectDeleter>;

#endif // H5BASECONTAINER_H
