#include "../../include/h5geo/private/h5volimpl.h"
#include "../../include/h5geo/h5volcontainer.h"
#include "../../include/h5geo/h5core.h"
#include "../../include/h5geo/private/h5enum_string.h"

#include <units/units.hpp>

#ifdef H5GEO_USE_GDAL
#include <gdal.h>
#include <gdal_priv.h>
#endif

H5VolImpl::H5VolImpl(const h5gt::Group &group) :
  H5BaseObjectImpl(group){}

bool H5VolImpl::setDomain(const h5geo::Domain& val){
  return h5geo::overwriteEnumAttribute(
        objG,
        std::string{h5geo::detail::Domain},
        val);
}

bool H5VolImpl::setOrigin(
    Eigen::Ref<Eigen::Vector3d> v,
    const std::string& lengthUnits = "",
    bool doCoordTransform = false)
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

bool H5VolImpl::setSpacings(
    Eigen::Ref<Eigen::Vector3d> v,
    const std::string& lengthUnits = "")
{
  return h5geo::overwriteAttribute(
        objG,
        std::string{h5geo::detail::spacings},
        v, lengthUnits, getLengthUnits());
}

bool H5VolImpl::setOrientation(
    double val,
    const std::string& angularUnits = "")
{
    return h5geo::overwriteAttribute(
        objG,
        std::string{h5geo::detail::orientation},
        val, angularUnits, getAngularUnits());
}

h5geo::Domain H5VolImpl::getDomain(){
  return h5geo::readEnumAttribute<h5gt::Group, h5geo::Domain>(
          objG,
          std::string{h5geo::detail::Domain});
}

Eigen::VectorXd H5VolImpl::getOrigin(
    const std::string& lengthUnits = "",
    bool doCoordTransform = false)
{
#ifdef H5GEO_USE_GDAL
  if (doCoordTransform){
    OGRCT_ptr coordTrans(createCoordinateTransformationToReadData(lengthUnits));
    if (!coordTrans)
      return Eigen::VectorXd();

    Eigen::VectorXd v = h5geo::readDoubleEigenVecAttribute(
          objG,
          std::string{h5geo::detail::origin});

    if (v.size() != 3)
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

Eigen::VectorXd H5VolImpl::getSpacings(
    const std::string& lengthUnits = "")
{
  return h5geo::readDoubleEigenVecAttribute(
      objG,
      std::string{h5geo::detail::spacings},
      getLengthUnits(), lengthUnits);
}

double H5VolImpl::getOrientation(
    const std::string& angularUnits = "")
{
  return h5geo::readDoubleAttribute(
      objG,
      std::string{h5geo::detail::spacings},
      getAngularUnits(), angularUnits);
}

size_t H5VolImpl::getNX()
{
  auto opt = this->getVolD();
  if (!opt.has_value())
    return 0;

  std::vector<size_t> dims = opt->getDimensions();
  if (dims.size() < 3)
    return 0;

  return dims[2];
}

size_t H5VolImpl::getNY()
{
  auto opt = this->getVolD();
  if (!opt.has_value())
    return 0;

  std::vector<size_t> dims = opt->getDimensions();
  if (dims.size() < 3)
    return 0;

  return dims[1];
}

size_t H5VolImpl::getNZ()
{
  auto opt = this->getVolD();
  if (!opt.has_value())
    return 0;

  std::vector<size_t> dims = opt->getDimensions();
  if (dims.size() < 3)
    return 0;

  return dims[0];
}

VolParam H5VolImpl::getParam()
{
  VolParam p;
  // BaseObjectParam
  p.spatialReference = getSpatialReference();
  p.lengthUnits = getLengthUnits();
  p.temporalUnits = getTemporalUnits();
  p.angularUnits = getAngularUnits();
  p.dataUnits = getDataUnits();

  // VolParam
  Eigen::VectorXd origin = getOrigin();
  if (origin.size() == 3){
    p.X0 = origin(0);
    p.Y0 = origin(1);
    p.Z0 = origin(2);
  }

  Eigen::VectorXd spacings = getSpacings();
  if (spacings.size() == 3){
    p.dX = spacings(0);
    p.dY = spacings(1);
    p.dZ = spacings(2);
  }

  p.orientation = getOrientation();

  p.nX = getNX();
  p.nY = getNY();
  p.nZ = getNZ();

  ptrdiff_t xChunkSize = -1;
  ptrdiff_t yChunkSize = -1;
  ptrdiff_t zChunkSize = -1;
  auto dsetOpt = getVolD();
  if (!dsetOpt.has_value())
    return p;

  auto dsetCreateProps = dsetOpt->getCreateProps();
  if (dsetCreateProps.isChunked()){
    std::vector<hsize_t> chunkSizeVec = dsetCreateProps.getChunk(dsetOpt->getDimensions().size());
    if (chunkSizeVec.size() > 2){
      p.xChunkSize = chunkSizeVec[2];
      p.yChunkSize = chunkSizeVec[1];
      p.zChunkSize = chunkSizeVec[0];
    }
  }

  return p;
}

H5VolContainer* H5VolImpl::openVolContainer() const{
  h5gt::File file = getH5File();
  return h5geo::createVolContainer(
        file, h5geo::CreationType::OPEN_OR_CREATE);
}

std::optional<h5gt::DataSet>
H5VolImpl::getVolD() const
{
  std::string name = std::string{h5geo::detail::vol_data};

  return getDatasetOpt(objG, name);
}
