#include "../../include/h5geo/misc/h5surfimpl.h"
#include "../../include/h5geo/h5surfcontainer.h"
#include "../../include/h5geo/h5core.h"

H5SurfImpl::H5SurfImpl(const h5gt::Group &group) :
  H5BaseObjectImpl(group){}

bool H5SurfImpl::writeData(
    const Eigen::Ref<const Eigen::MatrixXd>& M)
{
  auto opt = getSurfD();
  if (!opt.has_value())
    return false;

  if (M.size() != opt->getElementCount())
    return false;

  opt->write_raw(M.data());
  return true;
}

Eigen::MatrixXd H5SurfImpl::getData() const{
  auto opt = getSurfD();
  if (!opt.has_value())
    return Eigen::MatrixXd();

  std::vector<size_t> dims(opt->getDimensions());
  Eigen::MatrixXd M(dims[1], dims[0]);
  opt->read(M.data());

  return M;
}

bool H5SurfImpl::setDomain(const h5geo::Domain& val){
  return h5geo::setEnumFromObj(
        objG,
        std::string{h5geo::detail::Domain},
        static_cast<unsigned>(val));
}

bool H5SurfImpl::setSpatialUnits(const std::string& str){
  return h5geo::setStringFromObj(
        objG,
        std::string{h5geo::detail::spatial_units},
        str);
}

bool H5SurfImpl::setTemporalUnits(const std::string& str){
  return h5geo::setStringFromObj(
        objG,
        std::string{h5geo::detail::temporal_units},
        str);
}

bool H5SurfImpl::setDataUnits(const std::string& str){
  return h5geo::setStringFromObj(
        objG,
        std::string{h5geo::detail::data_units},
        str);
}

bool H5SurfImpl::setOrigin(const std::vector<double>& v){
  return h5geo::setFloatVecFromObj(
        objG,
        std::string{h5geo::detail::origin},
        v);
}

bool H5SurfImpl::setOrigin(const Eigen::Ref<const Eigen::Vector2d>& v){
  return h5geo::setFloatVecFromObj(
        objG,
        std::string{h5geo::detail::origin},
        v);
}

bool H5SurfImpl::setSpacing(const std::vector<double>& v){
  return h5geo::setFloatVecFromObj(
        objG,
        std::string{h5geo::detail::spacing},
        v);
}

bool H5SurfImpl::setSpacing(const Eigen::Ref<const Eigen::Vector2d>& v){
  return h5geo::setFloatVecFromObj(
        objG,
        std::string{h5geo::detail::spacing},
        v);
}

h5geo::Domain H5SurfImpl::getDomain(){
  return static_cast<h5geo::Domain>(
        h5geo::getEnumFromObj(
          objG,
          std::string{h5geo::detail::Domain}));
}

std::string H5SurfImpl::getSpatialUnits(){
  return h5geo::getStringFromObj(
        objG,
        std::string{h5geo::detail::spatial_units});
}

std::string H5SurfImpl::getTemporalUnits(){
  return h5geo::getStringFromObj(
        objG,
        std::string{h5geo::detail::temporal_units});
}

std::string H5SurfImpl::getDataUnits(){
  return h5geo::getStringFromObj(
        objG,
        std::string{h5geo::detail::data_units});
}

Eigen::Vector2d H5SurfImpl::getOrigin(){
  return h5geo::getEigenFloatVecFromObj(
        objG,
        std::string{h5geo::detail::origin});
}

Eigen::Vector2d H5SurfImpl::getSpacing(){
  return h5geo::getEigenFloatVecFromObj(
        objG,
        std::string{h5geo::detail::spacing});
}


H5SurfContainer* H5SurfImpl::getSurfContainer() const{
  h5gt::File file = getH5File();
  return h5geo::createSurfContainer(
        file, h5geo::CreationType::OPEN_OR_CREATE);
}

std::optional<h5gt::DataSet>
H5SurfImpl::getSurfD() const
{
  std::string name = std::string{magic_enum::enum_name(
        h5geo::detail::SurfDatasets::surf_data)};

  return getDatasetOpt(objG, name);
}
