#ifndef H5SEIS_H
#define H5SEIS_H

#include "../h5base/h5baseobject.h"
#include "../h5core/h5core.h"
#include "../h5algo/h5sort.h"
#include "../h5algo/h5find.h"
#include "../h5algo/h5easyhull.h"

#define _USE_MATH_DEFINES   // should be before <cmath>, include 'pi' val

#include <math.h>
#include <cmath>
#include <vector>
#include <optional>

class H5SeisContainer;

class H5GEO_EXPORT H5Seis : public H5BaseObject
{
private:
  explicit H5Seis(const h5gt::Group &group);

public:
  virtual ~H5Seis() override;

  static std::optional<H5Seis>
  create(h5gt::Group &group);

//  WRITERS
  bool writeTextHeader(const char (&txtHdr)[40][80]);
  /*!
   * \brief writeTextHeader Max 40x80 chars are to be written
   * \param txtHdr
   * \return
   */
  bool writeTextHeader(const std::vector<std::string>& txtHdr);
  bool writeBinHeader(const double (&binHdr)[30]);
  /*!
   * \brief writeBinHeader Vector length should be equal to getNBinHdr
   * \param binHdrVec
   * \return
   */
  bool writeBinHeader(const std::vector<double>& binHdrVec);
  bool writeBinHeader(const Eigen::VectorXd& binHdrVec);
  bool writeBinHeader(const std::string& hdrName,
      const double& value);
  bool writeBoundary(const Eigen::MatrixX2d& boundary);
  bool writeTrace(
      const Eigen::Ref<const Eigen::MatrixXf>& TRACE,
      const size_t& fromTrc = 0,
      const size_t& fromSampInd = 0);
  bool writeTraceHeader(
      const Eigen::Ref<const Eigen::MatrixXd>& HDR,
      const size_t& fromTrc = 0,
      const size_t& fromHdrInd = 0);
  bool writeTraceHeader(
      const std::string& hdrName,
      const Eigen::Ref<const Eigen::MatrixXd>& hdr,
      const size_t& fromTrc = 0);

//  GETTERS
  std::vector<std::string> getTextHeader();
  Eigen::VectorXd getBinHeader();
  double getBinHeader(
      const std::string& hdrName);
  /*!
   * \brief getTrace Get block of traces. If `nTrc` or
   * `nSamp` exceed max values then these values are
   * changed to max allowed (that is why they are not `const`)
   * \param fromTrc
   * \param nTrc
   * \param fromSampInd
   * \param nSamp
   * \return
   */
  Eigen::MatrixXf getTrace(
      const size_t& fromTrc,
      size_t nTrc = 1,
      const size_t& fromSampInd = 0,
      size_t nSamp = std::numeric_limits<size_t>::max());
  /*!
   * \brief getTraceHeader Get block of trace headers.
   * If `nTrc` or `nHdr` exceed max values then these
   * values are changed to max allowed (that is why
   * they are not `const`)
   * \param fromTrc
   * \param nTrc
   * \param fromHdr
   * \param nHdr
   * \return
   */
  Eigen::MatrixXd getTraceHeader(
      const size_t& fromTrc,
      size_t nTrc = 1,
      const size_t& fromHdr = 0,
      size_t nHdr = std::numeric_limits<size_t>::max());
  Eigen::VectorXd getTraceHeader(
      const std::string& hdrName,
      const size_t& fromTrc = 0,
      const size_t& nTrc = std::numeric_limits<size_t>::max());
  /*!
   * \brief readSortedData Get sorted data based on precalculated
   * primary sort keys (e.g. before using it you should prepare
   * primary sort keys with `addPKeySort(...)` method)
   * \param TRACE this is not Eigen::Ref<> because Eigen::Ref<>
   * doesn't allow to resize matrices
   * \param HDR this is not Eigen::Ref<> because Eigen::Ref<>
   * doesn't allow to resize matrices
   * \param keyList
   * \param minList
   * \param maxList
   * \param fromSampInd first sample index to read
   * (from 0 to getNSamp())
   * \param nSamp Number of samples to read (if 0 then
   * 'TRACE' will be empty). By default all samples
   * \return vector of trace indexes read
   */
  Eigen::VectorX<size_t> getSortedData(
      Eigen::MatrixXf& TRACE,
      Eigen::MatrixXd& HDR,
      const std::vector<std::string>& keyList,
      const std::vector<double>& minList,
      const std::vector<double>& maxList,
      size_t fromSampInd = 0,
      size_t nSamp = std::numeric_limits<size_t>::max());

  ptrdiff_t getBinHeaderIndex(const std::string& hdrName);
  ptrdiff_t getTraceHeaderIndex(const std::string& hdrName);
  Eigen::VectorX<size_t> getTracePKeyIndexes(
      const std::string& pName,
      const double& pMin, const double& pMax);

  /*! in millisec or meters !*/
  Eigen::VectorXd getSamples(const size_t& trcInd);
  /*! in millisec or meters !*/
  double getFirstSample(const size_t& trcInd);
  /*! in millisec or meters !*/
  double getLastSample(const size_t& trcInd);
  /*! in millisec or meters !*/
  double getSampRate();
  size_t getNSamp();
  size_t getNTrc();
  /*! usually 78 !*/
  size_t getNTrcHdr();
  size_t getNBinHdr();
  size_t getNTextHdrRows();
  std::vector<std::string> getPKeyNames();
  Eigen::VectorXd getTraceHeaderMin();
  Eigen::VectorXd getTraceHeaderMax();
  double getTraceHeaderMin(const std::string& hdrName);
  double getTraceHeaderMax(const std::string& hdrName);

  Eigen::MatrixX2d calcBoundaryStk2D();
  Eigen::MatrixX2d calcConvexHullBoundary();
  bool calcBinOriginOrientation3DStk(
      Eigen::Ref<Eigen::Vector2d> bin,
      Eigen::Ref<Eigen::Vector2d> origin,
      double& orientation);
  bool calcBinOriginOrientation3DStk(
      std::vector<double>& bin,
      std::vector<double>& origin,
      double& orientation);

  bool calcAndWriteBoundary();
  bool calcAndWriteTraceHeaderLimits(
      const size_t& nTrcBuffer = 1e7);

  /*!
   * \brief checkTraceLimits check *fromTrc* and *nTrc* and diminish
   *  *nTrc* to fit in data limits (if *fromTrc* is inside limit)
   * \param fromTrc first trace (to read for example)
   * \param nTrc number of trace (to read for example)
   * \return
   */
  bool checkTraceLimits(
      const size_t& fromTrc, size_t& nTrc);
  /*!
   * \brief checkTraceHeaderLimits check 'fromHdr' and 'nHdr' and diminish
   * 'nHdr' to fit in data limits (if 'fromTrc' is inside limit)
   * \param fromHdr first header (usually there are 78 headers so
   * the value should be less then this value)
   * \param nHdr number of headers (to read for example)
   * \return
   */
  bool checkTraceHeaderLimits(
      const size_t& fromHdr, size_t& nHdr);
  bool checkSampleLimits(
      const size_t& fromSampInd, size_t& nSamp);

  h5geo::SeisDataType getDataType();
  h5geo::SurveyType getSurveyType();

  bool hasPKeySort(const std::string& pKeyName);
  bool removePKeySort(const std::string& pKeyName);
  bool addPKeySort(const std::string& pKeyName);

  std::optional<H5SeisContainer> getSeisContainer();

  std::optional<h5gt::DataSet> getBoundaryD();
  std::optional<h5gt::DataSet> getTextHeaderD();
  std::optional<h5gt::DataSet> getBinHeaderD();
  std::optional<h5gt::DataSet> getTraceHeaderD();
  std::optional<h5gt::DataSet> getTraceD();
  std::optional<h5gt::Group> getSortG();
  std::optional<h5gt::Group> getUValG();
  std::optional<h5gt::Group> getIndexesG();

protected:
  h5gt::DataSet traceD, traceHeaderD;
};

#endif // H5SEIS_H
