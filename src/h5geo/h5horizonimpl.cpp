#include "../../include/h5geo/private/h5horizonimpl.h"
#include "../../include/h5geo/h5core.h"
#include "../../include/h5geo/private/h5enum_string.h"

#include <units/units.hpp>

H5HorizonImpl::H5HorizonImpl(const h5gt::Group &group) :
  H5BaseObjectImpl(group){}

bool H5HorizonImpl::writeData(
    Eigen::Ref<Eigen::MatrixXd> M,
    const std::string& unitsFrom,
    const std::string& unitsTo)
{
  auto opt = getHorizonD();
  if (!opt.has_value())
    return false;

  bool val = h5geo::overwriteResizableDataset(
        objG,
        opt->getPath(),
        M,
        unitsFrom, unitsTo);

  objG.flush();
  return val;
}

Eigen::MatrixXd H5HorizonImpl::getData(
    const std::string& unitsFrom,
    const std::string& unitsTo)
{
  auto opt = getHorizonD();
  if (!opt.has_value())
    return Eigen::MatrixXd();

  return h5geo::readDoubleEigenMtxDataset(
        objG,
        opt->getPath(),
        unitsFrom, unitsTo);
}

bool H5HorizonImpl::writeComponent(
    Eigen::Ref<Eigen::VectorXd> v,
    const std::string& componentName,
    const std::string& unitsFrom,
    const std::string& unitsTo)
{
  auto opt = getHorizonD();
  if (!opt.has_value())
    return false;

  bool val;
  if (!unitsFrom.empty() && !unitsTo.empty()){
    double coef = units::convert(
          units::unit_from_string(unitsFrom),
          units::unit_from_string(unitsTo));

    v *= coef;
  }

  val = h5geo::writeData2IndexedDataset(
        opt.value(),
        componentName,
        v,
        true);

  objG.flush();
  return val;
}

Eigen::VectorXd H5HorizonImpl::getComponent(
    const std::string& componentName,
    const std::string& unitsFrom,
    const std::string& unitsTo)
{
  auto opt = getHorizonD();
  if (!opt.has_value())
    return Eigen::VectorXd();

  Eigen::VectorXd v = h5geo::getDataFromIndexedDataset<double>(
        opt.value(), componentName);

  if (!unitsFrom.empty() && !unitsTo.empty()){
    double coef = units::convert(
          units::unit_from_string(unitsFrom),
          units::unit_from_string(unitsTo));

    if (!std::isnan(coef))
      return v*coef;

    return Eigen::VectorXd();
  }

  return v;
}

bool H5HorizonImpl::setNPoints(size_t n)
{
  auto opt = getHorizonD();
  if (!opt.has_value())
    return false;

  std::vector<size_t> dims = opt->getSpace().getDimensions();
  try {
    if (dims.size() == 1)
      opt->resize({n});
    else if (dims.size() == 2)
      opt->resize({dims[0], n});
    else
      return false;
    this->objG.flush();
    return true;
  } catch (h5gt::Exception e) {
    return false;
  }
}

bool H5HorizonImpl::setNComponents(size_t n)
{
  auto opt = getHorizonD();
  if (!opt.has_value())
    return false;

  std::vector<size_t> dims = opt->getSpace().getDimensions();
  try {
    if (dims.size() == 2)
      opt->resize({n, dims[1]});
    else
      return false;
    this->objG.flush();
    return true;
  } catch (h5gt::Exception e) {
    return false;
  }
}

bool H5HorizonImpl::setComponents(const std::map<std::string, size_t>& components)
{
  auto dsetOpt = this->getHorizonD();
  if (!dsetOpt.has_value())
    return false;

  std::vector attrNames = dsetOpt->listAttributeNames();
  try {
    for (const auto& attrName : attrNames)
      dsetOpt->deleteAttribute(attrName);
    dsetOpt->flush();
  } catch (h5gt::Exception e) {
    return false;
  }

  for (auto const& [key, val] : components)
    dsetOpt->createAttribute<size_t>(key, h5gt::DataSpace(1)).write(val);

  return true;
}

bool H5HorizonImpl::setDomain(const h5geo::Domain& val){
  return h5geo::_overwriteEnumAttribute(
        this->objG,
        std::string{h5geo::detail::Domain},
        &val,
        1);
}

H5BaseContainer* H5HorizonImpl::openContainer() const{
  h5gt::File file = this->getH5File();
  return h5geo::openContainer(file);
}

size_t H5HorizonImpl::getNPoints()
{
  auto opt = getHorizonD();
  if (!opt.has_value())
    return 0;

  std::vector<size_t> dims = opt->getSpace().getDimensions();
  if (dims.size() == 1)
    return dims[0];
  else if (dims.size() == 2)
    return dims[1];
  return 0;
}

size_t H5HorizonImpl::getNComponents()
{
  auto opt = getHorizonD();
  if (!opt.has_value())
    return 0;

  std::vector<size_t> dims = opt->getSpace().getDimensions();
  if (dims.size() == 1)
    return 1;
  else if (dims.size() == 2)
    return dims[0];
  return 0;
}

std::map<std::string, size_t> H5HorizonImpl::getComponents()
{
  auto dsetOpt = this->getHorizonD();
  if (!dsetOpt.has_value())
    return std::map<std::string, size_t>();

  std::vector attrNames = dsetOpt->listAttributeNames();
  std::map<std::string, size_t> components;
  for (const auto& attrName : attrNames){
    ptrdiff_t idx = h5geo::getIndexFromAttribute(dsetOpt.value(), attrName);
    if (idx < 0)
      continue;

    components[attrName] = idx;
  }
  return components;
}

h5geo::Domain H5HorizonImpl::getDomain(){
  return h5geo::readEnumAttribute<h5gt::Group, h5geo::Domain>(
          this->objG,
          std::string{h5geo::detail::Domain});
}

HorizonParam
H5HorizonImpl::getParam()
{
  HorizonParam p;
  // BaseObjectParam
  p.spatialReference = this->getSpatialReference();
  p.lengthUnits = this->getLengthUnits();
  p.temporalUnits = this->getTemporalUnits();
  p.angularUnits = this->getAngularUnits();
  p.dataUnits = this->getDataUnits();

  // PointsParam
  p.domain = this->getDomain();
  p.nPoints = this->getNPoints();
  p.components = this->getComponents();

  auto dsetOpt = this->getHorizonD();
  if (!dsetOpt.has_value())
    return p;

  auto dsetCreateProps = dsetOpt->getCreateProps();
  if (dsetCreateProps.isChunked()){
    std::vector<hsize_t> chunkSizeVec = dsetCreateProps.getChunk(dsetOpt->getDimensions().size());
    if (chunkSizeVec.size() == 1)
      p.pointsChunkSize = chunkSizeVec[0];
    else if (chunkSizeVec.size() == 2)
      p.pointsChunkSize = chunkSizeVec[1];
  }
  return p;
}

std::optional<h5gt::DataSet>
H5HorizonImpl::getHorizonD() const
{
  std::string name = std::string{h5geo::detail::horizon_data};

  return this->getDatasetOpt(this->objG, name);
}
