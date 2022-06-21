#include "../../include/h5geo/private/h5welltopsimpl.h"
#include "../../include/h5geo/private/h5wellimpl.h"
#include "../../include/h5geo/h5core.h"  // to build on Windows

H5WellTopsImpl::H5WellTopsImpl(const h5gt::Group &group) :
  H5Points1Impl(group){}

bool H5WellTopsImpl::writeData(
    const std::map<std::string, double>& data,
    const std::string& lengthUnits,
    const std::string& temporalUnits)
{
  h5geo::Point1Array arr(data.size());
  size_t i = 0;
  for (auto const& x : data){
    arr[i].setName(x.first);
    arr[i].p[0] = x.second;
    i++;
  }
  return this->writeData(arr, lengthUnits, temporalUnits);
}

std::map<std::string, double> H5WellTopsImpl::getDataAsMap(
    const std::string& lengthUnits,
    const std::string& temporalUnits)
{
  std::map<std::string, double> m;
  h5geo::Point1Array arr = this->getData(lengthUnits, temporalUnits);
  for (auto& point : arr)
    m[point.name] = point.p[0];

  return m;
}

H5Well* H5WellTopsImpl::openWell()
{
  auto optWellG = getParentG(h5geo::ObjectType::WELL);
  if (!optWellG.has_value())
    return nullptr;

  return new H5WellImpl(optWellG.value());
}
