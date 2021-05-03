#ifndef H5SEIS_H
#define H5SEIS_H

#include "h5baseobject.h"

#include <Eigen/Dense>

#define _USE_MATH_DEFINES   // should be before <cmath>, include 'pi' val
#include <math.h>

class H5SeisContainer;

class H5Seis : public virtual H5BaseObject
{
protected:
  virtual ~H5Seis() = default;

public:
//  WRITERS
  virtual bool writeTextHeader(const char (&txtHdr)[40][80]) = 0;
  /*!
   * \brief writeTextHeader Max 40x80 chars are to be written
   * \param txtHdr
   * \return
   */
  virtual bool writeTextHeader(const std::vector<std::string>& txtHdr) = 0;
  virtual bool writeBinHeader(const double (&binHdr)[30]) = 0;
  /*!
   * \brief writeBinHeader Vector length should be equal to getNBinHdr
   * \param binHdrVec
   * \return
   */
  virtual bool writeBinHeader(const std::vector<double>& binHdrVec) = 0;
  virtual bool writeBinHeader(const Eigen::VectorXd& binHdrVec) = 0;
  virtual bool writeBinHeader(const std::string& hdrName,
      const double& value) = 0;
  virtual bool writeBoundary(const Eigen::MatrixX2d& boundary) = 0;
  virtual bool writeTrace(
      const Eigen::Ref<const Eigen::MatrixXf>& TRACE,
      const size_t& fromTrc = 0,
      const size_t& fromSampInd = 0) = 0;
  virtual bool writeTraceHeader(
      const Eigen::Ref<const Eigen::MatrixXd>& HDR,
      const size_t& fromTrc = 0,
      const size_t& fromHdrInd = 0) = 0;
  virtual bool writeTraceHeader(
      const std::string& hdrName,
      const Eigen::Ref<const Eigen::MatrixXd>& hdr,
      const size_t& fromTrc = 0) = 0;

//  GETTERS
  virtual std::vector<std::string> getTextHeader() = 0;
  virtual Eigen::VectorXd getBinHeader() = 0;
  virtual double getBinHeader(
      const std::string& hdrName) = 0;
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
  virtual Eigen::MatrixXf getTrace(
      const size_t& fromTrc,
      size_t nTrc = 1,
      const size_t& fromSampInd = 0,
      size_t nSamp = std::numeric_limits<size_t>::max()) = 0;
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
  virtual Eigen::MatrixXd getTraceHeader(
      const size_t& fromTrc,
      size_t nTrc = 1,
      const size_t& fromHdr = 0,
      size_t nHdr = std::numeric_limits<size_t>::max()) = 0;
  virtual Eigen::VectorXd getTraceHeader(
      const std::string& hdrName,
      const size_t& fromTrc = 0,
      const size_t& nTrc = std::numeric_limits<size_t>::max()) = 0;
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
  virtual Eigen::VectorX<size_t> getSortedData(
      Eigen::MatrixXf& TRACE,
      Eigen::MatrixXd& HDR,
      const std::vector<std::string>& keyList,
      const std::vector<double>& minList,
      const std::vector<double>& maxList,
      size_t fromSampInd = 0,
      size_t nSamp = std::numeric_limits<size_t>::max()) = 0;

  virtual ptrdiff_t getBinHeaderIndex(const std::string& hdrName) = 0;
  virtual ptrdiff_t getTraceHeaderIndex(const std::string& hdrName) = 0;
  virtual Eigen::VectorX<size_t> getTracePKeyIndexes(
      const std::string& pName,
      const double& pMin, const double& pMax) = 0;

  /*! in millisec or meters !*/
  virtual Eigen::VectorXd getSamples(const size_t& trcInd) = 0;
  /*! in millisec or meters !*/
  virtual double getFirstSample(const size_t& trcInd) = 0;
  /*! in millisec or meters !*/
  virtual double getLastSample(const size_t& trcInd) = 0;
  /*! in millisec or meters !*/
  virtual double getSampRate() = 0;
  virtual size_t getNSamp() = 0;
  virtual size_t getNTrc() = 0;
  /*! usually 78 !*/
  virtual size_t getNTrcHdr() = 0;
  virtual size_t getNBinHdr() = 0;
  virtual size_t getNTextHdrRows() = 0;
  virtual std::vector<std::string> getPKeyNames() = 0;
  virtual Eigen::VectorXd getTraceHeaderMin() = 0;
  virtual Eigen::VectorXd getTraceHeaderMax() = 0;
  virtual double getTraceHeaderMin(const std::string& hdrName) = 0;
  virtual double getTraceHeaderMax(const std::string& hdrName) = 0;

  virtual Eigen::MatrixX2d calcBoundaryStk2D() = 0;
  virtual Eigen::MatrixX2d calcConvexHullBoundary() = 0;
  virtual bool calcBinOriginOrientation3DStk(
      Eigen::Ref<Eigen::Vector2d> bin,
      Eigen::Ref<Eigen::Vector2d> origin,
      double& orientation) = 0;
  virtual bool calcBinOriginOrientation3DStk(
      std::vector<double>& bin,
      std::vector<double>& origin,
      double& orientation) = 0;

  virtual bool calcAndWriteBoundary() = 0;
  virtual bool calcAndWriteTraceHeaderLimits(
      const size_t& nTrcBuffer = 1e7) = 0;

  /*!
   * \brief checkTraceLimits check *fromTrc* and *nTrc* and diminish
   *  *nTrc* to fit in data limits (if *fromTrc* is inside limit)
   * \param fromTrc first trace (to read for example)
   * \param nTrc number of trace (to read for example)
   * \return
   */
  virtual bool checkTraceLimits(
      const size_t& fromTrc, size_t& nTrc) = 0;
  /*!
   * \brief checkTraceHeaderLimits check 'fromHdr' and 'nHdr' and diminish
   * 'nHdr' to fit in data limits (if 'fromTrc' is inside limit)
   * \param fromHdr first header (usually there are 78 headers so
   * the value should be less then this value)
   * \param nHdr number of headers (to read for example)
   * \return
   */
  virtual bool checkTraceHeaderLimits(
      const size_t& fromHdr, size_t& nHdr) = 0;
  virtual bool checkSampleLimits(
      const size_t& fromSampInd, size_t& nSamp) = 0;

  virtual h5geo::SeisDataType getDataType() = 0;
  virtual h5geo::SurveyType getSurveyType() = 0;

  virtual bool hasPKeySort(const std::string& pKeyName) = 0;
  virtual bool removePKeySort(const std::string& pKeyName) = 0;
  virtual bool addPKeySort(const std::string& pKeyName) = 0;

  virtual H5SeisContainer* getSeisContainer() = 0;

  virtual std::optional<h5gt::DataSet> getBoundaryD() = 0;
  virtual std::optional<h5gt::DataSet> getTextHeaderD() = 0;
  virtual std::optional<h5gt::DataSet> getBinHeaderD() = 0;
  virtual std::optional<h5gt::DataSet> getTraceHeaderD() = 0;
  virtual std::optional<h5gt::DataSet> getTraceD() = 0;
  virtual std::optional<h5gt::Group> getSortG() = 0;
  virtual std::optional<h5gt::Group> getUValG() = 0;
  virtual std::optional<h5gt::Group> getIndexesG() = 0;
};

using H5Seis_ptr = std::unique_ptr<H5Seis, h5geo::ObjectDeleter>;

#endif // H5SEIS_H