#include "../../include/h5geo/private/h5volimpl.h"
#include "../../include/h5geo/h5volcontainer.h"
#include "../../include/h5geo/h5core.h"
#include "../../include/h5geo/private/h5enum_string.h"

#include <units/units.hpp>

H5VolImpl::H5VolImpl(const h5gt::Group &group) :
  H5BaseObjectImpl(group){}

VolParam H5VolImpl::getParam()
{
  VolParam p;
  // BaseObjectParam
  p.spatialReference = getSpatialReference();
  p.lengthUnits = getLengthUnits();
  p.temporalUnits = getTemporalUnits();
  p.angularUnits = getAngularUnits();
  p.dataUnits = getDataUnits();

  // // VolParam
  // Eigen::VectorXd origin = getOrigin();
  // if (origin.size() == 3){
  //   p.X0 = origin(0);
  //   p.Y0 = origin(1);
  //   p.Z0 = origin(2);
  // }

  // p.orientation = getOrientation();

  // Eigen::VectorXd p1 = getPoint1();
  // if (p1.size() == 2){
  //   p.X1 = p1(0);
  //   p.Y1 = p1(1);
  // }

  // Eigen::VectorXd p2 = getPoint2();
  // if (p2.size() == 2){
  //   p.X2 = p2(0);
  //   p.Y2 = p2(1);
  // }

  // p.nX = getNX();
  // p.nY = getNY();

  // ptrdiff_t xChunkSize = -1;
  // ptrdiff_t yChunkSize = -1;
  // auto dsetOpt = getMapD();
  // if (!dsetOpt.has_value())
  //   return p;

  // auto dsetCreateProps = dsetOpt->getCreateProps();
  // if (dsetCreateProps.isChunked()){
  //   std::vector<hsize_t> chunkSizeVec = dsetCreateProps.getChunk(dsetOpt->getDimensions().size());
  //   if (chunkSizeVec.size() > 1){
  //     p.xChunkSize = chunkSizeVec[1];
  //     p.yChunkSize = chunkSizeVec[0];
  //   }
  // }

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
