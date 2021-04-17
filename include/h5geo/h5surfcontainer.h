#ifndef H5SURFCONTAINER_H
#define H5SURFCONTAINER_H

#include "misc/h5geo_export.h"
#include "misc/h5basecontainer.h"

#include <memory>

class H5Surf;

class H5SurfContainer : public virtual H5BaseContainer
{
protected:
  virtual ~H5SurfContainer() = default;

public:
  virtual H5Surf* getSurf(
      std::string& name) = 0;
  virtual H5Surf* getSurf(
      h5gt::Group& group) = 0;
  virtual H5Surf* createSurf(
      std::string& name,
      SurfParam &p,
      h5geo::CreationType createFlag) = 0;
  virtual H5Surf* createSurf(
      h5gt::Group& group,
      SurfParam &p,
      h5geo::CreationType createFlag) = 0;

  virtual std::vector<H5Surf*> getSurfList() = 0;
};

namespace h5geo {
  extern "C" {
  H5GEO_EXPORT H5SurfContainer* createSurfContainer(
    h5gt::File &h5File, h5geo::CreationType createFlag);
  H5GEO_EXPORT H5SurfContainer* createSurfContainerByName(
    std::string& fileName, h5geo::CreationType createFlag);
  }
}

using H5SurfCnt_ptr = std::unique_ptr<H5SurfContainer, h5geo::ObjectDeleter>;

#endif // H5SURFCONTAINER_H
