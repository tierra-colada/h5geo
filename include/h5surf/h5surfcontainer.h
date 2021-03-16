#ifndef H5SURFCONTAINER_H
#define H5SURFCONTAINER_H

#include "h5surf.h"
#include "../h5base/h5basecontainer.h"

class H5GEO_EXPORT H5SurfContainer : public H5BaseContainer
{
protected:
  explicit H5SurfContainer(const h5gt::File &h5File);

public:
  virtual ~H5SurfContainer() override;

  static std::optional<H5SurfContainer>
  create(h5gt::File &h5File);
  static std::optional<H5SurfContainer>
  create(const std::string& fileName,
         const unsigned& openFlags =
      h5gt::File::ReadWrite |
      h5gt::File::OpenOrCreate);

  std::optional<H5Surf> getSurf(
      const std::string& name);
  std::optional<H5Surf> createSurf(
      h5gt::Group& group,
      SurfParam p,
      const h5geo::CreationType& createFlag =
      h5geo::CreationType::CREATE_OR_OVERWRITE);
  std::optional<H5Surf> createSurf(
      std::string& name,
      SurfParam p,
      const h5geo::CreationType& createFlag =
      h5geo::CreationType::CREATE_OR_OVERWRITE);

  std::vector<H5Surf> getSurfList();
};

#endif // H5SURFCONTAINER_H
