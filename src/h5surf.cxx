#include "../include/h5surf/h5surf.h"
#include "../include/h5surf/h5surfcontainer.h"

H5Surf::H5Surf(const h5gt::Group &group) :
  H5BaseObject(group)
{

}

H5Surf::~H5Surf(){

}

std::optional<H5Surf>
H5Surf::create(h5gt::Group &group){
  if (H5BaseContainer::isObject(
        group, h5geo::ObjectType::SURFACE))
    return H5Surf(group);
  else
    return std::nullopt;
}

bool H5Surf::writeData(
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

Eigen::MatrixXd H5Surf::getData(){
  auto opt = getSurfD();
  if (!opt.has_value())
    return Eigen::MatrixXd();

  std::vector<size_t> dims(opt->getDimensions());
  Eigen::MatrixXd M(dims[1], dims[0]);
  opt->read(M.data());

  return M;
}

std::optional<H5SurfContainer>
H5Surf::getSurfContainer(){
  h5gt::File file = getH5File();
  return H5SurfContainer::create(file);
}

std::optional<h5gt::DataSet>
H5Surf::getSurfD()
{
  std::string name = std::string{magic_enum::enum_name(
        h5geo::SurfDatasets::surf_data)};

  return getDatasetOpt(objG, name);
}
