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

bool H5VolImpl::writeData(
    Eigen::Ref<Eigen::MatrixXf> data,
    const size_t& iX0,
    const size_t& iY0,
    const size_t& iZ0,
    const size_t& nX,
    const size_t& nY,
    const size_t& nZ,
    const std::string& dataUnits)
{
  auto opt = this->getVolD();
  if (!opt.has_value())
    return false;

  std::vector<size_t> dims = opt->getDimensions();
  if (dims.size() != 3)
    return false;

  if (iX0+nX > dims[2] ||
      iY0+nY > dims[1] ||
      iZ0+nZ > dims[0])
    return false;

  std::string unitsTo = getDataUnits();
  if (!unitsTo.empty() && !dataUnits.empty()){
    double coef = units::convert(
          units::unit_from_string(dataUnits),
          units::unit_from_string(unitsTo));
    data = data*coef;
  }

  opt->select({iZ0, iY0, iX0},
              {nZ, nY, nX}).write_raw(data.data());
  return true;
}

bool H5VolImpl::readSEGYSTACK(
    const std::string& segy,
    const size_t& ilHdrOffset,
    const size_t& ilHdrSize,
    const size_t& xlHdrOffset,
    const size_t& xlHdrSize,
    const size_t& xHdrOffset,
    const size_t& xHdrSize,
    const size_t& yHdrOffset,
    const size_t& yHdrSize,
    double sampRate,
    size_t nSamp,
    size_t nTrc,
    h5geo::SegyFormat format,
    h5geo::Endian endian,
    std::function<void(double)> progressCallback)
{
  return h5geo::readSEGYSTACK(
    this, segy,
    ilHdrOffset, ilHdrSize,
    xlHdrOffset, xlHdrSize,
    xHdrOffset, xHdrSize,
    yHdrOffset, yHdrSize,
    sampRate, nSamp, nTrc,
    format, endian,
    progressCallback);
}

bool H5VolImpl::setDomain(const h5geo::Domain& val){
  return h5geo::overwriteEnumAttribute(
        objG,
        std::string{h5geo::detail::Domain},
        val);
}

bool H5VolImpl::setOrigin(
    Eigen::Ref<Eigen::Vector3d> v,
    const std::string& lengthUnits,
    const std::string& temporalUnits,
    bool doCoordTransform)
{
  std::string lengthUnitsTo = getLengthUnits();
  double coef = 1.0;
  if (!lengthUnits.empty() && !lengthUnitsTo.empty()){
    coef = units::convert(
          units::unit_from_string(lengthUnits),
          units::unit_from_string(lengthUnitsTo));

    v(0) = v(0)*coef;
    v(1) = v(1)*coef;
  }

  h5geo::Domain domain = getDomain();
  if (domain == h5geo::Domain::OWT ||
      domain == h5geo::Domain::TWT){
    std::string temporalUnitsTo = getTemporalUnits();
    if (!temporalUnits.empty() && !temporalUnitsTo.empty()){
      coef = units::convert(
            units::unit_from_string(temporalUnits),
            units::unit_from_string(temporalUnitsTo));

      v(2) = v(2)*coef;
    }
  } else {
    v(2) = v(2)*coef;
  }

#ifdef H5GEO_USE_GDAL
  if (doCoordTransform){
    // here we use `lengthUnitsTo` as `lengthUnitsFrom` as we already did units conversion
    OGRCT_ptr coordTrans(createCoordinateTransformationToWriteData(lengthUnitsTo));
    if (coordTrans){
      coordTrans->Transform(1, &v(0), &v(1));
    } else if (!coordTrans && !h5geo::sr::getIgnoreCoordTransformOnFailure()){
      return false;
    }
  }
#endif

  return h5geo::overwriteAttribute(
        objG,
        std::string{h5geo::detail::origin},
        v);
}

bool H5VolImpl::setSpacings(
    Eigen::Ref<Eigen::Vector3d> v,
    const std::string& lengthUnits,
    const std::string& temporalUnits)
{
  std::string lengthUnitsTo = getLengthUnits();
  double coef = 1.0;
  if (!lengthUnits.empty() && !lengthUnitsTo.empty()){
    coef = units::convert(
          units::unit_from_string(lengthUnits),
          units::unit_from_string(lengthUnitsTo));

    v(0) = v(0)*coef;
    v(1) = v(1)*coef;
  }

  h5geo::Domain domain = getDomain();
  if (domain == h5geo::Domain::OWT ||
      domain == h5geo::Domain::TWT){
    std::string temporalUnitsTo = getTemporalUnits();
    if (!temporalUnits.empty() && !temporalUnitsTo.empty()){
      coef = units::convert(
            units::unit_from_string(temporalUnits),
            units::unit_from_string(temporalUnitsTo));

      v(2) = v(2)*coef;
    }
  } else {
    v(2) = v(2)*coef;
  }

  return h5geo::overwriteAttribute(
        objG,
        std::string{h5geo::detail::spacings},
        v);
}

bool H5VolImpl::setOrientation(
    double val,
    const std::string& angularUnits)
{
    return h5geo::overwriteAttribute(
        objG,
        std::string{h5geo::detail::orientation},
        val, angularUnits, getAngularUnits());
}

bool H5VolImpl::resize(
    size_t nx, size_t ny, size_t nz)
{
  if (nx < 1 || ny < 1 || nz < 1)
    return false;

  auto opt = getVolD();
  if (!opt.has_value())
    return false;

  if (!opt->getCreateProps().isChunked())
    return false;

  try {
    opt->resize({nz, ny, nx});
    return true;
  } catch (h5gt::Exception e) {
    return false;
  }
}

Eigen::MatrixXf H5VolImpl::getData(
    const size_t& iX0,
    const size_t& iY0,
    const size_t& iZ0,
    const size_t& nX,
    const size_t& nY,
    const size_t& nZ,
    const std::string& dataUnits)
{
  auto opt = this->getVolD();
  if (!opt.has_value())
    return Eigen::MatrixXf();

  std::vector<size_t> dims = opt->getDimensions();
  if (dims.size() != 3)
    return Eigen::MatrixXf();

  if (iX0+nX > dims[2] ||
      iY0+nY > dims[1] ||
      iZ0+nZ > dims[0])
    return Eigen::MatrixXf();

  Eigen::MatrixXf data(nX*nY, nZ);
  opt->select({iZ0, iY0, iX0},
              {nZ, nY, nX}).read(data.data());
  if (!dataUnits.empty()){
    double coef = units::convert(
          units::unit_from_string(getDataUnits()),
          units::unit_from_string(dataUnits));
    if (!isnan(coef))
      return data*coef;

    return Eigen::MatrixXf();
  }

  return data;
}

h5geo::Domain H5VolImpl::getDomain(){
  return h5geo::readEnumAttribute<h5gt::Group, h5geo::Domain>(
          objG,
          std::string{h5geo::detail::Domain});
}

Eigen::VectorXd H5VolImpl::getOrigin(
    const std::string& lengthUnits,
    const std::string& temporalUnits,
    bool doCoordTransform)
{
  Eigen::VectorXd v = h5geo::readDoubleEigenVecAttribute(
      objG,
      std::string{h5geo::detail::origin});

  if (v.size() != 3)
    return v;

  std::string lengthUnitsFrom = getLengthUnits();
  double coef = 1.0;
  if (!lengthUnitsFrom.empty() && !lengthUnits.empty()){
    coef = units::convert(
          units::unit_from_string(lengthUnitsFrom),
          units::unit_from_string(lengthUnits));

    v(0) = v(0)*coef;
    v(1) = v(1)*coef;
  }

  h5geo::Domain domain = getDomain();
  if (domain == h5geo::Domain::OWT ||
      domain == h5geo::Domain::TWT){
    std::string temporalUnitsFrom = getTemporalUnits();
    if (!temporalUnitsFrom.empty() && !temporalUnits.empty()){
      coef = units::convert(
            units::unit_from_string(temporalUnitsFrom),
            units::unit_from_string(temporalUnits));

      v(2) = v(2)*coef;
    }
  } else {
    v(2) = v(2)*coef;
  }

#ifdef H5GEO_USE_GDAL
  if (doCoordTransform){
    // here we use `lengthUnitsFrom` as `lengthUnitsTo` as we already did units conversion
    OGRCT_ptr coordTrans(createCoordinateTransformationToReadData(lengthUnitsFrom));
    if (coordTrans){
      coordTrans->Transform(1, &v(0), &v(1));
    } else if (!coordTrans && !h5geo::sr::getIgnoreCoordTransformOnFailure()){
      return Eigen::VectorXd();
    }
  }
#endif

  return v;
}

Eigen::VectorXd H5VolImpl::getSpacings(
    const std::string& lengthUnits,
    const std::string& temporalUnits)
{
  Eigen::VectorXd v = h5geo::readDoubleEigenVecAttribute(
      objG,
      std::string{h5geo::detail::spacings});

  if (v.size() != 3)
    return v;

  std::string lengthUnitsFrom = getLengthUnits();
  double coef = 1.0;
  if (!lengthUnitsFrom.empty() && !lengthUnits.empty()){
    coef = units::convert(
          units::unit_from_string(lengthUnitsFrom),
          units::unit_from_string(lengthUnits));

    v(0) = v(0)*coef;
    v(1) = v(1)*coef;
  }

  h5geo::Domain domain = getDomain();
  if (domain == h5geo::Domain::OWT ||
      domain == h5geo::Domain::TWT){
    std::string temporalUnitsFrom = getTemporalUnits();
    if (!temporalUnitsFrom.empty() && !temporalUnits.empty()){
      coef = units::convert(
            units::unit_from_string(temporalUnitsFrom),
            units::unit_from_string(temporalUnits));

      v(2) = v(2)*coef;
    }
  } else {
    v(2) = v(2)*coef;
  }

  return v;
}

double H5VolImpl::getOrientation(
    const std::string& angularUnits)
{
  return h5geo::readDoubleAttribute(
      objG,
      std::string{h5geo::detail::orientation},
      getAngularUnits(), angularUnits);
}

size_t H5VolImpl::getNX()
{
  auto opt = this->getVolD();
  if (!opt.has_value())
    return 0;

  std::vector<size_t> dims = opt->getDimensions();
  if (dims.size() != 3)
    return 0;

  return dims[2];
}

size_t H5VolImpl::getNY()
{
  auto opt = this->getVolD();
  if (!opt.has_value())
    return 0;

  std::vector<size_t> dims = opt->getDimensions();
  if (dims.size() != 3)
    return 0;

  return dims[1];
}

size_t H5VolImpl::getNZ()
{
  auto opt = this->getVolD();
  if (!opt.has_value())
    return 0;

  std::vector<size_t> dims = opt->getDimensions();
  if (dims.size() != 3)
    return 0;

  return dims[0];
}

H5VolParam H5VolImpl::getParam()
{
  H5VolParam p;
  // H5BaseObjectParam
  p.spatialReference = getSpatialReference();
  p.lengthUnits = getLengthUnits();
  p.temporalUnits = getTemporalUnits();
  p.angularUnits = getAngularUnits();
  p.dataUnits = getDataUnits();

  // H5VolParam
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

bool H5VolImpl::exportToSEGY(
    const std::string& segyFile, 
    std::function<void(double)> progressCallback)
{
  char textHdr[40][80] = { " " };
  if (!h5geo::writeSEGYTextHeader(segyFile, textHdr, true))
    return false;

  double binHdr[30] = { 0 };
  // set sampRate
  Eigen::VectorXd spacings = this->getSpacings();
  if (spacings.size() != 3)
    return false;

  double sampRate = std::abs(spacings(2));
  h5geo::Domain domain = this->getDomain();
  if (domain == h5geo::Domain::TWT ||
      domain == h5geo::Domain::OWT){
    double tmp = std::abs(this->getSpacings("microsecond")[2]);
    if (!std::isnan(tmp))
      sampRate = tmp;
  } else if (domain == h5geo::Domain::TVD ||
             domain == h5geo::Domain::TVDSS) {
    double tmp = std::abs(this->getSpacings("", "cm")[2]);
    if (!std::isnan(tmp))
      sampRate = tmp;
  }
  binHdr[5] = sampRate;
  binHdr[6] = sampRate;
  // set nSamp
  size_t nX = this->getNX();
  size_t nY = this->getNY();
  size_t nZ = this->getNZ();
  size_t nSamp = nZ;
  binHdr[7] = double(nSamp);
  binHdr[8] = double(nSamp);
  // set SEGY format to IEEE
  binHdr[9] = 5.0;
  // fixed trace flag
  binHdr[28] = 1.0;
  if (!h5geo::writeSEGYBinHeader(segyFile, binHdr, false))
    return false;

  Eigen::VectorXd origin = this->getOrigin();
  if (origin.size() != 3)
    return false;

  double orientation = this->getOrientation("radian");
  if (std::isnan(orientation))
    return false;
  
  std::map<std::string, Eigen::VectorXd> geom = 
      h5geo::generateSTKGeometry(
          origin(0),spacings(0),nX,
          origin(1),spacings(1),nY,
          0, orientation);

  std::vector<std::string> keys;
  for (auto const& p: geom)
    keys.push_back(p.first);

  if (keys.size() < 1 || 
      geom[keys[0]].size() != nX*nY)
    return false;

  size_t N = 64;
  H5VolParam p = this->getParam();
  if (p.yChunkSize > 0 && p.yChunkSize < 100)
    N = p.yChunkSize;

  double progressOld = 0;
  double progressNew = 0;
  std::vector<std::string> trcHdrShortNames = h5geo::getTraceHeaderShortNames();
  for (size_t i = 0; i < nY; i+=N){
    if (progressCallback){
      progressNew = i / (double)nY;
      if (progressNew - progressOld >= 0.01){
        progressCallback( progressNew );
        progressOld = progressNew;
      }
    }
    if (i+N > nY)
      N = nY-i;

    Eigen::MatrixXf TRACE = this->getData(0,i,0,nX,N,nZ).
        transpose().
        colwise().reverse();
    Eigen::MatrixXd HDR = Eigen::MatrixXd::Zero(trcHdrShortNames.size(), TRACE.cols());
    for (size_t j = 0; j < trcHdrShortNames.size(); j++){
      if (std::find(keys.begin(), keys.end(),trcHdrShortNames[j])!=keys.end()){
        HDR.row(j) = geom[trcHdrShortNames[j]](Eigen::seq(i,i+nX*N-1));
      }
    }
    
    h5geo::writeSEGYTraces(segyFile, HDR, TRACE);
  }

  if (progressCallback)
    progressCallback( double(1) );
  return true;
}
