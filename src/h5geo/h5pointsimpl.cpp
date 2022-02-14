#include "../../include/h5geo/misc/h5pointsimpl.h"
#include "../../include/h5geo/h5basecontainer.h"
#include "../../include/h5geo/h5core.h"
#include "../../include/h5geo/misc/h5core_enum_string.h"

#include <units/units.hpp>

#ifdef H5GEO_USE_GDAL
#include <gdal/gdal.h>
#include <gdal/gdal_priv.h>
#endif


H5PointsImpl::H5PointsImpl(const h5gt::Group &group) :
  H5BaseObjectImpl(group){}

bool H5PointsImpl::writeData(
    h5geo::PointArray& data,
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

h5geo::PointArray H5PointsImpl::getData(
    const std::string& lengthUnits,
    const std::string& temporalUnits,
    bool doCoordTransform)
{
  auto opt = getPointsD();
  if (!opt.has_value())
    return h5geo::PointArray();

  auto dtype = opt->getDataType();
  if (!dtype.isTypeEqual(h5geo::compound_Point()))
    return h5geo::PointArray();

  h5geo::PointArray data(opt->getElementCount());
  opt->read(data);

  bool val = transformPoints(
        data,
        getLengthUnits(),
        lengthUnits,
        getTemporalUnits(),
        temporalUnits,
        doCoordTransform,
        createCoordinateTransformationToReadData(getLengthUnits()));

  if (!val)
    return h5geo::PointArray();

  return data;
}

bool H5PointsImpl::setNPoints(size_t n)
{
  auto opt = getPointsD();
  if (!opt.has_value())
    return false;

  try {
    opt->resize({n});
    objG.flush();
    return true;
  } catch (h5gt::Exception e) {
    return false;
  }
}

bool H5PointsImpl::setDomain(const h5geo::Domain& val){
  unsigned v = static_cast<unsigned>(val);
  return h5geo::overwriteAttribute(
        objG,
        std::string{h5geo::detail::Domain},
        v);
}

H5BaseContainer* H5PointsImpl::openContainer() const{
  h5gt::File file = getH5File();
  return h5geo::openContainer(file);
}

size_t H5PointsImpl::getNPoints()
{
  auto opt = getPointsD();
  if (!opt.has_value())
    return 0;

  return opt->getElementCount();
}

h5geo::Domain H5PointsImpl::getDomain(){
  return static_cast<h5geo::Domain>(
        h5geo::readEnumAttribute(
          objG,
          std::string{h5geo::detail::Domain}));
}

std::optional<h5gt::DataSet>
H5PointsImpl::getPointsD() const
{
  std::string name = std::string{h5geo::detail::points_data};

  return getDatasetOpt(objG, name);
}

H5Points* h5geo::openPoints(h5gt::Group group){
  if (isGeoObjectByType(group, h5geo::ObjectType::POINTS))
      return new H5PointsImpl(group);

  return nullptr;
}

bool H5PointsImpl::overwritePointsDataset(
    h5geo::PointArray& data,
    const std::string& lengthUnits,
    const std::string& temporalUnits,
    bool doCoordTransform)
{
  auto opt = getPointsD();
  if (!opt.has_value())
    return false;

  auto dtype = opt->getDataType();
  if (!dtype.isTypeEqual(h5geo::compound_Point())){
    return false;
  }

  bool val = transformPoints(
        data,
        lengthUnits,
        getLengthUnits(),
        temporalUnits,
        getTemporalUnits(),
        doCoordTransform,
        createCoordinateTransformationToWriteData(lengthUnits));

  if (!val)
    return false;

  try {
    opt->resize({data.size()});
    opt->write_raw(data.data(), h5geo::compound_Point());
    objG.flush();
    return true;
  } catch (h5gt::Exception e) {
    return false;
  }
}

bool H5PointsImpl::transformPoints(
    h5geo::PointArray& data,
    const std::string& lengthUnitsFrom,
    const std::string& lengthUnitsTo,
    const std::string& temporalUnitsFrom,
    const std::string& temporalUnitsTo,
    bool doCoordTransform,
    OGRCoordinateTransformation* coordTrans)
{
  h5geo::Domain domain = getDomain();
#ifdef H5GEO_USE_GDAL
  if (doCoordTransform){
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
