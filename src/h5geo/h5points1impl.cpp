#include "../../include/h5geo/private/h5points1impl.h"
#include "../../include/h5geo/private/h5welltopsimpl.h"
#include "../../include/h5geo/h5basecontainer.h"
#include "../../include/h5geo/h5core.h"
#include "../../include/h5geo/private/h5enum_string.h"

#include <units/units.hpp>

#ifdef H5GEO_USE_GDAL
#include <gdal.h>
#include <gdal_priv.h>
#endif


template <typename TBase>
H5Points1Impl<TBase>::H5Points1Impl(const h5gt::Group &group) :
  H5BasePointsImpl<TBase>(group){}

template <typename TBase>
bool H5Points1Impl<TBase>::writeData(
    h5geo::Point1Array& data,
    const std::string& lengthUnits,
    const std::string& temporalUnits)
{
  return this->overwritePointsDataset(
        data,
        lengthUnits,
        temporalUnits);
}

template <typename TBase>
h5geo::Point1Array H5Points1Impl<TBase>::getData(
    const std::string& lengthUnits,
    const std::string& temporalUnits)
{
  auto opt = this->getPointsD();
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
        this->getLengthUnits(),
        lengthUnits,
        this->getTemporalUnits(),
        temporalUnits);

  if (!val)
    return h5geo::Point1Array();

  return data;
}

template <typename TBase>
bool H5Points1Impl<TBase>::overwritePointsDataset(
    h5geo::Point1Array& data,
    const std::string& lengthUnits,
    const std::string& temporalUnits)
{
  auto opt = this->getPointsD();
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
        this->getLengthUnits(),
        temporalUnits,
        this->getTemporalUnits());

  if (!val)
    return false;

  try {
    opt->resize({data.size()});
    opt->write_raw(data.data(), h5geo::compound_Point1());
    this->objG.flush();
    return true;
  } catch (h5gt::Exception e) {
    return false;
  }
}

template <typename TBase>
bool H5Points1Impl<TBase>::transformPoints(
    h5geo::Point1Array& data,
    bool toReadData,
    const std::string& lengthUnitsFrom,
    const std::string& lengthUnitsTo,
    const std::string& temporalUnitsFrom,
    const std::string& temporalUnitsTo)
{
  h5geo::Domain domain = this->getDomain();
  double coef;
  if (!lengthUnitsFrom.empty() &&
      !lengthUnitsTo.empty() &&
      domain != h5geo::Domain::OWT &&
      domain != h5geo::Domain::TWT){
    coef = units::convert(
          units::unit_from_string(lengthUnitsFrom),
          units::unit_from_string(lengthUnitsTo));
    for (auto& point : data){
      point.p[0] *= coef;
    }
  }

  if (!temporalUnitsFrom.empty() &&
      !temporalUnitsTo.empty() &&
      (domain == h5geo::Domain::OWT ||
       domain == h5geo::Domain::TWT)){
    coef = units::convert(
          units::unit_from_string(temporalUnitsFrom),
          units::unit_from_string(temporalUnitsTo));
    for (auto& point : data){
      point.p[0] *= coef;
    }
  }

  return true;
}

// explicit instantiation (requires that corresponding headers are included)
template class H5Points1Impl<H5Points1>;
template class H5Points1Impl<H5WellTops>;
