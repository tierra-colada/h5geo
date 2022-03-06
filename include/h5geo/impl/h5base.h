#ifndef H5BASE_H
#define H5BASE_H

#ifndef H5GT_USE_EIGEN
#define H5GT_USE_EIGEN // should be defined before including h5gt
#endif

#ifdef H5GEO_USE_GDAL
#include "h5sr_settings.h"
#endif

#include "h5enum.h"
#include "h5point.h"
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
class H5Points;

///
/// \brief Base class for object params
/// 'spatialReference' in the form 'authName:code'
struct BaseObjectParam{
  // 'spatialReference' in the form 'authName:code'
  std::string spatialReference, lengthUnits, temporalUnits, angularUnits, dataUnits;
};

struct PointsParam : public BaseObjectParam{
  h5geo::Domain domain;
  size_t nPoints;
  hsize_t chunkSize = 10;
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
  hsize_t trcChunk = 20000, stdChunk = 100;
  bool mapSEGY = false;
  std::vector<std::string> segyFiles;
};

class H5Base
{
protected:
  virtual ~H5Base() = default;

public:
  /// Must be manually deleted and must be reimplemented
  /// in H5BaseObjectImpl<T> and H5BaseContainerImpl<T>
  virtual H5Base* clone() = 0;
  virtual void Delete() = 0;
};

#ifdef H5GEO_USE_GDAL
#include <gdal/gdal.h>
#include <gdal/gdal_priv.h>
#endif

namespace h5geo
{

struct ObjectDeleter
{
  void operator()(H5Base * ptr) const
  {
    ptr->Delete();
  }
};

#ifdef H5GEO_USE_GDAL
struct OGRCoordinateTransformationDeleter
{
  void operator()(OGRCoordinateTransformation * ptr) const
  {
    OGRCoordinateTransformation::DestroyCT(ptr);
  }
};
#endif

extern "C" {

//----------- CONTAINERS -----------
H5GEO_EXPORT H5BaseContainer* createContainer(
    h5gt::File h5File, h5geo::ContainerType cntType, h5geo::CreationType createFlag);

H5GEO_EXPORT H5BaseContainer* createContainerByName(
    std::string& fileName, h5geo::ContainerType cntType, h5geo::CreationType createFlag);

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
    const h5gt::Group& group);
H5GEO_EXPORT bool isGeoObjectByType(
    const h5gt::Group& group,
    const h5geo::ObjectType& objType);

H5GEO_EXPORT bool isPoints(const h5gt::Group &group);
H5GEO_EXPORT bool isMap(const h5gt::Group &group);
H5GEO_EXPORT bool isWell(const h5gt::Group &group);
H5GEO_EXPORT bool isLogCurve(const h5gt::Group &group);
H5GEO_EXPORT bool isDevCurve(const h5gt::Group &group);
H5GEO_EXPORT bool isSeis(const h5gt::Group &group);

}

} // h5geo

using H5Base_ptr = std::unique_ptr<H5Base, h5geo::ObjectDeleter>;

#ifdef H5GEO_USE_GDAL
using OGRCT_ptr = std::unique_ptr<OGRCoordinateTransformation, h5geo::OGRCoordinateTransformationDeleter>;
#endif

#endif // H5BASE_H
