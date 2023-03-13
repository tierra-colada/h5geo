#ifndef H5SEIS_H
#define H5SEIS_H

#include "h5baseobject.h"

#include <Eigen/Dense>

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES   // should be before <cmath>, include 'pi' val
#endif
#include <math.h>

class H5SeisContainer;
class H5Vol;

/// \class H5Seis
/// \brief Provides API to work with seismic
///
/// Seis is complex object that is represented by: 
///  * text header dataset
///  * binary header dataset
///  * trace header dataset
///  * trace data dataset
///  * optional datasets for sorting and other purposes
/// 
/// <b> Dont forget to call: </b>
///  * H5Seis::updateTraceHeaderLimits() when trace header limits were modified
///  * H5Seis::addPKeySort() before doing any sorting dependent related operations
///
/// It is designed to be fast and convenient. \n
/// <INS> SEGY reader is included. </INS> 
class H5Seis : public H5BaseObject
{
protected:
  virtual ~H5Seis() = default;

public:

  // WRITERS
  /// \brief Read text header from `SEGY` file
  virtual bool readSEGYTextHeader(
      const std::string& segy,
      h5geo::TextEncoding encoding = static_cast<h5geo::TextEncoding>(0)) = 0;
  /// \brief Read binary header from `SEGY` file
  virtual bool readSEGYBinHeader(
      const std::string& segy,
      h5geo::Endian endian = static_cast<h5geo::Endian>(0)) = 0;
  /// \brief Read trace headers and trace data from `SEGY` file
  /// \param segyFiles segy files to read traces and trace headers
  /// \param formats segy format for each segy (empty to autodefine)
  /// \param endians PC endian for each segy (empty to autodefine)
  /// \param trcHdrNamesArr use only those defined in 'getTraceHeaderShortNames',
  /// but you can change their order thus fix probably messed up trace header bytes
  /// (empty to use defined in 'getTraceHeaderShortNames' func)
  /// \param trcBuffer number of traces per thread to read before writing them at once
  /// \param nThreads number of threads (to use all threads pass any number `<1`)
  /// \param progressCallback callback function of form `void foo(double progress)`
  virtual bool readSEGYTraces(
      const std::vector<std::string>& segyFiles,
      std::vector<h5geo::SegyFormat> formats = std::vector<h5geo::SegyFormat>(),
      std::vector<h5geo::Endian> endians = std::vector<h5geo::Endian>(),
      std::vector<std::vector<std::string>> trcHdrNamesArr = std::vector<std::vector<std::string>>(),
      size_t trcBuffer = 10000,
      int nThreads = -1,
      std::function<void(double)> progressCallback = nullptr) = 0;

  /// \brief Write text header
  virtual bool writeTextHeader(const char (&txtHdr)[40][80]) = 0;

  /// \brief Write text header
  ///
  /// Maximum 40x80 chars are possible (vector of size 40 with string less or equal to 80)
  virtual bool writeTextHeader(const std::vector<std::string>& txtHdr) = 0;
  /// \brief Write binary header
  virtual bool writeBinHeader(const double (&binHdr)[30]) = 0;

  /// \brief Write text header
  ///
  /// Vector length should be equal to H5Seis::getNBinHdr()
  virtual bool writeBinHeader(const std::vector<double>& binHdrVec) = 0;
  /// \brief Write text header
  ///
  /// Vector length should be equal to H5Seis::getNBinHdr()
  virtual bool writeBinHeader(
      const Eigen::Ref<const Eigen::VectorXd>& binHdrVec) = 0;
  /// \brief Write text header
  virtual bool writeBinHeader(
      const std::string& hdrName,
      const double& value,
      const std::string& unitsFrom = "",
      const std::string& unitsTo = "") = 0;
  /// \brief Write block of traces starting from trace `fromTrc` and from sample `fromSampInd`
  virtual bool writeTrace(
      Eigen::Ref<Eigen::MatrixXf> TRACE,
      const size_t& fromTrc = 0,
      const size_t& fromSampInd = 0,
      const std::string& dataUnits = "") = 0;
  /// \brief Write traces using indexes
  ///
  /// Return `true` even if max `trcInd` exceeds `nTrc`.
  virtual bool writeTrace(
      Eigen::Ref<Eigen::MatrixXf> TRACE,
      const Eigen::Ref<const Eigen::VectorX<size_t>>& trcInd,
      const size_t& fromSampInd = 0,
      const std::string& dataUnits = "") = 0;
  /// \brief Write block of trace headers starting from trace `fromTrc` and from header index `fromHdrInd`
  virtual bool writeTraceHeader(
      const Eigen::Ref<const Eigen::MatrixXd>& HDR,
      const size_t& fromTrc = 0,
      const size_t& fromHdrInd = 0) = 0;
  /// \brief Write trace header by name
  virtual bool writeTraceHeader(
      const std::string& hdrName,
      Eigen::Ref<Eigen::MatrixXd> hdr,
      const size_t& fromTrc = 0,
      const std::string& unitsFrom = "",
      const std::string& unitsTo = "") = 0;
  /// \brief Write trace header by name and trace indexes
  ///
  /// Return `true` even if max `trcInd` exceeds `nTrc`.
  virtual bool writeTraceHeader(
      const std::string& hdrName,
      Eigen::Ref<Eigen::MatrixXd> hdr,
      const Eigen::Ref<const Eigen::VectorX<size_t>>& trcInd,
      const std::string& unitsFrom = "",
      const std::string& unitsTo = "") = 0;

  /// \brief Write `XY` trace headers (two columns in Eigen column-major matrix)
  ///
  /// Same as H5Seis::writeTraceHeaders() but also able to do a coordinate transformation.
  virtual bool writeXYTraceHeaders(
      const std::vector<std::string>& xyHdrNames,
      Eigen::Ref<Eigen::MatrixX2d>& xy,
      const size_t& fromTrc = 0,
      const std::string& lengthUnits = "",
      bool doCoordTransform = false) = 0;
  /// \brief Write `XY` trace headers (two columns in Eigen column-major matrix)
  ///
  /// Same as H5Seis::writeTraceHeaders() but also able to do a coordinate transformation. \n
  /// Return `true` even if max `trcInd` exceeds `nTrc`.
  virtual bool writeXYTraceHeaders(
      const std::vector<std::string>& xyHdrNames,
      Eigen::Ref<Eigen::MatrixX2d>& xy,
      const Eigen::Ref<const Eigen::VectorX<size_t>>& trcInd,
      const std::string& lengthUnits = "",
      bool doCoordTransform = false) = 0;

  /// \brief Resize trace and trace header DataSets
  virtual bool setNTrc(size_t nTrc) = 0;
  /// \brief Resize trace DataSet
  virtual bool setNSamp(size_t nSamp) = 0;

  // GETTERS
  /// \brief Get text header
  virtual std::vector<std::string> getTextHeader() = 0;
  /// \brief Get binary header
  virtual std::map<std::string, double> getBinHeader() = 0;
  /// \brief Get text header by name
  virtual double getBinHeader(
      const std::string& hdrName,
      const std::string& unitsFrom = "",
      const std::string& unitsTo = "") = 0;

  /// \brief Get block of traces
  ///
  /// If `nTrc` or `nSamp` exceed max values then these values are
  /// changed to max allowed (that is why they are not `const`)
  virtual Eigen::MatrixXf getTrace(
      const size_t& fromTrc,
      size_t nTrc = 1,
      const size_t& fromSampInd = 0,
      size_t nSamp = std::numeric_limits<size_t>::max(),
      const std::string& dataUnits = "") = 0;

  /// \brief Get traces by indexes
  ///
  /// Return empty matrix if max `trcInd` exceeds `nTrc`. \n
  /// That is done intensionally to keep `trcInd` size and
  /// returned number of traces equal.
  virtual Eigen::MatrixXf getTrace(
      const Eigen::Ref<const Eigen::VectorX<size_t>>& trcInd,
      const size_t& fromSampInd = 0,
      size_t nSamp = std::numeric_limits<size_t>::max(),
      const std::string& dataUnits = "") = 0;

  /// \brief Get block of trace headers
  ///
  /// If `nTrc` or `nHdr` exceed max values then these values are
  /// changed to max allowed (that is why they are not `const`)
  virtual Eigen::MatrixXd getTraceHeader(
      const size_t& fromTrc,
      size_t nTrc = 1,
      const size_t& fromHdr = 0,
      size_t nHdr = std::numeric_limits<size_t>::max(),
      const std::vector<std::string>& unitsFrom = std::vector<std::string>(),
      const std::vector<std::string>& unitsTo = std::vector<std::string>()) = 0;
  /// \brief Get block of trace header by name
  ///
  /// If `nTrc` exceeds max value then this value is
  /// changed to max allowed (that is why it is not `const`)
  virtual Eigen::VectorXd getTraceHeader(
      const std::string& hdrName,
      const size_t& fromTrc = 0,
      size_t nTrc = 1,
      const std::string& unitsFrom = "",
      const std::string& unitsTo = "") = 0;
  /// \brief Get trace headers by indexes
  ///
  /// Return empty matrix if max `trcInd` exceeds `nTrc`. \n
  /// That is done intensionally to keep `trcInd` size and
  /// returned number of traces in trace headers equal.
  virtual Eigen::MatrixXd getTraceHeader(
      const std::vector<size_t>& trcInd,
      const std::vector<size_t>& trcHdrInd,
      const std::vector<std::string>& unitsFrom = std::vector<std::string>(),
      const std::vector<std::string>& unitsTo = std::vector<std::string>()) = 0;
  /// \brief Get trace headers by indexes
  ///
  /// Return empty matrix if max `trcInd` exceeds `nTrc`. \n
  /// That is done intensionally to keep `trcInd` size and
  /// returned number of traces in trace headers equal.
  virtual Eigen::MatrixXd getTraceHeader(
      const Eigen::Ref<const Eigen::VectorX<size_t>>& trcInd,
      const Eigen::Ref<const Eigen::VectorX<size_t>>& trcHdrInd,
      const std::vector<std::string>& unitsFrom = std::vector<std::string>(),
      const std::vector<std::string>& unitsTo = std::vector<std::string>()) = 0;
  /// \brief Get trace header by indexes and name
  ///
  /// Return empty matrix if max `trcInd` exceeds `nTrc`. \n
  /// That is done intensionally to keep `trcInd` size and
  /// returned number of traces in trace header equal.
  virtual Eigen::MatrixXd getTraceHeader(
      const std::vector<std::string>& hdrNames,
      const std::vector<size_t>& trcInd,
      const std::vector<std::string>& unitsFrom = std::vector<std::string>(),
      const std::vector<std::string>& unitsTo = std::vector<std::string>()) = 0;
  /// \brief Get trace headers by indexes and names
  ///
  /// Return empty matrix if max `trcInd` exceeds `nTrc`. \n
  /// That is done intensionally to keep `trcInd` size and
  /// returned number of traces in trace headers equal.
  virtual Eigen::MatrixXd getTraceHeader(
      const std::vector<std::string>& hdrNames,
      const Eigen::Ref<const Eigen::VectorX<size_t>>& trcInd,
      const std::vector<std::string>& unitsFrom = std::vector<std::string>(),
      const std::vector<std::string>& unitsTo = std::vector<std::string>()) = 0;

  /// \brief Get `XY` trace headers (two columns in Eigen column-major matrix)
  ///
  /// Same as H5Seis::getTraceHeader() but also able to do a coordinate transformation.
  virtual Eigen::MatrixXd getXYTraceHeaders(
      const std::vector<std::string>& xyHdrNames,
      const size_t& fromTrc = 0,
      size_t nTrc = std::numeric_limits<size_t>::max(),
      const std::string& lengthUnits = "",
      bool doCoordTransform = false) = 0;
  /// \brief Get `XY` trace headers (two columns in Eigen column-major matrix)
  ///
  /// Same as H5Seis::getTraceHeader() but also able to do a coordinate transformation. \n
  /// Return empty matrix if max `trcInd` exceeds `nTrc`. \n
  /// That is done intensionally to keep `trcInd` size and
  /// returned number of traces in trace headers equal.
  virtual Eigen::MatrixXd getXYTraceHeaders(
      const std::vector<std::string>& xyHdrNames,
      const Eigen::Ref<const Eigen::VectorX<size_t>>& trcInd,
      const std::string& lengthUnits = "",
      bool doCoordTransform = false) = 0;

  /// \brief Get sorted data based on precalculated primary sort keys
  ///
  /// Before using it one should prepare primary sort keys with H5Seis::addPKeySort() method.
  /// \param TRACE not Eigen::Ref<> because Eigen::Ref<> doesn't allow to resize matrices
  /// \param HDR not Eigen::Ref<> because Eigen::Ref<> doesn't allow to resize matrices
  /// \param keyList trace header names to be worked with (first is treated as `PKey`)
  /// \param minList minimal value for each key
  /// \param maxList maximal value for each key
  /// \param pStep retrieve only n-th pkeys (for ex: SP=[1,5,6,8,9], pStep=2 -> SP_out=[1,6,9])
  /// \param fromSampInd first sample index to read (in range [0, H5Seis::getNSamp()])
  /// \param nSamp number of samples to be read (if 0 then `TRACE` will be empty). By default all samples
  /// \param dataUnits tansform data to these units
  /// \param lengthUnits works only in pair with 'doCoordTransform'
  /// \param doCoordTransform works only if two header names are passed (one for `X` and another `Y`)
  /// \return vector of trace indexes read
  virtual Eigen::VectorX<size_t> getSortedData(
      Eigen::MatrixXf& TRACE,
      Eigen::MatrixXd& HDR,
      const std::vector<std::string>& keyList,
      const std::vector<double>& minList,
      const std::vector<double>& maxList,
      size_t pStep = 1,
      size_t fromSampInd = 0,
      size_t nSamp = std::numeric_limits<size_t>::max(),
      const std::string& dataUnits = "",
      const std::string& lengthUnits = "",
      bool doCoordTransform = false) = 0;

  /// \brief Get index (position within 1D dataset) for a given binary header
  virtual ptrdiff_t getBinHeaderIndex(const std::string& hdrName) = 0;
  /// \brief Get index (row/col within 2D dataset) for a given trace header
  virtual ptrdiff_t getTraceHeaderIndex(const std::string& hdrName) = 0;

  /// \brief Get vector of equally spaced samples in specified units
  virtual Eigen::VectorXd getSamples(
      const size_t& trcInd,
      const std::string& units = "") = 0;
  /// \brief Get first sample
  virtual double getFirstSample(
      const size_t& trcInd,
      const std::string& units = "") = 0;
  /// \brief Get last sample
  virtual double getLastSample(
      const size_t& trcInd,
      const std::string& units = "") = 0;
  /// \brief Get sampling rate
  virtual double getSampRate(
      const std::string& units = "") = 0;
  /// \brief Get number of samples
  virtual size_t getNSamp() = 0;
  /// \brief Get number of traces
  virtual size_t getNTrc() = 0;
  /// \brief Get number trace headers (usually 78)
  virtual size_t getNTrcHdr() = 0;
  /// \brief Get number of binary headers
  virtual size_t getNBinHdr() = 0;
  /// \brief Get number of text header rows
  virtual size_t getNTextHdrRows() = 0;
  /// \brief Get trace indexes for given `PKey`
  ///
  /// Before using it one should prepare primary sort keys with H5Seis::addPKeySort() method.
  /// \param pKey primary key name
  /// \param pMin primary key minimal value
  /// \param pMax primary key maximal value
  /// \param pStep primary key step (0 and 1 means every pKey; else every n-th selected pKey)
  virtual Eigen::VectorX<size_t> getPKeyIndexes(
      const std::string& pKey,
      double pMin, double pMax,
      size_t pStep = 1) = 0;
  /// \brief Get `PKey` unique values
  ///
  /// Before using it one should prepare primary sort keys with H5Seis::addPKeySort() method.
  virtual Eigen::VectorXd getPKeyValues(
      const std::string& pKey,
      const std::string& unitsFrom = "",
      const std::string& unitsTo = "") = 0;
  /// \brief Get number of unique values for a given `PKey`
  ///
  /// Before using it one should prepare primary sort keys with H5Seis::addPKeySort() method.
  virtual size_t getPKeySize(const std::string& pKey) = 0;
  /// \brief Get number of unique values for a given `PKey`
  ///
  /// Before using it one should prepare primary sort keys with H5Seis::addPKeySort() method.
  virtual size_t getPKeySize(
      const std::string& pKey,
      double pMin, double pMax,
      size_t pStep = 1) = 0;
  /// \brief Get number of traces to be selected for a given `PKey`
  ///
  /// Before using it one should prepare primary sort keys with H5Seis::addPKeySort() method.
  /// \param pKey primary key name
  /// \param pMin primary key minimal value
  /// \param pMax primary key maximal value
  /// \param pStep primary key step (0 and 1 means every pKey; else every n-th selected pKey)
  virtual size_t getPKeyTraceSize(
      const std::string& pKey,
      double pMin, double pMax,
      size_t pStep = 1) = 0;
  /// \brief Get names of prepared `PKeys` (names of prepared sortings `PKeys`)
  virtual std::vector<std::string> getPKeyNames() = 0;
  /// \brief Get trace header minimal values
  virtual std::map<std::string, double> getTraceHeaderMin() = 0;
  /// \brief Get trace header maximal values
  virtual std::map<std::string, double> getTraceHeaderMax() = 0;
  /// \brief Get trace header minimal value for a given trace header
  virtual double getTraceHeaderMin(
      const std::string& hdrName,
      const std::string& unitsFrom = "",
      const std::string& unitsTo = "") = 0;
  /// \brief Get trace header maximal value for a given trace header
  virtual double getTraceHeaderMax(
      const std::string& hdrName,
      const std::string& unitsFrom = "",
      const std::string& unitsTo = "") = 0;

  /// \brief Get parameters that were used to create current seis
  virtual SeisParam getParam() = 0;

  /// \brief Check `fromTrc`, `nTrc` (passed by reference) and diminish
  /// `nTrc` to fit in data limits (if `fromTrc` is inside limit)
  /// \param fromTrc first trace (to read for example)
  /// \param nTrc number of trace (to read for example)
  /// \return
  virtual bool checkTraceLimits(
      const size_t& fromTrc, size_t& nTrc) = 0;

  /// \brief Check `fromHdr` and `nHdr` (passed by reference) and diminish
  /// `nHdr` to fit in data limits (if `fromTrc` is inside limit)
  /// \param fromHdr first header (usually there are 78 headers so
  /// the value should be less then this value)
  /// \param nHdr number of headers (to read for example)
  /// \return
  virtual bool checkTraceHeaderLimits(
      const size_t& fromHdr, size_t& nHdr) = 0;
  /// \brief Check `fromSampInd` and `nSamp` (passed by reference) and diminish
  /// `nSamp` to fit in data limits (if `fromSampInd` is inside limit)
  /// \param fromSampInd first sample index
  /// \param nSamp number of samples (to read for example)
  /// \return
  virtual bool checkSampleLimits(
      const size_t& fromSampInd, size_t& nSamp) = 0;

  /// \brief Convenient function to prepare geometry for `PRESTACK` data
  /// \param orientation counter clock angle (in radians)
  virtual bool generatePRESTKGeometry(
      double src_x0, double src_dx, size_t src_nx,
      double src_y0, double src_dy, size_t src_ny,
      double src_z,
      double rec_x0, double rec_dx, size_t rec_nx,
      double rec_y0, double rec_dy, size_t rec_ny,
      double rec_z,
      double orientation,
      bool moveRec,
      const std::string& lengthUnits = "",
      bool doCoordTransform = false) = 0;
  /// \brief Convenient function to prepare geometry for `STACK` data
  /// \param orientation counter clock angle (in radians)
  virtual bool generateSTKGeometry(
      double x0, double dx, size_t nx,
      double y0, double dy, size_t ny,
      double z,
      double orientation,
      const std::string& lengthUnits = "",
      bool doCoordTransform = false) = 0;

  /// \brief Set domain for the seismic (`TVD`, `TVDSS`, `TWT`, `OWT`)
  virtual bool setDomain(const h5geo::Domain& domain) = 0;
  /// \brief Set datatype for the seismic (`STACK` or `PRESTACK`)
  virtual bool setDataType(const h5geo::SeisDataType& seisType) = 0;
  /// \brief Set survey type for the seismic (`TWO_D` or `THREE_D`)
  virtual bool setSurveyType(const h5geo::SurveyType& surveyType) = 0;
  /// \brief Set Seismic Reference Datum
  virtual bool setSRD(double val, const std::string& lengthUnits = "") = 0;
  /// \brief Set sampling rate
  virtual bool setSampRate(double val, const std::string& units = "") = 0;
  /// \brief Set first sample
  virtual bool setFirstSample(double val, const std::string& units = "") = 0;

  /// \brief Get domain (`TVD`, `TVDSS`, `TWT`, `OWT`)
  virtual h5geo::Domain getDomain() = 0;
  /// \brief Get datatype for the seismic (`STACK` or `PRESTACK`)
  virtual h5geo::SeisDataType getDataType() = 0;
  /// \brief Set survey type for the seismic (`TWO_D` or `THREE_D`)
  virtual h5geo::SurveyType getSurveyType() = 0;
  /// \brief Get Seismic Reference Datum
  virtual double getSRD(const std::string& lengthUnits = "") = 0;

  /// \brief Check if `PKey` sort is prepared
  virtual bool hasPKeySort(const std::string& pKeyName) = 0;
  /// \brief Remove `PKey` sorting
  virtual bool removePKeySort(const std::string& pKeyName) = 0;
  /// \brief Prepare sorting
  ///
  /// Sorting is needed for any sorting related operations (like H5Seis::getSortedData()). \n
  /// If you plan to get `CDP-DSREG` data, you have to call H5Seis::addPKeySort("CDP") first.
  virtual bool addPKeySort(const std::string& pKeyName) = 0;

  /// \brief Open H5SeisContainer where current seismic resides
  virtual H5SeisContainer* openSeisContainer() = 0;

  /// \brief Get text header DataSet
  virtual std::optional<h5gt::DataSet> getTextHeaderD() = 0;
  /// \brief Get binary header DataSet
  virtual std::optional<h5gt::DataSet> getBinHeaderD() = 0;
  /// \brief Get trace header DataSet
  virtual std::optional<h5gt::DataSet> getTraceHeaderD() = 0;
  /// \brief Get trace DataSet
  virtual std::optional<h5gt::DataSet> getTraceD() = 0;
  /// \brief Get sorting Group
  virtual std::optional<h5gt::Group> getSortG() = 0;
  /// \brief Get sorting unique values Group
  virtual std::optional<h5gt::Group> getUValG() = 0;
  /// \brief Get sorting indexes Group
  virtual std::optional<h5gt::Group> getIndexesG() = 0;

  /// \brief Get `SEGY` Group (for mapped H5Seis only)
  virtual std::optional<h5gt::Group> getSEGYG() = 0;
  /// \brief Get `SEGY` text header DataSet (for mapped H5Seis only)
  virtual std::optional<h5gt::DataSet> getSEGYTextHeaderD() = 0;
  /// \brief Get `SEGY` 2-bytes binary header DataSet (for mapped H5Seis only)
  virtual std::optional<h5gt::DataSet> getSEGYBinHeader2BytesD() = 0;
  /// \brief Get `SEGY` 4-bytes binary header DataSet (for mapped H5Seis only)
  virtual std::optional<h5gt::DataSet> getSEGYBinHeader4BytesD() = 0;
  /// \brief Get `SEGY` 2-bytes trace header DataSet (for mapped H5Seis only)
  virtual std::optional<h5gt::DataSet> getSEGYTraceHeader2BytesD() = 0;
  /// \brief Get `SEGY` 4-bytes trace header DataSet (for mapped H5Seis only)
  virtual std::optional<h5gt::DataSet> getSEGYTraceHeader4BytesD() = 0;
  /// \brief Get `SEGY` float trace DataSet (for mapped H5Seis only)
  virtual std::optional<h5gt::DataSet> getSEGYTraceFloatD() = 0;

  /// \brief Calculate and write min/max trace headers
  virtual bool updateTraceHeaderLimits(size_t nTrcBuffer = 1e7) = 0;
  /// \brief Update sorting for prepared `PKey`
  virtual bool updatePKeySort(const std::string& pKeyName) = 0;

  /// \brief Calculate `XY` boundary around the survey
  ///
  /// Return two cols Eigen matrix. Use it to write as H5Horizon.
  virtual Eigen::MatrixXd calcBoundary(
      const std::string& lengthUnits = "",
      bool doCoordTransform = false) = 0;

  /// \brief Export seismic to `H5Vol`. 
  /// \note Selected traces must shape a rectangle.
  virtual bool exportToVol(H5Vol* vol, 
      const std::string& xHeader = "CDP_X",
      const std::string& yHeader = "CDP_Y",
      const std::string& ilHeader = "INLINE",
      const std::string& xlHeader = "XLINE",
      double ilMin = std::numeric_limits<double>::min(),
      double ilMax = std::numeric_limits<double>::max(),
      double xlMin = std::numeric_limits<double>::min(),
      double xlMax = std::numeric_limits<double>::max(),
      size_t fromSampInd = 0,
      size_t nSamp = std::numeric_limits<size_t>::max()) = 0;

  virtual bool exportToSEGY(
      const std::string& segyFile, 
      size_t trcBuffer = 10000, 
      std::function<void(double)> progressCallback = nullptr) = 0;
};

using H5Seis_ptr = std::unique_ptr<H5Seis, h5geo::ObjectDeleter>;

#endif // H5SEIS_H
