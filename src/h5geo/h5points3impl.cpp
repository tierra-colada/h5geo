#include "../../include/h5geo/private/h5points3impl.h"
#include "../../include/h5geo/h5basecontainer.h"
#include "../../include/h5geo/h5core.h"
#include "../../include/h5geo/private/h5enum_string.h"

#include <units/units.hpp>

#ifdef H5GEO_USE_GDAL
#include <gdal.h>
#include <gdal_priv.h>
#endif


template <typename TBase>
H5Points3Impl<TBase>::H5Points3Impl(const h5gt::Group &group) :
  H5BasePointsImpl<TBase>(group){}

template <typename TBase>
bool H5Points3Impl<TBase>::writeData(
    h5geo::Point3Array& data,
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

template <typename TBase>
h5geo::Point3Array H5Points3Impl<TBase>::getData(
    const std::string& lengthUnits,
    const std::string& temporalUnits,
    bool doCoordTransform)
{
  auto opt = this->getPointsD();
  if (!opt.has_value())
    return h5geo::Point3Array();

  auto dtype = opt->getDataType();
  if (!dtype.isTypeEqual(h5geo::create_compound_Point3()))
    return h5geo::Point3Array();

  h5geo::Point3Array data(opt->getElementCount());
  opt->read(data);

  bool val = transformPoints(
        data,
        true,
        this->getLengthUnits(),
        lengthUnits,
        this->getTemporalUnits(),
        temporalUnits,
        doCoordTransform);

  if (!val)
    return h5geo::Point3Array();

  return data;
}

template <typename TBase>
bool H5Points3Impl<TBase>::overwritePointsDataset(
    h5geo::Point3Array& data,
    const std::string& lengthUnits,
    const std::string& temporalUnits,
    bool doCoordTransform)
{
  auto opt = this->getPointsD();
  if (!opt.has_value())
    return false;

  auto dtype = opt->getDataType();
  if (!dtype.isTypeEqual(h5geo::create_compound_Point3())){
    return false;
  }

  bool val = transformPoints(
        data,
        false,
        lengthUnits,
        this->getLengthUnits(),
        temporalUnits,
        this->getTemporalUnits(),
        doCoordTransform);

  if (!val)
    return false;

  try {
    opt->resize({data.size()});
    opt->write_raw(data.data(), h5geo::create_compound_Point3());
    this->objG.flush();
    return true;
  } catch (h5gt::Exception e) {
    return false;
  }
}

template <typename TBase>
bool H5Points3Impl<TBase>::transformPoints(
    h5geo::Point3Array& data,
    bool toReadData,
    const std::string& lengthUnitsFrom,
    const std::string& lengthUnitsTo,
    const std::string& temporalUnitsFrom,
    const std::string& temporalUnitsTo,
    bool doCoordTransform)
{
  h5geo::Domain domain = this->getDomain();
#ifdef H5GEO_USE_GDAL
  if (doCoordTransform){
    OGRCT_ptr coordTrans;
    if (toReadData)
      coordTrans.reset(this->createCoordinateTransformationToReadData(
                         lengthUnitsTo));
    else
      coordTrans.reset(this->createCoordinateTransformationToWriteData(
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

// explicit instantiation (requires that corresponding headers are included)
template class H5Points3Impl<H5Points3>;
