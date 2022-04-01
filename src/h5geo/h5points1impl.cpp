#include "../../include/h5geo/impl/h5points1impl.h"
#include "../../include/h5geo/h5basecontainer.h"
#include "../../include/h5geo/misc/h5core.h"
#include "../../include/h5geo/misc/h5enum_string.h"

#include <units/units.hpp>

#ifdef H5GEO_USE_GDAL
#include <gdal.h>
#include <gdal_priv.h>
#endif


H5Points1Impl::H5Points1Impl(const h5gt::Group &group) :
  H5BasePointsImpl(group){}

bool H5Points1Impl::writeData(
    h5geo::Point1Array& data,
    const std::string& lengthUnits,
    const std::string& temporalUnits,
    bool doCoordTransform)
{
  return this->overwritePointsDataset(
        data,
        lengthUnits,
        temporalUnits,
        doCoordTransform);
}

h5geo::Point1Array H5Points1Impl::getData(
    const std::string& lengthUnits,
    const std::string& temporalUnits,
    bool doCoordTransform)
{
  auto opt = getPointsD();
  if (!opt.has_value())
    return h5geo::Point1Array();

  auto dtype = opt->getDataType();
  if (!dtype.isTypeEqual(h5geo::compound_Point1()))
    return h5geo::Point1Array();

  h5geo::Point1Array data(opt->getElementCount());
  opt->read(data);

  bool val = transformPoints(
        data,
        true,
        getLengthUnits(),
        lengthUnits,
        getTemporalUnits(),
        temporalUnits,
        doCoordTransform);

  if (!val)
    return h5geo::Point1Array();

  return data;
}

bool H5Points1Impl::overwritePointsDataset(
    h5geo::Point1Array& data,
    const std::string& lengthUnits,
    const std::string& temporalUnits,
    bool doCoordTransform)
{
  auto opt = getPointsD();
  if (!opt.has_value())
    return false;

  auto dtype = opt->getDataType();
  if (!dtype.isTypeEqual(h5geo::compound_Point1())){
    return false;
  }

  bool val = transformPoints(
        data,
        false,
        lengthUnits,
        getLengthUnits(),
        temporalUnits,
        getTemporalUnits(),
        doCoordTransform);

  if (!val)
    return false;

  try {
    opt->resize({data.size()});
    opt->write_raw(data.data(), h5geo::compound_Point1());
    objG.flush();
    return true;
  } catch (h5gt::Exception e) {
    return false;
  }
}

bool H5Points1Impl::transformPoints(
    h5geo::Point1Array& data,
    bool toReadData,
    const std::string& lengthUnitsFrom,
    const std::string& lengthUnitsTo,
    const std::string& temporalUnitsFrom,
    const std::string& temporalUnitsTo,
    bool doCoordTransform)
{
  h5geo::Domain domain = getDomain();
#ifdef H5GEO_USE_GDAL
  if (doCoordTransform){
    OGRCT_ptr coordTrans;
    if (toReadData)
      coordTrans.reset(createCoordinateTransformationToReadData(
                         lengthUnitsTo));
    else
      coordTrans.reset(createCoordinateTransformationToWriteData(
                         lengthUnitsFrom));
    if (!coordTrans)
      return false;

    for (auto& point : data)
      coordTrans->Transform(1, &point.p[0], &point.p[1]);

    double coef;
    if (!lengthUnitsFrom.empty() &&
        !lengthUnitsTo.empty() &&
        domain != h5geo::Domain::OWT &&
        domain != h5geo::Domain::TWT){
      coef = units::convert(
            units::unit_from_string(lengthUnitsFrom),
            units::unit_from_string(lengthUnitsTo));
      for (auto& point : data)
        point.p[2] *= coef;
    }

    if (!temporalUnitsFrom.empty() &&
        !temporalUnitsTo.empty() &&
        (domain == h5geo::Domain::OWT ||
         domain == h5geo::Domain::TWT)){
      coef = units::convert(
            units::unit_from_string(temporalUnitsFrom),
            units::unit_from_string(temporalUnitsTo));
      for (auto& point : data)
        point.p[2] *= coef;
    }

    return true;
  }
#endif

  double coef;
  if (!lengthUnitsFrom.empty() &&
      !lengthUnitsTo.empty()){
    coef = units::convert(
          units::unit_from_string(lengthUnitsFrom),
          units::unit_from_string(lengthUnitsTo));
    if (domain == h5geo::Domain::OWT ||
        domain == h5geo::Domain::TWT){
      for (auto& point : data){
        point.p[0] *= coef;
        point.p[1] *= coef;
      }
    } else {
      for (auto& point : data){
        point.p[0] *= coef;
        point.p[1] *= coef;
        point.p[2] *= coef;
      }
    }
  }

  if (!temporalUnitsFrom.empty() &&
      !temporalUnitsTo.empty() &&
      (domain == h5geo::Domain::OWT ||
       domain == h5geo::Domain::TWT)){
    coef = units::convert(
          units::unit_from_string(temporalUnitsFrom),
          units::unit_from_string(temporalUnitsTo));
    for (auto& point : data)
      point.p[2] *= coef;
  }

  return true;
}
