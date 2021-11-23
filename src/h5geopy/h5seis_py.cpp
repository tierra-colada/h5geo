#include "../../include/h5geopy/h5seis_py.h"

namespace h5geopy {

namespace ext {

std::tuple<Eigen::MatrixXf, Eigen::MatrixXd, Eigen::VectorX<size_t>>
getSortedData(
    H5SeisImpl* self,
    const std::vector<std::string>& keyList,
    const std::vector<double>& minList,
    const std::vector<double>& maxList,
    size_t fromSampInd = 0,
    size_t nSamp = std::numeric_limits<size_t>::max(),
    bool readTraceByTrace = true,
    const std::string& dataUnits = "")
{
  Eigen::MatrixXf TRACE;
  Eigen::MatrixXd HDR;
  Eigen::VectorX<size_t> idx = self->getSortedData(
        TRACE, HDR, keyList, minList, maxList, fromSampInd, nSamp, readTraceByTrace, dataUnits);
  return std::make_tuple(std::move(TRACE), std::move(HDR), std::move(idx));
}

std::tuple<Eigen::Vector2d, Eigen::Vector2d, double, bool>
calcSpacingOriginOrientation3DStk(H5SeisImpl* self)
{
  Eigen::Vector2d bin, origin;
  double orientation;
  bool val = self->calcSpacingOriginOrientation3DStk(
        bin, origin, orientation);
  return std::make_tuple(std::move(bin), std::move(origin), orientation, val);
}

std::tuple<size_t, bool>
checkTraceLimits(H5SeisImpl* self, const size_t& fromTrc, size_t& nTrc)
{
  bool val = self->checkTraceLimits(
        fromTrc, nTrc);
  return std::make_tuple(nTrc, val);
}

std::tuple<size_t, bool>
checkTraceHeaderLimits(H5SeisImpl* self, const size_t& fromHdr, size_t& nHdr)
{
  bool val = self->checkTraceHeaderLimits(
        fromHdr, nHdr);
  return std::make_tuple(nHdr, val);
}

std::tuple<size_t, bool>
checkSampleLimits(H5SeisImpl* self, const size_t& fromSampInd, size_t& nSamp)
{
  bool val = self->checkSampleLimits(
        fromSampInd, nSamp);
  return std::make_tuple(nSamp, val);
}


}

void H5Seis_py(
    py::class_<
    H5SeisImpl,
    std::unique_ptr<H5SeisImpl, ObjectDeleter>,
    H5BaseObjectImpl,
    H5Seis> &py_obj){
  py_obj
      //  WRITERS
      .def("writeTextHeader", py::overload_cast<
           const std::vector<std::string>&>(
             &H5SeisImpl::writeTextHeader),
           py::arg("txtHdr"))
      .def("writeBinHeader", py::overload_cast<
           const std::vector<double>&>(
             &H5SeisImpl::writeBinHeader),
           py::arg("binHdr"))
      .def("writeBinHeader", py::overload_cast<
           const Eigen::Ref<const Eigen::VectorXd>&>(
             &H5SeisImpl::writeBinHeader),
           py::arg("binHdr"))
      .def("writeBinHeader", py::overload_cast<
           const std::string&,
           const double&,
           const std::string&,
           const std::string&>(
             &H5SeisImpl::writeBinHeader),
           py::arg("hdrName"),
           py::arg("value"),
           py::arg_v("unitsFrom", "", "str()"),
           py::arg_v("unitsTo", "", "str()"))
      .def("writeBoundary", &H5SeisImpl::writeBoundary,
           py::arg("boundary"),
           py::arg_v("spatialUnits", "", "str()"),
           "write boundary of 2d (a line) or 3d (usually convex hull or concave hull) seismic survey. "
"Input argument is `MatrixX2d` where first col - `X` coord, second - `Y` coord")
      .def("writeTrace", &H5SeisImpl::writeTrace,
           py::arg("TRACE"),
           py::arg_v("fromTrc", 0, "0"),
           py::arg_v("fromSampInd", 0, "0"))
      .def("writeTraceHeader", py::overload_cast<
           const Eigen::Ref<const Eigen::MatrixXd>&,
           const size_t&,
           const size_t&>(
             &H5SeisImpl::writeTraceHeader),
           py::arg("HDR"),
           py::arg_v("fromTrc", 0, "0"),
           py::arg_v("fromHdrInd", 0, "0"))
      .def("writeTraceHeader", py::overload_cast<
           const std::string&,
           const Eigen::Ref<const Eigen::MatrixXd>&,
           const size_t&,
           const std::string&,
           const std::string&>(
             &H5SeisImpl::writeTraceHeader),
           py::arg("hdrName"),
           py::arg("hdr"),
           py::arg_v("fromTrc", 0, "0"),
           py::arg_v("unitsFrom", "", "str()"),
           py::arg_v("unitsTo", "", "str()"))

      //  GETTERS
      .def("getTextHeader", &H5SeisImpl::getTextHeader)
      .def("getBinHeader", py::overload_cast<>(
             &H5SeisImpl::getBinHeader))
      .def("getBinHeader", py::overload_cast<
           const std::string&,
           const std::string&,
           const std::string&>(
             &H5SeisImpl::getBinHeader),
           py::arg("hdrName"),
           py::arg_v("unitsFrom", "", "str()"),
           py::arg_v("unitsTo", "", "str()"))
      .def("getTrace", &H5SeisImpl::getTrace,
           py::arg("fromTrc"),
           py::arg_v("nTrc", 1, "1"),
           py::arg_v("fromSampInd", 0, "0"),
           py::arg_v("nSamp", std::numeric_limits<size_t>::max(), "sys.maxint"),
           py::arg_v("dataUnits", "", "str()"),
           "Get block of traces. If `nTrc` or `nSamp` exceed max values then these values are changed to max allowed (that is why they are not `const`)")
      .def("getTraceHeader", py::overload_cast<
           const size_t&,
           size_t,
           const size_t&,
           size_t,const std::vector<std::string>&,
           const std::vector<std::string>&>(
             &H5SeisImpl::getTraceHeader),
           py::arg("fromTrc"),
           py::arg_v("nTrc", 1, "1"),
           py::arg_v("fromHdr", 0, "0"),
           py::arg_v("nHdr", std::numeric_limits<size_t>::max(), "sys.maxint"),
           py::arg_v("unitsFrom", std::vector<std::string>(), "list()"),
           py::arg_v("unitsTo", std::vector<std::string>(), "list()"),
           "Get block of trace headers. If `nTrc` or `nHdr` exceed max values then these values are changed to max allowed (that is why they are not `const`)")
      .def("getTraceHeader", py::overload_cast<
           const std::string&,
           const size_t&,
           const size_t&,
           const std::string&,
           const std::string&>(
             &H5SeisImpl::getTraceHeader),
           py::arg("hdrName"),
           py::arg_v("fromTrc", 0, "0"),
           py::arg_v("nTrc", std::numeric_limits<size_t>::max(), "sys.maxint"),
           py::arg_v("unitsFrom", "", "str()"),
           py::arg_v("unitsTo", "", "str()"))
      .def("getTraceHeader", py::overload_cast<
           const std::vector<size_t>&,
           const std::vector<size_t>&,
           const std::vector<std::string>&,
           const std::vector<std::string>&>(
             &H5SeisImpl::getTraceHeader),
           py::arg("trcInd"),
           py::arg("trcHdrInd"),
           py::arg_v("unitsFrom", std::vector<std::string>(), "list()"),
           py::arg_v("unitsTo", std::vector<std::string>(), "list()"))
      .def("getTraceHeader", py::overload_cast<
           const std::vector<std::string>&,
           const std::vector<size_t>&,
           const std::vector<std::string>&,
           const std::vector<std::string>&>(
             &H5SeisImpl::getTraceHeader),
           py::arg("hdrNames"),
           py::arg("trcHdrInd"),
           py::arg_v("unitsFrom", std::vector<std::string>(), "list()"),
           py::arg_v("unitsTo", std::vector<std::string>(), "list()"))
      .def("getSortedData", &ext::getSortedData,
           py::arg("keyList"),
           py::arg("minList"),
           py::arg("maxList"),
           py::arg_v("fromSampInd", 0, "0"),
           py::arg_v("nSamp", std::numeric_limits<size_t>::max(), "sys.maxint"),
           py::arg_v("readTraceByTrace", true, "True"),
           py::arg_v("dataUnits", "", "str()"),
           "Get sorted data based on precalculated primary sort keys (e.g. before using it you should prepare primary sort keys with `addPKeySort(...)` method)."
"Return `TRACE` (traces matrix), `HDR` (hdr matrix) and `idx` (vector of trace indexes read)")

      .def("getBinHeaderIndex", &H5SeisImpl::getBinHeaderIndex,
           py::arg("hdrName"))
      .def("getTraceHeaderIndex", &H5SeisImpl::getTraceHeaderIndex,
           py::arg("hdrName"))

      .def("getSamples", &H5SeisImpl::getSamples,
           py::arg("trcInd"),
           py::arg_v("units", "", "str()"),
           "in units according to `Domain` (`METER` or `SECOND` or else...)")
      .def("getFirstSample", &H5SeisImpl::getFirstSample,
           py::arg("trcInd"),
           py::arg_v("units", "", "str()"),
           "in units according to `Domain` (`METER` or `SECOND` or else...)")
      .def("getLastSample", &H5SeisImpl::getLastSample,
           py::arg("trcInd"),
           py::arg_v("units", "", "str()"),
           "in units according to `Domain` (`METER` or `SECOND` or else...)")
      .def("getSampRate", &H5SeisImpl::getSampRate,
           py::arg_v("units", "", "str()"),
           "in units according to `Domain` (`METER` or `SECOND` or else...)")
      .def("getNSamp", &H5SeisImpl::getNSamp,
           "get number of samples (i.e. trace length)")
      .def("getNTrc", &H5SeisImpl::getNTrc,
           "get number of traces")
      .def("getNTrcHdr", &H5SeisImpl::getNTrcHdr,
           "get number of trace headers (usually 78)")
      .def("getNBinHdr", &H5SeisImpl::getNBinHdr,
           "get number of bin headers")
      .def("getNTextHdrRows", &H5SeisImpl::getNTextHdrRows,
           "get number lines of text header")
      .def("getPKeyIndexes", &H5SeisImpl::getPKeyIndexes,
           py::arg("pName"),
           py::arg("pMin"),
           py::arg("pMax"))
      .def("getPKeyValues", &H5SeisImpl::getPKeyValues,
           py::arg("pkey"),
           py::arg_v("unitsFrom", "", "str()"),
           py::arg_v("unitsTo", "", "str()"))
      .def("getPKeySize", &H5SeisImpl::getPKeySize,
           py::arg("pName"))
      .def("getPKeyNames", &H5SeisImpl::getPKeyNames,
           "get primary key names (usually they are used in sorting)")
      .def("getTraceHeaderMin", py::overload_cast<>(
             &H5SeisImpl::getTraceHeaderMin))
      .def("getTraceHeaderMax", py::overload_cast<>(
             &H5SeisImpl::getTraceHeaderMax))
      .def("getTraceHeaderMin", py::overload_cast<
           const std::string&,
           const std::string&,
           const std::string&>(
             &H5SeisImpl::getTraceHeaderMin),
           py::arg("hdrName"),
           py::arg_v("unitsFrom", "", "str()"),
           py::arg_v("unitsTo", "", "str()"))
      .def("getTraceHeaderMax", py::overload_cast<
           const std::string&,
           const std::string&,
           const std::string&>(
             &H5SeisImpl::getTraceHeaderMax),
           py::arg("hdrName"),
           py::arg_v("unitsFrom", "", "str()"),
           py::arg_v("unitsTo", "", "str()"))

      .def("calcBoundaryStk2D", &H5SeisImpl::calcBoundaryStk2D,
           "calculate boundary for 2D stk seismic")
      .def("calcConvexHullBoundary", &H5SeisImpl::calcConvexHullBoundary,
           "calculate convex boundary (usually used in 3D seismic or 2D prestack seismic)")
      .def("calcSpacingOriginOrientation3DStk", &ext::calcSpacingOriginOrientation3DStk,
           "calculate `bin` (along `INLINE` and `XLINE` respectively)`, "
"`origin` (`XY` point coordinate where sorted: 1) INLINE->min, 2) CDP_X->min, 3) CDP_Y->min),"
"`orientation` an azimuth along `INLINE` (in radians). All these parameters are needed when displaying in VTK scene."
"Also returns `bool val` set to true if successful.")

      .def("calcAndWriteBoundary", &H5SeisImpl::calcAndWriteBoundary,
           "calculate boundary for any seismic survey type and write it. Retun true if successful")
      .def("calcAndWriteTraceHeaderLimits", &H5SeisImpl::calcAndWriteTraceHeaderLimits,
           py::arg_v("nTrcBuffer", 1e7, "1e7"),
           "calculate and write min and max values for each header. "
"This is needed by some operations. By default trace buffer is set `nTrcBuffer` to 10 millions of traces")

      .def("checkTraceLimits", &ext::checkTraceLimits,
           py::arg("fromTrc"),
           py::arg("nTrc"),
           "check 'fromTrc' and 'nTrc' and diminish 'nTrc' to fit in data limits (if 'fromTrc' is inside limit)"
"`fromTrc` first trace (the value should be less then number of traces)"
"`nTrc` number of traces (to read for example). Return corrected `nTrc`")
      .def("checkTraceHeaderLimits", &ext::checkTraceHeaderLimits,
           py::arg("fromHdr"),
           py::arg("nHdr"),
           "check 'fromHdr' and 'nHdr' and diminish 'nHdr' to fit in data limits (if 'fromHdr' is inside limit)"
"`fromHdr` first header (usually there are 78 headers so the value should be less then this value)"
"`nHdr` number of headers (to read for example). Return corrected `nHdr`")
      .def("checkSampleLimits", &ext::checkSampleLimits,
           py::arg("fromSampInd"),
           py::arg("nSamp"),
           "check 'fromSampInd' and 'nSamp' and diminish 'nSamp' to fit in data limits (if 'fromSampInd' is inside limit)"
"`fromSampInd` first index (the value should be less then number of samples)"
"`nSamp` number of samples (to read for example). Return corrected `nSamp`")

      .def("setDomain", &H5SeisImpl::setDomain)
      .def("setDataType", &H5SeisImpl::setDataType)
      .def("setSurveyType", &H5SeisImpl::setSurveyType)
      .def("setSRD", &H5SeisImpl::setSRD,
           py::arg("value"),
           py::arg_v("units", "", "str()"))
      .def("setSpatialUnits", &H5SeisImpl::setSpatialUnits)
      .def("setTemporalUnits", &H5SeisImpl::setTemporalUnits)
      .def("setAngleUnits", &H5SeisImpl::setAngleUnits)
      .def("setDataUnits", &H5SeisImpl::setDataUnits)
      .def("setOrientation", &H5SeisImpl::setOrientation,
           py::arg("value"),
           py::arg_v("units", "", "str()"))
      .def("setOrigin", &H5SeisImpl::setOrigin,
           py::arg("value"),
           py::arg_v("units", "", "str()"))
      .def("setSpacing", &H5SeisImpl::setSpacing,
           py::arg("value"),
           py::arg_v("units", "", "str()"))

      .def("getDomain", &H5SeisImpl::getDomain)
      .def("getDataType", &H5SeisImpl::getDataType)
      .def("getSurveyType", &H5SeisImpl::getSurveyType)
      .def("getSRD", &H5SeisImpl::getSRD,
           py::arg_v("units", "", "str()"))
      .def("getSpatialUnits", &H5SeisImpl::getSpatialUnits)
      .def("getTemporalUnits", &H5SeisImpl::getTemporalUnits)
      .def("getAngleUnits", &H5SeisImpl::getAngleUnits)
      .def("getDataUnits", &H5SeisImpl::getDataUnits)
      .def("getOrientation", &H5SeisImpl::getOrientation,
           py::arg_v("units", "", "str()"))
      .def("getOrigin", &H5SeisImpl::getOrigin,
           py::arg_v("units", "", "str()"))
      .def("getSpacing", &H5SeisImpl::getSpacing,
           py::arg_v("units", "", "str()"))
      .def("getBoundary", &H5SeisImpl::getBoundary,
           py::arg_v("units", "", "str()"))

      .def("hasPKeySort", &H5SeisImpl::hasPKeySort,
           py::arg("pKeyName"))
      .def("removePKeySort", &H5SeisImpl::removePKeySort,
           py::arg("pKeyName"))
      .def("addPKeySort", &H5SeisImpl::addPKeySort,
           py::arg("pKeyName"))

      .def("getSeisContainer", &H5SeisImpl::getSeisContainer)

      .def("getBoundaryD", &H5SeisImpl::getBoundaryD)
      .def("getTextHeaderD", &H5SeisImpl::getTextHeaderD)
      .def("getBinHeaderD", &H5SeisImpl::getBinHeaderD)
      .def("getTraceHeaderD", &H5SeisImpl::getTraceHeaderD)
      .def("getTraceD", &H5SeisImpl::getTraceD)
      .def("getSortG", &H5SeisImpl::getSortG)
      .def("getUValG", &H5SeisImpl::getUValG)
      .def("getIndexesG", &H5SeisImpl::getIndexesG);
}


} // h5geopy
