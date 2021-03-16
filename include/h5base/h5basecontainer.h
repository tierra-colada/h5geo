#ifndef H5BASECONTAINER_H
#define H5BASECONTAINER_H

#include "h5base.h"

class H5BaseObject;

class H5GEO_EXPORT H5BaseContainer : public H5Base
{
public:
  virtual ~H5BaseContainer();
  h5gt::File getH5File();

  bool operator == (H5BaseContainer& other);
  bool operator != (H5BaseContainer& other);

protected:
  explicit H5BaseContainer(const h5gt::File &h5File);

  std::optional<h5gt::File>
  static createContainer(
      h5gt::File &h5File,
      const h5geo::ContainerType& containerType);

  static std::optional<h5gt::File>
  createContainer(
      const std::string& filename,
      const h5geo::ContainerType& containerType,
      const unsigned& openFlags =
      h5gt::File::OpenOrCreate,
      const h5gt::FileAccessProps& fileAccessProps =
      h5gt::FileDriver());

protected:
  h5gt::File h5File;
};

#endif // H5BASECONTAINER_H
