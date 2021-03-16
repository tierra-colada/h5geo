#ifndef H5WELLCONTAINER_H
#define H5WELLCONTAINER_H

#include "h5well.h"
#include "../h5base/h5basecontainer.h"

class H5GEO_EXPORT H5WellContainer : public H5BaseContainer
{
private:
  explicit H5WellContainer(const h5gt::File &h5File);

public:
  virtual ~H5WellContainer() override;

  static std::optional<H5WellContainer>
  create(h5gt::File &h5File);
  static std::optional<H5WellContainer>
  create(const std::string& fileName,
         const size_t& openFlags =
      h5gt::File::ReadWrite |
      h5gt::File::OpenOrCreate);

  std::optional<H5Well> getWell(
      const std::string& name);
  std::optional<H5Well> createWell(
      h5gt::Group& group,
      WellParam p,
      const h5geo::CreationType &createFlag =
      h5geo::CreationType::CREATE_OR_OVERWRITE);
  std::optional<H5Well> createWell(
      std::string& name,
      WellParam p,
      const h5geo::CreationType &createFlag =
      h5geo::CreationType::CREATE_OR_OVERWRITE);

  std::vector<H5Well> getWellList();
};

#endif // H5WELLCONTAINER_H
