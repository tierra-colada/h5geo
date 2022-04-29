#include "../../include/h5geo/impl/h5seisimpl.h"
#include "../../include/h5geo/h5seiscontainer.h"
#include "../../include/h5geo/misc/h5core.h"
#include "../../include/h5geo/misc/h5segy.h"

#include <fstream>
#include <climits>
#include <filesystem>

#include <units/units.hpp>

#ifdef H5GEO_USE_GDAL
#include <gdal.h>
#include <gdal_priv.h>
#endif

H5SeisImpl::H5SeisImpl(const h5gt::Group &group) :
  H5BaseObjectImpl(group),
  traceD(objG.getDataSet("trace")),
  traceHeaderD(objG.getDataSet("trace_header")){}

bool H5SeisImpl::readSEGYTextHeader(const std::string& segy)
{
  char txtHdr[40][80];
  if (!h5geo::readSEGYTextHeader(segy, txtHdr))
    return false;

  return this->writeTextHeader(txtHdr);
}

bool H5SeisImpl::readSEGYBinHeader(const std::string& segy)
{
  double binHdr[30];
  if (!h5geo::readSEGYBinHeader(segy, binHdr))
    return false;

  return this->writeBinHeader(binHdr);
}

bool H5SeisImpl::readSEGYTraces(
    const std::vector<std::string>& segyFiles,
    size_t trcBuffer)
{
  if (segyFiles.size() < 1)
    return false;

  size_t nSamp = h5geo::getSEGYNSamp(segyFiles[0]);
  if (nSamp < 1)
    return false;

  size_t nTrcOverall = 0;
  std::vector<size_t> nTrcVec;
  for (size_t i = 0; i < segyFiles.size(); i++){
    if (h5geo::getSEGYNSamp(segyFiles[i]) != nSamp)
      return false;

    size_t nTrc = h5geo::getSEGYNTrc(segyFiles[i]);
    if (nTrc < 1)
      return false;

    nTrcOverall += nTrc;
    nTrcVec.push_back(nTrc);
  }

  size_t fromTrc = 0;
  for (size_t i = 0; i < segyFiles.size(); i++){
    bool val = h5geo::readSEGYTraces(
          this,
          segyFiles[i],
          nSamp,
          nTrcVec[i],
          fromTrc,
          trcBuffer);

    if (!val)
      return false;

    fromTrc += nTrcVec[i];
  }

  return true;
}

bool H5SeisImpl::writeTextHeader(const char (&txtHdr)[40][80]){
  auto opt = getTextHeaderD();
  if (!opt.has_value())
    return false;

  opt->write(txtHdr);
  objG.flush();
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
  objG.flush();
  return true;
}

bool H5SeisImpl::writeBinHeader(const double (&binHdr)[30])
{
  auto opt = getBinHeaderD();
  if (!opt.has_value())
    return false;

  opt->write(binHdr);
  objG.flush();
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
  objG.flush();
  return true;
}

bool H5SeisImpl::writeBinHeader(
    const Eigen::Ref<const Eigen::VectorXd>& binHdrVec)
{
  auto opt = getBinHeaderD();
  if (!opt.has_value())
    return false;

  if (binHdrVec.size() != getNBinHdr())
    return false;

  opt->write_raw(binHdrVec.data());
  objG.flush();
  return true;
}

bool H5SeisImpl::writeBinHeader(
    const std::string& hdrName,
    const double& value,
    const std::string& unitsFrom,
    const std::string& unitsTo)
{
  auto opt = getBinHeaderD();
  if (!opt.has_value())
    return false;

  ptrdiff_t ind = getBinHeaderIndex(hdrName);

  if (ind < 0)
    return false;

  if (!unitsFrom.empty() && !unitsTo.empty()){
    double coef = units::convert(
          units::unit_from_string(unitsFrom),
          units::unit_from_string(unitsTo));
    opt->select({size_t(ind)}, {1}).write(value*coef);
    return true;
  }

  opt->select({size_t(ind)}, {1}).write(value);
  return true;
}

bool H5SeisImpl::writeBoundary(
    Eigen::Ref<Eigen::MatrixX2d> M,
    const std::string& lengthUnits,
    bool doCoordTransform)
{
  auto opt = getBoundaryD();
  if (!opt.has_value())
    return false;

  bool val;
#ifdef H5GEO_USE_GDAL
  if (doCoordTransform){
    OGRCT_ptr coordTrans(createCoordinateTransformationToWriteData(lengthUnits));
    if (!coordTrans)
      return false;

    coordTrans->Transform(M.rows(), M.col(0).data(), M.col(1).data());
    val = h5geo::overwriteResizableDataset(
          objG,
          opt->getPath(),
          M);

    objG.flush();
    return val;
  }
#endif

  val = h5geo::overwriteResizableDataset(
        objG,
        opt->getPath(),
        M,
        lengthUnits, getDataUnits());

  objG.flush();
  return val;
}

bool H5SeisImpl::writeTrace(
    Eigen::Ref<Eigen::MatrixXf> TRACE,
    const size_t& fromTrc,
    const size_t& fromSampInd,
    const std::string& dataUnits)
{
  if (fromTrc+TRACE.cols() > getNTrc())
    return false;

  if (TRACE.rows()+fromSampInd > getNSamp())
    return false;

  std::string unitsTo = getDataUnits();
  if (!unitsTo.empty() && !dataUnits.empty()){
    double coef = units::convert(
          units::unit_from_string(dataUnits),
          units::unit_from_string(unitsTo));
    TRACE = TRACE*coef;
  }

  traceD.select({fromTrc, fromSampInd},
                {(size_t)TRACE.cols(),
                 (size_t)TRACE.rows()}).write_raw(TRACE.data());
  return true;
}

bool H5SeisImpl::writeTrace(
    Eigen::Ref<Eigen::MatrixXf> TRACE,
    const Eigen::Ref<const Eigen::VectorX<size_t>>& trcInd,
    const size_t& fromSampInd,
    const std::string& dataUnits)
{
  if (TRACE.cols() != trcInd.size())
    return false;

  if (trcInd.maxCoeff() >= getNTrc())
    return false;

  if (TRACE.rows()+fromSampInd > getNSamp())
    return false;

  std::string unitsTo = getDataUnits();
  if (!unitsTo.empty() && !dataUnits.empty()){
    double coef = units::convert(
          units::unit_from_string(dataUnits),
          units::unit_from_string(unitsTo));
    TRACE = TRACE*coef;
  }

  Eigen::VectorX<ptrdiff_t> ind = h5geo::sort(trcInd);
  Eigen::VectorX<size_t> trcInd_sorted = trcInd(ind);
  bool hasDuplicates = std::adjacent_find(
        trcInd_sorted.begin(), trcInd_sorted.end()) != trcInd_sorted.end();
  if (hasDuplicates)
    return false;

  std::vector<size_t> trcInd_stl;
  trcInd_stl.resize(trcInd_sorted.size());
  Eigen::VectorX<size_t>::Map(&trcInd_stl[0], trcInd_sorted.size()) = trcInd_sorted;

  // don't move traces within TRACES as this may break the logic of outer application
  traceD.select_rows(trcInd_stl, fromSampInd).write_raw(TRACE(Eigen::all, ind).eval().data());
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
    Eigen::Ref<Eigen::MatrixXd> hdr,
    const size_t& fromTrc,
    const std::string& unitsFrom,
    const std::string& unitsTo)
{
  ptrdiff_t hdrInd = getTraceHeaderIndex(hdrName);
  if (hdrInd < 0)
    return false;

  if (!unitsFrom.empty() && !unitsTo.empty()){
    double coef = units::convert(
          units::unit_from_string(unitsFrom),
          units::unit_from_string(unitsTo));
    hdr = hdr*coef;
  }

  traceHeaderD.select({size_t(hdrInd), fromTrc},
                      {(size_t)1,
                       (size_t)hdr.size()}).write_raw(hdr.data());
  return true;
}

bool H5SeisImpl::writeTraceHeader(
    const std::string& hdrName,
    Eigen::Ref<Eigen::MatrixXd> hdr,
    const Eigen::Ref<const Eigen::VectorX<size_t>>& trcInd,
    const std::string& unitsFrom,
    const std::string& unitsTo)
{
  ptrdiff_t hdrInd = getTraceHeaderIndex(hdrName);
  if (hdrInd < 0 || hdrInd >= getNTrcHdr())
    return false;

  if (trcInd.maxCoeff() >= getNTrc())
    return false;

  h5gt::ElementSet elSet = h5geo::rowCols2ElementSet(hdrInd, trcInd);
  if (!unitsFrom.empty() && !unitsTo.empty()){
    double coef = units::convert(
          units::unit_from_string(unitsFrom),
          units::unit_from_string(unitsTo));
    hdr = hdr*coef;
  }

  traceHeaderD.select(elSet).write_raw(hdr.data());
  return true;
}

bool H5SeisImpl::writeXYTraceHeaders(
    const std::vector<std::string>& xyHdrNames,
    Eigen::Ref<Eigen::MatrixXd>& xy,
    const size_t& fromTrc,
    const std::string& lengthUnits,
    bool doCoordTransform)
{
  if (xyHdrNames.size() != 2 ||
      xy.cols() != 2)
    return false;

  if (fromTrc+xy.rows() > getNTrc())
    return false;

  ptrdiff_t hdrInd_0 = getTraceHeaderIndex(xyHdrNames[0]);
  ptrdiff_t hdrInd_1 = getTraceHeaderIndex(xyHdrNames[1]);
  if (hdrInd_0 < 0 || hdrInd_0 >= getNTrcHdr() ||
      hdrInd_1 < 0 || hdrInd_1 >= getNTrcHdr())
    return false;

#ifdef H5GEO_USE_GDAL
  if (doCoordTransform){
    OGRCT_ptr coordTrans(createCoordinateTransformationToWriteData(lengthUnits));
    if (!coordTrans)
      return false;

    coordTrans->Transform(xy.rows(), xy.col(0).data(), xy.col(1).data());
    traceHeaderD.select({size_t(hdrInd_0), fromTrc},
                        {(size_t)1,
                         (size_t)xy.size()}).write_raw(xy.col(0).data());
    traceHeaderD.select({size_t(hdrInd_1), fromTrc},
                        {(size_t)1,
                         (size_t)xy.size()}).write_raw(xy.col(1).data());
    return true;
  }
#endif

  std::string unitsTo = getLengthUnits();
  if (!unitsTo.empty() && !lengthUnits.empty()){
    double coef = units::convert(
          units::unit_from_string(lengthUnits),
          units::unit_from_string(unitsTo));
    xy = xy*coef;
  }

  traceHeaderD.select({size_t(hdrInd_0), fromTrc},
                      {(size_t)1,
                       (size_t)xy.size()}).write_raw(xy.col(0).data());
  traceHeaderD.select({size_t(hdrInd_1), fromTrc},
                      {(size_t)1,
                       (size_t)xy.size()}).write_raw(xy.col(1).data());
  return true;
}

bool H5SeisImpl::writeXYTraceHeaders(
    const std::vector<std::string>& xyHdrNames,
    Eigen::Ref<Eigen::MatrixXd>& xy,
    const Eigen::Ref<const Eigen::VectorX<size_t>>& trcInd,
    const std::string& lengthUnits,
    bool doCoordTransform)
{
  if (xyHdrNames.size() != 2)
    return false;

  if (trcInd.maxCoeff() >= getNTrc())
    return false;

  ptrdiff_t hdrInd_0 = getTraceHeaderIndex(xyHdrNames[0]);
  ptrdiff_t hdrInd_1 = getTraceHeaderIndex(xyHdrNames[1]);
  if (hdrInd_0 < 0 || hdrInd_0 >= getNTrcHdr() ||
      hdrInd_1 < 0 || hdrInd_1 >= getNTrcHdr())
    return false;

  h5gt::ElementSet elSet_0 = h5geo::rowCols2ElementSet(hdrInd_0, trcInd);
  h5gt::ElementSet elSet_1 = h5geo::rowCols2ElementSet(hdrInd_1, trcInd);
#ifdef H5GEO_USE_GDAL
  if (doCoordTransform){
    OGRCT_ptr coordTrans(createCoordinateTransformationToWriteData(lengthUnits));
    if (!coordTrans)
      return false;

    coordTrans->Transform(xy.rows(), xy.col(0).data(), xy.col(1).data());
    traceHeaderD.select(elSet_0).write_raw(xy.col(0).data());
    traceHeaderD.select(elSet_1).write_raw(xy.col(1).data());
    return true;
  }
#endif

  std::string unitsTo = getLengthUnits();
  if (!unitsTo.empty() && !lengthUnits.empty()){
    double coef = units::convert(
          units::unit_from_string(lengthUnits),
          units::unit_from_string(unitsTo));
    xy = xy*coef;
  }

  traceHeaderD.select(elSet_0).write_raw(xy.col(0).data());
  traceHeaderD.select(elSet_1).write_raw(xy.col(1).data());
  return true;
}

bool H5SeisImpl::setNTrc(size_t nTrc)
{
  std::vector<size_t> trcHdrDims = traceHeaderD.getDimensions();
  if (trcHdrDims.size() != 2)
    return false;
  std::vector<size_t> trcDims = traceD.getDimensions();
  if (trcDims.size() != 2)
    return false;

  try {
    traceHeaderD.resize({trcHdrDims[0], nTrc});
    traceD.resize({nTrc, trcDims[1]});
    return true;
  } catch (h5gt::Exception e) {
    return false;
  }
}

bool H5SeisImpl::setNSamp(size_t nSamp)
{
  std::vector<size_t> trcDims = traceD.getDimensions();
  if (trcDims.size() != 2)
    return false;

  try {
    traceD.resize({trcDims[0], nSamp});
    return true;
  } catch (h5gt::Exception e) {
    return false;
  }
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

std::map<std::string, double> H5SeisImpl::getBinHeader()
{
  auto opt = getBinHeaderD();
  if (!opt.has_value())
    return std::map<std::string, double>();

  std::vector<double> binHdr;
  opt->read(binHdr);

  std::vector<std::string> fullHdrNames, shortHdrNames;
  h5geo::getBinHeaderNames(fullHdrNames, shortHdrNames);
  std::map<std::string, double> m;
  for (const auto& name : shortHdrNames){
    int ind = getBinHeaderIndex(name);
    if (ind < 0 || ind >= binHdr.size())
      continue;

    m[name] = binHdr[ind];
  }

  return m;
}

double H5SeisImpl::getBinHeader(
    const std::string& hdrName,
    const std::string& unitsFrom,
    const std::string& unitsTo)
{
  if (hdrName.empty())
    return NAN;

  auto opt = getBinHeaderD();
  if (!opt.has_value())
    return NAN;

  ptrdiff_t ind = getBinHeaderIndex(hdrName);
  if (ind < 0 || ind >= opt->getElementCount())
    return NAN;

  double hdr;
  opt->select({size_t(ind)}, {1}).read(hdr);

  if (!unitsFrom.empty() && !unitsTo.empty()){
    double coef = units::convert(
          units::unit_from_string(unitsFrom),
          units::unit_from_string(unitsTo));
    return hdr*coef;
  }

  return hdr;
}

Eigen::MatrixXf H5SeisImpl::getTrace(
    const size_t& fromTrc, size_t nTrc,
    const size_t& fromSampInd, size_t nSamp,
    const std::string& dataUnits)
{
  if (!checkTraceLimits(fromTrc, nTrc))
    return Eigen::MatrixXf();

  if (!checkSampleLimits(fromSampInd, nSamp))
    return Eigen::MatrixXf();

  Eigen::MatrixXf TRACE(nSamp, nTrc);

  std::vector<size_t> offset({fromTrc, fromSampInd});
  std::vector<size_t> count({nTrc, nSamp});

  traceD.select(offset, count).read(TRACE.data());
  if (!dataUnits.empty()){
    double coef = units::convert(
          units::unit_from_string(getDataUnits()),
          units::unit_from_string(dataUnits));
    if (!isnan(coef))
      return TRACE*coef;

    return Eigen::MatrixXf();
  }

  return TRACE;
}

Eigen::MatrixXf H5SeisImpl::getTrace(
    const Eigen::Ref<const Eigen::VectorX<size_t>>& trcInd,
    const size_t& fromSampInd,
    size_t nSamp,
    const std::string& dataUnits)
{
  if (trcInd.maxCoeff() >= getNTrc())
    return Eigen::MatrixXf();

  if (!checkSampleLimits(fromSampInd, nSamp))
    return Eigen::MatrixXf();

  Eigen::VectorX<ptrdiff_t> ind = h5geo::sort(trcInd);
  Eigen::VectorX<size_t> trcInd_sorted = trcInd(ind);
  bool hasDuplicates = std::adjacent_find(
        trcInd_sorted.begin(), trcInd_sorted.end()) != trcInd_sorted.end();
  if (hasDuplicates)
    return Eigen::MatrixXf();

  std::vector<size_t> trcInd_stl;
  trcInd_stl.resize(trcInd_sorted.size());
  Eigen::VectorX<size_t>::Map(&trcInd_stl[0], trcInd_sorted.size()) = trcInd_sorted;

  Eigen::MatrixXf TRACE(nSamp, trcInd.size());

  // h5gt rows/cols selection implicitly sorts the indexes
  // we need remove that effect
  traceD.select_rows(trcInd_stl, fromSampInd, nSamp).read(TRACE.data());
  TRACE = TRACE(Eigen::all, ind).eval();
  if (!dataUnits.empty()){
    double coef = units::convert(
          units::unit_from_string(getDataUnits()),
          units::unit_from_string(dataUnits));
    if (!isnan(coef))
      return TRACE*coef;

    return Eigen::MatrixXf();
  }

  return TRACE;
}

Eigen::MatrixXd H5SeisImpl::getTraceHeader(
    const size_t& fromTrc,
    size_t nTrc,
    const size_t& fromHdr,
    size_t nHdr,
    const std::vector<std::string>& unitsFrom,
    const std::vector<std::string>& unitsTo)
{
  if (!checkTraceLimits(fromTrc, nTrc))
    return Eigen::VectorXd();

  if (!checkTraceHeaderLimits(fromHdr, nHdr))
    return Eigen::VectorXd();

  Eigen::MatrixXd HDR(nTrc, nHdr);

  std::vector<size_t> offset({fromHdr, fromTrc});
  std::vector<size_t> count({nHdr, nTrc});

  traceHeaderD.select(offset, count).read(HDR.data());

  if (unitsFrom.size() == HDR.cols() &&
      unitsTo.size() == HDR.cols()){
    for (size_t i = 0; i < HDR.cols(); i++){
      if (!unitsFrom[i].empty() && !unitsTo[i].empty()){
        double coef = units::convert(
              units::unit_from_string(unitsFrom[i]),
              units::unit_from_string(unitsTo[i]));
        if (!isnan(coef))
          HDR.col(i) = HDR.col(i)*coef;
      }
    }
  }

  return HDR;
}

Eigen::VectorXd H5SeisImpl::getTraceHeader(
    const std::string& hdrName,
    const size_t& fromTrc,
    const size_t& nTrc,
    const std::string& unitsFrom,
    const std::string& unitsTo)
{
  ptrdiff_t ind = getTraceHeaderIndex(hdrName);
  if (ind < 0)
    return Eigen::VectorXd();

  Eigen::VectorXd hdr = getTraceHeader(
        fromTrc, nTrc, ind, 1,
        std::vector<std::string>({unitsFrom}),
        std::vector<std::string>({unitsTo}));

  return hdr;
}

Eigen::MatrixXd H5SeisImpl::getTraceHeader(
    const std::vector<size_t>& trcInd,
    const std::vector<size_t>& trcHdrInd,
    const std::vector<std::string>& unitsFrom,
    const std::vector<std::string>& unitsTo)
{
  Eigen::MatrixXd HDR(trcInd.size(), trcHdrInd.size());
  h5gt::ElementSet elSet =
      h5geo::rowsCols2ElementSet(trcHdrInd, trcInd);
  traceHeaderD.select(elSet).read(HDR.data());

  if (unitsFrom.size() == HDR.cols() &&
      unitsTo.size() == HDR.cols()){
    for (size_t i = 0; i < HDR.cols(); i++){
      if (!unitsFrom[i].empty() && !unitsTo[i].empty()){
        double coef = units::convert(
              units::unit_from_string(unitsFrom[i]),
              units::unit_from_string(unitsTo[i]));
        if (!isnan(coef))
          HDR.col(i) = HDR.col(i)*coef;
      }
    }
  }

  return HDR;
}

Eigen::MatrixXd H5SeisImpl::getTraceHeader(
    const Eigen::Ref<const Eigen::VectorX<size_t>>& trcInd,
    const Eigen::Ref<const Eigen::VectorX<size_t>>& trcHdrInd,
    const std::vector<std::string>& unitsFrom,
    const std::vector<std::string>& unitsTo)
{
  Eigen::MatrixXd HDR(trcInd.size(), trcHdrInd.size());
  h5gt::ElementSet elSet =
      h5geo::rowsCols2ElementSet(trcHdrInd, trcInd);
  traceHeaderD.select(elSet).read(HDR.data());

  if (unitsFrom.size() == HDR.cols() &&
      unitsTo.size() == HDR.cols()){
    for (size_t i = 0; i < HDR.cols(); i++){
      if (!unitsFrom[i].empty() && !unitsTo[i].empty()){
        double coef = units::convert(
              units::unit_from_string(unitsFrom[i]),
              units::unit_from_string(unitsTo[i]));
        if (!isnan(coef))
          HDR.col(i) = HDR.col(i)*coef;
      }
    }
  }

  return HDR;
}

Eigen::MatrixXd H5SeisImpl::getTraceHeader(
    const std::vector<std::string>& hdrNames,
    const std::vector<size_t>& trcInd,
    const std::vector<std::string>& unitsFrom,
    const std::vector<std::string>& unitsTo)
{
  if (hdrNames.empty())
    return Eigen::MatrixXd();

  std::vector<size_t> hdrInd(hdrNames.size());
  for (size_t i = 0; i < hdrNames.size(); i++){
    ptrdiff_t ind = getTraceHeaderIndex(hdrNames[i]);
    if (ind < 0)
      return Eigen::MatrixXd();
    hdrInd[i] = ind;
  }
  return getTraceHeader(
        trcInd,
        hdrInd,
        unitsFrom,
        unitsTo);
}

Eigen::MatrixXd H5SeisImpl::getTraceHeader(
    const std::vector<std::string>& hdrNames,
    const Eigen::Ref<const Eigen::VectorX<size_t>>& trcInd,
    const std::vector<std::string>& unitsFrom,
    const std::vector<std::string>& unitsTo)
{
  if (hdrNames.empty())
    return Eigen::MatrixXd();

  Eigen::VectorX<size_t> hdrInd(hdrNames.size());
  for (size_t i = 0; i < hdrNames.size(); i++){
    ptrdiff_t ind = getTraceHeaderIndex(hdrNames[i]);
    if (ind < 0)
      return Eigen::MatrixXd();
    hdrInd(i) = ind;
  }
  return getTraceHeader(
        trcInd,
        hdrInd,
        unitsFrom,
        unitsTo);
}

Eigen::MatrixXd H5SeisImpl::getXYTraceHeaders(
    const std::vector<std::string>& xyHdrNames,
    const size_t& fromTrc,
    size_t nTrc,
    const std::string& lengthUnits,
    bool doCoordTransform)
{
  if (xyHdrNames.size() != 2)
    return Eigen::MatrixXd();

  ptrdiff_t hdrInd_0 = getTraceHeaderIndex(xyHdrNames[0]);
  ptrdiff_t hdrInd_1 = getTraceHeaderIndex(xyHdrNames[1]);
  if (hdrInd_0 < 0 || hdrInd_0 >= getNTrcHdr() ||
      hdrInd_1 < 0 || hdrInd_1 >= getNTrcHdr())
    return Eigen::MatrixXd();

  // reduce nTrc if necessary to create matrix of appropriate size
  if (!checkTraceLimits(fromTrc, nTrc))
    return Eigen::VectorXd();

  Eigen::MatrixXd xy(nTrc, 2);
#ifdef H5GEO_USE_GDAL
  if (doCoordTransform){
    OGRCT_ptr coordTrans(createCoordinateTransformationToReadData(lengthUnits));
    if (!coordTrans)
      return Eigen::MatrixXd();

    xy.col(0) = this->getTraceHeader(
          fromTrc, nTrc, hdrInd_0, 1);
    xy.col(1) = this->getTraceHeader(
          fromTrc, nTrc, hdrInd_1, 1);

    if (xy.cols() != 2 || xy.rows() < 1)
      return Eigen::MatrixXd();

    coordTrans->Transform(xy.rows(), xy.col(0).data(), xy.col(1).data());
    return xy;
  }
#endif

  std::string unitsFrom = this->getLengthUnits();
  xy.col(0) = this->getTraceHeader(
        fromTrc, nTrc, hdrInd_0, 1, {unitsFrom}, {lengthUnits});
  xy.col(1) = this->getTraceHeader(
        fromTrc, nTrc, hdrInd_1, 1, {unitsFrom}, {lengthUnits});
  return xy;
}

Eigen::MatrixXd H5SeisImpl::getXYTraceHeaders(
    const std::vector<std::string>& xyHdrNames,
    const Eigen::Ref<const Eigen::VectorX<size_t>>& trcInd,
    const std::string& lengthUnits,
    bool doCoordTransform)
{
  if (xyHdrNames.size() != 2)
    return Eigen::MatrixXd();

#ifdef H5GEO_USE_GDAL
  if (doCoordTransform){
    OGRCT_ptr coordTrans(createCoordinateTransformationToReadData(lengthUnits));
    if (!coordTrans)
      return Eigen::MatrixXd();

    Eigen::MatrixXd xy = this->getTraceHeader(
          xyHdrNames, trcInd);

    if (xy.cols() != 2 || xy.rows() < 1)
      return Eigen::MatrixXd();

    coordTrans->Transform(xy.rows(), xy.col(0).data(), xy.col(1).data());
    return xy;
  }
#endif

  std::string unitsFrom = this->getLengthUnits();
  return this->getTraceHeader(
        xyHdrNames, trcInd,
        {unitsFrom, unitsFrom},
        {lengthUnits, lengthUnits});
}

Eigen::VectorX<size_t> H5SeisImpl::getSortedData(
    Eigen::MatrixXf& TRACE,
    Eigen::MatrixXd& HDR,
    const std::vector<std::string>& keyList,
    const std::vector<double>& minList,
    const std::vector<double>& maxList,
    size_t fromSampInd,
    size_t nSamp,
    const std::string& dataUnits,
    const std::string& lengthUnits,
    bool doCoordTransform)
{
  if (keyList.empty() || minList.empty() || maxList.empty())
    return Eigen::VectorX<size_t>();

  if (keyList.size() != minList.size() ||
      keyList.size() != maxList.size() ||
      minList.size() != maxList.size())
    return Eigen::VectorX<size_t>();

  // define trace and header indexes, convert them to ElementSet
  // and read preliminary PKey indexed headers
  Eigen::VectorX<size_t> traceIndexes = getPKeyIndexes(
        keyList[0], minList[0], maxList[0]);

  if (traceIndexes.size() < 1)
    return Eigen::VectorX<size_t>();

  Eigen::VectorX<size_t> headerIndex(keyList.size());
  for (size_t i = 0; i < keyList.size(); i++){
    ptrdiff_t ind = getTraceHeaderIndex(keyList[i]);
    if (ind < 0)
      return Eigen::VectorX<size_t>();

    headerIndex[i] = ind;
  }

  h5gt::ElementSet hdrElSet = h5geo::rowsCols2ElementSet(
        headerIndex, traceIndexes);

  HDR.conservativeResize(traceIndexes.size(), headerIndex.size());

  traceHeaderD.select(hdrElSet).read(HDR.data());

  // take in account SKeys and remove unecessary rows (traces),
  // correct 'traceIndexes'
  Eigen::VectorX<bool> idx_bool = Eigen::VectorX<bool>::Ones(HDR.rows());
  for (size_t i = 1; i < keyList.size(); i++){
    idx_bool = idx_bool.array() && (
          HDR.col(i).array() >= minList[i] &&
          HDR.col(i).array() <= maxList[i]);
  }

  Eigen::VectorX<ptrdiff_t> idx = h5geo::find_index(idx_bool);
  HDR = HDR(idx, Eigen::all).eval();
  traceIndexes = traceIndexes(idx).eval();
  if (HDR.size() < 1 || traceIndexes.size() < 1)
    return Eigen::VectorX<size_t>();

  Eigen::MatrixXd urows;
  Eigen::MatrixX2<ptrdiff_t> urows_from_size;
  Eigen::VectorX<ptrdiff_t> sortInd = h5geo::sort_rows_unique(
        HDR, urows, urows_from_size);

  HDR = HDR(sortInd, Eigen::all).eval();
  traceIndexes = traceIndexes(sortInd).eval();

#ifdef H5GEO_USE_GDAL
  if (doCoordTransform && HDR.cols() == 2){
    OGRCT_ptr coordTrans(createCoordinateTransformationToReadData(lengthUnits));
    if (coordTrans)
      coordTrans->Transform(HDR.rows(), HDR.col(0).data(), HDR.col(1).data());
  }
#endif

  if (fromSampInd >= getNSamp() || nSamp == 0)
    return traceIndexes;

  checkSampleLimits(fromSampInd, nSamp);
  TRACE = getTrace(traceIndexes, fromSampInd, nSamp, dataUnits);
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

Eigen::VectorXd H5SeisImpl::getSamples(
    const size_t& trcInd,
    const std::string& units){
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

  if (!units.empty()){
    double coef;
    if (getDomain() == h5geo::Domain::OWT |
        getDomain() == h5geo::Domain::TWT)
      coef = units::convert(
            units::unit_from_string(getTemporalUnits()),
            units::unit_from_string(units));
    else
      coef = units::convert(
            units::unit_from_string(getLengthUnits()),
            units::unit_from_string(units));
    if (!isnan(coef))
      return samp*coef;

    return Eigen::VectorXd();
  }

  return samp;
}

double H5SeisImpl::getFirstSample(
    const size_t& trcInd,
    const std::string& units){
  // DELRECT - Delay Recording time
  Eigen::VectorXd firstSamp = getTraceHeader(
        "DELRECT", trcInd, 1);

  if (firstSamp.size() == 0)
    return NAN;

  if (!units.empty()){
    double coef;
    if (getDomain() == h5geo::Domain::OWT |
        getDomain() == h5geo::Domain::TWT)
      coef = units::convert(
            units::unit_from_string(getTemporalUnits()),
            units::unit_from_string(units));
    else
      coef = units::convert(
            units::unit_from_string(getLengthUnits()),
            units::unit_from_string(units));
    return firstSamp(0)*coef;
  }

  return firstSamp(0);
}

double H5SeisImpl::getLastSample(
    const size_t& trcInd, const std::string& units){
  return getSamples(trcInd, units)(Eigen::last);
}

double H5SeisImpl::getSampRate(const std::string& units){
  double sampRate = getBinHeader("SAMP_RATE");

  if (!units.empty()){
    double coef;
    if (getDomain() == h5geo::Domain::OWT |
        getDomain() == h5geo::Domain::TWT)
      coef = units::convert(
            units::unit_from_string(getTemporalUnits()),
            units::unit_from_string(units));
    else
      coef = units::convert(
            units::unit_from_string(getLengthUnits()),
            units::unit_from_string(units));
    return sampRate*coef;
  }

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

Eigen::VectorX<size_t> H5SeisImpl::getPKeyIndexes(
    const std::string& pKey,
    double pMin, double pMax)
{
  auto optUValG = getUValG();
  if (!optUValG.has_value())
    return Eigen::VectorX<size_t>();

  auto optIndexesG = getIndexesG();
  if (!optIndexesG.has_value())
    return Eigen::VectorX<size_t>();

  if (!optIndexesG->exist(pKey) ||
      optIndexesG->getObjectType(pKey) != h5gt::ObjectType::Group)
    return Eigen::VectorX<size_t>();

  h5gt::Group pGroup = optIndexesG->getGroup(pKey);
  std::vector<double> uHeader(pGroup.getNumberObjects());
  optUValG->getDataSet(pKey).read(uHeader);
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

Eigen::VectorXd H5SeisImpl::getPKeyValues(
    const std::string& pKey,
    const std::string& unitsFrom,
    const std::string& unitsTo){
  auto uvalG = getUValG();
  if (!uvalG.has_value())
    return Eigen::VectorXd();

  if (!uvalG->hasObject(pKey, h5gt::ObjectType::Dataset))
    return Eigen::VectorXd();

  Eigen::VectorXd v(
        uvalG->getDataSet(pKey).
        getMemSpace().
        getElementCount());

  uvalG->getDataSet(pKey).read(v.data());

  if (!unitsFrom.empty() && !unitsTo.empty()){
    double coef = units::convert(
          units::unit_from_string(unitsFrom),
          units::unit_from_string(unitsTo));
    if (!isnan(coef))
      return v*coef;

    return Eigen::VectorXd();
  }

  return v;
}

size_t H5SeisImpl::getPKeySize(const std::string& pKey){
  auto uvalG = getUValG();
  if (!uvalG.has_value())
    return 0;

  if (!uvalG->hasObject(pKey, h5gt::ObjectType::Dataset))
    return 0;

  return uvalG->getDataSet(pKey).
      getMemSpace().
      getElementCount();
}

size_t H5SeisImpl::getPKeyTraceSize(const std::string& pKey, double pMin, double pMax){
  auto optUValG = getUValG();
  if (!optUValG.has_value())
    return 0;

  auto optIndexesG = getIndexesG();
  if (!optIndexesG.has_value())
    return 0;

  if (!optIndexesG->exist(pKey) ||
      optIndexesG->getObjectType(pKey) != h5gt::ObjectType::Group)
    return 0;

  h5gt::Group pGroup = optIndexesG->getGroup(pKey);
  std::vector<double> uHeader(pGroup.getNumberObjects());
  optUValG->getDataSet(pKey).read(uHeader);
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

  return pKeyCount;
}

std::vector<std::string> H5SeisImpl::getPKeyNames(){
  auto opt = getIndexesG();
  if (!opt.has_value())
    return {""};

  return opt->listObjectNames();
}

std::map<std::string, double> H5SeisImpl::getTraceHeaderMin(){
  if (!traceHeaderD.hasAttribute("min"))
    return std::map<std::string, double>();

  auto attr = traceHeaderD.getAttribute("min");
  std::vector<double> hdr;
  attr.read(hdr);

  std::vector<std::string> fullHdrNames, shortHdrNames;
  h5geo::getTraceHeaderNames(fullHdrNames, shortHdrNames);
  std::map<std::string, double> m;
  for (const auto& name : shortHdrNames){
    int ind = getTraceHeaderIndex(name);
    if (ind < 0 || ind >= hdr.size())
      continue;

    m[name] = hdr[ind];
  }

  return m;
}

std::map<std::string, double> H5SeisImpl::getTraceHeaderMax(){
  if (!traceHeaderD.hasAttribute("max"))
    return std::map<std::string, double>();

  auto attr = traceHeaderD.getAttribute("max");
  std::vector<double> hdr;
  attr.read(hdr);

  std::vector<std::string> fullHdrNames, shortHdrNames;
  h5geo::getTraceHeaderNames(fullHdrNames, shortHdrNames);
  std::map<std::string, double> m;
  for (const auto& name : shortHdrNames){
    int ind = getTraceHeaderIndex(name);
    if (ind < 0 || ind >= hdr.size())
      continue;

    m[name] = hdr[ind];
  }

  return m;
}

double H5SeisImpl::getTraceHeaderMin(
    const std::string& hdrName,
    const std::string& unitsFrom,
    const std::string& unitsTo)
{
  if (hdrName.empty())
    return NAN;

  if (!traceHeaderD.hasAttribute("min"))
    return NAN;

  auto attr = traceHeaderD.getAttribute("min");
  int ind = getTraceHeaderIndex(hdrName);
  if (ind < 0)
    return NAN;

  std::vector<double> hdr;
  attr.read(hdr);
  if (ind >= hdr.size())
    return NAN;

  if (!unitsFrom.empty() && !unitsTo.empty()){
    double coef = units::convert(
          units::unit_from_string(unitsFrom),
          units::unit_from_string(unitsTo));
    return hdr[ind]*coef;
  }

  return hdr[ind];
}

double H5SeisImpl::getTraceHeaderMax(
    const std::string& hdrName,
    const std::string& unitsFrom,
    const std::string& unitsTo)
{
  if (hdrName.empty())
    return NAN;

  if (!traceHeaderD.hasAttribute("max"))
    return NAN;

  auto attr = traceHeaderD.getAttribute("max");

  int ind = getTraceHeaderIndex(hdrName);
  if (ind < 0)
    return NAN;

  std::vector<double> hdr;
  attr.read(hdr);
  if (ind >= hdr.size())
    return NAN;

  if (!unitsFrom.empty() && !unitsTo.empty()){
    double coef = units::convert(
          units::unit_from_string(unitsFrom),
          units::unit_from_string(unitsTo));
    return hdr[ind]*coef;
  }

  return hdr[ind];
}

SeisParam H5SeisImpl::getParam()
{
  SeisParam p;
  // BaseObjectParam
  p.spatialReference = getSpatialReference();
  p.lengthUnits = getLengthUnits();
  p.temporalUnits = getTemporalUnits();
  p.angularUnits = getAngularUnits();
  p.dataUnits = getDataUnits();

  // SeisParam
  p.domain = getDomain();
  p.dataType = getDataType();
  p.surveyType = getSurveyType();
  p.nTrc = getNTrc();
  p.nSamp = getNSamp();
  p.srd = getSRD();

  auto dsetOpt = getTraceD();
  if (!dsetOpt.has_value())
    return p;

  auto dsetCreateProps = dsetOpt->getCreateProps();
  if (dsetCreateProps.isChunked()){
    std::vector<hsize_t> chunkSizeVec = dsetCreateProps.getChunk(dsetOpt->getDimensions().size());
    if (chunkSizeVec.size() > 0)
      p.trcChunk = dsetCreateProps.getChunk(dsetOpt->getDimensions().size())[0];
  }

  return p;
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

  // nHdr maybe passed as std::numeric_limits<size_t> and
  // then adding 1 to it results in 0 (nHdr > NHdr check is necessary)
  if (nHdr > NHdr || fromHdr+nHdr > NHdr)
    nHdr = NHdr-fromHdr;

  return true;
}

bool H5SeisImpl::checkSampleLimits(
    const size_t& fromSampInd, size_t& nSamp)
{
  size_t NSamp = getNSamp();
  if (fromSampInd > NSamp)
    return false;

  // nSamp maybe passed as std::numeric_limits<size_t> and
  // then adding 1 to it results in 0 (nSamp > NSamp check is necessary)
  if (nSamp > NSamp || fromSampInd+nSamp > NSamp)
    nSamp = NSamp-fromSampInd;

  return true;
}

void H5SeisImpl::calcGrid3D(
    const Eigen::Ref<Eigen::VectorXd>& x,
    const Eigen::Ref<Eigen::VectorXd>& y,
    double z,
    Eigen::VectorXd& x_loc,
    Eigen::VectorXd& y_loc,
    Eigen::VectorXd& z_loc)
{
  if (x.size() < 1 || y.size() < 1)
    return;

  ptrdiff_t nx = x.size();
  ptrdiff_t ny = y.size();

  x_loc = Eigen::VectorXd::Zero(nx*ny);
  y_loc = Eigen::VectorXd::Zero(nx*ny);
  z_loc = Eigen::VectorXd::Zero(nx*ny);

  ptrdiff_t idx = 0;
  for (ptrdiff_t k = 0; k < ny; k++){
    for (ptrdiff_t j = 0; j < nx; j++){
      x_loc(idx) = x(j);
      y_loc(idx) = y(k);
      z_loc(idx) = z;
      idx++;
    }
  }
}

bool H5SeisImpl::generatePRESTKGeometry(
    double src_x0, double src_dx, size_t src_nx,
    double src_y0, double src_dy, size_t src_ny,
    double src_z,
    double rec_x0, double rec_dx, size_t rec_nx,
    double rec_y0, double rec_dy, size_t rec_ny,
    double rec_z,
    bool moveRec,
    const std::string& lengthUnits,
    bool doCoordTransform)
{
#ifdef H5GEO_USE_GDAL
  if (doCoordTransform){
    OGRCT_ptr coordTrans(createCoordinateTransformationToWriteData(lengthUnits));
    if (!coordTrans)
      return false;

    coordTrans->Transform(1, &src_x0, &src_y0);
    coordTrans->Transform(1, &rec_x0, &rec_y0);
  }
#endif

  if (!lengthUnits.empty()){
    double coef = units::convert(
          units::unit_from_string(lengthUnits),
          units::unit_from_string(getLengthUnits()));
    src_dx *= coef;
    src_dy *= coef;
    rec_dx *= coef;
    rec_dy *= coef;
    if (!doCoordTransform){
      src_x0 *= coef;
      src_y0 *= coef;
      rec_x0 *= coef;
      rec_y0 *= coef;
    }
  }

  Eigen::VectorXd src_x = Eigen::VectorXd::LinSpaced(src_nx, src_x0, src_x0+src_dx*(src_nx-1));
  Eigen::VectorXd src_y = Eigen::VectorXd::LinSpaced(src_ny, src_y0, src_y0+src_dy*(src_ny-1));
  Eigen::VectorXd src_xloc, src_yloc, src_zloc;
  this->calcGrid3D(src_x, src_y, src_z, src_xloc, src_yloc, src_zloc);
  if (src_xloc.size() < 1 || src_yloc.size() < 1 || src_zloc.size() < 1)
    return false;

  Eigen::VectorXd rec_x = Eigen::VectorXd::LinSpaced(rec_nx, rec_x0, rec_x0+rec_dx*(rec_nx-1));
  Eigen::VectorXd rec_y = Eigen::VectorXd::LinSpaced(rec_ny, rec_y0, rec_y0+rec_dy*(rec_ny-1));
  Eigen::VectorXd rec_xloc, rec_yloc, rec_zloc;
  this->calcGrid3D(rec_x, rec_y, rec_z, rec_xloc, rec_yloc, rec_zloc);
  if (rec_xloc.size() < 1 || rec_yloc.size() < 1 || rec_zloc.size() < 1)
    return false;

  size_t nShot = src_nx*src_ny;
  size_t nRec = rec_nx*rec_ny;
  size_t nTrc = nShot*nRec;
  if (this->getNTrc() != nTrc)
    if (!this->setNTrc(nTrc))
      return false;

  double src_xmin = src_x.minCoeff();
  double src_ymin = src_y.minCoeff();
  double src_xmax = src_x.maxCoeff();
  double src_ymax = src_y.maxCoeff();

  double rec_xmin = rec_x.minCoeff();
  double rec_ymin = rec_y.minCoeff();
  double rec_xmax = rec_x.maxCoeff();
  double rec_ymax = rec_y.maxCoeff();
  if (moveRec){
    rec_xmin += src_xmin - src_x0;
    rec_ymin += src_ymin - src_y0;
    rec_xmax += src_xmax - src_x0;
    rec_ymax += src_ymax - src_y0;
  }

  double cdp_xmin = (src_xmin + rec_xmin)/2;
  double cdp_ymin = (src_ymin + rec_ymin)/2;
  double cdp_xmax = (src_xmax + rec_xmax)/2;
  double cdp_ymax = (src_ymax + rec_ymax)/2;

  Eigen::VectorXd lind = Eigen::VectorXd::LinSpaced(nRec, 1, nRec);
  Eigen::VectorXd ones_arr = Eigen::VectorXd::Ones(nRec);

  rec_x = rec_xloc;
  rec_y = rec_yloc;
  for (size_t i = 0; i < nShot; i++){
    size_t fromTrc = i*nRec;
    lind.array() += nRec*bool(i);
    src_x = ones_arr*src_xloc(i);
    src_y = ones_arr*src_yloc(i);
    if (moveRec){
      double dx = src_xloc(i) - src_x0;
      double dy = src_yloc(i) - src_y0;
      rec_x = rec_xloc.array() + dx;
      rec_y = rec_yloc.array() + dy;
    }
    Eigen::VectorXd cdp_x = (src_x+rec_x)/2;
    Eigen::VectorXd cdp_y = (src_y+rec_y)/2;
    Eigen::VectorXd il, xl;
    if (moveRec){
      if (rec_dy == 0 && src_dy == 0)
        il = ones_arr;
      else if (rec_dy == 0)
        il = ((rec_y.array()-rec_ymin)/abs(src_dy)).rint() + 1;
      else
        il = ((rec_y.array()-rec_ymin)/abs(rec_dy)).rint() + 1;

      if (rec_dx == 0 && src_dx == 0)
        xl = ones_arr;
      else if (rec_dx == 0)
        xl = ((rec_x.array()-rec_xmin)/abs(src_dx)).rint() + 1;
      else
        xl = ((rec_x.array()-rec_xmin)/abs(rec_dx)).rint() + 1;
    } else {
      if (rec_dy == 0)
        il = ones_arr;
      else
        il = ((rec_y.array()-rec_ymin)/abs(rec_dy)).rint() + 1;

      if (rec_dx == 0)
        xl = ones_arr;
      else
        xl = ((rec_x.array()-rec_xmin)/abs(rec_dx)).rint() + 1;
    }

    Eigen::VectorXd dx = rec_x-src_x;
    Eigen::VectorXd dy = rec_y-src_y;
    Eigen::VectorXd offset = (dx.array().square() + dy.array().square()).array().sqrt();
    Eigen::VectorXd sp = ones_arr.array()+i;
    Eigen::VectorXd ffid = sp;
    Eigen::VectorXd srcx = ones_arr*src_xloc(i);
    Eigen::VectorXd srcy = ones_arr*src_yloc(i);
    Eigen::VectorXd ses = ones_arr*src_zloc(i);

    this->writeTraceHeader("SEQWL", lind, fromTrc);
    this->writeTraceHeader("SEQWR", lind, fromTrc);
    this->writeTraceHeader("TRCFLD", lind, fromTrc);
    this->writeTraceHeader("TRCID", ones_arr, fromTrc);
    this->writeTraceHeader("DU", ones_arr, fromTrc);
    this->writeTraceHeader("SAED", ones_arr, fromTrc);
    this->writeTraceHeader("SAC", ones_arr, fromTrc);
    this->writeTraceHeader("SP", sp, fromTrc);
    this->writeTraceHeader("FFID", ffid, fromTrc);
    this->writeTraceHeader("SRCX", srcx, fromTrc);
    this->writeTraceHeader("SRCY", srcy, fromTrc);
    this->writeTraceHeader("GRPX", rec_x, fromTrc);
    this->writeTraceHeader("GRPY", rec_y, fromTrc);
    this->writeTraceHeader("RGE", rec_zloc, fromTrc);
    this->writeTraceHeader("SES", ses, fromTrc);
    this->writeTraceHeader("CDP_X", cdp_x, fromTrc);
    this->writeTraceHeader("CDP_Y", cdp_y, fromTrc);
    this->writeTraceHeader("INLINE", il, fromTrc);
    this->writeTraceHeader("XLINE", xl, fromTrc);
    this->writeTraceHeader("DSREG", offset, fromTrc);
  }

  // update cdp numbers
  Eigen::MatrixXd cdp_yx = this->getXYTraceHeaders({"CDP_Y", "CDP_X"});
  Eigen::VectorX<ptrdiff_t> cdp_yx_ind = h5geo::sort_rows(cdp_yx);
  Eigen::VectorXd cdp = Eigen::VectorXd::LinSpaced(cdp_yx_ind.size(), 1, cdp_yx_ind.size())(cdp_yx_ind);
  this->writeTraceHeader("CDP", cdp);
  this->setDataType(h5geo::SeisDataType::PRESTACK);
  return true;
}

bool H5SeisImpl::generateSTKGeometry(
    double x0, double dx, size_t nx,
    double y0, double dy, size_t ny,
    double z,
    const std::string& lengthUnits,
    bool doCoordTransform)
{
#ifdef H5GEO_USE_GDAL
  if (doCoordTransform){
    OGRCT_ptr coordTrans(createCoordinateTransformationToWriteData(lengthUnits));
    if (!coordTrans)
      return false;

    coordTrans->Transform(1, &x0, &y0);
  }
#endif

  if (!lengthUnits.empty()){
    double coef = units::convert(
          units::unit_from_string(lengthUnits),
          units::unit_from_string(getLengthUnits()));
    dx *= coef;
    dy *= coef;
    if (!doCoordTransform){
      x0 *= coef;
      y0 *= coef;
    }
  }

  Eigen::VectorXd x = Eigen::VectorXd::LinSpaced(nx, x0, x0+dx*(nx-1));
  Eigen::VectorXd y = Eigen::VectorXd::LinSpaced(ny, y0, y0+dy*(ny-1));
  Eigen::VectorXd xloc, yloc, zloc;
  this->calcGrid3D(x, y, z, xloc, yloc, zloc);
  if (xloc.size() < 1 || yloc.size() < 1 || zloc.size() < 1)
    return false;

  Eigen::VectorXd xl = Eigen::VectorXd::LinSpaced(nx, 1, nx);
  Eigen::VectorXd il = Eigen::VectorXd::LinSpaced(ny, 1, ny);
  Eigen::VectorXd xlloc, illoc, notUsed;
  this->calcGrid3D(xl, il, 0, xlloc, illoc, notUsed);
  if (xlloc.size() < 1 || illoc.size() < 1)
    return false;

  size_t nTrc = xloc.size();
  if (this->getNTrc() != nTrc)
    if (!this->setNTrc(nTrc))
      return false;

  Eigen::VectorXd lind = Eigen::VectorXd::LinSpaced(nTrc, 1, nTrc);
  Eigen::VectorXd ones_arr = Eigen::VectorXd::Ones(nTrc);
  this->writeTraceHeader("SEQWL", lind);
  this->writeTraceHeader("SEQWR", lind);
  this->writeTraceHeader("TRCFLD", lind);
  this->writeTraceHeader("CDP", lind);
  this->writeTraceHeader("TRCID", ones_arr);
  this->writeTraceHeader("DU", ones_arr);
  this->writeTraceHeader("SAED", ones_arr);
  this->writeTraceHeader("SAC", ones_arr);
  this->writeTraceHeader("RGE", zloc);
  this->writeTraceHeader("SES", zloc);
  this->writeTraceHeader("CDP_X", xloc);
  this->writeTraceHeader("CDP_Y", yloc);
  this->writeTraceHeader("INLINE", il);
  this->writeTraceHeader("XLINE", xl);
  this->setDataType(h5geo::SeisDataType::STACK);
  return true;
}

bool H5SeisImpl::setDomain(const h5geo::Domain& val){
  unsigned v = static_cast<unsigned>(val);
  return h5geo::overwriteAttribute(
        objG,
        std::string{h5geo::detail::Domain},
        v);
}

bool H5SeisImpl::setDataType(const h5geo::SeisDataType& val){
  unsigned v = static_cast<unsigned>(val);
  return h5geo::overwriteAttribute(
        objG,
        std::string{h5geo::detail::SeisDataType},
        v);
}

bool H5SeisImpl::setSurveyType(const h5geo::SurveyType& val){
  unsigned v = static_cast<unsigned>(val);
  return h5geo::overwriteAttribute(
        objG,
        std::string{h5geo::detail::SurveyType},
        v);
}

bool H5SeisImpl::setSRD(double val, const std::string& lengthUnits){
  return h5geo::overwriteAttribute(
        objG,
        std::string{h5geo::detail::SRD},
        val, lengthUnits, getLengthUnits());
}

bool H5SeisImpl::setSampRate(double val, const std::string& units){
  h5geo::Domain domain = this->getDomain();
  if (domain == h5geo::Domain::OWT ||
      domain == h5geo::Domain::TWT){
    return this->writeBinHeader("SAMP_RATE", val, units, getTemporalUnits());
  } else {
    return this->writeBinHeader("SAMP_RATE", val, units, getLengthUnits());
  }
}

bool H5SeisImpl::setFirstSample(double val, const std::string& units){
  if (!units.empty()){
    double coef;
    if (getDomain() == h5geo::Domain::OWT |
        getDomain() == h5geo::Domain::TWT)
      coef = units::convert(
            units::unit_from_string(getTemporalUnits()),
            units::unit_from_string(units));
    else
      coef = units::convert(
            units::unit_from_string(getLengthUnits()),
            units::unit_from_string(units));
    val = val*coef;
  }

  size_t nTrc = getNTrc();
  if (nTrc < 1)
    return false;

  Eigen::VectorXd v(nTrc);
  v.fill(val);
  return writeTraceHeader("DELRECT", v, 0);
}

h5geo::Domain H5SeisImpl::getDomain(){
  return static_cast<h5geo::Domain>(
        h5geo::readEnumAttribute(
          objG,
          std::string{h5geo::detail::Domain}));
}

h5geo::SeisDataType H5SeisImpl::getDataType(){
  return static_cast<h5geo::SeisDataType>(
        h5geo::readEnumAttribute(
          objG,
          std::string{h5geo::detail::SeisDataType}));
}

h5geo::SurveyType H5SeisImpl::getSurveyType(){
  return static_cast<h5geo::SurveyType>(
        h5geo::readEnumAttribute(
          objG,
          std::string{h5geo::detail::SurveyType}));
}

double H5SeisImpl::getSRD(const std::string& lengthUnits){
  return h5geo::readDoubleAttribute(
        objG,
        std::string{h5geo::detail::SRD},
        getLengthUnits(), lengthUnits);
}

Eigen::MatrixXd H5SeisImpl::getBoundary(
    const std::string& lengthUnits,
    bool doCoordTransform)
{
  auto opt = getBoundaryD();
  if (!opt.has_value())
    return Eigen::MatrixXd();

#ifdef H5GEO_USE_GDAL
  if (doCoordTransform){
    OGRCT_ptr coordTrans(createCoordinateTransformationToReadData(lengthUnits));
    if (!coordTrans)
      return Eigen::MatrixXd();

    Eigen::MatrixXd M = h5geo::readDoubleEigenMtxDataset(
          objG,
          opt->getPath());

    if (M.cols() != 2)
      return Eigen::MatrixXd();

    coordTrans->Transform(M.rows(), M.col(0).data(), M.col(1).data());
    return M;
  }
#endif

  return h5geo::readDoubleEigenMtxDataset(
        objG,
        opt->getPath(),
        getLengthUnits(), lengthUnits);
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

  objG.flush();
  return true;
}

H5SeisContainer* H5SeisImpl::openSeisContainer(){
  h5gt::File file = getH5File();
  return h5geo::createSeisContainer(
        file, h5geo::CreationType::OPEN_OR_CREATE);
}

std::optional<h5gt::DataSet>
H5SeisImpl::getBoundaryD()
{
  std::string name = std::string{h5geo::detail::boundary};
  return getDatasetOpt(objG, name);
}

std::optional<h5gt::DataSet>
H5SeisImpl::getTextHeaderD()
{
  std::string name = std::string{h5geo::detail::text_header};
  return getDatasetOpt(objG, name);
}

std::optional<h5gt::DataSet>
H5SeisImpl::getBinHeaderD()
{
  std::string name = std::string{h5geo::detail::bin_header};
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
  std::string name = std::string{h5geo::detail::sort};
  return getGroupOpt(objG, name);
}

std::optional<h5gt::Group>
H5SeisImpl::getUValG()
{
  auto opt = getSortG();
  if (!opt.has_value())
    return std::nullopt;

  std::string name = std::string{h5geo::detail::unique_values};
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

  std::string name = std::string{h5geo::detail::indexes};
  if (!opt->hasObject(name, h5gt::ObjectType::Group))
    return std::nullopt;

  return opt->getGroup(name);
}

std::optional<h5gt::Group> H5SeisImpl::getSEGYG()
{
  std::string name = std::string{h5geo::detail::segy};
  if (!objG.hasObject(name, h5gt::ObjectType::Group))
    return std::nullopt;

  return objG.getGroup(name);
}

std::optional<h5gt::DataSet> H5SeisImpl::getSEGYTextHeaderD()
{
  auto opt = getSEGYG();
  if (!opt.has_value())
    return std::nullopt;

  std::string name = std::string{h5geo::detail::text_header};
  if (!opt->hasObject(name, h5gt::ObjectType::Dataset))
    return std::nullopt;

  return opt->getDataSet(name);
}

std::optional<h5gt::DataSet> H5SeisImpl::getSEGYBinHeader2BytesD()
{
  auto opt = getSEGYG();
  if (!opt.has_value())
    return std::nullopt;

  std::string name = std::string{h5geo::detail::bin_header_2bytes};
  if (!opt->hasObject(name, h5gt::ObjectType::Dataset))
    return std::nullopt;

  return opt->getDataSet(name);
}

std::optional<h5gt::DataSet> H5SeisImpl::getSEGYBinHeader4BytesD()
{
  auto opt = getSEGYG();
  if (!opt.has_value())
    return std::nullopt;

  std::string name = std::string{h5geo::detail::bin_header_4bytes};
  if (!opt->hasObject(name, h5gt::ObjectType::Dataset))
    return std::nullopt;

  return opt->getDataSet(name);
}

std::optional<h5gt::DataSet> H5SeisImpl::getSEGYTraceHeader2BytesD()
{
  auto opt = getSEGYG();
  if (!opt.has_value())
    return std::nullopt;

  std::string name = std::string{h5geo::detail::trace_header_2bytes};
  if (!opt->hasObject(name, h5gt::ObjectType::Dataset))
    return std::nullopt;

  return opt->getDataSet(name);
}

std::optional<h5gt::DataSet> H5SeisImpl::getSEGYTraceHeader4BytesD()
{
  auto opt = getSEGYG();
  if (!opt.has_value())
    return std::nullopt;

  std::string name = std::string{h5geo::detail::trace_header_4bytes};
  if (!opt->hasObject(name, h5gt::ObjectType::Dataset))
    return std::nullopt;

  return opt->getDataSet(name);
}

std::optional<h5gt::DataSet> H5SeisImpl::getSEGYTraceFloatD()
{
  auto opt = getSEGYG();
  if (!opt.has_value())
    return std::nullopt;

  std::string name = std::string{h5geo::detail::trace_float};
  if (!opt->hasObject(name, h5gt::ObjectType::Dataset))
    return std::nullopt;

  return opt->getDataSet(name);
}

H5Seis* h5geo::openSeis(h5gt::Group group){
  if (isGeoObjectByType(group, h5geo::ObjectType::SEISMIC))
    return new H5SeisImpl(group);

  return nullptr;
}

bool H5SeisImpl::updateTraceHeaderLimits(size_t nTrcBuffer)
{
  if (nTrcBuffer < 1)
    return false;

  size_t nHdr = getNTrcHdr();
  if (nHdr < 1)
    return false;

  Eigen::VectorXd hdr, minHdr(nHdr), maxHdr(nHdr);
  for (size_t i = 0; i < nHdr; i++){
    size_t fromTrc = 0;
    double min = std::numeric_limits<double>::infinity(),
        max = -std::numeric_limits<double>::infinity();
    do {
      hdr = getTraceHeader(
            fromTrc, nTrcBuffer, i, 1);
      if (hdr.rows() < 1)
        break;

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


  objG.flush();
  return true;
}

bool H5SeisImpl::updateBoundary()
{
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
  }

  bool val = h5geo::overwriteResizableDataset(
        objG,
        opt->getPath(),
        boundary);

  objG.flush();
  return val;
}

bool H5SeisImpl::updatePKeySort(const std::string& pKeyName)
{
  removePKeySort(pKeyName);
  return addPKeySort(pKeyName);
}

/*-------------------------------------------------------*/
/*---------------------- PROTECTED ----------------------*/
/*-------------------------------------------------------*/

Eigen::MatrixX2d H5SeisImpl::calcBoundaryStk2D(){
  if (getDataType() != h5geo::SeisDataType::STACK ||
      getSurveyType() != h5geo::SurveyType::TWO_D)
    return Eigen::MatrixX2d();

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
