#include "../include/h5geo/misc/h5surfimpl.h"
#include "../include/h5geo/h5surfcontainer.h"
#include "../include/h5geo/h5core.h"

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

H5SurfContainer* H5SurfImpl::getSurfContainer() const{
  h5gt::File file = getH5File();
  return h5geo::createSurfContainer(
        file, h5geo::CreationType::OPEN_OR_CREATE);
}

std::optional<h5gt::DataSet>
H5SurfImpl::getSurfD() const
{
  std::string name = std::string{magic_enum::enum_name(
        h5geo::SurfDatasets::surf_data)};

  return getDatasetOpt(objG, name);
}
