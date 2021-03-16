#ifndef H5SEISCONTAINER_H
#define H5SEISCONTAINER_H

#include "../h5geo_export.h"
#include "../h5base/h5basecontainer.h"
#include "h5seis.h"

#include <string>

class H5GEO_EXPORT H5SeisContainer : public H5BaseContainer
{
private:
  explicit H5SeisContainer(const h5gt::File &h5File);

public:
  virtual ~H5SeisContainer() override;

  static std::optional<H5SeisContainer>
  create(h5gt::File &h5File);
  static std::optional<H5SeisContainer>
  create(const std::string& fileName,
         const unsigned& openFlags =
      h5gt::File::ReadWrite |
      h5gt::File::OpenOrCreate);

  std::optional<H5Seis> getSeis(
      const std::string& name);
  std::optional<H5Seis> createSeis(
      h5gt::Group& group,
      SeisParam p,
      const h5geo::CreationType& createFlag =
      h5geo::CreationType::CREATE_OR_OVERWRITE);
  std::optional<H5Seis> createSeis(
      std::string name,
      SeisParam p,
      const h5geo::CreationType& createFlag =
      h5geo::CreationType::CREATE_OR_OVERWRITE);

  std::vector<H5Seis> getSeisList();
};

#endif // H5SEISCONTAINER_H
