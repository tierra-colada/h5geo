#ifndef H5WELLCONTAINER_H
#define H5WELLCONTAINER_H

#include "h5geo_export.h"
#include "h5basecontainer.h"

#include <memory>

class H5Well;

class H5WellContainer : public H5BaseContainer
{
protected:
  virtual ~H5WellContainer() = default;

public:
  virtual H5Well* openWell(
      const std::string& name) = 0;
  virtual H5Well* openWell(
      h5gt::Group group) = 0;
  virtual H5Well* createWell(
      std::string& name,
      WellParam& p,
      h5geo::CreationType createFlag) = 0;
  virtual H5Well* createWell(
      h5gt::Group group,
      WellParam& p,
      h5geo::CreationType createFlag) = 0;

  virtual H5Well* openWellByUWI(
      const std::string& name) = 0;
};

namespace h5geo {
  extern "C" {
  H5GEO_EXPORT H5WellContainer* createWellContainer(
    h5gt::File h5File, h5geo::CreationType createFlag);
  H5GEO_EXPORT H5WellContainer* createWellContainerByName(
    std::string& fileName, h5geo::CreationType createFlag);
  }
}

using H5WellCnt_ptr = std::unique_ptr<H5WellContainer, h5geo::ObjectDeleter>;

#endif // H5WELLCONTAINER_H
