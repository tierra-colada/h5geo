#include "../../include/h5geopy/h5seis_py.h"

namespace h5geopy {

namespace ext {

std::tuple<Eigen::MatrixXf, Eigen::MatrixXd, Eigen::VectorX<size_t>>
getSortedData(
    H5Seis* self,
    const std::vector<std::string>& keyList,
    const std::vector<double>& minList,
    const std::vector<double>& maxList,
    size_t pStep = 1,
    size_t fromSampInd = 0,
    size_t nSamp = std::numeric_limits<size_t>::max(),
    const std::string& dataUnits = "",
    const std::string& lengthUnits = "",
    bool doCoordTransform = false)
{
  Eigen::MatrixXf TRACE;
  Eigen::MatrixXd HDR;
  Eigen::VectorX<size_t> idx = self->getSortedData(
        TRACE, HDR,
        keyList, minList, maxList,
        pStep,
        fromSampInd, nSamp,
        dataUnits, lengthUnits, doCoordTransform);
  return std::make_tuple(std::move(TRACE), std::move(HDR), std::move(idx));
}

std::tuple<size_t, bool>
checkTraceLimits(H5Seis* self, const size_t& fromTrc, size_t& nTrc)
{
  bool val = self->checkTraceLimits(
        fromTrc, nTrc);
  return std::make_tuple(nTrc, val);
}

std::tuple<size_t, bool>
checkTraceHeaderLimits(H5Seis* self, const size_t& fromHdr, size_t& nHdr)
{
  bool val = self->checkTraceHeaderLimits(
        fromHdr, nHdr);
  return std::make_tuple(nHdr, val);
}

std::tuple<size_t, bool>
checkSampleLimits(H5Seis* self, const size_t& fromSampInd, size_t& nSamp)
{
  bool val = self->checkSampleLimits(
        fromSampInd, nSamp);
  return std::make_tuple(nSamp, val);
}


}

void H5Seis_py(
    py::class_<
    H5Seis,
    H5SeisImpl,
    H5BaseObject,
    H5Base,
    std::unique_ptr<H5Seis, ObjectDeleter>>
    &py_obj){
  py_obj
      // WRITERS
      .def("readSEGYTextHeader", &H5Seis::readSEGYTextHeader,
           py::arg("segy"),
           py::arg_v("encoding", static_cast<h5geo::TextEncoding>(0), "_h5geo.TextEncoding(0)"))
      .def("readSEGYBinHeader", &H5Seis::readSEGYBinHeader,
           py::arg("segy"),
           py::arg_v("endian", static_cast<h5geo::Endian>(0), "_h5geo.Endian(0)"))
      .def("readSEGYTraces", &H5Seis::readSEGYTraces,
           py::arg("segyFiles"),
           py::arg_v("formats", std::vector<h5geo::SegyFormat>(), "list()"),
           py::arg_v("endians", std::vector<h5geo::Endian>(), "list()"),
           py::arg_v("trcHdrNames", std::vector<std::vector<std::string>>(), "list()"),
           py::arg_v("trcBuffer", 10000, "10000"),
           py::arg_v("nThreads", -1, "-1"),
           py::arg_v("progressCallback", nullptr, "None"))

      .def("writeTextHeader", py::overload_cast<
           const std::vector<std::string>&>(
             &H5Seis::writeTextHeader),
           py::arg("txtHdr"))
      .def("writeBinHeader", py::overload_cast<
           const std::vector<double>&>(
             &H5Seis::writeBinHeader),
           py::arg("binHdr"))
      .def("writeBinHeader", py::overload_cast<
           const Eigen::Ref<const Eigen::VectorXd>&>(
             &H5Seis::writeBinHeader),
           py::arg("binHdr"))
      .def("writeBinHeader", py::overload_cast<
           const std::string&,
           const double&,
           const std::string&,
           const std::string&>(
             &H5Seis::writeBinHeader),
           py::arg("hdrName"),
           py::arg("value"),
           py::arg_v("unitsFrom", "", "str()"),
           py::arg_v("unitsTo", "", "str()"))
      .def("writeTrace", py::overload_cast<
           Eigen::Ref<Eigen::MatrixXf>,
           const size_t&,
           const size_t&,
           const std::string&>(
             &H5Seis::writeTrace),
           py::arg("TRACE"),
           py::arg_v("fromTrc", 0, "0"),
           py::arg_v("fromSampInd", 0, "0"),
           py::arg_v("dataUnits", "", "str()"))
      .def("writeTrace", py::overload_cast<
           Eigen::Ref<Eigen::MatrixXf>,
           const Eigen::Ref<const Eigen::VectorX<size_t>>&,
           const size_t&,
           const std::string&>(
             &H5Seis::writeTrace),
           py::arg("TRACE"),
           py::arg("trcInd"),
           py::arg_v("fromSampInd", 0, "0"),
           py::arg_v("dataUnits", "", "str()"))
      .def("writeTraceHeader", py::overload_cast<
           const Eigen::Ref<const Eigen::MatrixXd>&,
           const size_t&,
           const size_t&>(
             &H5Seis::writeTraceHeader),
           py::arg("HDR"),
           py::arg_v("fromTrc", 0, "0"),
           py::arg_v("fromHdrInd", 0, "0"))
      .def("writeTraceHeader", py::overload_cast<
           const std::string&,
           Eigen::Ref<Eigen::MatrixXd>,
           const size_t&,
           const std::string&,
           const std::string&>(
             &H5Seis::writeTraceHeader),
           py::arg("hdrName"),
           py::arg("hdr"),
           py::arg_v("fromTrc", 0, "0"),
           py::arg_v("unitsFrom", "", "str()"),
           py::arg_v("unitsTo", "", "str()"))
      .def("writeTraceHeader", py::overload_cast<
           const std::string&,
           Eigen::Ref<Eigen::MatrixXd>,
           const Eigen::Ref<const Eigen::VectorX<size_t>>&,
           const std::string&,
           const std::string&>(
             &H5Seis::writeTraceHeader),
           py::arg("hdrName"),
           py::arg("hdr"),
           py::arg("trcInd"),
           py::arg_v("unitsFrom", "", "str()"),
           py::arg_v("unitsTo", "", "str()"))

      .def("writeXYTraceHeaders", py::overload_cast<
           const std::vector<std::string>&,
           Eigen::Ref<Eigen::MatrixX2d>&,
           const size_t&,
           const std::string&,
           bool>(
             &H5Seis::writeXYTraceHeaders),
           py::arg("xhHdrNames"),
           py::arg("xy"),
           py::arg_v("fromTrc", 0, "0"),
           py::arg_v("lengthUnits", "", "str()"),
           py::arg_v("doCoordTransform", false, "False"))
      .def("writeXYTraceHeaders", py::overload_cast<
           const std::vector<std::string>&,
           Eigen::Ref<Eigen::MatrixX2d>&,
           const Eigen::Ref<const Eigen::VectorX<size_t>>&,
           const std::string&,
           bool>(
             &H5Seis::writeXYTraceHeaders),
           py::arg("xhHdrNames"),
           py::arg("xy"),
           py::arg("trcInd"),
           py::arg_v("lengthUnits", "", "str()"),
           py::arg_v("doCoordTransform", false, "False"))

      .def("setNTrc", &H5Seis::setNTrc)
      .def("setNSamp", &H5Seis::setNSamp)

      //  GETTERS
      .def("getTextHeader", &H5Seis::getTextHeader)
      .def("getBinHeader", py::overload_cast<>(
             &H5Seis::getBinHeader))
      .def("getBinHeader", py::overload_cast<
           const std::string&,
           const std::string&,
           const std::string&>(
             &H5Seis::getBinHeader),
           py::arg("hdrName"),
           py::arg_v("unitsFrom", "", "str()"),
           py::arg_v("unitsTo", "", "str()"))
      .def("getTrace", py::overload_cast<
           const size_t&,
           size_t,
           const size_t&,
           size_t,
           const std::string&>(
             &H5Seis::getTrace),
           py::arg("fromTrc"),
           py::arg_v("nTrc", 1, "1"),
           py::arg_v("fromSampInd", 0, "0"),
           py::arg_v("nSamp", std::numeric_limits<size_t>::max(), "sys.maxint"),
           py::arg_v("dataUnits", "", "str()"),
           "Get block of traces. If `nTrc` or `nSamp` exceed max values then these values are changed to max allowed (that is why they are not `const`)")
      .def("getTrace", py::overload_cast<
           const Eigen::Ref<const Eigen::VectorX<size_t>>&,
           const size_t&,
           size_t,
           const std::string&>(
             &H5Seis::getTrace),
           py::arg("trcInd"),
           py::arg_v("fromSampInd", 0, "0"),
           py::arg_v("nSamp", std::numeric_limits<size_t>::max(), "sys.maxint"),
           py::arg_v("dataUnits", "", "str()"))
      .def("getTraceHeader", py::overload_cast<
           const size_t&,
           size_t,
           const size_t&,
           size_t,const std::vector<std::string>&,
           const std::vector<std::string>&>(
             &H5Seis::getTraceHeader),
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
           size_t,
           const std::string&,
           const std::string&>(
             &H5Seis::getTraceHeader),
           py::arg("hdrName"),
           py::arg_v("fromTrc", 0, "0"),
           py::arg_v("nTrc", 1, "1"),
           py::arg_v("unitsFrom", "", "str()"),
           py::arg_v("unitsTo", "", "str()"))
      .def("getTraceHeader", py::overload_cast<
           const std::vector<size_t>&,
           const std::vector<size_t>&,
           const std::vector<std::string>&,
           const std::vector<std::string>&>(
             &H5Seis::getTraceHeader),
           py::arg("trcInd"),
           py::arg("trcHdrInd"),
           py::arg_v("unitsFrom", std::vector<std::string>(), "list()"),
           py::arg_v("unitsTo", std::vector<std::string>(), "list()"))
      .def("getTraceHeader", py::overload_cast<
           const std::vector<std::string>&,
           const std::vector<size_t>&,
           const std::vector<std::string>&,
           const std::vector<std::string>&>(
             &H5Seis::getTraceHeader),
           py::arg("hdrNames"),
           py::arg("trcHdrInd"),
           py::arg_v("unitsFrom", std::vector<std::string>(), "list()"),
           py::arg_v("unitsTo", std::vector<std::string>(), "list()"))

      .def("getXYTraceHeaders", py::overload_cast<
           const std::vector<std::string>&,
           const size_t&,
           size_t,
           const std::string&,
           bool>(
             &H5Seis::getXYTraceHeaders),
           py::arg("xyHdrNames"),
           py::arg_v("fromTrc", 0, "0"),
           py::arg_v("nTrc", std::numeric_limits<size_t>::max(), "sys.maxint"),
           py::arg_v("lengthUnits", "", "str()"),
           py::arg_v("doCoordTransform", false, "False"))
      .def("getXYTraceHeaders", py::overload_cast<
           const std::vector<std::string>&,
           const Eigen::Ref<const Eigen::VectorX<size_t>>&,
           const std::string&,
           bool>(
             &H5Seis::getXYTraceHeaders),
           py::arg("xyHdrNames"),
           py::arg("trcInd"),
           py::arg_v("lengthUnits", "", "str()"),
           py::arg_v("doCoordTransform", false, "False"))


      .def("getSortedData", &ext::getSortedData,
           py::arg("keyList"),
           py::arg("minList"),
           py::arg("maxList"),
           py::arg_v("pStep", 1, "1"),
           py::arg_v("fromSampInd", 0, "0"),
           py::arg_v("nSamp", std::numeric_limits<size_t>::max(), "sys.maxint"),
           py::arg_v("dataUnits", "", "str()"),
           py::arg_v("lengthUnits", "", "str()"),
           py::arg_v("doCoordTransform", false, "False"),
           "Get sorted data based on precalculated primary sort keys (e.g. before using it you should prepare primary sort keys with `addPKeySort(...)` method)."
"Return `TRACE` (traces matrix), `HDR` (hdr matrix) and `idx` (vector of trace indexes read)")

      .def("getBinHeaderIndex", &H5Seis::getBinHeaderIndex,
           py::arg("hdrName"))
      .def("getTraceHeaderIndex", &H5Seis::getTraceHeaderIndex,
           py::arg("hdrName"))

      .def("getSamples", &H5Seis::getSamples,
           py::arg("trcInd"),
           py::arg_v("units", "", "str()"),
           "in units according to `Domain` (`METER` or `SECOND` or else...)")
      .def("getFirstSample", &H5Seis::getFirstSample,
           py::arg("trcInd"),
           py::arg_v("units", "", "str()"),
           "in units according to `Domain` (`METER` or `SECOND` or else...)")
      .def("getLastSample", &H5Seis::getLastSample,
           py::arg("trcInd"),
           py::arg_v("units", "", "str()"),
           "in units according to `Domain` (`METER` or `SECOND` or else...)")
      .def("getSampRate", &H5Seis::getSampRate,
           py::arg_v("units", "", "str()"),
           "in units according to `Domain` (`METER` or `SECOND` or else...)")
      .def("getNSamp", &H5Seis::getNSamp,
           "get number of samples (i.e. trace length)")
      .def("getNTrc", &H5Seis::getNTrc,
           "get number of traces")
      .def("getNTrcHdr", &H5Seis::getNTrcHdr,
           "get number of trace headers (usually 78)")
      .def("getNBinHdr", &H5Seis::getNBinHdr,
           "get number of bin headers")
      .def("getNTextHdrRows", &H5Seis::getNTextHdrRows,
           "get number lines of text header")
      .def("getPKeyIndexes", &H5Seis::getPKeyIndexes,
           py::arg("pKey"),
           py::arg("pMin"),
           py::arg("pMax"),
           py::arg_v("pStep", 1, "1"))
      .def("getPKeyValues", &H5Seis::getPKeyValues,
           py::arg("pkey"),
           py::arg_v("unitsFrom", "", "str()"),
           py::arg_v("unitsTo", "", "str()"))
      .def("getPKeySize", py::overload_cast<const std::string&, double, double, size_t>(
             &H5Seis::getPKeySize),
           py::arg("pKey"),
           py::arg("pMin"),
           py::arg("pMax"),
           py::arg_v("pStep", 1, "1"))
      .def("getPKeySize", py::overload_cast<const std::string&>(
             &H5Seis::getPKeySize),
           py::arg("pKey"))
      .def("getPKeyTraceSize", &H5Seis::getPKeyTraceSize,
           py::arg("pKey"),
           py::arg("pMin"),
           py::arg("pMax"),
           py::arg_v("pStep", 1, "1"))
      .def("getPKeyNames", &H5Seis::getPKeyNames,
           "get primary key names (usually they are used in sorting)")
      .def("getTraceHeaderMin", py::overload_cast<>(
             &H5Seis::getTraceHeaderMin))
      .def("getTraceHeaderMax", py::overload_cast<>(
             &H5Seis::getTraceHeaderMax))
      .def("getTraceHeaderMin", py::overload_cast<
           const std::string&,
           const std::string&,
           const std::string&>(
             &H5Seis::getTraceHeaderMin),
           py::arg("hdrName"),
           py::arg_v("unitsFrom", "", "str()"),
           py::arg_v("unitsTo", "", "str()"))
      .def("getTraceHeaderMax", py::overload_cast<
           const std::string&,
           const std::string&,
           const std::string&>(
             &H5Seis::getTraceHeaderMax),
           py::arg("hdrName"),
           py::arg_v("unitsFrom", "", "str()"),
           py::arg_v("unitsTo", "", "str()"))

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
      .def("generatePRESTKGeometry", &H5Seis::generatePRESTKGeometry,
           py::arg("src_x0"), py::arg("src_dx"), py::arg("src_nx"),
           py::arg("src_y0"), py::arg("src_dy"), py::arg("src_ny"),
           py::arg("src_z"),
           py::arg("rec_x0"), py::arg("rec_dx"), py::arg("rec_nx"),
           py::arg("rec_y0"), py::arg("rec_dy"), py::arg("rec_ny"),
           py::arg("rec_z"),
           py::arg("moveRec"),
           py::arg_v("lengthUnits", "", "str()"),
           py::arg_v("doCoordTransform", false, "False"))
      .def("generateSTKGeometry", &H5Seis::generateSTKGeometry,
           py::arg("x0"), py::arg("dx"), py::arg("nx"),
           py::arg("y0"), py::arg("dy"), py::arg("ny"),
           py::arg("z"),
           py::arg_v("lengthUnits", "", "str()"),
           py::arg_v("doCoordTransform", false, "False"))

      .def("setDomain", &H5Seis::setDomain)
      .def("setDataType", &H5Seis::setDataType)
      .def("setSurveyType", &H5Seis::setSurveyType)
      .def("setSRD", &H5Seis::setSRD,
           py::arg("value"),
           py::arg_v("units", "", "str()"))
      .def("setSampRate", &H5Seis::setSampRate,
           py::arg("value"),
           py::arg_v("units", "", "str()"))
      .def("setFirstSample", &H5Seis::setFirstSample,
           py::arg("value"),
           py::arg_v("units", "", "str()"))

      .def("getDomain", &H5Seis::getDomain)
      .def("getDataType", &H5Seis::getDataType)
      .def("getSurveyType", &H5Seis::getSurveyType)
      .def("getSRD", &H5Seis::getSRD,
           py::arg_v("units", "", "str()"))

      .def("hasPKeySort", &H5Seis::hasPKeySort,
           py::arg("pKeyName"))
      .def("removePKeySort", &H5Seis::removePKeySort,
           py::arg("pKeyName"))
      .def("addPKeySort", &H5Seis::addPKeySort,
           py::arg("pKeyName"))

      .def("openSeisContainer", &H5Seis::openSeisContainer)

      .def("getTextHeaderD", &H5Seis::getTextHeaderD)
      .def("getBinHeaderD", &H5Seis::getBinHeaderD)
      .def("getTraceHeaderD", &H5Seis::getTraceHeaderD)
      .def("getTraceD", &H5Seis::getTraceD)
      .def("getSortG", &H5Seis::getSortG)
      .def("getUValG", &H5Seis::getUValG)
      .def("getIndexesG", &H5Seis::getIndexesG)

      .def("getSEGYG", &H5Seis::getSEGYG)
      .def("getSEGYTextHeaderD", &H5Seis::getSEGYTextHeaderD)
      .def("getSEGYBinHeader2BytesD", &H5Seis::getSEGYBinHeader2BytesD)
      .def("getSEGYBinHeader4BytesD", &H5Seis::getSEGYBinHeader4BytesD)
      .def("getSEGYTraceHeader2BytesD", &H5Seis::getSEGYTraceHeader2BytesD)
      .def("getSEGYTraceHeader4BytesD", &H5Seis::getSEGYTraceHeader4BytesD)
      .def("getSEGYTraceFloatD", &H5Seis::getSEGYTraceFloatD)
      .def("getParam", &H5Seis::getParam)

      .def("updateTraceHeaderLimits", &H5Seis::updateTraceHeaderLimits,
           py::arg_v("nTrcBuffer", 1e7, "int(1e7)")) // `int` is important
      .def("updatePKeySort", &H5Seis::updatePKeySort,
           py::arg("pKeyName"))

      .def("calcBoundary", &H5Seis::calcBoundary,
           py::arg_v("lengthUnits", "", "str()"),
           py::arg_v("doCoordTransform", false, "False"),
           "calculate boundary of 2D or 3D seismic survey")

      .def("exportToSEGY", &H5Seis::exportToSEGY,
           py::arg("segyFile"),
           py::arg_v("trcBuffer", 10000, "10000"),
           py::arg_v("progressCallback", nullptr, "None"),
           "export seismic to SEGY");
}


} // h5geopy
