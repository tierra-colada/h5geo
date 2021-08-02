#ifndef H5BASE_H
#define H5BASE_H

#include "h5core_enum.h"

#include <vector>
#include <memory>

#include <iostream>

typedef unsigned long long hsize_t ;

namespace std {
template<typename T> class optional;
}

namespace h5gt {
class File;
class Group;
class DataSet;
}

namespace h5geo {
enum class CreationType: unsigned;
enum class DevDataType: unsigned;
}

class H5BaseContainer;
class H5BaseObject;
class H5SeisContainer;
class H5SurfContainer;
class H5WellContainer;

struct SurfParam{
  size_t nX, nY;
  double X0, Y0, dX, dY;
  h5geo::Domain domain;
  std::string spatialUnits, temporalUnits, dataUnits;
};

struct WellParam{
  double headX, headY, kb;
  std::string spatialUnits, uwi;
};

struct DevCurveParam{
  std::string spatialUnits, temporalUnits, angleUnits;
  bool setActive = false;
  hsize_t chunkSize = 1000;
};

struct LogCurveParam{
  std::string dataUnits, spatialUnits;
  hsize_t chunkSize = 1000;
};

struct SeisParam{
  h5geo::Domain domain;
  h5geo::SeisDataType dataType;
  h5geo::SurveyType surveyType;
  std::string spatialUnits, temporalUnits, dataUnits;
  size_t nTrc;
  size_t nSamp;
  double srd = 0;
  hsize_t trcChunk = 20000;
  hsize_t stdChunk = 1000;
};

class H5Base
{
protected:
  virtual ~H5Base() = default;

public:
  virtual void Delete() = 0;
  virtual std::vector<h5gt::Group>
  getChildList(h5gt::Group& group,
               const h5geo::ObjectType& objType) = 0;
};

namespace h5geo
{
struct ObjectDeleter
{
  void operator()(H5Base * ptr) const
  {
    ptr->Delete();
  }
};

extern "C" {
H5GEO_EXPORT H5BaseContainer* openBaseContainer(
    h5gt::File h5File);
H5GEO_EXPORT H5BaseContainer* openBaseContainerByName(
    const std::string& fileName);

H5GEO_EXPORT H5BaseObject* openBaseObject(
    h5gt::Group group);

H5GEO_EXPORT H5SeisContainer* openSeisContainer(
    h5gt::File h5File);
H5GEO_EXPORT H5SeisContainer* openSeisContainerByName(
    std::string& fileName);

H5GEO_EXPORT H5SurfContainer* openSurfContainer(
    h5gt::File h5File);
H5GEO_EXPORT H5SurfContainer* openSurfContainerByName(
    std::string& fileName);

H5GEO_EXPORT H5WellContainer* openWellContainer(
    h5gt::File h5File);
H5GEO_EXPORT H5WellContainer* openWellContainerByName(
    std::string& fileName);
}

} // h5geo

using H5Base_ptr = std::unique_ptr<H5Base, h5geo::ObjectDeleter>;

#endif // H5BASE_H
