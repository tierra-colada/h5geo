#ifndef H5SEISCONTAINER_H
#define H5SEISCONTAINER_H

#include "misc/h5geo_export.h"
#include "h5basecontainer.h"

#include <memory>

class H5Seis;

class H5SeisContainer : public H5BaseContainer
{
protected:
  virtual ~H5SeisContainer() = default;

public:
  virtual H5Seis* openSeis(
      const std::string& name) = 0;
  virtual H5Seis* openSeis(
      h5gt::Group group) = 0;
  virtual H5Seis* createSeis(
      std::string& name,
      SeisParam& p,
      h5geo::CreationType createFlag) = 0;
  virtual H5Seis* createSeis(
      h5gt::Group group,
      SeisParam& p,
      h5geo::CreationType createFlag) = 0;

  virtual std::vector<H5Seis*> openSeisList() = 0;
};

namespace h5geo {
  extern "C" {
  H5GEO_EXPORT H5SeisContainer* createSeisContainer(
    h5gt::File h5File, h5geo::CreationType createFlag);
  H5GEO_EXPORT H5SeisContainer* createSeisContainerByName(
    std::string& fileName, h5geo::CreationType createFlag);
  }
}

using H5SeisCnt_ptr = std::unique_ptr<H5SeisContainer, h5geo::ObjectDeleter>;

#endif // H5SEISCONTAINER_H
