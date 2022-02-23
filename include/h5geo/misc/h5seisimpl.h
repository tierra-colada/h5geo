#ifndef H5SEISIMPL_H
#define H5SEISIMPL_H

#include "../h5seis.h"
#include "h5baseobjectimpl.h"

#include <h5gt/H5DataSet.hpp>

class H5SeisContainer;

class H5SeisImpl : public H5BaseObjectImpl<H5Seis>
{
protected:
  explicit H5SeisImpl(const h5gt::Group &group);
  virtual ~H5SeisImpl() = default;

public:
  // WRITERS
  virtual bool writeTextHeader(const char (&txtHdr)[40][80]) override;
  virtual bool writeTextHeader(const std::vector<std::string>& txtHdr) override;

  virtual bool writeBinHeader(const double (&binHdr)[30]) override;
  virtual bool writeBinHeader(const std::vector<double>& binHdrVec) override;
  virtual bool writeBinHeader(
      const Eigen::Ref<const Eigen::VectorXd>& binHdrVec) override;
  virtual bool writeBinHeader(
      const std::string& hdrName,
      const double& value,
      const std::string& unitsFrom = "",
      const std::string& unitsTo = "") override;

  virtual bool writeBoundary(
      Eigen::Ref<Eigen::MatrixX2d> M,
      const std::string& lengthUnits = "",
      bool doCoordTransform = false) override;

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
      Eigen::Ref<Eigen::MatrixXd> hdr,
      const size_t& fromTrc = 0,
      const std::string& unitsFrom = "",
      const std::string& unitsTo = "") override;
  virtual bool writeTraceHeader(
      const std::string& hdrName,
      Eigen::Ref<Eigen::MatrixXd> hdr,
      const Eigen::Ref<const Eigen::VectorX<size_t>>& trcInd,
      const std::string& unitsFrom = "",
      const std::string& unitsTo = "") override;

  virtual bool writeXYTraceHeaders(
      const std::vector<std::string>& xyHdrNames,
      Eigen::Ref<Eigen::MatrixXd>& xy,
      const size_t& fromTrc = 0,
      const std::string& lengthUnits = "",
      bool doCoordTransform = false) override;
  virtual bool writeXYTraceHeaders(
      const std::vector<std::string>& xyHdrNames,
      Eigen::Ref<Eigen::MatrixXd>& xy,
      const Eigen::Ref<const Eigen::VectorX<size_t>>& trcInd,
      const std::string& lengthUnits = "",
      bool doCoordTransform = false) override;

  virtual bool setNTrc(size_t nTrc) override;
  virtual bool setNSamp(size_t nSamp) override;

  // GETTERS
  virtual std::vector<std::string> getTextHeader() override;
  virtual std::map<std::string, double> getBinHeader() override;
  virtual double getBinHeader(
      const std::string& hdrName,
      const std::string& unitsFrom = "",
      const std::string& unitsTo = "") override;

  virtual Eigen::MatrixXf getTrace(
      const size_t& fromTrc,
      size_t nTrc = 1,
      const size_t& fromSampInd = 0,
      size_t nSamp = std::numeric_limits<size_t>::max(),
      const std::string& dataUnits = "") override;

  virtual Eigen::MatrixXd getTraceHeader(
      const size_t& fromTrc,
      size_t nTrc = 1,
      const size_t& fromHdr = 0,
      size_t nHdr = std::numeric_limits<size_t>::max(),
      const std::vector<std::string>& unitsFrom = std::vector<std::string>(),
      const std::vector<std::string>& unitsTo = std::vector<std::string>()) override;
  virtual Eigen::VectorXd getTraceHeader(
      const std::string& hdrName,
      const size_t& fromTrc = 0,
      const size_t& nTrc = std::numeric_limits<size_t>::max(),
      const std::string& unitsFrom = "",
      const std::string& unitsTo = "") override;
  virtual Eigen::MatrixXd getTraceHeader(
      const std::vector<size_t>& trcInd,
      const std::vector<size_t>& trcHdrInd,
      const std::vector<std::string>& unitsFrom = std::vector<std::string>(),
      const std::vector<std::string>& unitsTo = std::vector<std::string>()) override;
  virtual Eigen::MatrixXd getTraceHeader(
      const Eigen::Ref<const Eigen::VectorX<size_t>>& trcInd,
      const Eigen::Ref<const Eigen::VectorX<size_t>>& trcHdrInd,
      const std::vector<std::string>& unitsFrom = std::vector<std::string>(),
      const std::vector<std::string>& unitsTo = std::vector<std::string>()) override;
  virtual Eigen::MatrixXd getTraceHeader(
      const std::vector<std::string>& hdrNames,
      const std::vector<size_t>& trcInd,
      const std::vector<std::string>& unitsFrom = std::vector<std::string>(),
      const std::vector<std::string>& unitsTo = std::vector<std::string>()) override;
  virtual Eigen::MatrixXd getTraceHeader(
      const std::vector<std::string>& hdrNames,
      const Eigen::Ref<const Eigen::VectorX<size_t>>& trcInd,
      const std::vector<std::string>& unitsFrom = std::vector<std::string>(),
      const std::vector<std::string>& unitsTo = std::vector<std::string>()) override;

  virtual Eigen::MatrixXd getXYTraceHeaders(
      const std::vector<std::string>& xyHdrNames,
      const size_t& fromTrc = 0,
      size_t nTrc = std::numeric_limits<size_t>::max(),
      const std::string& lengthUnits = "",
      bool doCoordTransform = false) override;
  virtual Eigen::MatrixXd getXYTraceHeaders(
      const std::vector<std::string>& xyHdrNames,
      const Eigen::Ref<const Eigen::VectorX<size_t>>& trcInd,
      const std::string& lengthUnits = "",
      bool doCoordTransform = false) override;

  virtual Eigen::VectorX<size_t> getSortedData(
      Eigen::MatrixXf& TRACE,
      Eigen::MatrixXd& HDR,
      const std::vector<std::string>& keyList,
      const std::vector<double>& minList,
      const std::vector<double>& maxList,
      size_t fromSampInd = 0,
      size_t nSamp = std::numeric_limits<size_t>::max(),
      bool readTraceByTrace = true,
      const std::string& dataUnits = "",
      const std::string& lengthUnits = "",
      bool doCoordTransform = false) override;

  virtual ptrdiff_t getBinHeaderIndex(const std::string& hdrName) override;
  virtual ptrdiff_t getTraceHeaderIndex(const std::string& hdrName) override;

  virtual Eigen::VectorXd getSamples(
      const size_t& trcInd,
      const std::string& units = "") override;
  virtual double getFirstSample(
      const size_t& trcInd,
      const std::string& units = "") override;
  virtual double getLastSample(
      const size_t& trcInd,
      const std::string& units = "") override;
  virtual double getSampRate(const std::string& units = "") override;
  virtual size_t getNSamp() override;
  virtual size_t getNTrc() override;
  /*! usually 78 !*/
  virtual size_t getNTrcHdr() override;
  virtual size_t getNBinHdr() override;
  virtual size_t getNTextHdrRows() override;
  virtual Eigen::VectorX<size_t> getPKeyIndexes(
      const std::string& pName,
      const double& pMin, const double& pMax) override;
  virtual Eigen::VectorXd getPKeyValues(
      const std::string& pkey,
      const std::string& unitsFrom = "",
      const std::string& unitsTo = "") override;
  virtual size_t getPKeySize(const std::string& pkey) override;
  virtual std::vector<std::string> getPKeyNames() override;
  virtual std::map<std::string, double> getTraceHeaderMin() override;
  virtual std::map<std::string, double> getTraceHeaderMax() override;
  virtual double getTraceHeaderMin(
      const std::string& hdrName,
      const std::string& unitsFrom = "",
      const std::string& unitsTo = "") override;
  virtual double getTraceHeaderMax(
      const std::string& hdrName,
      const std::string& unitsFrom = "",
      const std::string& unitsTo = "") override;

  virtual bool checkTraceLimits(
      const size_t& fromTrc, size_t& nTrc) override;

  virtual bool checkTraceHeaderLimits(
      const size_t& fromHdr, size_t& nHdr) override;
  virtual bool checkSampleLimits(
      const size_t& fromSampInd, size_t& nSamp) override;

  /// Generate geometry and set nTrc to match it and
  /// set seis datatype to PRESTACK
  virtual bool generatePRESTKGeometry(
      double src_x0, double src_dx, size_t src_nx,
      double src_y0, double src_dy, size_t src_ny,
      double src_z,
      double rec_x0, double rec_dx, size_t rec_nx,
      double rec_y0, double rec_dy, size_t rec_ny,
      double rec_z,
      bool moveRec,
      const std::string& lengthUnits = "",
      bool doCoordTransform = false) override;
  /// Generate geometry and set nTrc to match it and
  /// set seis datatype to STACK
  virtual bool generateSTKGeometry(
      double x0, double dx, size_t nx,
      double y0, double dy, size_t ny,
      double z,
      const std::string& lengthUnits = "",
      bool doCoordTransform = false) override;

  virtual bool setDomain(const h5geo::Domain& val) override;
  virtual bool setDataType(const h5geo::SeisDataType& val) override;
  virtual bool setSurveyType(const h5geo::SurveyType& val) override;
  virtual bool setSRD(double val, const std::string& lengthUnits = "") override;
  virtual bool setSampRate(double val, const std::string& units = "") override;

  virtual h5geo::Domain getDomain() override;
  virtual h5geo::SeisDataType getDataType() override;
  virtual h5geo::SurveyType getSurveyType() override;
  virtual double getSRD(const std::string& lengthUnits = "") override;
  virtual Eigen::MatrixXd getBoundary(
      const std::string& lengthUnits = "",
      bool doCoordTransform = false) override;

  virtual bool hasPKeySort(const std::string& pKeyName) override;
  virtual bool removePKeySort(const std::string& pKeyName) override;
  virtual bool addPKeySort(const std::string& pKeyName) override;

  virtual H5SeisContainer* openSeisContainer() override;

  virtual std::optional<h5gt::DataSet> getBoundaryD() override;
  virtual std::optional<h5gt::DataSet> getTextHeaderD() override;
  virtual std::optional<h5gt::DataSet> getBinHeaderD() override;
  virtual std::optional<h5gt::DataSet> getTraceHeaderD() override;
  virtual std::optional<h5gt::DataSet> getTraceD() override;
  virtual std::optional<h5gt::Group> getSortG() override;
  virtual std::optional<h5gt::Group> getUValG() override;
  virtual std::optional<h5gt::Group> getIndexesG() override;

  virtual bool updateTraceHeaderLimits(size_t nTrcBuffer = 1e7) override;
  virtual bool updateBoundary() override;
  virtual bool updatePKeySort(const std::string& pKeyName) override;

protected:
  virtual Eigen::MatrixX2d calcBoundaryStk2D();
  virtual Eigen::MatrixX2d calcConvexHullBoundary();
  virtual void calcGrid3D(
      const Eigen::Ref<Eigen::VectorXd>& x,
      const Eigen::Ref<Eigen::VectorXd>& y,
      double z,
      Eigen::Ref<Eigen::VectorXd> x_loc,
      Eigen::Ref<Eigen::VectorXd> y_loc,
      Eigen::Ref<Eigen::VectorXd> z_loc);

protected:
  h5gt::DataSet traceD, traceHeaderD;


  //----------- FRIEND CLASSES -----------
  friend class H5SeisContainerImpl;
  friend class H5BaseObjectImpl<H5Seis>;
  friend H5Seis* h5geo::openSeis(h5gt::Group group);
};

#endif // H5SEISIMPL_H
