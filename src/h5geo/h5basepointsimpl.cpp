#include "../../include/h5geo/private/h5basepointsimpl.h"
#include "../../include/h5geo/private/h5points1impl.h"
#include "../../include/h5geo/private/h5points2impl.h"
#include "../../include/h5geo/private/h5points3impl.h"
#include "../../include/h5geo/private/h5points4impl.h"
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
H5BasePointsImpl<TBase>::H5BasePointsImpl(const h5gt::Group &group) :
  H5BaseObjectImpl<TBase>(group){}

template <typename TBase>
bool H5BasePointsImpl<TBase>::setNPoints(size_t n)
{
  auto opt = getPointsD();
  if (!opt.has_value())
    return false;

  try {
    opt->resize({n});
    this->objG.flush();
    return true;
  } catch (h5gt::Exception e) {
    return false;
  }
}

template <typename TBase>
bool H5BasePointsImpl<TBase>::setDomain(const h5geo::Domain& val){
  return h5geo::_overwriteEnumAttribute(
        this->objG,
        std::string{h5geo::detail::Domain},
        &val,
        1);
}

template <typename TBase>
H5BaseContainer* H5BasePointsImpl<TBase>::openContainer() const{
  h5gt::File file = this->getH5File();
  return h5geo::openContainer(file);
}

template <typename TBase>
size_t H5BasePointsImpl<TBase>::getNPoints()
{
  auto opt = getPointsD();
  if (!opt.has_value())
    return 0;

  return opt->getElementCount();
}

template <typename TBase>
h5geo::Domain H5BasePointsImpl<TBase>::getDomain(){
  return h5geo::readEnumAttribute<h5gt::Group, h5geo::Domain>(
          this->objG,
          std::string{h5geo::detail::Domain});
}

template <typename TBase>
std::optional<h5gt::DataSet>
H5BasePointsImpl<TBase>::getPointsD() const
{
  std::string name = std::string{h5geo::detail::points_data};

  return this->getDatasetOpt(this->objG, name);
}

template <typename TBase>
H5PointsParam
H5BasePointsImpl<TBase>::getParam()
{
  H5PointsParam p;
  // H5BaseObjectParam
  p.spatialReference = this->getSpatialReference();
  p.lengthUnits = this->getLengthUnits();
  p.temporalUnits = this->getTemporalUnits();
  p.angularUnits = this->getAngularUnits();
  p.dataUnits = this->getDataUnits();

  // H5PointsParam
  p.domain = this->getDomain();
  p.nPoints = this->getNPoints();
  return p;
}

// explicit instantiation (requires that corresponding headers are included)
template class H5BasePointsImpl<H5BasePoints>;
template class H5BasePointsImpl<H5Points1>;
template class H5BasePointsImpl<H5Points2>;
template class H5BasePointsImpl<H5Points3>;
template class H5BasePointsImpl<H5Points4>;
template class H5BasePointsImpl<H5WellTops>;
