#include "../../include/h5geopy/h5core_sr_settings_py.h"

#include <h5geo/h5core.h>

namespace h5geopy {

namespace ext {

Eigen::MatrixXd convertUnitsD(
    Eigen::Ref<Eigen::MatrixXd> m,
    const std::string& unitsFrom,
    const std::string& unitsTo)
{
  if (!sr::convertUnits(m, unitsFrom, unitsTo))
    return Eigen::MatrixXd();

  return m;
}

Eigen::MatrixXf convertUnitsF(
    Eigen::Ref<Eigen::MatrixXf> m,
    const std::string& unitsFrom,
    const std::string& unitsTo)
{
  if (!sr::convertUnits(m, unitsFrom, unitsTo))
    return Eigen::MatrixXf();

  return m;
}

// Transfrom FROM
std::tuple<Eigen::MatrixXd, Eigen::MatrixXd> transformCoordFromD(
    Eigen::Ref<Eigen::MatrixXd> x,
    Eigen::Ref<Eigen::MatrixXd> y,
    const std::string& unitsFrom,
    const std::string& srAuthAndCodeFrom)
{
  if (!sr::transformCoordFrom(x, y, unitsFrom, srAuthAndCodeFrom))
    return std::make_tuple(Eigen::MatrixXd(), Eigen::MatrixXd());

  return std::make_tuple(std::move(x), std::move(y));
}

std::tuple<Eigen::MatrixXd, Eigen::MatrixXd> transformCoordFromDW(
    Eigen::Ref<Eigen::MatrixXd> x,
    Eigen::Ref<Eigen::MatrixXd> y,
    const std::string& unitsFrom,
    const std::string& authNameFrom,
    const std::string& codeFrom)
{
  if (!sr::transformCoordFrom(x, y, unitsFrom, authNameFrom, codeFrom))
    return std::make_tuple(Eigen::MatrixXd(), Eigen::MatrixXd());

  return std::make_tuple(std::move(x), std::move(y));
}

std::tuple<Eigen::MatrixXf, Eigen::MatrixXf> transformCoordFromF(
    Eigen::Ref<Eigen::MatrixXf> x,
    Eigen::Ref<Eigen::MatrixXf> y,
    const std::string& unitsFrom,
    const std::string& srAuthAndCodeFrom)
{
  if (!sr::transformCoordFrom(x, y, unitsFrom, srAuthAndCodeFrom))
    return std::make_tuple(Eigen::MatrixXf(), Eigen::MatrixXf());

  return std::make_tuple(std::move(x), std::move(y));
}

std::tuple<Eigen::MatrixXf, Eigen::MatrixXf> transformCoordFromFW(
    Eigen::Ref<Eigen::MatrixXf> x,
    Eigen::Ref<Eigen::MatrixXf> y,
    const std::string& unitsFrom,
    const std::string& authNameFrom,
    const std::string& codeFrom)
{
  if (!sr::transformCoordFrom(x, y, unitsFrom, authNameFrom, codeFrom))
    return std::make_tuple(Eigen::MatrixXf(), Eigen::MatrixXf());

  return std::make_tuple(std::move(x), std::move(y));
}

// Transform coord TO
std::tuple<Eigen::MatrixXd, Eigen::MatrixXd> transformCoordToD(
    Eigen::Ref<Eigen::MatrixXd> x,
    Eigen::Ref<Eigen::MatrixXd> y,
    const std::string& unitsTo,
    const std::string& srAuthAndCodeTo)
{
  if (!sr::transformCoordTo(x, y, unitsTo, srAuthAndCodeTo))
    return std::make_tuple(Eigen::MatrixXd(), Eigen::MatrixXd());

  return std::make_tuple(std::move(x), std::move(y));
}

std::tuple<Eigen::MatrixXd, Eigen::MatrixXd> transformCoordToDW(
    Eigen::Ref<Eigen::MatrixXd> x,
    Eigen::Ref<Eigen::MatrixXd> y,
    const std::string& unitsTo,
    const std::string& authNameTo,
    const std::string& codeTo)
{
  if (!sr::transformCoordTo(x, y, unitsTo, authNameTo, codeTo))
    return std::make_tuple(Eigen::MatrixXd(), Eigen::MatrixXd());

  return std::make_tuple(std::move(x), std::move(y));
}

std::tuple<Eigen::MatrixXf, Eigen::MatrixXf> transformCoordToF(
    Eigen::Ref<Eigen::MatrixXf> x,
    Eigen::Ref<Eigen::MatrixXf> y,
    const std::string& unitsTo,
    const std::string& srAuthAndCodeTo)
{
  if (!sr::transformCoordTo(x, y, unitsTo, srAuthAndCodeTo))
    return std::make_tuple(Eigen::MatrixXf(), Eigen::MatrixXf());

  return std::make_tuple(std::move(x), std::move(y));
}

std::tuple<Eigen::MatrixXf, Eigen::MatrixXf> transformCoordToFW(
    Eigen::Ref<Eigen::MatrixXf> x,
    Eigen::Ref<Eigen::MatrixXf> y,
    const std::string& unitsTo,
    const std::string& authNameTo,
    const std::string& codeTo)
{
  if (!sr::transformCoordTo(x, y, unitsTo, authNameTo, codeTo))
    return std::make_tuple(Eigen::MatrixXf(), Eigen::MatrixXf());

  return std::make_tuple(std::move(x), std::move(y));
}


} // ext


void defineSRSettingsFunctions(py::module_& m_sr){
//  m_sr.attr("SpatialReference")= &sr::SpatialReference;

  m_sr.def("setSpatialReference", &sr::setSpatialReference);
  m_sr.def("setSpatialReferenceFromUserInput", py::overload_cast<
           const std::string&>(
             &sr::setSpatialReferenceFromUserInput));
  m_sr.def("setSpatialReferenceFromUserInput", py::overload_cast<
           const std::string&, const std::string&>(
             &sr::setSpatialReferenceFromUserInput));
  m_sr.def("getSpatialReference", &sr::getSpatialReference);
//  m_sr.def("getSpatialReference",[]() {
//    OGRSpatialReference sref = sr::getSpatialReference();
//    py::object osr = py::module::import("osgeo").attr("osr");
//    return py::cast(sref);
//  });
  m_sr.def("setLengthUnits", &sr::setLengthUnits);
  m_sr.def("setAngularUnits", &sr::setAngularUnits);
  m_sr.def("setTemporalUnits", &sr::setTemporalUnits);
  m_sr.def("getLengthUnits", &sr::getLengthUnits);
  m_sr.def("getAngularUnits", &sr::getAngularUnits);
  m_sr.def("getTemporalUnits", &sr::getTemporalUnits);
  m_sr.def("setDomain", py::overload_cast<const std::string&>(&sr::setDomain));
  m_sr.def("setDomain", py::overload_cast<const h5geo::Domain&>(&sr::setDomain));
  m_sr.def("getDomain", &sr::getDomain);
  m_sr.def("getDomainEnum", &sr::getDomainEnum);
  m_sr.def("convertUnits", &ext::convertUnitsD,
           py::arg("m"), py::arg("unitsFrom"), py::arg("unitsTo"));
  m_sr.def("convertUnits", &ext::convertUnitsF,
           py::arg("m"), py::arg("unitsFrom"), py::arg("unitsTo"));
  m_sr.def("transformCoordFrom", &ext::transformCoordFromD,
           py::arg("x"), py::arg("y"), py::arg("unitsFrom"), py::arg("srAuthAndCodeFrom"));
  m_sr.def("transformCoordFrom", &ext::transformCoordFromDW,
           py::arg("x"), py::arg("y"), py::arg("unitsFrom"), py::arg("authNameFrom"), py::arg("codeFrom"));
  m_sr.def("transformCoordFrom", &ext::transformCoordFromF,
           py::arg("x"), py::arg("y"), py::arg("unitsFrom"), py::arg("srAuthAndCodeFrom"));
  m_sr.def("transformCoordFrom", &ext::transformCoordFromFW,
           py::arg("x"), py::arg("y"), py::arg("unitsFrom"), py::arg("authNameFrom"), py::arg("codeFrom"));
  m_sr.def("transformCoordTo", &ext::transformCoordToD,
           py::arg("x"), py::arg("y"), py::arg("unitsTo"), py::arg("srAuthAndCodeTo"));
  m_sr.def("transformCoordTo", &ext::transformCoordToDW,
           py::arg("x"), py::arg("y"), py::arg("unitsTo"), py::arg("authNameTo"), py::arg("codeTo"));
  m_sr.def("transformCoordTo", &ext::transformCoordToF,
           py::arg("x"), py::arg("y"), py::arg("unitsTo"), py::arg("srAuthAndCodeTo"));
  m_sr.def("transformCoordTo", &ext::transformCoordToFW,
           py::arg("x"), py::arg("y"), py::arg("unitsTo"), py::arg("authNameTo"), py::arg("codeTo"));
}


} // h5geopy
