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
  unsigned v = static_cast<unsigned>(val);
  return h5geo::overwriteAttribute(
        objG,
        std::string{h5geo::detail::Domain},
        v);
}

bool H5MapImpl::setOrigin(
    Eigen::Ref<Eigen::Vector2d> v,
    const std::string& lengthUnits,
    bool doCoordTransform)
{
#ifdef H5GEO_USE_GDAL
  if (doCoordTransform){
    OGRCT_ptr coordTrans(createCoordinateTransformationToWriteData(lengthUnits));
    if (!coordTrans)
      return false;

    coordTrans->Transform(1, &v(0), &v(1));
    return h5geo::overwriteAttribute(
          objG,
          std::string{h5geo::detail::origin},
          v);
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
    if (!coordTrans)
      return false;

    coordTrans->Transform(1, &v(0), &v(1));
    return h5geo::overwriteAttribute(
          objG,
          std::string{h5geo::detail::point1},
          v);
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
    if (!coordTrans)
      return false;

    coordTrans->Transform(1, &v(0), &v(1));
    return h5geo::overwriteAttribute(
          objG,
          std::string{h5geo::detail::point2},
          v);
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
  return static_cast<h5geo::Domain>(
        h5geo::readEnumAttribute(
          objG,
          std::string{h5geo::detail::Domain}));
}

Eigen::VectorXd H5MapImpl::getOrigin(
    const std::string& lengthUnits,
    bool doCoordTransform)
{
#ifdef H5GEO_USE_GDAL
  if (doCoordTransform){
    OGRCT_ptr coordTrans(createCoordinateTransformationToReadData(lengthUnits));
    if (!coordTrans)
      return Eigen::VectorXd();

    Eigen::VectorXd v = h5geo::readDoubleEigenVecAttribute(
          objG,
          std::string{h5geo::detail::origin});

    if (v.size() != 2)
      return Eigen::VectorXd();

    coordTrans->Transform(1, &v(0), &v(1));
    return v;
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
    if (!coordTrans)
      return Eigen::VectorXd();

    Eigen::VectorXd v = h5geo::readDoubleEigenVecAttribute(
          objG,
          std::string{h5geo::detail::point1});

    if (v.size() != 2)
      return Eigen::VectorXd();

    coordTrans->Transform(1, &v(0), &v(1));
    return v;
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
    if (!coordTrans)
      return Eigen::VectorXd();

    Eigen::VectorXd v = h5geo::readDoubleEigenVecAttribute(
          objG,
          std::string{h5geo::detail::point2});

    if (v.size() != 2)
      return Eigen::VectorXd();

    coordTrans->Transform(1, &v(0), &v(1));
    return v;
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

MapParam H5MapImpl::getParam()
{
  MapParam p;
  // BaseObjectParam
  p.spatialReference = getSpatialReference();
  p.lengthUnits = getLengthUnits();
  p.temporalUnits = getTemporalUnits();
  p.angularUnits = getAngularUnits();
  p.dataUnits = getDataUnits();

  // MapParam
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
      p.xChunkSize = dsetCreateProps.getChunk(dsetOpt->getDimensions().size())[1];
      p.yChunkSize = dsetCreateProps.getChunk(dsetOpt->getDimensions().size())[0];
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
