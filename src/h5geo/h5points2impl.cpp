#include "../../include/h5geo/private/h5points2impl.h"
#include "../../include/h5geo/h5basecontainer.h"
#include "../../include/h5geo/h5core.h"
#include "../../include/h5geo/private/h5enum_string.h"

#include <units/units.hpp>

#ifdef H5GEO_USE_GDAL
#include <gdal.h>
#include <gdal_priv.h>
#endif


template <typename TBase>
H5Points2Impl<TBase>::H5Points2Impl(const h5gt::Group &group) :
  H5BasePointsImpl<TBase>(group){}

template <typename TBase>
bool H5Points2Impl<TBase>::writeData(
    h5geo::Point2Array& data,
    const std::string& lengthUnits,
    bool doCoordTransform)
{
  return this->overwritePointsDataset(
        data,
        lengthUnits,
        doCoordTransform);
}

template <typename TBase>
h5geo::Point2Array H5Points2Impl<TBase>::getData(
    const std::string& lengthUnits,
    bool doCoordTransform)
{
  auto opt = this->getPointsD();
  if (!opt.has_value())
    return h5geo::Point2Array();

  auto dtype = opt->getDataType();
  if (!dtype.isTypeEqual(h5geo::create_compound_Point2()))
    return h5geo::Point2Array();

  h5geo::Point2Array data(opt->getElementCount());
  opt->read(data);

  bool val = transformPoints(
        data,
        true,
        this->getLengthUnits(),
        lengthUnits,
        doCoordTransform);

  if (!val)
    return h5geo::Point2Array();

  return data;
}

template <typename TBase>
bool H5Points2Impl<TBase>::overwritePointsDataset(
    h5geo::Point2Array& data,
    const std::string& lengthUnits,
    bool doCoordTransform)
{
  auto opt = this->getPointsD();
  if (!opt.has_value())
    return false;

  auto dtype = opt->getDataType();
  if (!dtype.isTypeEqual(h5geo::create_compound_Point2())){
    return false;
  }

  bool val = transformPoints(
        data,
        false,
        lengthUnits,
        this->getLengthUnits(),
        doCoordTransform);

  if (!val)
    return false;

  try {
    opt->resize({data.size()});
    opt->write_raw(data.data(), h5geo::create_compound_Point2());
    this->objG.flush();
    return true;
  } catch (h5gt::Exception e) {
    return false;
  }
}

template <typename TBase>
bool H5Points2Impl<TBase>::transformPoints(
    h5geo::Point2Array& data,
    bool toReadData,
    const std::string& lengthUnitsFrom,
    const std::string& lengthUnitsTo,
    bool doCoordTransform)
{
#ifdef H5GEO_USE_GDAL
  if (doCoordTransform){
    OGRCT_ptr coordTrans;
    if (toReadData)
      coordTrans.reset(this->createCoordinateTransformationToReadData(
                         lengthUnitsTo));
    else
      coordTrans.reset(this->createCoordinateTransformationToWriteData(
                         lengthUnitsFrom));

    if (coordTrans){
      for (auto& point : data)
        coordTrans->Transform(1, &point.p[0], &point.p[1]);

      return true;
    } else if (!coordTrans && !h5geo::sr::getIgnoreCoordTransformOnFailure()){
      return false;
    }
  }
#endif

  double coef;
  if (!lengthUnitsFrom.empty() &&
      !lengthUnitsTo.empty()){
    coef = units::convert(
          units::unit_from_string(lengthUnitsFrom),
          units::unit_from_string(lengthUnitsTo));
    for (auto& point : data){
      point.p[0] *= coef;
      point.p[1] *= coef;
    }
  }

  return true;
}

// explicit instantiation (requires that corresponding headers are included)
template class H5Points2Impl<H5Points2>;
