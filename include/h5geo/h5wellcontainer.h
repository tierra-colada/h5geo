#ifndef H5WELLCONTAINER_H
#define H5WELLCONTAINER_H

#include "misc/h5geo_export.h"
#include "misc/h5basecontainer.h"

#include <memory>

class H5Well;

class H5WellContainer : public virtual H5BaseContainer
{
protected:
  virtual ~H5WellContainer() = default;

public:
  virtual H5Well* getWell(
      std::string& name) = 0;
  virtual H5Well* getWell(
      h5gt::Group& group) = 0;
  virtual H5Well* createWell(
      std::string& name,
      WellParam& p,
      h5geo::CreationType createFlag) = 0;
  virtual H5Well* createWell(
      h5gt::Group& group,
      WellParam& p,
      h5geo::CreationType createFlag) = 0;

  virtual std::vector<H5Well*> getWellList() = 0;
};

namespace h5geo {
  extern "C" {
  H5GEO_EXPORT H5WellContainer* createWellContainer(
    h5gt::File &h5File, h5geo::CreationType createFlag);
  H5GEO_EXPORT H5WellContainer* createWellContainerByName(
    std::string& fileName, h5geo::CreationType createFlag);
  }
}

using H5WellCnt_ptr = std::unique_ptr<H5WellContainer, h5geo::ObjectDeleter>;

#endif // H5WELLCONTAINER_H
