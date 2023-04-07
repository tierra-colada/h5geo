#include "../../include/h5geo/private/h5mapimpl.h"
#include "../../include/h5geo/h5mapcontainer.h"
#include "../../include/h5geo/h5core.h"
#include "../../include/h5geo/private/h5enum_string.h"

#include <units/units.hpp>

#ifdef H5GEO_USE_GDAL
#include <gdal.h>
#include <gdal_priv.h>
#endif

H5MapImpl::H5MapImpl(const h5gt::Group &group) :
  H5BaseObjectImpl(group){}

#ifdef H5GEO_USE_GDAL
bool H5MapImpl::readRasterCoordinates(
    const std::string& file, const std::string& lengthUnits)
{
  GDALDS_ptr ds((GDALDataset *)GDALOpen(file.c_str(), GA_ReadOnly));
  if(!ds)
    return false;

  int rasterXSize = ds->GetRasterXSize();
  int rasterYSize = ds->GetRasterYSize();

  if (rasterXSize < 1 || rasterYSize < 1)
    return false;

  double GT[6];
  ds->GetGeoTransform(GT);

  // from here:
  // https://gdal.org/tutorials/geotransforms_tut.html
  double x0 = GT[0];
  double y0 = GT[3];
  double x1 = GT[0] + 0*GT[1] + rasterYSize*GT[2];
  double y1 = GT[3] + 0*GT[4] + rasterYSize*GT[5];
  double x2 = GT[0] + rasterXSize*GT[1] + 0*GT[2];
  double y2 = GT[3] + rasterXSize*GT[4] + 0*GT[5];

  Eigen::Vector2d origin, p1, p2;
  origin(0) = x0;
  origin(1) = y0;
  p1(0) = x1;
  p1(1) = y1;
  p2(0) = x2;
  p2(1) = y2;

  if (!setOrigin(origin, lengthUnits))
    return false;

  if (!setPoint1(p1, lengthUnits))
    return false;

  if (!setPoint2(p2, lengthUnits))
    return false;

  return true;
}

bool H5MapImpl::readRasterSpatialReference(const std::string& file)
{
  GDALDS_ptr ds((GDALDataset *)GDALOpen(file.c_str(), GA_ReadOnly));
  if(!ds)
    return false;

  const OGRSpatialReference* crs = ds->GetGCPSpatialRef();
  if (!crs)
    return false;

  std::string name, code;
  if (crs->GetAuthorityName(NULL))
    name = crs->GetAuthorityName(NULL);

  if (crs->GetAuthorityCode(NULL))
    code = crs->GetAuthorityCode(NULL);

  if (name.empty() || code.empty())
    return false;

  return setSpatialReference(name + ":" + code);
}

bool H5MapImpl::readRasterLengthUnits(const std::string& file)
{
  GDALDS_ptr ds((GDALDataset *)GDALOpen(file.c_str(), GA_ReadOnly));
  if(!ds)
    return false;

  const OGRSpatialReference* crs = ds->GetGCPSpatialRef();
  if (!crs)
    return false;

  const char *units = nullptr;
  crs->GetLinearUnits(&units);
  if (!units)
    return false;

  if (!this->setLengthUnits(std::string(units)))
    return false;

  return true;
}

bool H5MapImpl::readRasterData(
    const std::string& file, const std::string& dataUnits)
{
  GDALDS_ptr ds((GDALDataset *)GDALOpen(file.c_str(), GA_ReadOnly));
  if(!ds)
    return false;

  GDALRasterBand *dsBand = ds->GetRasterBand(1);
  if (!dsBand)
    return false;

  int nXSize = dsBand->GetXSize();
  int nYSize = dsBand->GetYSize();
  Eigen::MatrixXd m(nXSize, nYSize);
  CPLErr status = dsBand->RasterIO(GF_Read, 0, 0, nXSize, nYSize,
                   m.data(), nXSize, nYSize, GDT_Float64,
                   0, 0);
  if (status == CPLErr::CE_Failure || status == CPLErr::CE_Fatal)
    return false;
  m.transposeInPlace();
  return this->writeData(m, dataUnits);
}

#endif  // H5GEO_USE_GDAL

bool H5MapImpl::writeData(
    Eigen::Ref<Eigen::MatrixXd> M,
    const std::string& dataUnits)
{
  auto opt = getMapD();
  if (!opt.has_value())
    return false;

  bool val = h5geo::overwriteResizableDataset(
        objG,
        opt->getPath(),
        M,
        dataUnits, getDataUnits());

  objG.flush();
  return val;
}

Eigen::MatrixXd H5MapImpl::getData(const std::string& dataUnits){
  auto opt = getMapD();
  if (!opt.has_value())
    return Eigen::MatrixXd();

  return h5geo::readDoubleEigenMtxDataset(
        objG,
        opt->getPath(),
        getDataUnits(), dataUnits);
}

bool H5MapImpl::setDomain(const h5geo::Domain& val){
  return h5geo::overwriteEnumAttribute(
        objG,
        std::string{h5geo::detail::Domain},
        val);
}

bool H5MapImpl::setOrigin(
    Eigen::Ref<Eigen::Vector2d> v,
    const std::string& lengthUnits,
    bool doCoordTransform)
{
#ifdef H5GEO_USE_GDAL
  if (doCoordTransform){
    OGRCT_ptr coordTrans(createCoordinateTransformationToWriteData(lengthUnits));
    if (coordTrans){
      coordTrans->Transform(1, &v(0), &v(1));
      return h5geo::overwriteAttribute(
            objG,
            std::string{h5geo::detail::origin},
            v);
    } else if (!coordTrans && !h5geo::sr::getIgnoreCoordTransformOnFailure()){
      return false;
    }
  }
#endif

  return h5geo::overwriteAttribute(
        objG,
        std::string{h5geo::detail::origin},
        v, lengthUnits, getLengthUnits());
}

bool H5MapImpl::setPoint1(
    Eigen::Ref<Eigen::Vector2d> v,
    const std::string& lengthUnits,
    bool doCoordTransform)
{
#ifdef H5GEO_USE_GDAL
  if (doCoordTransform){
    OGRCT_ptr coordTrans(createCoordinateTransformationToWriteData(lengthUnits));
    if (coordTrans){
      coordTrans->Transform(1, &v(0), &v(1));
      return h5geo::overwriteAttribute(
            objG,
            std::string{h5geo::detail::point1},
            v);
    } else if (!coordTrans && !h5geo::sr::getIgnoreCoordTransformOnFailure()){
      return false;
    }
  }
#endif

  return h5geo::overwriteAttribute(
        objG,
        std::string{h5geo::detail::point1},
        v, lengthUnits, getLengthUnits());
}

bool H5MapImpl::setPoint2(
    Eigen::Ref<Eigen::Vector2d> v,
    const std::string& lengthUnits,
    bool doCoordTransform){
#ifdef H5GEO_USE_GDAL
  if (doCoordTransform){
    OGRCT_ptr coordTrans(createCoordinateTransformationToWriteData(lengthUnits));
    if (coordTrans){
      coordTrans->Transform(1, &v(0), &v(1));
      return h5geo::overwriteAttribute(
            objG,
            std::string{h5geo::detail::point2},
            v);
    } else if (!coordTrans && !h5geo::sr::getIgnoreCoordTransformOnFailure()){
      return false;
    }
  }
#endif

  return h5geo::overwriteAttribute(
        objG,
        std::string{h5geo::detail::point2},
        v, lengthUnits, getLengthUnits());
}

std::optional<h5gt::Group> H5MapImpl::addAttributeMap(H5Map* map, std::string name){
  if (this->getH5File() == map->getH5File())
    return addInternalAttributeMap(map, name);
  else
    return addExternalAttributeMap(map, name);
}

std::optional<h5gt::Group> H5MapImpl::addInternalAttributeMap(H5Map* map, std::string name){
  if (!map)
    return std::nullopt;

  if (this->getH5File() != map->getH5File())
    return std::nullopt;

  if (this->getNX() != map->getNX() ||
      this->getNY() != map->getNY())
    return std::nullopt;

  if (name.empty())
    name = map->getName();

  if (objG.hasObject(name, h5gt::ObjectType::Group))
    return std::nullopt;

  return objG.createLink(map->getObjG(), name, h5gt::LinkType::Soft);
}

std::optional<h5gt::Group> H5MapImpl::addExternalAttributeMap(H5Map* map, std::string name){
  if (!map)
    return std::nullopt;

  if (this->getH5File() == map->getH5File())
    return std::nullopt;

  if (this->getNX() != map->getNX() ||
      this->getNY() != map->getNY())
    return std::nullopt;

  if (name.empty())
    name = map->getName();

  if (objG.hasObject(name, h5gt::ObjectType::Group))
    return std::nullopt;

  return objG.createLink(map->getObjG(), name, h5gt::LinkType::External);
}

bool H5MapImpl::removeAttributeMap(const std::string& name){
  if (name.empty())
    return false;

  if (!objG.hasObject(name, h5gt::ObjectType::Group))
    return false;

  objG.unlink(name);
  return true;
}

H5Map* H5MapImpl::openAttributeMap(const std::string& name){
  if (!objG.hasObject(name, h5gt::ObjectType::Group))
    return nullptr;

  h5gt::Group group = objG.getGroup(name);
  if (!isGeoObjectByType(group, h5geo::ObjectType::MAP))
    return nullptr;

  return new H5MapImpl(group);
}

std::vector<h5gt::Group> H5MapImpl::getAttributeMapGroupList(){
  return getChildGroupList(objG, h5geo::ObjectType::MAP, false);
}

std::vector<std::string> H5MapImpl::getAttributeMapNameList(){
  return getChildNameList(objG, h5geo::ObjectType::MAP, objG.getPath());
}

size_t H5MapImpl::getAttributeMapCount(){
  return getChildCount(objG, h5geo::ObjectType::MAP);
}

h5geo::Domain H5MapImpl::getDomain(){
  return h5geo::readEnumAttribute<h5gt::Group, h5geo::Domain>(
          objG,
          std::string{h5geo::detail::Domain});
}

Eigen::VectorXd H5MapImpl::getOrigin(
    const std::string& lengthUnits,
    bool doCoordTransform)
{
#ifdef H5GEO_USE_GDAL
  if (doCoordTransform){
    OGRCT_ptr coordTrans(createCoordinateTransformationToReadData(lengthUnits));
    if (coordTrans){
      Eigen::VectorXd v = h5geo::readDoubleEigenVecAttribute(
            objG,
            std::string{h5geo::detail::origin});

      if (v.size() != 2)
        return Eigen::VectorXd();

      coordTrans->Transform(1, &v(0), &v(1));
      return v;
    } else if (!coordTrans && !h5geo::sr::getIgnoreCoordTransformOnFailure()){
      return Eigen::VectorXd();
    }
  }
#endif

  return h5geo::readDoubleEigenVecAttribute(
        objG,
        std::string{h5geo::detail::origin},
        getLengthUnits(), lengthUnits);
}

Eigen::VectorXd H5MapImpl::getPoint1(
    const std::string& lengthUnits,
    bool doCoordTransform)
{
#ifdef H5GEO_USE_GDAL
  if (doCoordTransform){
    OGRCT_ptr coordTrans(createCoordinateTransformationToReadData(lengthUnits));
    if (coordTrans){
      Eigen::VectorXd v = h5geo::readDoubleEigenVecAttribute(
            objG,
            std::string{h5geo::detail::point1});

      if (v.size() != 2)
        return Eigen::VectorXd();

      coordTrans->Transform(1, &v(0), &v(1));
      return v;
    } else if (!coordTrans && !h5geo::sr::getIgnoreCoordTransformOnFailure()){
      return Eigen::VectorXd();
    }
  }
#endif

  return h5geo::readDoubleEigenVecAttribute(
        objG,
        std::string{h5geo::detail::point1},
        getLengthUnits(), lengthUnits);
}

Eigen::VectorXd H5MapImpl::getPoint2(
    const std::string& lengthUnits,
    bool doCoordTransform)
{
#ifdef H5GEO_USE_GDAL
  if (doCoordTransform){
    OGRCT_ptr coordTrans(createCoordinateTransformationToReadData(lengthUnits));
    if (coordTrans){
      Eigen::VectorXd v = h5geo::readDoubleEigenVecAttribute(
            objG,
            std::string{h5geo::detail::point2});

      if (v.size() != 2)
        return Eigen::VectorXd();

      coordTrans->Transform(1, &v(0), &v(1));
      return v;
    } else if (!coordTrans && !h5geo::sr::getIgnoreCoordTransformOnFailure()){
      return Eigen::VectorXd();
    }
  }
#endif

  return h5geo::readDoubleEigenVecAttribute(
        objG,
        std::string{h5geo::detail::point2},
        getLengthUnits(), lengthUnits);
}

size_t H5MapImpl::getNX()
{
  auto opt = this->getMapD();
  if (!opt.has_value())
    return 0;

  std::vector<size_t> dims = opt->getDimensions();
  if (dims.size() < 2)
    return 0;

  return dims[1];
}

size_t H5MapImpl::getNY()
{
  auto opt = this->getMapD();
  if (!opt.has_value())
    return 0;

  std::vector<size_t> dims = opt->getDimensions();
  if (dims.size() < 2)
    return 0;

  return dims[0];
}

H5MapParam H5MapImpl::getParam()
{
  H5MapParam p;
  // H5BaseObjectParam
  p.spatialReference = getSpatialReference();
  p.lengthUnits = getLengthUnits();
  p.temporalUnits = getTemporalUnits();
  p.angularUnits = getAngularUnits();
  p.dataUnits = getDataUnits();

  // H5MapParam
  Eigen::VectorXd origin = getOrigin();
  if (origin.size() == 2){
    p.X0 = origin(0);
    p.Y0 = origin(1);
  }

  Eigen::VectorXd p1 = getPoint1();
  if (p1.size() == 2){
    p.X1 = p1(0);
    p.Y1 = p1(1);
  }

  Eigen::VectorXd p2 = getPoint2();
  if (p2.size() == 2){
    p.X2 = p2(0);
    p.Y2 = p2(1);
  }

  p.nX = getNX();
  p.nY = getNY();

  ptrdiff_t xChunkSize = -1;
  ptrdiff_t yChunkSize = -1;
  auto dsetOpt = getMapD();
  if (!dsetOpt.has_value())
    return p;

  auto dsetCreateProps = dsetOpt->getCreateProps();
  if (dsetCreateProps.isChunked()){
    std::vector<hsize_t> chunkSizeVec = dsetCreateProps.getChunk(dsetOpt->getDimensions().size());
    if (chunkSizeVec.size() > 1){
      p.xChunkSize = chunkSizeVec[1];
      p.yChunkSize = chunkSizeVec[0];
    }
  }

  return p;
}

H5MapContainer* H5MapImpl::openMapContainer() const{
  h5gt::File file = getH5File();
  return h5geo::createMapContainer(
        file, h5geo::CreationType::OPEN_OR_CREATE);
}

std::optional<h5gt::DataSet>
H5MapImpl::getMapD() const
{
  std::string name = std::string{h5geo::detail::map_data};

  return getDatasetOpt(objG, name);
}
