#ifndef H5SEISIMPL_H
#define H5SEISIMPL_H

#include "../h5seis.h"
#include "h5baseobjectimpl.h"

#include <h5gt/H5DataSet.hpp>

class H5SeisContainer;

class H5SeisImpl :
    public virtual H5Seis,
    public virtual H5BaseObjectImpl
{
protected:
  explicit H5SeisImpl(const h5gt::Group &group);
  virtual ~H5SeisImpl() = default;

public:
//  WRITERS
  virtual bool writeTextHeader(const char (&txtHdr)[40][80]) override;
  /*!
   * \brief writeTextHeader Max 40x80 chars are to be written
   * \param txtHdr
   * \return
   */
  virtual bool writeTextHeader(const std::vector<std::string>& txtHdr) override;
  virtual bool writeBinHeader(const double (&binHdr)[30]) override;
  /*!
   * \brief writeBinHeader Vector length should be equal to getNBinHdr
   * \param binHdrVec
   * \return
   */
  virtual bool writeBinHeader(const std::vector<double>& binHdrVec) override;
  virtual bool writeBinHeader(const Eigen::VectorXd& binHdrVec) override;
  virtual bool writeBinHeader(const std::string& hdrName,
      const double& value) override;
  virtual bool writeBoundary(const Eigen::MatrixX2d& boundary) override;
  virtual bool writeTrace(
      const Eigen::Ref<const Eigen::MatrixXf>& TRACE,
      const size_t& fromTrc = 0,
      const size_t& fromSampInd = 0) override;
  virtual bool writeTraceHeader(
      const Eigen::Ref<const Eigen::MatrixXd>& HDR,
      const size_t& fromTrc = 0,
      const size_t& fromHdrInd = 0) override;
  virtual bool writeTraceHeader(
      const std::string& hdrName,
      const Eigen::Ref<const Eigen::MatrixXd>& hdr,
      const size_t& fromTrc = 0) override;

//  GETTERS
  virtual std::vector<std::string> getTextHeader() override;
  virtual Eigen::VectorXd getBinHeader() override;
  virtual double getBinHeader(
      const std::string& hdrName) override;
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
      size_t nSamp = std::numeric_limits<size_t>::max()) override;
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
      size_t nHdr = std::numeric_limits<size_t>::max()) override;
  virtual Eigen::VectorXd getTraceHeader(
      const std::string& hdrName,
      const size_t& fromTrc = 0,
      const size_t& nTrc = std::numeric_limits<size_t>::max()) override;
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
      size_t nSamp = std::numeric_limits<size_t>::max()) override;

  virtual ptrdiff_t getBinHeaderIndex(const std::string& hdrName) override;
  virtual ptrdiff_t getTraceHeaderIndex(const std::string& hdrName) override;
  virtual Eigen::VectorX<size_t> getTracePKeyIndexes(
      const std::string& pName,
      const double& pMin, const double& pMax) override;

  /*! in millisec or meters !*/
  virtual Eigen::VectorXd getSamples(const size_t& trcInd) override;
  /*! in millisec or meters !*/
  virtual double getFirstSample(const size_t& trcInd) override;
  /*! in millisec or meters !*/
  virtual double getLastSample(const size_t& trcInd) override;
  /*! in millisec or meters !*/
  virtual double getSampRate() override;
  virtual size_t getNSamp() override;
  virtual size_t getNTrc() override;
  /*! usually 78 !*/
  virtual size_t getNTrcHdr() override;
  virtual size_t getNBinHdr() override;
  virtual size_t getNTextHdrRows() override;
  virtual std::vector<std::string> getPKeyNames() override;
  virtual Eigen::VectorXd getTraceHeaderMin() override;
  virtual Eigen::VectorXd getTraceHeaderMax() override;
  virtual double getTraceHeaderMin(const std::string& hdrName) override;
  virtual double getTraceHeaderMax(const std::string& hdrName) override;

  virtual Eigen::MatrixX2d calcBoundaryStk2D() override;
  virtual Eigen::MatrixX2d calcConvexHullBoundary() override;
  virtual bool calcBinOriginOrientation3DStk(
      Eigen::Ref<Eigen::Vector2d> bin,
      Eigen::Ref<Eigen::Vector2d> origin,
      double& orientation) override;
  virtual bool calcBinOriginOrientation3DStk(
      std::vector<double>& bin,
      std::vector<double>& origin,
      double& orientation) override;

  virtual bool calcAndWriteBoundary() override;
  virtual bool calcAndWriteTraceHeaderLimits(
      const size_t& nTrcBuffer = 1e7) override;

  /*!
   * \brief checkTraceLimits check *fromTrc* and *nTrc* and diminish
   *  *nTrc* to fit in data limits (if *fromTrc* is inside limit)
   * \param fromTrc first trace (to read for example)
   * \param nTrc number of trace (to read for example)
   * \return
   */
  virtual bool checkTraceLimits(
      const size_t& fromTrc, size_t& nTrc) override;
  /*!
   * \brief checkTraceHeaderLimits check 'fromHdr' and 'nHdr' and diminish
   * 'nHdr' to fit in data limits (if 'fromTrc' is inside limit)
   * \param fromHdr first header (usually there are 78 headers so
   * the value should be less then this value)
   * \param nHdr number of headers (to read for example)
   * \return
   */
  virtual bool checkTraceHeaderLimits(
      const size_t& fromHdr, size_t& nHdr) override;
  virtual bool checkSampleLimits(
      const size_t& fromSampInd, size_t& nSamp) override;

  virtual h5geo::SeisDataType getDataType() override;
  virtual h5geo::SurveyType getSurveyType() override;

  virtual bool hasPKeySort(const std::string& pKeyName) override;
  virtual bool removePKeySort(const std::string& pKeyName) override;
  virtual bool addPKeySort(const std::string& pKeyName) override;

  virtual H5SeisContainer* getSeisContainer() override;

  virtual std::optional<h5gt::DataSet> getBoundaryD() override;
  virtual std::optional<h5gt::DataSet> getTextHeaderD() override;
  virtual std::optional<h5gt::DataSet> getBinHeaderD() override;
  virtual std::optional<h5gt::DataSet> getTraceHeaderD() override;
  virtual std::optional<h5gt::DataSet> getTraceD() override;
  virtual std::optional<h5gt::Group> getSortG() override;
  virtual std::optional<h5gt::Group> getUValG() override;
  virtual std::optional<h5gt::Group> getIndexesG() override;

protected:
  h5gt::DataSet traceD, traceHeaderD;


  //----------- FRIEND CLASSES -----------
  friend class H5SeisContainerImpl;
};

#endif // H5SEISIMPL_H