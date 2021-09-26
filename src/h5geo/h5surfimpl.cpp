#include "../../include/h5geo/misc/h5surfimpl.h"
#include "../../include/h5geo/h5surfcontainer.h"
#include "../../include/h5geo/h5core.h"

#include <units/units.hpp>

H5SurfImpl::H5SurfImpl(const h5gt::Group &group) :
  H5BaseObjectImpl(group){}

bool H5SurfImpl::writeData(
    Eigen::Ref<Eigen::MatrixXd> M,
    const std::string& dataUnits)
{
  auto opt = getSurfD();
  if (!opt.has_value())
    return false;

  return h5geo::overwriteDataset(
        objG,
        opt->getPath(),
        M,
        dataUnits, getDataUnits());
}

Eigen::MatrixXd H5SurfImpl::getData(const std::string& dataUnits){
  auto opt = getSurfD();
  if (!opt.has_value())
    return Eigen::MatrixXd();

  return h5geo::readDoubleEigenMtxDataset(
        objG,
        opt->getPath(),
        getDataUnits(), dataUnits);
}

bool H5SurfImpl::setDomain(const h5geo::Domain& val){
  unsigned v = static_cast<unsigned>(val);
  return h5geo::overwriteAttribute(
        objG,
        std::string{h5geo::detail::Domain},
        v);
}

bool H5SurfImpl::setSpatialUnits(const std::string& str){
  return h5geo::overwriteAttribute(
        objG,
        std::string{h5geo::detail::spatial_units},
        str);
}

bool H5SurfImpl::setDataUnits(const std::string& str){
  return h5geo::overwriteAttribute(
        objG,
        std::string{h5geo::detail::data_units},
        str);
}

bool H5SurfImpl::setOrientation(double orientation){
  return h5geo::overwriteAttribute(
        objG,
        std::string{h5geo::detail::orientation},
        orientation);
}

bool H5SurfImpl::setOrigin(
    std::vector<double>& v, const std::string& spatialUnits){
  return h5geo::overwriteAttribute(
        objG,
        std::string{h5geo::detail::origin},
        v, spatialUnits, getSpatialUnits());
}

bool H5SurfImpl::setOrigin(
    Eigen::Ref<Eigen::Vector2d> v, const std::string& spatialUnits){
  return h5geo::overwriteAttribute(
        objG,
        std::string{h5geo::detail::origin},
        v, spatialUnits, getSpatialUnits());
}

bool H5SurfImpl::setSpacing(
    std::vector<double>& v, const std::string& spatialUnits){
  return h5geo::overwriteAttribute(
        objG,
        std::string{h5geo::detail::spacing},
        v, spatialUnits, getSpatialUnits());
}

bool H5SurfImpl::setSpacing(
    Eigen::Ref<Eigen::Vector2d> v, const std::string& spatialUnits){
  return h5geo::overwriteAttribute(
        objG,
        std::string{h5geo::detail::spacing},
        v, spatialUnits, getSpatialUnits());
}

h5geo::Domain H5SurfImpl::getDomain(){
  return static_cast<h5geo::Domain>(
        h5geo::readEnumAttribute(
          objG,
          std::string{h5geo::detail::Domain}));
}

std::string H5SurfImpl::getSpatialUnits(){
  return h5geo::readStringAttribute(
        objG,
        std::string{h5geo::detail::spatial_units});
}

std::string H5SurfImpl::getDataUnits(){
  return h5geo::readStringAttribute(
        objG,
        std::string{h5geo::detail::data_units});
}

double H5SurfImpl::getOrientation(){
  return h5geo::readDoubleAttribute(
        objG,
        std::string{h5geo::detail::orientation});
}

Eigen::VectorXd H5SurfImpl::getOrigin(const std::string& spatialUnits){
  return h5geo::readDoubleEigenVecAttribute(
        objG,
        std::string{h5geo::detail::origin},
        getSpatialUnits(), spatialUnits);
}

Eigen::VectorXd H5SurfImpl::getSpacing(const std::string& spatialUnits){
  return h5geo::readDoubleEigenVecAttribute(
        objG,
        std::string{h5geo::detail::spacing},
        getSpatialUnits(), spatialUnits);
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
