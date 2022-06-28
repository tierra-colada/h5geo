#ifndef H5BASE_H
#define H5BASE_H

#ifndef H5GT_USE_EIGEN
#define H5GT_USE_EIGEN // should be defined before including h5gt
#endif

#ifdef H5GEO_USE_GDAL
#include "private/h5core_sr_settings.h"
#endif

#include "h5geo_export.h"
#include "private/h5enum.h"
#include "private/h5point.h"

#include <vector>
#include <map>
#include <memory>
#include <optional>

typedef unsigned long long hsize_t ;

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
class H5BasePoints;
class H5Points1;
class H5Points2;
class H5Points3;
class H5Points4;
class H5WellTops;
class H5Horizon;

/// \struct BaseObjectParam
/// \brief Base class for object parameters
///
/// Parameters are needed when creating geo-objects.
struct BaseObjectParam{
  std::string spatialReference; ///< usually in the form `authName:code` given by 'proj.db' (probably using `name` will also work)
	std::string lengthUnits; ///< all geo-object's `lengthUnits` data must follow it (if any)
	std::string temporalUnits; ///< all geo-object's `temporalUnits` data must follow it (if any)
	std::string angularUnits; ///< all geo-object's `angularUnits` data must follow it (if any)
	std::string dataUnits; ///< all geo-object's `dataUnits` data must follow it (if any)
  double nullValue = NAN; ///< usually used for missed values
};

/// \struct PointsParam
/// \brief Common class for creating H5Points1, H5Points2, H5Points3, H5Points4
///
/// Points parameters are needed when creating any Points geo-object.
struct PointsParam : public BaseObjectParam{
  h5geo::Domain domain;	///< time or depth (TWT, TVD etc)
  size_t nPoints; ///< number of points
  hsize_t chunkSize = 10; ///< see HDF5 chunking
};

/// \struct WellTopsParam
/// \brief Class for creating H5WellTops
///
/// WellTops parameters are needed when creating any WellTops geo-object.
struct WellTopsParam : public PointsParam{};

/// \struct HorizonParam
/// \brief Class for creating H5Horizon
///
/// Horizon parameters are needed when creating any H5Horizon geo-object.
struct HorizonParam : public BaseObjectParam{
  h5geo::Domain domain;	///< time or depth (TWT, TVD etc)
  size_t nPoints; ///< number of points (columns within HDF5 DataSet)
  std::map<std::string, size_t> components; ///< component name and corresponding HDF5 row number
  hsize_t pointsChunkSize = 10; ///< see HDF5 chunking
};

/// \struct MapParam
/// \brief Class for creating H5Map
///
/// Map parameters are needed when creating any Map geo-object.
struct MapParam : public BaseObjectParam{
  double X0; ///< X-coordinate of origin (upper-left corner of column-major Eigen matrix)
	double Y0; ///< Y-coordinate of origin (upper-left corner of column-major Eigen matrix)
	double X1; ///< X-coordinate of first point (upper-right corner of column-major Eigen matrix)
	double Y1; ///< Y-coordinate of first point (upper-right corner of column-major Eigen matrix)
	double X2; ///< X-coordinate of second point (lower-left corner of column-major Eigen matrix)
	double Y2; ///< Y-coordinate of second point (lower-left corner of column-major Eigen matrix)
  size_t nX; ///< number of columns in column-major Eigen matrix
	size_t nY; ///< number of rows in column-major Eigen matrix
  h5geo::Domain domain; ///< time or depth (TWT, TVD etc)
  hsize_t xChunkSize = 10; ///< see HDF5 chunking
	hsize_t yChunkSize = 10; ///< see HDF5 chunking
};

/// \struct WellParam
/// \brief Class for creating H5Well
///
/// Well parameters are needed when creating any Well geo-object.
struct WellParam : public BaseObjectParam{
  double headX; ///< well head X-coordinate
	double headY; ///< well head Y-coordinate
	double kb = 0; ///< kelly bushing
  std::string uwi; /// Unique Well Identifier
};

/// \struct DevCurveParam
/// \brief Class for creating H5DevCurve
///
/// DevCurve parameters are needed when creating any DevCurve geo-object.
struct DevCurveParam : public BaseObjectParam{
  hsize_t chunkSize = 10; ///< see HDF5 chunking
};

/// \struct LogCurveParam
/// \brief Class for creating H5LogCurve
///
/// LogCurve parameters are needed when creating any LogCurve geo-object.
struct LogCurveParam : public BaseObjectParam{
  hsize_t chunkSize = 10; ///< see HDF5 chunking
};

/// \struct SeisParam
/// \brief Class for creating H5Seis
///
/// Seis parameters are needed when creating any Seis geo-object.
struct SeisParam : public BaseObjectParam{
  h5geo::Domain domain; ///< time or depth (TWT, TVD etc)
  h5geo::SeisDataType dataType; ///< STACK or PRESTACK
  h5geo::SurveyType surveyType; ///< TWO_D or THREE_D
  size_t nTrc; ///< number of traces
	size_t nSamp; ///< number of samples
  double srd = 0; ///< Seismic Reference Datum
  hsize_t trcChunk = 20000; ///< number of traces per chunk (see HDF5 chunking)
	hsize_t stdChunk = 100; ///< used secondary for creating datasets within Seis geo-object (see HDF5 chunking)
  bool mapSEGY = false; ///< SEGY mapping used at Seis creation time (use in pair with `segyFiles`)
  std::vector<std::string> segyFiles; ///< used to map SEGY files (use in pair with `mapSEGY`)
};

/// \class H5Base
/// \brief Base class for all geo-containers and geo-objects
///
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
class OGRCoordinateTransformation;
class GDALDataset;
#endif

/// \namespace h5geo
/// \brief Basic namespace
///
namespace h5geo
{

/// \class ObjectDeleter
/// \brief Deleter for all H5Base inherited objects
///
struct H5GEO_EXPORT ObjectDeleter
{
  void operator()(H5Base * ptr) const;
};

#ifdef H5GEO_USE_GDAL
struct  H5GEO_EXPORT OGRCoordinateTransformationDeleter
{
  void operator()(OGRCoordinateTransformation * ptr) const;
};

struct  H5GEO_EXPORT GDALDatasetDeleter
{
  void operator()(GDALDataset * ptr) const;
};
#endif


extern "C" {

//----------- CONTAINERS -----------
/// \brief Factory function for creating geo-container
///
/// Use `dynamic_cast<>` to cast it to specified `h5geo::ContainerType`
H5GEO_EXPORT H5BaseContainer* createContainer(
    h5gt::File h5File, h5geo::ContainerType cntType, h5geo::CreationType createFlag);

/// \brief Factory function for creating geo-container
///
/// Use `dynamic_cast<>` to cast it to specified `h5geo::ContainerType`
H5GEO_EXPORT H5BaseContainer* createContainerByName(
    std::string& fileName, h5geo::ContainerType cntType, h5geo::CreationType createFlag);

/// \brief Factory function for creating H5MapContainer
H5GEO_EXPORT H5MapContainer* createMapContainer(
  h5gt::File h5File, h5geo::CreationType createFlag);
/// \brief Factory function for creating H5MapContainer
H5GEO_EXPORT H5MapContainer* createMapContainerByName(
  std::string& fileName, h5geo::CreationType createFlag);

/// \brief Factory function for creating H5SeisContainer
H5GEO_EXPORT H5SeisContainer* createSeisContainer(
  h5gt::File h5File, h5geo::CreationType createFlag);
/// \brief Factory function for creating H5SeisContainer
H5GEO_EXPORT H5SeisContainer* createSeisContainerByName(
  std::string& fileName, h5geo::CreationType createFlag);

/// \brief Factory function for creating H5WellContainer
H5GEO_EXPORT H5WellContainer* createWellContainer(
  h5gt::File h5File, h5geo::CreationType createFlag);
/// \brief Factory function for creating H5WellContainer
H5GEO_EXPORT H5WellContainer* createWellContainerByName(
  std::string& fileName, h5geo::CreationType createFlag);

/// \brief Factory function for opening geo-container
///
/// Check if the file belongs to any H5BasContainer derived type
/// and create appropriate object. \n
/// Use dynamic_cast<> to cast it to the correct type.
H5GEO_EXPORT H5BaseContainer* openContainer(
    h5gt::File h5File);

/// \brief Factory function for opening geo-container
///
/// Check if the file belongs to any H5BasContainer derived type
/// and create appropriate object. \n
/// Use dynamic_cast<> to cast it to the correct type.
H5GEO_EXPORT H5BaseContainer* openContainerByName(
    const std::string& fileName);

/// \brief Factory function for opening H5BaseContainer
H5GEO_EXPORT H5BaseContainer* openBaseContainer(
    h5gt::File h5File);
/// \brief Factory function for opening H5BaseContainer
H5GEO_EXPORT H5BaseContainer* openBaseContainerByName(
    const std::string& fileName);

/// \brief Factory function for opening H5SeisContainer
H5GEO_EXPORT H5SeisContainer* openSeisContainer(
    h5gt::File h5File);
/// \brief Factory function for opening H5SeisContainer
H5GEO_EXPORT H5SeisContainer* openSeisContainerByName(
    const std::string& fileName);

/// \brief Factory function for opening H5MapContainer
H5GEO_EXPORT H5MapContainer* openMapContainer(
    h5gt::File h5File);
/// \brief Factory function for opening H5MapContainer
H5GEO_EXPORT H5MapContainer* openMapContainerByName(
    const std::string& fileName);

/// \brief Factory function for opening H5WellContainer
H5GEO_EXPORT H5WellContainer* openWellContainer(
    h5gt::File h5File);
/// \brief Factory function for opening H5WellContainer
H5GEO_EXPORT H5WellContainer* openWellContainerByName(
    const std::string& fileName);

/// \brief Check if the container belongs to any geo-type
H5GEO_EXPORT bool isGeoContainer(
    h5gt::File file);
/// \brief Check if the container belongs to any geo-type
H5GEO_EXPORT bool isGeoContainerByType(
    h5gt::File file,
    const h5geo::ContainerType& cntType);

/// \brief Get container geo-type
H5GEO_EXPORT h5geo::ContainerType getGeoContainerType(
    h5gt::File file);

//----------- OBJECTS -----------
/// \brief Factory function for opening geo-object
///
/// Check if HDF5 object belongs to any H5BaseObject derived type
/// and create appropriate object. \n
/// Use dynamic_cast<> to cast it to the correct type.
H5GEO_EXPORT H5BaseObject* openObject(
    h5gt::Group group);

/// \brief Factory function for opening geo-object
///
/// Check if HDF5 object belongs to any H5BaseObject derived type
/// and create appropriate object. \n
/// Use dynamic_cast<> to cast it to the correct type.
H5GEO_EXPORT H5BaseObject* openObjectByName(
    const std::string& fileName, const std::string& objName);

/// \brief Factory function for opening H5BaseObject
H5GEO_EXPORT H5BaseObject* openBaseObject(
    h5gt::Group group);
/// \brief Factory function for opening H5BaseObject
H5GEO_EXPORT H5BaseObject* openBaseObjectByName(
    const std::string& fileName, const std::string& objName);

/// \brief Factory function for opening H5Map
H5GEO_EXPORT H5Map* openMap(
    h5gt::Group group);
/// \brief Factory function for opening H5Map
H5GEO_EXPORT H5Map* openMapByName(
    const std::string& fileName, const std::string& objName);

/// \brief Factory function for opening H5Seis
H5GEO_EXPORT H5Seis* openSeis(
    h5gt::Group group);
/// \brief Factory function for opening H5Seis
H5GEO_EXPORT H5Seis* openSeisByName(
    const std::string& fileName, const std::string& objName);

/// \brief Factory function for opening H5Well
H5GEO_EXPORT H5Well* openWell(
    h5gt::Group group);
/// \brief Factory function for opening H5Well
H5GEO_EXPORT H5Well* openWellByName(
    const std::string& fileName, const std::string& objName);

/// \brief Factory function for opening H5DevCurve
H5GEO_EXPORT H5DevCurve* openDevCurve(
    h5gt::Group group);
/// \brief Factory function for opening H5DevCurve
H5GEO_EXPORT H5DevCurve* openDevCurveByName(
    const std::string& fileName, const std::string& objName);

/// \brief Factory function for opening H5LogCurve
H5GEO_EXPORT H5LogCurve* openLogCurve(
    h5gt::Group group);
/// \brief Factory function for opening H5LogCurve
H5GEO_EXPORT H5LogCurve* openLogCurveByName(
    const std::string& fileName, const std::string& objName);

/// \brief Factory function for opening H5BasePoints derived points
H5GEO_EXPORT H5BasePoints* openPoints(
    h5gt::Group group);
/// \brief Factory function for opening H5BasePoints derived points
H5GEO_EXPORT H5BasePoints* openPointsByName(
    const std::string& fileName, const std::string& objName);

/// \brief Factory function for opening H5Points1
H5GEO_EXPORT H5Points1* openPoints1(
    h5gt::Group group);
/// \brief Factory function for opening H5Points1
H5GEO_EXPORT H5Points1* openPoints1ByName(
    const std::string& fileName, const std::string& objName);

/// \brief Factory function for opening H5Points2
H5GEO_EXPORT H5Points2* openPoints2(
    h5gt::Group group);
/// \brief Factory function for opening H5Points2
H5GEO_EXPORT H5Points2* openPoints2ByName(
    const std::string& fileName, const std::string& objName);

/// \brief Factory function for opening H5Points3
H5GEO_EXPORT H5Points3* openPoints3(
    h5gt::Group group);
/// \brief Factory function for opening H5Points3
H5GEO_EXPORT H5Points3* openPoints3ByName(
    const std::string& fileName, const std::string& objName);

/// \brief Factory function for opening H5Points4
H5GEO_EXPORT H5Points4* openPoints4(
    h5gt::Group group);
/// \brief Factory function for opening H5Points4
H5GEO_EXPORT H5Points4* openPoints4ByName(
    const std::string& fileName, const std::string& objName);

/// \brief Factory function for opening H5WellTops
H5GEO_EXPORT H5WellTops* openWellTops(
    h5gt::Group group);
/// \brief Factory function for opening H5WellTops
H5GEO_EXPORT H5WellTops* openWellTopsByName(
    const std::string& fileName, const std::string& objName);

/// \brief Factory function for opening H5Horizon
H5GEO_EXPORT H5Horizon* openHorizon(
    h5gt::Group group);
/// \brief Factory function for opening H5Horizon
H5GEO_EXPORT H5Horizon* openHorizonByName(
    const std::string& fileName, const std::string& objName);

/// \brief Check if HDF5 object belongs to any geo-type
H5GEO_EXPORT bool isGeoObject(
    const h5gt::Group& group);
/// \brief Check if HDF5 object belongs to specified geo-type
H5GEO_EXPORT bool isGeoObjectByType(
    const h5gt::Group& group,
    const h5geo::ObjectType& objType);

/// \brief Check if HDF5 object is H5Horizon
H5GEO_EXPORT bool isHorizon(const h5gt::Group &group);
/// \brief Check if HDF5 object is H5Points1
H5GEO_EXPORT bool isPoints1(const h5gt::Group &group);
/// \brief Check if HDF5 object is H5Points2
H5GEO_EXPORT bool isPoints2(const h5gt::Group &group);
/// \brief Check if HDF5 object is H5Points3
H5GEO_EXPORT bool isPoints3(const h5gt::Group &group);
/// \brief Check if HDF5 object is H5Points4
H5GEO_EXPORT bool isPoints4(const h5gt::Group &group);
/// \brief Check if HDF5 object is H5WellTops
H5GEO_EXPORT bool isWellTops(const h5gt::Group &group);
/// \brief Check if HDF5 object belongs to any H5BasePoints derived type
H5GEO_EXPORT bool isPoints(const h5gt::Group &group);
/// \brief Check if HDF5 object is H5Map
H5GEO_EXPORT bool isMap(const h5gt::Group &group);
/// \brief Check if HDF5 object is H5Well
H5GEO_EXPORT bool isWell(const h5gt::Group &group);
/// \brief Check if HDF5 object is H5LogCurve
H5GEO_EXPORT bool isLogCurve(const h5gt::Group &group);
/// \brief Check if HDF5 object is H5DevCurve
H5GEO_EXPORT bool isDevCurve(const h5gt::Group &group);
/// \brief Check if HDF5 object is H5Seis
H5GEO_EXPORT bool isSeis(const h5gt::Group &group);

/// \brief Get HDF5 object's geo-type
H5GEO_EXPORT h5geo::ObjectType getGeoObjectType(
    const h5gt::Group& group);

}

} // h5geo

using H5Base_ptr = std::unique_ptr<H5Base, h5geo::ObjectDeleter>;

#ifdef H5GEO_USE_GDAL
using OGRCT_ptr = std::unique_ptr<OGRCoordinateTransformation, h5geo::OGRCoordinateTransformationDeleter>;
using GDALDS_ptr = std::unique_ptr<GDALDataset, h5geo::GDALDatasetDeleter>;
#endif

#endif // H5BASE_H
