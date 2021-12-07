#ifndef H5BASE_H
#define H5BASE_H

#ifndef H5GT_USE_EIGEN
#define H5GT_USE_EIGEN // should be defined before including h5gt
#endif

#include "h5core_enum.h"
#include "h5geo_export.h"

#include <vector>
#include <memory>

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
class H5MapContainer;
class H5WellContainer;
class H5Seis;
class H5Map;
class H5Well;
class H5DevCurve;
class H5LogCurve;

///
/// \brief Base class for object params
/// 'spatialReference' in the form 'authName:code'
struct BaseObjectParam{
  // 'spatialReference' in the form 'authName:code'
  std::string spatialReference, lengthUnits, temporalUnits, angularUnits, dataUnits;
};

struct MapParam : public BaseObjectParam{
  double X0, Y0, X1, Y1, X2, Y2;
  size_t nX, nY;
  h5geo::Domain domain;
  hsize_t xChunkSize = 10, yChunkSize = 10;
};

struct WellParam : public BaseObjectParam{
  double headX, headY, kb = 0;
  std::string uwi;
};

struct DevCurveParam : public BaseObjectParam{
  hsize_t chunkSize = 10;
};

struct LogCurveParam : public BaseObjectParam{
  hsize_t chunkSize = 10;
};

struct SeisParam : public BaseObjectParam{
  h5geo::Domain domain;
  h5geo::SeisDataType dataType;
  h5geo::SurveyType surveyType;
  size_t nTrc, nSamp;
  double srd = 0;
  double X0, Y0, X1, Y1, X2, Y2;
  hsize_t trcChunk = 20000, stdChunk = 1000;
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

//----------- CONTAINERS -----------
/// Check if the file can be treated as
/// H5SeisContainer/H5MapContaier/H5WellContainer etc
/// and create appropriate object.
/// Use dynamic_cast<> to cast it to the correct type.
H5GEO_EXPORT H5BaseContainer* openContainer(
    h5gt::File h5File);

/// Check if the file can be treated as
/// H5SeisContainer/H5MapContaier/H5WellContainer etc
/// and create appropriate object.
/// Use dynamic_cast<> to cast it to the correct type.
H5GEO_EXPORT H5BaseContainer* openContainerByName(
    const std::string& fileName);

H5GEO_EXPORT H5BaseContainer* openBaseContainer(
    h5gt::File h5File);
H5GEO_EXPORT H5BaseContainer* openBaseContainerByName(
    const std::string& fileName);

H5GEO_EXPORT H5SeisContainer* openSeisContainer(
    h5gt::File h5File);
H5GEO_EXPORT H5SeisContainer* openSeisContainerByName(
    const std::string& fileName);

H5GEO_EXPORT H5MapContainer* openMapContainer(
    h5gt::File h5File);
H5GEO_EXPORT H5MapContainer* openMapContainerByName(
    const std::string& fileName);

H5GEO_EXPORT H5WellContainer* openWellContainer(
    h5gt::File h5File);
H5GEO_EXPORT H5WellContainer* openWellContainerByName(
    const std::string& fileName);

H5GEO_EXPORT bool isGeoContainer(
    h5gt::File file);
H5GEO_EXPORT bool isGeoContainerByType(
    h5gt::File file,
    const h5geo::ContainerType& cntType);

//----------- OBJECTS -----------
/// Check if the group can be treated as H5Seis/H5Map/H5Well etc
/// and create appropriate object.
/// Use dynamic_cast<> to cast it to the correct type.
H5GEO_EXPORT H5BaseObject* openObject(
    h5gt::Group group);

H5GEO_EXPORT bool isGeoObject(
    h5gt::Group& group);
H5GEO_EXPORT bool isGeoObjectByType(
    h5gt::Group& group,
    const h5geo::ObjectType& objType);

H5GEO_EXPORT bool isMap(h5gt::Group &group);
H5GEO_EXPORT bool isWell(h5gt::Group &group);
H5GEO_EXPORT bool isLogCurve(h5gt::Group &group);
H5GEO_EXPORT bool isDevCurve(h5gt::Group &group);
H5GEO_EXPORT bool isSeis(h5gt::Group &group);

}

} // h5geo

using H5Base_ptr = std::unique_ptr<H5Base, h5geo::ObjectDeleter>;

#endif // H5BASE_H
