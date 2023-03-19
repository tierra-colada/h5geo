#include "../../include/h5geopy/h5geofunctions_py.h"

#include <h5geo/h5mapcontainer.h>
#include <h5geo/h5map.h>
#include <h5geo/h5seiscontainer.h>
#include <h5geo/h5seis.h>
#include <h5geo/h5wellcontainer.h>
#include <h5geo/h5well.h>
#include <h5geo/h5devcurve.h>
#include <h5geo/h5logcurve.h>
#include <h5geo/h5points.h>
#include <h5geo/h5welltops.h>
#include <h5geo/h5horizon.h>
#include <h5geo/h5core.h>

namespace h5geopy {

namespace ext {

std::tuple<std::vector<std::string>, std::vector<std::string>>
getTraceHeaderNames()
{
  std::vector<std::string> fullHeaderNames, shortHeaderNames;
  h5geo::getTraceHeaderNames(fullHeaderNames, shortHeaderNames);
  return std::make_tuple(fullHeaderNames, shortHeaderNames);
}

std::tuple<std::vector<std::string>, std::vector<std::string>>
getBinHeaderNames()
{
  std::vector<std::string> fullHeaderNames, shortHeaderNames;
  h5geo::getBinHeaderNames(fullHeaderNames, shortHeaderNames);
  return std::make_tuple(fullHeaderNames, shortHeaderNames);
}

std::tuple<std::vector<int>, std::vector<int>>
getTraceHeaderBytes()
{
  std::vector<int> bytesStart, nBytes;
  h5geo::getTraceHeaderBytes(bytesStart, nBytes);
  return std::make_tuple(bytesStart, nBytes);
}

std::tuple<std::vector<int>, std::vector<int>>
getBinHeaderBytes()
{
  std::vector<int> bytesStart, nBytes;
  h5geo::getBinHeaderBytes(bytesStart, nBytes);
  return std::make_tuple(bytesStart, nBytes);
}

template<typename Scalar>
std::tuple<
bool,
double, double, double, double, double,
bool, bool, bool,
Eigen::MatrixX<Scalar>, Eigen::VectorX<Scalar>, Eigen::VectorX<Scalar>>
getSurveyInfoFromUnsortedData(
    Eigen::Ref<Eigen::MatrixX<Scalar>> il_xl,
    Eigen::Ref<Eigen::VectorX<Scalar>> x,
    Eigen::Ref<Eigen::VectorX<Scalar>> y)
{
  double origin_x;
  double origin_y;
  double orientation;
  double ilSpacing;
  double xlSpacing;
  bool isILReversed;
  bool isXLReversed;
  bool isPlanReversed;

  bool status = h5geo::getSurveyInfoFromUnsortedData(
        il_xl,
        x, y,
        origin_x, origin_y,
        orientation,
        ilSpacing, xlSpacing,
        isILReversed, isXLReversed,
        isPlanReversed);

  return std::make_tuple(
        std::move(status),
        std::move(origin_x), std::move(origin_y), std::move(orientation),
        std::move(ilSpacing), std::move(xlSpacing),
        std::move(isILReversed), std::move(isXLReversed), std::move(isPlanReversed),
        std::move(il_xl), std::move(x), std::move(y));
}

template<typename Scalar>
std::tuple<
bool,
double, double, double, double, double,
bool, bool, bool>
getSurveyInfoFromSortedData(
    const Eigen::Ref<const Eigen::VectorXd>& il,
    const Eigen::Ref<const Eigen::VectorXd>& xl,
    const Eigen::Ref<const Eigen::VectorXd>& x,
    const Eigen::Ref<const Eigen::VectorXd>& y)
{
  double origin_x;
  double origin_y;
  double orientation;
  double ilSpacing;
  double xlSpacing;
  bool isILReversed;
  bool isXLReversed;
  bool isPlanReversed;

  bool status = h5geo::getSurveyInfoFromSortedData(
        il, xl,
        x, y,
        origin_x, origin_y,
        orientation,
        ilSpacing, xlSpacing,
        isILReversed, isXLReversed,
        isPlanReversed);

  return std::make_tuple(
        std::move(status),
        std::move(origin_x), std::move(origin_y), std::move(orientation),
        std::move(ilSpacing), std::move(xlSpacing),
        std::move(isILReversed), std::move(isXLReversed), std::move(isPlanReversed));
}


} // ext


void defineGeoFunctions(py::module_& m){
  m.def("openContainer", &openContainer);
  m.def("openContainerByName", &openContainerByName);
  m.def("openBaseContainer", &openBaseContainer);
  m.def("openBaseContainerByName", &openBaseContainerByName);
  m.def("openSeisContainer", &openSeisContainer);
  m.def("openSeisContainerByName", &openSeisContainerByName);
  m.def("openVolContainer", &openVolContainer);
  m.def("openVolContainerByName", &openVolContainerByName);
  m.def("openMapContainer", &openMapContainer);
  m.def("openMapContainerByName", &openMapContainerByName);
  m.def("openWellContainer", &openWellContainer);
  m.def("openWellContainerByName", &openWellContainerByName);
  m.def("openObject", &openObject);
  m.def("openObjectByName", &openObjectByName);
  m.def("openBaseObject", &openBaseObject);
  m.def("openBaseObjectByName", &openBaseObjectByName);
  m.def("openVol", &openVol);
  m.def("openVolByName", &openVolByName);
  m.def("openMap", &openMap);
  m.def("openMapByName", &openMapByName);
  m.def("openWell", &openWell);
  m.def("openWellByName", &openWellByName);
  m.def("openDevCurve", &openDevCurve);
  m.def("openDevCurveByName", &openDevCurveByName);
  m.def("openLogCurve", &openLogCurve);
  m.def("openLogCurveByName", &openLogCurveByName);
  m.def("openPoints", &openPoints);
  m.def("openPointsByName", &openPointsByName);
  m.def("openPoints1", &openPoints1);
  m.def("openPoints1ByName", &openPoints1ByName);
  m.def("openPoints2", &openPoints2);
  m.def("openPoints2ByName", &openPoints2ByName);
  m.def("openPoints3", &openPoints3);
  m.def("openPoints3ByName", &openPoints3ByName);
  m.def("openPoints4", &openPoints4);
  m.def("openPoints4ByName", &openPoints4ByName);
  m.def("openWellTops", &openWellTops);
  m.def("openWellTopsByName", &openWellTopsByName);
  m.def("openHorizon", &openHorizon);
  m.def("openHorizonByName", &openHorizonByName);

  m.def("createContainer", &createContainer);
  m.def("createContainerByName", &createContainerByName);
  m.def("createMapContainer", &createMapContainer);
  m.def("createMapContainerByName", &createMapContainerByName);
  m.def("createWellContainer", &createWellContainer);
  m.def("createWellContainerByName", &createWellContainerByName);
  m.def("createSeisContainer", &createSeisContainer);
  m.def("createSeisContainerByName", &createSeisContainerByName);
  m.def("createVolContainer", &createVolContainer);
  m.def("createVolContainerByName", &createVolContainerByName);

  m.def("getGeoContainerType", &getGeoContainerType);
  m.def("getGeoObjectType", &getGeoObjectType);

  m.def("isGeoContainer", &isGeoContainer);
  m.def("isGeoContainerByType", &isGeoContainerByType);
  m.def("isGeoObject", &isGeoObject);
  m.def("isGeoObjectByType", &isGeoObjectByType);
  m.def("isMap", &isMap);
  m.def("isWell", &isWell);
  m.def("isLogCurve", &isLogCurve);
  m.def("isDevCurve", &isDevCurve);
  m.def("isSeis", &isSeis);
  m.def("isVol", &isVol);
  m.def("isPoints", &isPoints);
  m.def("isWellTops", &isWellTops);

  // from UTIL
  m.def("openFile", &openFile);
  m.def("openGroup", &openGroup);
  m.def("openDataSet", &openDataSet);

  m.def("getTraceHeaderNames", &ext::getTraceHeaderNames);
  m.def("getBinHeaderNames", &ext::getTraceHeaderNames);
  m.def("getTraceHeaderBytes", &ext::getTraceHeaderNames);
  m.def("getBinHeaderBytes", &ext::getTraceHeaderNames);
  m.def("getTraceHeaderCount", &getTraceHeaderNames);
  m.def("getBinHeaderCount", &getTraceHeaderNames);

  m.def("getSurveyInfoFromUnsortedData", &ext::getSurveyInfoFromUnsortedData<float>,
        py::arg("il_xl"),
        py::arg("x"),
        py::arg("y"),
        "Return: status, origin_x, origin_y, orientation, ilSpacing, xlSpacing, "
        "isILReversed, isXLReversed, isPlanReversed, "
        "il_xl_sorted, x_sorted, y_sorted");
  m.def("getSurveyInfoFromUnsortedData", &ext::getSurveyInfoFromUnsortedData<double>,
        py::arg("il_xl"),
        py::arg("x"),
        py::arg("y"),
        "Return: status, origin_x, origin_y, orientation, ilSpacing, xlSpacing, "
        "isILReversed, isXLReversed, isPlanReversed, "
        "il_xl_sorted, x_sorted, y_sorted");

  m.def("getSurveyInfoFromSortedData", &ext::getSurveyInfoFromSortedData<float>,
        py::arg("il"),
        py::arg("xl"),
        py::arg("x"),
        py::arg("y"),
        "It is assumed that `il, xl, x, y` are IL_XL sorted: ind=sort_rows([il, xl]), il=il[ind,:], xl=xl[ind,:], x=x[ind,:], y=y[ind,:]"
        "Return: status, origin_x, origin_y, orientation, ilSpacing, xlSpacing, "
        "isILReversed, isXLReversed, isPlanReversed");
  m.def("getSurveyInfoFromSortedData", &ext::getSurveyInfoFromSortedData<double>,
        py::arg("il"),
        py::arg("xl"),
        py::arg("x"),
        py::arg("y"),
        "It is assumed that `il, xl, x, y` are IL_XL sorted: ind=sort_rows([il, xl]), il=il[ind,:], xl=xl[ind,:], x=x[ind,:], y=y[ind,:]"
        "Return: status, origin_x, origin_y, orientation, ilSpacing, xlSpacing, "
        "isILReversed, isXLReversed, isPlanReversed");

  #ifdef H5GEO_USE_GDAL
  // init GDAL readers
  m.def("GDALAllRegister", &GDALAllRegister,
        "Must be called before using any GDAL readers");
  #endif  // H5GEO_USE_GDAL
}


} // h5geopy
