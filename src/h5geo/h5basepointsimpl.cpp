#include "../../include/h5geo/impl/h5basepointsimpl.h"
#include "../../include/h5geo/impl/h5points3impl.h"
#include "../../include/h5geo/h5basecontainer.h"
#include "../../include/h5geo/misc/h5core.h"
#include "../../include/h5geo/misc/h5enum_string.h"

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
  unsigned v = static_cast<unsigned>(val);
  return h5geo::overwriteAttribute(
        this->objG,
        std::string{h5geo::detail::Domain},
        v);
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
  return static_cast<h5geo::Domain>(
        h5geo::readEnumAttribute(
          this->objG,
          std::string{h5geo::detail::Domain}));
}

template <typename TBase>
std::optional<h5gt::DataSet>
H5BasePointsImpl<TBase>::getPointsD() const
{
  std::string name = std::string{h5geo::detail::points_data};

  return this->getDatasetOpt(this->objG, name);
}

H5BasePoints*
h5geo::openPoints(h5gt::Group group)
{
  return nullptr;
//  return new H5BasePointsImpl<H5BasePoints>(group);
}

// explicit instantiation (requires that corresponding headers are included)
template class H5BasePointsImpl<H5BasePoints>;
template class H5BasePointsImpl<H5Points3>;
