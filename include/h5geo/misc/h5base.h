#ifndef H5BASE_H
#define H5BASE_H

#include "h5core_enum.h"

#include <vector>

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

struct SurfParam{
  size_t nX, nY;
  double X0, Y0, dX, dY;
  h5geo::Domain domain;
  h5geo::SpatialUnits spatialUnits;
  h5geo::TemporalUnits temporalUnits;
  std::string dataUnits;
};

struct WellParam{
  double headX, headY, kb;
  std::string uwi;
};

struct DevCurveParam{
  h5geo::SpatialUnits spatialUnits;
  h5geo::TemporalUnits temporalUnits;
  h5geo::AngleUnits angleUnits;
  bool setActive = false;
  hsize_t chunkSize = 1000;
};

struct LogCurveParam{
  h5geo::SpatialUnits spatialUnits;
  std::string dataUnits;
  hsize_t chunkSize = 1000;
};

struct SeisParam{
  h5geo::Domain domain;
  h5geo::SpatialUnits spatialUnits;
  h5geo::TemporalUnits temporalUnits;
  std::string dataUnits;
  h5geo::SeisDataType dataType;
  h5geo::SurveyType surveyType;
  size_t nTrc;
  size_t nSamp;
  double srd = 0;
  hsize_t trcChunk = 20000;
  hsize_t stdChunk = 1000;
  h5geo::CreationType seisCreateType =
      h5geo::CreationType::CREATE_OR_OVERWRITE;
};

class H5Base
{
protected:
  virtual ~H5Base() = default;;

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
} // h5geo


#endif // H5BASE_H
