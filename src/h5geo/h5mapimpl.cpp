#include "../../include/h5geo/misc/h5mapimpl.h"
#include "../../include/h5geo/h5mapcontainer.h"
#include "../../include/h5geo/h5core.h"

#include <units/units.hpp>

H5MapImpl::H5MapImpl(const h5gt::Group &group) :
  H5BaseObjectImpl(group){}

bool H5MapImpl::writeData(
    Eigen::Ref<Eigen::MatrixXd> M,
    const std::string& dataUnits)
{
  auto opt = getMapD();
  if (!opt.has_value())
    return false;

  return h5geo::overwriteDataset(
        objG,
        opt->getPath(),
        M,
        dataUnits, getDataUnits());
}

Eigen::MatrixXd H5MapImpl::getData(const std::string& dataUnits){
  auto opt = getMapD();
  if (!opt.has_value())
    return Eigen::MatrixXd();

  return h5geo::readDoubleEigenMtxDataset(
        objG,
        opt->getPath(),
        getDataUnits(), dataUnits);
}

bool H5MapImpl::setDomain(const h5geo::Domain& val){
  unsigned v = static_cast<unsigned>(val);
  return h5geo::overwriteAttribute(
        objG,
        std::string{h5geo::detail::Domain},
        v);
}

bool H5MapImpl::setSpatialUnits(const std::string& str){
  return h5geo::overwriteAttribute(
        objG,
        std::string{h5geo::detail::spatial_units},
        str);
}

bool H5MapImpl::setDataUnits(const std::string& str){
  return h5geo::overwriteAttribute(
        objG,
        std::string{h5geo::detail::data_units},
        str);
}

bool H5MapImpl::setOrientation(double orientation){
  return h5geo::overwriteAttribute(
        objG,
        std::string{h5geo::detail::orientation},
        orientation);
}

bool H5MapImpl::setOrigin(
    std::vector<double>& v, const std::string& spatialUnits){
  return h5geo::overwriteAttribute(
        objG,
        std::string{h5geo::detail::origin},
        v, spatialUnits, getSpatialUnits());
}

bool H5MapImpl::setOrigin(
    Eigen::Ref<Eigen::Vector2d> v, const std::string& spatialUnits){
  return h5geo::overwriteAttribute(
        objG,
        std::string{h5geo::detail::origin},
        v, spatialUnits, getSpatialUnits());
}

bool H5MapImpl::setSpacing(
    std::vector<double>& v, const std::string& spatialUnits){
  return h5geo::overwriteAttribute(
        objG,
        std::string{h5geo::detail::spacing},
        v, spatialUnits, getSpatialUnits());
}

bool H5MapImpl::setSpacing(
    Eigen::Ref<Eigen::Vector2d> v, const std::string& spatialUnits){
  return h5geo::overwriteAttribute(
        objG,
        std::string{h5geo::detail::spacing},
        v, spatialUnits, getSpatialUnits());
}

bool H5MapImpl::addAttribute(H5Map* map, std::string name){
  if (this->getH5File() != map->getH5File())
    return false;

  if (name.empty())
    name = map->getName();

  if (objG.hasObject(name, h5gt::ObjectType::Group))
    return false;

  objG.createLink(map->getObjG(), name, h5gt::LinkType::Soft);
  return true;
}

bool H5MapImpl::addExternalAttribute(H5Map* map, std::string name){
  if (this->getH5File() == map->getH5File())
    return false;

  if (name.empty())
    name = map->getName();

  if (objG.hasObject(name, h5gt::ObjectType::Group))
    return false;

  objG.createLink(map->getObjG(), name, h5gt::LinkType::External);
  return true;
}

bool H5MapImpl::removeAttribute(const std::string& name){
  if (name.empty())
    return false;

  if (!objG.hasObject(name, h5gt::ObjectType::Group))
    return false;

  objG.unlink(name);
  return true;
}

H5Map* H5MapImpl::getAttribute(const std::string& name){
  if (!objG.hasObject(name, h5gt::ObjectType::Group))
    return nullptr;

  h5gt::Group group = objG.getGroup(name);
  if (!isGeoObjectByType(group, h5geo::ObjectType::MAP))
    return nullptr;

  return new H5MapImpl(group);
}

h5geo::Domain H5MapImpl::getDomain(){
  return static_cast<h5geo::Domain>(
        h5geo::readEnumAttribute(
          objG,
          std::string{h5geo::detail::Domain}));
}

std::string H5MapImpl::getSpatialUnits(){
  return h5geo::readStringAttribute(
        objG,
        std::string{h5geo::detail::spatial_units});
}

std::string H5MapImpl::getDataUnits(){
  return h5geo::readStringAttribute(
        objG,
        std::string{h5geo::detail::data_units});
}

double H5MapImpl::getOrientation(){
  return h5geo::readDoubleAttribute(
        objG,
        std::string{h5geo::detail::orientation});
}

Eigen::VectorXd H5MapImpl::getOrigin(const std::string& spatialUnits){
  return h5geo::readDoubleEigenVecAttribute(
        objG,
        std::string{h5geo::detail::origin},
        getSpatialUnits(), spatialUnits);
}

Eigen::VectorXd H5MapImpl::getSpacing(const std::string& spatialUnits){
  return h5geo::readDoubleEigenVecAttribute(
        objG,
        std::string{h5geo::detail::spacing},
        getSpatialUnits(), spatialUnits);
}


H5MapContainer* H5MapImpl::getMapContainer() const{
  h5gt::File file = getH5File();
  return h5geo::createMapContainer(
        file, h5geo::CreationType::OPEN_OR_CREATE);
}

std::optional<h5gt::DataSet>
H5MapImpl::getMapD() const
{
  std::string name = std::string{magic_enum::enum_name(
        h5geo::detail::MapDatasets::map_data)};

  return getDatasetOpt(objG, name);
}
