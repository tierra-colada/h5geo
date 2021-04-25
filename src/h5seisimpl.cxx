#include "../include/h5geo/misc/h5seisimpl.h"
#include "../include/h5geo/h5seiscontainer.h"
#include "../include/h5geo/h5core.h"

H5SeisImpl::H5SeisImpl(const h5gt::Group &group) :
  H5BaseObjectImpl(group),
  traceD(objG.getDataSet("trace")),
  traceHeaderD(objG.getDataSet("trace_header")){}

bool H5SeisImpl::writeTextHeader(const char (&txtHdr)[40][80]){
  auto opt = getTextHeaderD();
  if (!opt.has_value())
    return false;

  opt->write(txtHdr);
  return true;
}

bool H5SeisImpl::writeTextHeader(
    const std::vector<std::string>& txtHdr)
{
  auto opt = getTextHeaderD();
  if (!opt.has_value())
    return false;

  if (txtHdr.empty())
    return false;

  /* if use as:
   * h5gt::FixedLenStringArray<80> array(txtHdr);
   * then the array size always lesser then txtHdr size
   * by one (I don't know why) */
  h5gt::FixedLenStringArray<80> array;
  for(size_t i = 0; i < getNTextHdrRows(); i++){
    if (i < txtHdr.size())
      array.push_back(txtHdr[i]);
    else
      array.push_back("");
  }

  opt->write(array);
  return true;
}

bool H5SeisImpl::writeBinHeader(const double (&binHdr)[30])
{
  auto opt = getBinHeaderD();
  if (!opt.has_value())
    return false;

  opt->write(binHdr);
  return true;
}

bool H5SeisImpl::writeBinHeader(
    const std::vector<double> &binHdrVec)
{
  auto opt = getBinHeaderD();
  if (!opt.has_value())
    return false;

  if (binHdrVec.size() != getNBinHdr())
    return false;

  opt->write(binHdrVec);
  return true;
}

bool H5SeisImpl::writeBinHeader(
    const Eigen::VectorXd& binHdrVec)
{
  auto opt = getBinHeaderD();
  if (!opt.has_value())
    return false;

  if (binHdrVec.size() != getNBinHdr())
    return false;

  opt->write_raw(binHdrVec.data());
  return true;
}

bool H5SeisImpl::writeBinHeader(
    const std::string& hdrName,
    const double& value)
{
  auto opt = getBinHeaderD();
  if (!opt.has_value())
    return false;

  ptrdiff_t ind = getBinHeaderIndex(hdrName);

  if (ind < 0)
    return false;

  opt->select({size_t(ind)}, {1}).write(value);
  return true;
}

bool H5SeisImpl::writeBoundary(
    const Eigen::MatrixX2d& boundary)
{
  auto opt = getBoundaryD();
  if (!opt.has_value())
    return false;

  std::vector<size_t> dims = {
    size_t(boundary.cols()), size_t(boundary.rows())};
  opt->resize(dims);
  opt->write_raw(boundary.data());
  return true;
}

bool H5SeisImpl::writeTrace(
    const Eigen::Ref<const Eigen::MatrixXf>& TRACE,
    const size_t& fromTrc,
    const size_t& fromSampInd)
{
  if (fromTrc+TRACE.cols() > getNTrc())
    return false;

  if (TRACE.rows()+fromSampInd > getNSamp())
    return false;

  traceD.select({fromTrc, fromSampInd},
                {(size_t)TRACE.cols(),
                 (size_t)TRACE.rows()}).write_raw(TRACE.data());
  return true;
}

bool H5SeisImpl::writeTraceHeader(
    const Eigen::Ref<const Eigen::MatrixXd>& HDR,
    const size_t& fromTrc,
    const size_t& fromHdrInd)
{
  if (fromTrc+HDR.rows() > getNTrc())
    return false;

  if (HDR.cols()+fromHdrInd > getNTrcHdr())
    return false;

  traceHeaderD.select({fromHdrInd, fromTrc},
                      {(size_t)HDR.cols(),
                       (size_t)HDR.rows()}).write_raw(HDR.data());
  return true;
}

bool H5SeisImpl::writeTraceHeader(
    const std::string& hdrName,
    const Eigen::Ref<const Eigen::MatrixXd>& hdr,
    const size_t& fromTrc)
{
  size_t ind = getTraceHeaderIndex(hdrName);
  return writeTraceHeader(hdr, fromTrc, ind);
}

std::vector<std::string>
H5SeisImpl::getTextHeader()
{
  auto opt = getTextHeaderD();
  if (!opt.has_value())
    return {std::string()};

  h5gt::FixedLenStringArray<80> array;

  opt->read(array);

  if (array.empty())
    return std::vector<std::string>();

  std::vector<std::string> v;
  v.reserve(array.size());
  for(size_t i = 0; i < array.size(); i++)
    v.push_back(array.getString(i));
  v.shrink_to_fit();
  return v;
}

Eigen::VectorXd H5SeisImpl::getBinHeader()
{
  auto opt = getBinHeaderD();
  if (!opt.has_value())
    return Eigen::VectorXd();

  Eigen::VectorXd binHdr(getNBinHdr());
  opt->read(binHdr.data());
  return binHdr;
}

double H5SeisImpl::getBinHeader(
    const std::string& hdrName)
{
  auto opt = getBinHeaderD();
  if (!opt.has_value())
    return NAN;

  ptrdiff_t ind = getBinHeaderIndex(hdrName);

  if (ind < 0)
    return NAN;

  double hdr;
  opt->select({size_t(ind)}, {1}).read(hdr);
  return hdr;
}

Eigen::MatrixXf H5SeisImpl::getTrace(
    const size_t& fromTrc, size_t nTrc,
    const size_t& fromSampInd, size_t nSamp)
{
  if (!checkTraceLimits(fromTrc, nTrc))
    return Eigen::MatrixXf();

  if (!checkSampleLimits(fromSampInd, nSamp))
    return Eigen::MatrixXf();

  Eigen::MatrixXf TRACE(nSamp, nTrc);

  std::vector<size_t> offset({fromTrc, fromSampInd});
  std::vector<size_t> count({nTrc, nSamp});

  traceD.select(offset, count).read(TRACE.data());
  return TRACE;
}

Eigen::MatrixXd H5SeisImpl::getTraceHeader(
    const size_t& fromTrc,
    size_t nTrc,
    const size_t& fromHdr,
    size_t nHdr)
{
  if (!checkTraceLimits(fromTrc, nTrc))
    return Eigen::VectorXd();

  if (!checkTraceHeaderLimits(fromHdr, nHdr))
    return Eigen::VectorXd();

  Eigen::MatrixXd HDR(nTrc, nHdr);

  std::vector<size_t> offset({fromHdr, fromTrc});
  std::vector<size_t> count({nHdr, nTrc});

  traceHeaderD.select(offset, count).read(HDR.data());
  return HDR;
}

Eigen::VectorXd H5SeisImpl::getTraceHeader(
    const std::string& hdrName,
    const size_t& fromTrc,
    const size_t& nTrc)
{
  size_t ind = getTraceHeaderIndex(hdrName);
  return getTraceHeader(fromTrc, nTrc, ind, 1);
}

Eigen::VectorX<size_t> H5SeisImpl::getSortedData(
    Eigen::MatrixXf& TRACE,
    Eigen::MatrixXd& HDR,
    const std::vector<std::string>& keyList,
    const std::vector<double>& minList,
    const std::vector<double>& maxList,
    size_t fromSampInd,
    size_t nSamp)
{
  if (keyList.empty() || minList.empty() || maxList.empty())
    return Eigen::VectorX<size_t>();

  if (keyList.size() != minList.size() ||
      keyList.size() != maxList.size() ||
      minList.size() != maxList.size())
    return Eigen::VectorX<size_t>();

  // define trace and header indexes, convert them to ElementSet
  // and read preliminary PKey indexed headers
  Eigen::VectorX<size_t> traceIndexes = getTracePKeyIndexes(
        keyList[0], minList[0], maxList[0]);

  if (traceIndexes.size() <= 0)
    return Eigen::VectorX<size_t>();

  Eigen::VectorX<size_t> headerIndex(keyList.size());
  for (size_t i = 0; i < keyList.size(); i++){
    ptrdiff_t ind = getTraceHeaderIndex(keyList[i]);
    if (ind < 0)
      return Eigen::VectorX<size_t>();

    headerIndex[i] = ind;
  }

  if (traceIndexes.size() <= 0)
    return Eigen::VectorX<size_t>();

  h5gt::ElementSet hdrElSet = h5geo::rowCol2ElementSet(
        headerIndex, traceIndexes);

  Eigen::MatrixXd tmpHDR(
        traceIndexes.size(), headerIndex.size());

  traceHeaderD.select(hdrElSet).read(tmpHDR.data());

  // take in account SKeys and remove unecessary rows (traces),
  // correct 'traceIndexes'
  Eigen::VectorX<bool> idx_bool = Eigen::VectorX<bool>::Ones(tmpHDR.rows());
  for (size_t i = 1; i < keyList.size(); i++){
    idx_bool = idx_bool.array() && (
          tmpHDR.col(i).array() >= minList[i] &&
          tmpHDR.col(i).array() <= maxList[i]);
  }

  Eigen::VectorX<ptrdiff_t> idx = h5geo::find_index(idx_bool);

  for (ptrdiff_t i = 0; i < idx.size(); i++){
    tmpHDR.row(i) = tmpHDR.row(idx(i));
    traceIndexes(i) = traceIndexes(idx(i));
  }
  tmpHDR.conservativeResize(idx.size(), Eigen::NoChange);
  traceIndexes.conservativeResize(idx.size());

  if (tmpHDR.size() == 0 || traceIndexes.size() == 0)
    return Eigen::VectorX<size_t>();

  Eigen::MatrixXd urows;
  Eigen::MatrixX2<ptrdiff_t> urows_from_size;
  Eigen::VectorX<ptrdiff_t> sortInd = h5geo::sort_rows_unique(
        tmpHDR, urows, urows_from_size);

  HDR = tmpHDR(sortInd, Eigen::all);
  traceIndexes = traceIndexes(sortInd);

  if (fromSampInd >= getNSamp() || nSamp == 0)
    return traceIndexes;

  checkSampleLimits(fromSampInd, nSamp);
  TRACE.resize(nSamp, HDR.rows());
  Eigen::VectorX<float> tmpTrace(nSamp);
  for (ptrdiff_t i = 0; i < traceIndexes.size(); i++){
    TRACE.col(i) = H5SeisImpl::getTrace(
          traceIndexes(i), 1, fromSampInd, nSamp);
  }

  return traceIndexes;
}

ptrdiff_t H5SeisImpl::getBinHeaderIndex(
    const std::string& hdrName)
{
  auto opt = getBinHeaderD();
  if (!opt.has_value())
    return -1;

  ptrdiff_t idx = h5geo::getIndexFromAttribute(opt.value(), hdrName);
  if (idx >= getNBinHdr())
    return -1;

  return idx;
}

ptrdiff_t H5SeisImpl::getTraceHeaderIndex(
    const std::string& hdrName)
{
  ptrdiff_t idx = h5geo::getIndexFromAttribute(traceHeaderD, hdrName);
  if (idx >= getNTrcHdr())
    return -1;

  return idx;
}

Eigen::VectorX<size_t> H5SeisImpl::getTracePKeyIndexes(
    const std::string& pName,
    const double& pMin, const double& pMax)
{
  auto optUValG = getUValG();
  if (!optUValG.has_value())
    return Eigen::VectorX<size_t>();

  auto optIndexesG = getIndexesG();
  if (!optIndexesG.has_value())
    return Eigen::VectorX<size_t>();

  if (!optIndexesG->exist(pName) ||
      optIndexesG->getObjectType(pName) != h5gt::ObjectType::Group)
    return Eigen::VectorX<size_t>();

  h5gt::Group pGroup = optIndexesG->getGroup(pName);
  std::vector<double> uHeader(pGroup.getNumberObjects());
  optUValG->getDataSet(pName).read(uHeader);
  std::vector<std::string> pDatasetsNames;
  pDatasetsNames.reserve(pGroup.getNumberObjects());
  for (size_t i = 0; i < uHeader.size(); i++){
    if (uHeader[i] >= pMin &&
        uHeader[i] <= pMax){
      pDatasetsNames.push_back(std::to_string(i));
    }
  }
  pDatasetsNames.shrink_to_fit();

  size_t pKeyCount = 0;
  for (size_t i = 0; i < pDatasetsNames.size(); i++)
    pKeyCount = pGroup.getDataSet(pDatasetsNames[i]).getElementCount() + pKeyCount;

  /* tracePKeyIndexes and hdrColVec defines header data that needs to be extracted from h5-file */
  Eigen::VectorX<size_t> tracePKeyIndexes(pKeyCount);
  size_t n = 0;
  for (const std::string& name : pDatasetsNames){
    pGroup.getDataSet(name).read(&tracePKeyIndexes(n));
    n = pGroup.getDataSet(name).getElementCount() + n;
  }

  return tracePKeyIndexes;
}

Eigen::VectorXd H5SeisImpl::getSamples(const size_t& trcInd){
  double firstSamp = getFirstSample(trcInd);

  if (isnan(firstSamp))
    return Eigen::VectorXd();

  double sampRate = getSampRate();

  if (isnan(sampRate))
    return Eigen::VectorXd();

  size_t nSamp = getNSamp();

  if (nSamp == 0)
    return Eigen::VectorXd();

  Eigen::VectorXd samp = Eigen::ArrayXd::LinSpaced(
        nSamp, firstSamp, (nSamp-1)*sampRate+firstSamp);
  return samp;
}

double H5SeisImpl::getFirstSample(const size_t& trcInd){
  Eigen::VectorXd firstSamp = getTraceHeader("DELRECT", trcInd, 1);

  if (firstSamp.size() == 0)
    return NAN;

  return firstSamp(0);
}

double H5SeisImpl::getLastSample(const size_t& trcInd){
  return getSamples(trcInd)(Eigen::last);
}

double H5SeisImpl::getSampRate(){
  double sampRate = getBinHeader("SAMP_RATE");
  return sampRate;
}

size_t H5SeisImpl::getNSamp(){
  std::vector<size_t> dims = traceD.getDimensions();
  return dims[1];
}

size_t H5SeisImpl::getNTrc(){
  std::vector<size_t> dims = traceD.getDimensions();
  return dims[0];
}

size_t H5SeisImpl::getNTrcHdr(){
  std::vector<size_t> dims = traceHeaderD.getDimensions();
  return dims[0];
}

size_t H5SeisImpl::getNBinHdr(){
  auto opt = getBinHeaderD();
  if (!opt.has_value())
    return 0;

  return opt->getElementCount();
}

size_t H5SeisImpl::getNTextHdrRows(){
  auto opt = getTextHeaderD();
  if (!opt.has_value())
    return 0;

  return opt->getDimensions()[0];
}

std::vector<std::string> H5SeisImpl::getPKeyNames(){
  auto opt = getIndexesG();
  if (!opt.has_value())
    return {""};

  return opt->listObjectNames();
}

Eigen::VectorXd H5SeisImpl::getTraceHeaderMin(){
  if (!traceHeaderD.hasAttribute("min"))
    return Eigen::VectorXd();

  Eigen::VectorXd hdr(getNTrcHdr());
  traceHeaderD.getAttribute("min").read(hdr.data());
  return hdr;
}

Eigen::VectorXd H5SeisImpl::getTraceHeaderMax(){
  if (!traceHeaderD.hasAttribute("max"))
    return Eigen::VectorXd();

  Eigen::VectorXd hdr(getNTrcHdr());
  traceHeaderD.getAttribute("max").read(hdr.data());
  return hdr;
}

double H5SeisImpl::getTraceHeaderMin(
    const std::string& hdrName)
{
  int ind = getTraceHeaderIndex(hdrName);

  if (ind < 0)
    return NAN;

  Eigen::VectorXd hdr = getTraceHeaderMin();

  if (ind >= hdr.size())
    return NAN;

  return hdr(ind);
}

double H5SeisImpl::getTraceHeaderMax(
    const std::string& hdrName)
{
  int ind = getTraceHeaderIndex(hdrName);

  if (ind < 0)
    return NAN;

  Eigen::VectorXd hdr = getTraceHeaderMax();

  if (ind >= hdr.size())
    return NAN;

  return hdr(ind);
}

Eigen::MatrixX2d H5SeisImpl::calcBoundaryStk2D(){
  Eigen::MatrixX3d hdr(getNTrc(), 3), hdrSorted(getNTrc(), 3);
  hdr.col(0) = getTraceHeader("CDP", 0, getNTrc());
  hdr.col(1) = getTraceHeader("CDP_X", 0, getNTrc());
  hdr.col(2) = getTraceHeader("CDP_Y", 0, getNTrc());

  h5geo::sort_rows(hdr, hdrSorted);

  return hdrSorted.rightCols(2);
}

Eigen::MatrixX2d H5SeisImpl::calcConvexHullBoundary(){
  Eigen::MatrixX2d hdr(getNTrc(), 2);
  hdr.col(0) = getTraceHeader("CDP_X", 0, getNTrc());
  hdr.col(1) = getTraceHeader("CDP_Y", 0, getNTrc());

  return h5geo::quickHull2D(hdr);
}

bool H5SeisImpl::calcBinOriginOrientation3DStk(
    Eigen::Ref<Eigen::Vector2d> bin,
    Eigen::Ref<Eigen::Vector2d> origin,
    double& orientation)
{
  double minIL = getTraceHeaderMin("INLINE");
  double maxIL = getTraceHeaderMax("INLINE");
  double minXL = getTraceHeaderMin("XLINE");
  double maxXL = getTraceHeaderMax("XLINE");
  double minX = getTraceHeaderMin("CDP_X");
  double maxX = getTraceHeaderMax("CDP_X");
  double minY = getTraceHeaderMin("CDP_Y");
  double maxY = getTraceHeaderMax("CDP_Y");

  if (isnan(minIL) || isnan(maxIL) ||
      isnan(minXL) || isnan(maxXL) ||
      isnan(minX) || isnan(maxX) ||
      isnan(minY) || isnan(maxY))
    return false;

  std::vector<std::string> keyList({"INLINE", "CDP_X", "CDP_Y"});
  /* get first INLINE */
  std::vector<double> minList({minIL, minX, minY});
  std::vector<double> maxList({minIL, maxX, maxY});

  Eigen::MatrixXf TRACE;
  Eigen::MatrixXd HDR;

  getSortedData(
        TRACE, HDR,
        keyList, minList, maxList,
        0, 0);

  origin.resize(2);
  origin(0) = HDR(0, 1);
  origin(1) = HDR(0, 2);

  double dx_iL = ( HDR(Eigen::last, 1)-HDR(0, 1) ) / ( HDR.rows()-1 );
  double dy_iL = ( HDR(Eigen::last, 2)-HDR(0, 2) ) / ( HDR.rows()-1 );

  keyList[0] = "XLINE";
  /* get first XLINE */
  minList[0] = minXL;
  maxList[0] = minXL;

  getSortedData(
        TRACE, HDR,
        keyList, minList, maxList,
        0, 0);

  double dx_xL = ( HDR(Eigen::last-1, 1)-HDR(0, 1) ) / ( HDR.rows()-1 );
  double dy_xL = ( HDR(Eigen::last-1, 2)-HDR(0, 2) ) / ( HDR.rows()-1 );

  double bin_iL = std::sqrt(std::pow(dx_iL,2)+std::pow(dy_iL,2));
  double bin_xL = std::sqrt(std::pow(dx_xL,2)+std::pow(dy_xL,2));

  bin.resize(2);
  bin(0) = bin_iL;
  bin(1) = bin_xL;

  double cos_alpha_iL = dx_iL/bin_iL;
  orientation = 180*std::acos(cos_alpha_iL)/M_PI; // orientation in degrees

  return true;
}

bool H5SeisImpl::calcBinOriginOrientation3DStk(
    std::vector<double>& bin,
    std::vector<double>& origin,
    double& orientation)
{
  double minIL = getTraceHeaderMin("INLINE");
  double maxIL = getTraceHeaderMax("INLINE");
  double minXL = getTraceHeaderMin("XLINE");
  double maxXL = getTraceHeaderMax("XLINE");
  double minX = getTraceHeaderMin("CDP_X");
  double maxX = getTraceHeaderMax("CDP_X");
  double minY = getTraceHeaderMin("CDP_Y");
  double maxY = getTraceHeaderMax("CDP_Y");

  if (isnan(minIL) || isnan(maxIL) ||
      isnan(minXL) || isnan(maxXL) ||
      isnan(minX) || isnan(maxX) ||
      isnan(minY) || isnan(maxY))
    return false;

  std::vector<std::string> keyList({"INLINE", "CDP_X", "CDP_Y"});
  /* get first INLINE */
  std::vector<double> minList({minIL, minX, minY});
  std::vector<double> maxList({minIL, maxX, maxY});

  Eigen::MatrixXf TRACE;
  Eigen::MatrixXd HDR;

  getSortedData(
        TRACE, HDR,
        keyList, minList, maxList,
        0, 0);

  origin.resize(2);
  origin[0] = HDR(0, 1);
  origin[1] = HDR(0, 2);

  double dx_iL = ( HDR(Eigen::last, 1)-HDR(0, 1) ) / ( HDR.rows()-1 );
  double dy_iL = ( HDR(Eigen::last, 2)-HDR(0, 2) ) / ( HDR.rows()-1 );

  keyList[0] = "XLINE";
  /* get first XLINE */
  minList[0] = minXL;
  maxList[0] = minXL;

  getSortedData(
        TRACE, HDR,
        keyList, minList, maxList,
        0, 0);

  double dx_xL = ( HDR(Eigen::last, 1)-HDR(0, 1) ) / ( HDR.rows()-1 );
  double dy_xL = ( HDR(Eigen::last, 2)-HDR(0, 2) ) / ( HDR.rows()-1 );

  double bin_iL = std::sqrt(std::pow(dx_iL,2)+std::pow(dy_iL,2));
  double bin_xL = std::sqrt(std::pow(dx_xL,2)+std::pow(dy_xL,2));

  bin.resize(2);
  bin[0] = bin_iL;
  bin[1] = bin_xL;

  double cos_alpha_iL = dx_iL/bin_iL;
  orientation = 180*std::acos(cos_alpha_iL)/M_PI; // orientation in degrees

  return true;
}

bool H5SeisImpl::calcAndWriteBoundary(){
  auto opt = getBoundaryD();
  if (!opt.has_value())
    return false;

  Eigen::MatrixX2d boundary;


  if (getDataType() == h5geo::SeisDataType::STACK &&
      getSurveyType() == h5geo::SurveyType::TWO_D){
    boundary = calcBoundaryStk2D();
    if (boundary.size() == 0)
      return false;
  } else {
    boundary = calcConvexHullBoundary();
    if (boundary.size() == 0)
      return false;

    if (getDataType() == h5geo::SeisDataType::STACK &&
        getSurveyType() == h5geo::SurveyType::THREE_D){
      std::vector<double> bin, origin;
      double orientation = 0;
      calcBinOriginOrientation3DStk(bin, origin, orientation);
      if (!objG.hasAttribute("bin"))
        objG.createAttribute<double>("bin", h5gt::DataSpace::From(bin)).write(bin);
      if (!objG.hasAttribute("origin"))
        objG.createAttribute<double>("origin", h5gt::DataSpace::From(origin)).write(origin);
      if (!objG.hasAttribute("orientation"))
        objG.createAttribute<double>("orientation", h5gt::DataSpace(1)).write(orientation);
    }
  }
  std::vector<size_t> dims{2, (size_t)boundary.rows()};
  opt->resize({2, (size_t)boundary.rows()});
  opt->write(boundary.data());

  return true;
}

bool H5SeisImpl::calcAndWriteTraceHeaderLimits(
    const size_t& nTrcBuffer)
{
  size_t nHdr = getNTrcHdr();
  Eigen::VectorXd hdr, minHdr(nHdr), maxHdr(nHdr);
  for (size_t i = 0; i < nHdr; i++){
    size_t fromTrc = 0;
    double min = INFINITY, max = -INFINITY;
    do {
      hdr = getTraceHeader(
            fromTrc, nTrcBuffer, i, 1);
      fromTrc += nTrcBuffer;

      min = std::min(min, hdr.minCoeff());
      max = std::max(max, hdr.maxCoeff());
    } while (hdr.size() > 0);

    minHdr(i) = min;
    maxHdr(i) = max;
  }
  if (!h5geo::overwriteAttribute(traceHeaderD, "min", minHdr))
    return false;

  if (!h5geo::overwriteAttribute(traceHeaderD, "max", maxHdr))
    return false;

  return true;
}

bool H5SeisImpl::checkTraceLimits(
    const size_t& fromTrc, size_t& nTrc)
{
  size_t NTrc = getNTrc();

  if (fromTrc > NTrc)
    return false;

  if (fromTrc+nTrc > NTrc)
    nTrc = NTrc-fromTrc;

  return true;
}

bool H5SeisImpl::checkTraceHeaderLimits(
    const size_t& fromHdr, size_t& nHdr)
{
  size_t NHdr = getNTrcHdr();

  if (fromHdr > NHdr)
    return false;

  if (fromHdr+nHdr > NHdr)
    nHdr = NHdr-fromHdr;

  return true;
}

bool H5SeisImpl::checkSampleLimits(
    const size_t& fromSampInd, size_t& nSamp)
{
  size_t NSamp = getNSamp();

  if (fromSampInd > NSamp)
    return false;

  if (fromSampInd+nSamp > NSamp)
    nSamp = NSamp-fromSampInd;

  return true;
}

h5geo::SeisDataType H5SeisImpl::getDataType(){
  return static_cast<h5geo::SeisDataType>(
        h5geo::getEnumFromObj(
          objG,
          std::string{magic_enum::enum_name(h5geo::SeisAttributes::SeisDataType)}));
}

h5geo::SurveyType H5SeisImpl::getSurveyType(){
  return static_cast<h5geo::SurveyType>(
        h5geo::getEnumFromObj(
          objG,
          std::string{magic_enum::enum_name(h5geo::SeisAttributes::SurveyType)}));
}

bool H5SeisImpl::hasPKeySort(const std::string& pKeyName)
{
  auto optUValG = getUValG();
  if (!optUValG.has_value())
    return false;

  auto optIndexesG = getIndexesG();
  if (!optIndexesG.has_value())
    return false;

  if (optUValG->exist(pKeyName) &&
      optUValG->getObjectType(pKeyName) ==
      h5gt::ObjectType::Dataset &&
      optIndexesG->exist(pKeyName) &&
      optIndexesG->getObjectType(pKeyName) ==
      h5gt::ObjectType::Group &&
      optIndexesG->getGroup(pKeyName).getNumberObjects() > 0)
    return true;

  return false;
}

bool H5SeisImpl::removePKeySort(const std::string& pKeyName){
  auto optUValG = getUValG();
  if (!optUValG.has_value())
    return false;

  auto optIndexesG = getIndexesG();
  if (!optIndexesG.has_value())
    return false;

  if (optUValG->exist(pKeyName))
    optUValG->unlink(pKeyName);

  if (optIndexesG->exist(pKeyName))
    optIndexesG->unlink(pKeyName);

  return true;
}

bool H5SeisImpl::addPKeySort(const std::string& pKeyName){
  auto optUValG = getUValG();
  if (!optUValG.has_value())
    return false;

  auto optIndexesG = getIndexesG();
  if (!optIndexesG.has_value())
    return false;

  Eigen::VectorXd hdr = getTraceHeader(pKeyName, 0, getNTrc());
  if (hdr.size() == 0)
    return false;

  if (hasPKeySort(pKeyName))
    removePKeySort(pKeyName);

  Eigen::VectorXd uhdr;
  Eigen::MatrixX2<ptrdiff_t> uhdr_from_size;

  Eigen::VectorX<ptrdiff_t> idx = h5geo::sort_unique(
        hdr, uhdr, uhdr_from_size);

  // create and write unique header values
  h5gt::DataSpace uhdrS({(size_t)uhdr.size()});
  h5gt::DataSet uhdrD = optUValG->createDataSet<double>(
        pKeyName, uhdrS);

  uhdrD.write_raw(uhdr.data());

  // create and write indexes for every unique header value
  h5gt::Group uidxG = optIndexesG->createGroup(pKeyName);
  for (ptrdiff_t i = 0; i < uhdr_from_size.rows(); i++){
    Eigen::VectorX<ptrdiff_t> uidx = idx.segment(
          uhdr_from_size(i,0), uhdr_from_size(i,1));
    h5gt::DataSpace uidxS({(size_t)uidx.size()});
    uidxG.createDataSet<ptrdiff_t>(
          std::to_string(i), uidxS).write_raw(uidx.data());
  }
  return true;
}

H5SeisContainer* H5SeisImpl::getSeisContainer(){
  h5gt::File file = getH5File();
  return h5geo::createSeisContainer(
        file, h5geo::CreationType::OPEN_OR_CREATE);
}

std::optional<h5gt::DataSet>
H5SeisImpl::getBoundaryD()
{
  std::string name = std::string{magic_enum::enum_name(
        h5geo::SeisDatasets::boundary)};

  return getDatasetOpt(objG, name);
}

std::optional<h5gt::DataSet>
H5SeisImpl::getTextHeaderD()
{
  std::string name = std::string{magic_enum::enum_name(
        h5geo::SeisDatasets::text_header)};

  return getDatasetOpt(objG, name);
}

std::optional<h5gt::DataSet>
H5SeisImpl::getBinHeaderD()
{
  std::string name = std::string{magic_enum::enum_name(
        h5geo::SeisDatasets::bin_header)};

  return getDatasetOpt(objG, name);
}

std::optional<h5gt::DataSet>
H5SeisImpl::getTraceHeaderD()
{
  return traceHeaderD;
}

std::optional<h5gt::DataSet>
H5SeisImpl::getTraceD()
{
  return traceD;
}

std::optional<h5gt::Group>
H5SeisImpl::getSortG()
{
  std::string name = std::string{magic_enum::enum_name(
        h5geo::SeisGroups::sort)};

  return getGroupOpt(objG, name);
}

std::optional<h5gt::Group>
H5SeisImpl::getUValG()
{
  auto opt = getSortG();
  if (!opt.has_value())
    return std::nullopt;

  std::string name = std::string{magic_enum::enum_name(
        h5geo::SeisGroups::unique_values)};

  if (!opt->hasObject(name, h5gt::ObjectType::Group))
    return std::nullopt;

  return opt->getGroup(name);
}

std::optional<h5gt::Group>
H5SeisImpl::getIndexesG()
{
  auto opt = getSortG();
  if (!opt.has_value())
    return std::nullopt;

  std::string name = std::string{magic_enum::enum_name(
        h5geo::SeisGroups::indexes)};

  if (!opt->hasObject(name, h5gt::ObjectType::Group))
    return std::nullopt;

  return opt->getGroup(name);
}
