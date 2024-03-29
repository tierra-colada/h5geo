#include "../../include/h5geo/private/h5baseimpl.h"
#include "../../include/h5geo/private/h5basecontainerimpl.h"
#include "../../include/h5geo/private/h5mapcontainerimpl.h"
#include "../../include/h5geo/private/h5seiscontainerimpl.h"
#include "../../include/h5geo/private/h5volcontainerimpl.h"
#include "../../include/h5geo/private/h5wellcontainerimpl.h"
#include "../../include/h5geo/private/h5baseobjectimpl.h"
#include "../../include/h5geo/private/h5mapimpl.h"
#include "../../include/h5geo/private/h5seisimpl.h"
#include "../../include/h5geo/private/h5volimpl.h"
#include "../../include/h5geo/private/h5wellimpl.h"
#include "../../include/h5geo/private/h5devcurveimpl.h"
#include "../../include/h5geo/private/h5logcurveimpl.h"
#include "../../include/h5geo/private/h5basepointsimpl.h"
#include "../../include/h5geo/private/h5points1impl.h"
#include "../../include/h5geo/private/h5points2impl.h"
#include "../../include/h5geo/private/h5points3impl.h"
#include "../../include/h5geo/private/h5points4impl.h"
#include "../../include/h5geo/private/h5welltopsimpl.h"
#include "../../include/h5geo/private/h5horizonimpl.h"
#include "../../include/h5geo/h5core.h"
#include "../../include/h5geo/private/h5enum_string.h"

#include <h5gt/H5File.hpp>
#include <h5gt/H5Group.hpp>
#include <h5gt/H5DataSet.hpp>

#ifdef H5GEO_USE_GDAL
#include <gdal.h>
#include <gdal_priv.h>
#endif

#ifdef H5GEO_USE_GDAL
#include <gdal.h>
#include <gdal_priv.h>
#endif

#include <optional>
#include <filesystem>
namespace fs = std::filesystem;

void h5geo::ObjectDeleter::operator()(H5Base * ptr) const
{
  ptr->Delete();
}

#ifdef H5GEO_USE_GDAL
void h5geo::OGRCoordinateTransformationDeleter::operator()(OGRCoordinateTransformation * ptr) const
{
  OGRCoordinateTransformation::DestroyCT(ptr);
}

void h5geo::GDALDatasetDeleter::operator()(GDALDataset * ptr) const
{
  GDALClose(ptr);
}
#endif

template <typename TBase>
H5Base* H5BaseImpl<TBase>::clone()
{
  return nullptr;
}

template <typename TBase>
void H5BaseImpl<TBase>::Delete()
{
  delete this;
}

template <typename TBase>
std::vector<h5gt::Group>
H5BaseImpl<TBase>::getChildGroupList(
    h5gt::Group& group,
    const h5geo::ObjectType& objType,
    bool recursive)
{
  std::vector<h5gt::Group> childList;
  std::vector<std::string> nameList = group.listObjectNames();
  std::string activeDevName = std::string{h5geo::detail::ACTIVE};
  for (const auto& name : nameList){
    if (group.getObjectType(name) != h5gt::ObjectType::Group)
      continue;

    h5gt::Group childG = group.getGroup(name);
    if (objType != h5geo::ObjectType::DEVCURVE && h5geo::isGeoObjectByType(childG, objType)){
      childList.push_back(childG);
    } else if (objType == h5geo::ObjectType::DEVCURVE && h5geo::isGeoObjectByType(childG, objType)){
      if (name == activeDevName)
        continue;
      childList.push_back(childG);
    } else if (recursive) {
      std::vector<h5gt::Group> subChildList = getChildGroupList(childG, objType, recursive);
      childList.reserve(childList.size() + subChildList.size());
      childList.insert(
            childList.end(),
            std::make_move_iterator(subChildList.begin()),
            std::make_move_iterator(subChildList.end()));
      /* if don't erase then `subChildList`stays in undefined
       * but safe-to-destruct state. */
      subChildList.erase(subChildList.begin(), subChildList.end());
    }
  }
  return childList;
}

template <typename TBase>
std::vector<std::string>
H5BaseImpl<TBase>::getChildNameList(
    h5gt::Group& group,
    const h5geo::ObjectType& objType,
    const std::string& referencePath,
    bool recursive)
{
  std::vector<std::string> childList;
  std::vector<std::string> nameList = group.listObjectNames();
  std::string activeDevName = std::string{h5geo::detail::ACTIVE};
  for (const auto& name : nameList){
    if (group.getObjectType(name) != h5gt::ObjectType::Group)
      continue;

    h5gt::Group childG = group.getGroup(name);
    if (objType != h5geo::ObjectType::DEVCURVE && h5geo::isGeoObjectByType(childG, objType)){
      childList.push_back(h5geo::getRelativePath(
                            referencePath, childG.getPath(),
                            h5geo::CaseSensitivity::CASE_INSENSITIVE));
    } else if (objType == h5geo::ObjectType::DEVCURVE && h5geo::isGeoObjectByType(childG, objType)){
      if (name == activeDevName)
        continue;
      childList.push_back(h5geo::getRelativePath(
                            referencePath, childG.getPath(),
                            h5geo::CaseSensitivity::CASE_INSENSITIVE));
    } else if (recursive){
      std::vector<std::string> subChildList = getChildNameList(childG, objType, referencePath, recursive);
      childList.reserve(childList.size() + subChildList.size());
      childList.insert(
            childList.end(),
            std::make_move_iterator(subChildList.begin()),
            std::make_move_iterator(subChildList.end()));
      /* if don't erase then `subChildList`stays in undefined
       * but safe-to-destruct state. */
      subChildList.erase(subChildList.begin(), subChildList.end());
    }
  }
  return childList;
}

template <typename TBase>
size_t
H5BaseImpl<TBase>::getChildCount(
    h5gt::Group& group,
    const h5geo::ObjectType& objType,
    bool recursive)
{
  size_t n = 0;
  std::vector<std::string> nameList = group.listObjectNames();
  std::string activeDevName = std::string{h5geo::detail::ACTIVE};
  for (const auto& name : nameList){
    if (group.getObjectType(name) != h5gt::ObjectType::Group)
      continue;

    h5gt::Group childG = group.getGroup(name);
    if (objType != h5geo::ObjectType::DEVCURVE && h5geo::isGeoObjectByType(childG, objType)){
      n += 1;
    } else if (objType == h5geo::ObjectType::DEVCURVE && h5geo::isGeoObjectByType(childG, objType)){
      if (name == activeDevName)
        continue;
      n += 1;
    } else if (recursive) {
      n += getChildCount(childG, objType, recursive);
    }
  }
  return n;
}

template <typename TBase>
std::optional<h5gt::File>
H5BaseImpl<TBase>::createContainer(
    std::string& fileName,
    const h5geo::ContainerType& containerType,
    h5geo::CreationType createFlag)
{
  if (fileName.empty() &&
      createFlag != h5geo::CreationType::CREATE_UNDER_NEW_NAME){
    return std::nullopt;
  } else if (fileName.empty() &&
             createFlag == h5geo::CreationType::CREATE_UNDER_NEW_NAME){
    fileName = std::string{magic_enum::enum_name(containerType)} + ".h5";
  }
  bool fileExist = fs::exists(fileName);

  try {

    if (createFlag == h5geo::CreationType::CREATE_UNDER_NEW_NAME){
      if (fileExist){
        fs::path p = fileName;
        std::vector<std::string> fileNames;
        if (p.has_parent_path()){
          for (const auto & file : std::filesystem::directory_iterator(p.parent_path()))
            fileNames.push_back(file.path().stem().generic_string());
        } else {
          for (const auto & file : std::filesystem::directory_iterator(fs::current_path()))
            fileNames.push_back(file.path().stem().generic_string());
        }

        fileName = h5geo::generateName(
              fileNames, p.stem().generic_string());

        // it no parent path then we don't want to make new filename as `/name` (think how UNIX would feel)
        if (p.has_parent_path()){
          fileName = p.parent_path().generic_string() + "/" +
              fileName + p.extension().generic_string();
        } else {
          fileName = fileName + p.extension().generic_string();
        }

        fileExist = false;  // now when the `fileName` is unique we change  `fileExist` to `false`
      }
      createFlag = h5geo::CreationType::CREATE;
    }

    if (fileExist){
      // `createFlag` defines whether h5file is isGeoContainer or not
      // thus the if flag is inapropriate hen it will be filtered
      // in the following call to the `createContainer()` function below
      if (createFlag == h5geo::CreationType::OPEN |
          createFlag == h5geo::CreationType::CREATE |
          createFlag == h5geo::CreationType::OPEN_OR_CREATE){
        if (H5Fis_hdf5(fileName.c_str()) > 0){
          h5gt::File h5File(
                fileName,
                h5gt::File::ReadWrite |
                h5gt::File::OpenOrCreate);
          return createContainer(h5File, containerType, createFlag);
        }
        return std::nullopt;
      }
    }

    h5gt::File h5File(
          fileName,
          h5gt::File::ReadWrite |
          h5gt::File::Create |
          h5gt::File::Truncate);
    return createContainer(h5File, containerType, createFlag);

  } catch (h5gt::Exception& err) {
    return std::nullopt;
  }
}

template <typename TBase>
std::optional<h5gt::File>
H5BaseImpl<TBase>::createContainer(
    h5gt::File h5File,
    const h5geo::ContainerType& containerType,
    h5geo::CreationType createFlag)
{
  switch (createFlag) {
  case h5geo::CreationType::OPEN: {
    if (h5geo::isGeoContainerByType(h5File, containerType))
      return h5File;

    return std::nullopt;
  } case h5geo::CreationType::CREATE: {
    if (h5geo::isGeoContainer(h5File))
      return std::nullopt;

    return createNewContainer(h5File, containerType);
  } case h5geo::CreationType::OPEN_OR_CREATE: {
    if (h5geo::isGeoContainerByType(h5File, containerType))
      return h5File;

    return createContainer(h5File, containerType, h5geo::CreationType::CREATE);
  } case h5geo::CreationType::CREATE_OR_OVERWRITE: {
    return createNewContainer(h5File, containerType);
  } case h5geo::CreationType::CREATE_UNDER_NEW_NAME: {
    return std::nullopt;
  } default :
    return std::nullopt;
  }
}

template <typename TBase>
std::optional<h5gt::Group>
H5BaseImpl<TBase>::createObject(
    std::string& objName,
    h5gt::File parentFile,
    const h5geo::ObjectType& objType,
    void* p,
    h5geo::CreationType createFlag)
{
  h5gt::Group parentGroup = parentFile.getGroup("/");
  return createObject(
        objName, parentGroup, objType, p, createFlag);
}

template <typename TBase>
std::optional<h5gt::Group>
H5BaseImpl<TBase>::createObject(
    std::string& objName,
    h5gt::Group parentGroup,
    const h5geo::ObjectType& objType,
    void* p,
    h5geo::CreationType createFlag)
{
  if (objName.empty() &&
      createFlag != h5geo::CreationType::CREATE_UNDER_NEW_NAME){
    return std::nullopt;
  } else if (objName.empty() &&
             createFlag == h5geo::CreationType::CREATE_UNDER_NEW_NAME) {
    objName = std::string{magic_enum::enum_name(objType)};
  }

  if (createFlag == h5geo::CreationType::CREATE_UNDER_NEW_NAME){
    if (parentGroup.hasObject(objName, h5gt::ObjectType::Group)){
      h5gt::Group objG = parentGroup.getGroup(objName);
      h5gt::Group closestParent = objG.getParent();
      std::vector<std::string> nameList =
          closestParent.listObjectNames();
      h5geo::splitPathToParentAndObj(objName, objName); // now `objName` contains pure filename without path
      objName = h5geo::generateName(nameList, objName);
      objName = closestParent.getPath() + "/" + objName;
    }
    createFlag = h5geo::CreationType::CREATE;
  }

  if (parentGroup.hasObject(objName, h5gt::ObjectType::Group)){
    h5gt::Group objG = parentGroup.getGroup(objName);
    return createObject(
          objG, objType, p, createFlag);
  }

  h5gt::Group objG = parentGroup.createGroup(objName);
  return createObject(
        objG, objType, p, createFlag);
}

template <typename TBase>
std::optional<h5gt::Group>
H5BaseImpl<TBase>::createObject(
    h5gt::Group objG,
    const h5geo::ObjectType& objType,
    void* p,
    h5geo::CreationType createFlag)
{
  switch (createFlag) {
  case h5geo::CreationType::OPEN: {
    if (h5geo::isGeoObjectByType(objG, objType))
      return objG;

    return std::nullopt;
  } case h5geo::CreationType::CREATE: {
    if (h5geo::isGeoObject(objG))
      return std::nullopt;

    return createNewObject(objG, objType, p);
  } case h5geo::CreationType::OPEN_OR_CREATE: {
    if (h5geo::isGeoObjectByType(objG, objType))
      return objG;

    return createObject(objG, objType, p, h5geo::CreationType::CREATE);
  } case h5geo::CreationType::CREATE_OR_OVERWRITE: {
    if (!h5geo::unlinkContent(objG))
      return std::nullopt;
    if (!h5geo::deleteAllAttributes(objG))
      return std::nullopt;

    return createNewObject(objG, objType, p);
  } case h5geo::CreationType::CREATE_UNDER_NEW_NAME: {
    return std::nullopt;
  }  default:
    return std::nullopt;
  }
}

template <typename TBase>
std::optional<h5gt::File>
H5BaseImpl<TBase>::createNewContainer(
    h5gt::File &file,
    const h5geo::ContainerType& containerType)
{
  h5geo::overwriteEnumAttribute(
        file,
        std::string{h5geo::detail::ContainerType},
        containerType);
  return file;
}

/// \brief H5BaseImpl::createNewObject This method should work only
///  with clean groups that don't have any attributes or content
/// \param group is going to be object group (objG)
/// \param objType
/// \param p
template <typename TBase>
std::optional<h5gt::Group>
H5BaseImpl<TBase>::createNewObject(
    h5gt::Group &group,
    const h5geo::ObjectType& objType,
    void* p)
{
  switch (objType) {
  case h5geo::ObjectType::MAP :
    return createNewMap(group, p);
  case h5geo::ObjectType::WELL :
    return createNewWell(group, p);
  case h5geo::ObjectType::LOGCURVE :
    return createNewLogCurve(group, p);
  case h5geo::ObjectType::DEVCURVE :
    return createNewDevCurve(group, p);
  case h5geo::ObjectType::SEISMIC :
    return createNewSeis(group, p);
  case h5geo::ObjectType::VOLUME :
    return createNewVol(group, p);
  case h5geo::ObjectType::POINTS_1 :
    return createNewPoints(group, p, objType);
  case h5geo::ObjectType::POINTS_2 :
    return createNewPoints(group, p, objType);
  case h5geo::ObjectType::POINTS_3 :
    return createNewPoints(group, p, objType);
  case h5geo::ObjectType::POINTS_4 :
    return createNewPoints(group, p, objType);
  case h5geo::ObjectType::WELLTOPS :
    return createNewWellTops(group, p);
  case h5geo::ObjectType::HORIZON :
    return createNewHorizon(group, p);
  default:
    return std::nullopt;
  }
}

template <typename TBase>
std::optional<h5gt::Group>
H5BaseImpl<TBase>::createNewPoints(h5gt::Group &group, void* p, h5geo::ObjectType pointsType)
{
  H5PointsParam param = *(static_cast<H5PointsParam*>(p));

  // try-catch can't handle this situation
  if (param.nPoints < 1 ||
      param.chunkSize < 1)
    return std::nullopt;

  std::vector<size_t> count = {param.nPoints};
  std::vector<size_t> max_count = {h5gt::DataSpace::UNLIMITED};
  std::vector<hsize_t> cdims = {param.chunkSize};
  h5gt::DataSetCreateProps props;
  props.setChunk(cdims);
  h5gt::DataSpace dataspace(count, max_count);

  try {

    group.createAttribute<double>(
          std::string{h5geo::detail::null_value},
          h5gt::DataSpace(1)).
        write(param.nullValue);
    group.createAttribute<h5geo::Domain>(
          std::string{h5geo::detail::Domain},
          h5gt::DataSpace(1)).
        write(param.domain);
    group.createAttribute<std::string>(
          std::string{h5geo::detail::spatial_reference},
          h5gt::DataSpace::From(param.spatialReference)).
        write(param.spatialReference);
    group.createAttribute<std::string>(
          std::string{h5geo::detail::length_units},
          h5gt::DataSpace::From(param.lengthUnits)).
        write(param.lengthUnits);
    group.createAttribute<std::string>(
          std::string{h5geo::detail::temporal_units},
          h5gt::DataSpace::From(param.temporalUnits)).
        write(param.temporalUnits);
    group.createAttribute<std::string>(
          std::string{h5geo::detail::data_units},
          h5gt::DataSpace::From(param.dataUnits)).
        write(param.dataUnits);

    switch (pointsType) {
    case h5geo::ObjectType::POINTS_1 :
      group.createDataSet(
            std::string{h5geo::detail::points_data},
            dataspace, h5geo::create_compound_Point1(), h5gt::LinkCreateProps(), props);
      break;
    case h5geo::ObjectType::POINTS_2 :
      group.createDataSet(
            std::string{h5geo::detail::points_data},
            dataspace, h5geo::create_compound_Point2(), h5gt::LinkCreateProps(), props);
      break;
    case h5geo::ObjectType::POINTS_3 :
      group.createDataSet(
            std::string{h5geo::detail::points_data},
            dataspace, h5geo::create_compound_Point3(), h5gt::LinkCreateProps(), props);
      break;
    case h5geo::ObjectType::POINTS_4 :
      group.createDataSet(
            std::string{h5geo::detail::points_data},
            dataspace, h5geo::create_compound_Point4(), h5gt::LinkCreateProps(), props);
      break;
    default:
      return std::nullopt;
    }

    return group;

  } catch (h5gt::Exception& err) {
    return std::nullopt;
  }


}

template <typename TBase>
std::optional<h5gt::Group>
H5BaseImpl<TBase>::createNewWellTops(h5gt::Group &group, void* p)
{
  return createNewPoints(group, p, h5geo::ObjectType::POINTS_1);
}

template <typename TBase>
std::optional<h5gt::Group>
H5BaseImpl<TBase>::createNewHorizon(h5gt::Group &group, void* p)
{
  H5HorizonParam param = *(static_cast<H5HorizonParam*>(p));

  // try-catch can't handle this situation
  if (param.nPoints < 1 ||
      param.pointsChunkSize < 1 ||
      param.components.size() < 1)
    return std::nullopt;

  std::vector<size_t> count = {param.components.size(), param.nPoints};
  std::vector<size_t> max_count = {h5gt::DataSpace::UNLIMITED, h5gt::DataSpace::UNLIMITED};
  std::vector<hsize_t> cdims = {param.components.size(), param.pointsChunkSize};
  h5gt::DataSetCreateProps props;
  props.setChunk(cdims);
  h5gt::DataSpace dataspace(count, max_count);


  try {

    group.createAttribute<double>(
          std::string{h5geo::detail::null_value},
          h5gt::DataSpace(1)).
        write(param.nullValue);
    group.createAttribute<h5geo::Domain>(
          std::string{h5geo::detail::Domain},
          h5gt::DataSpace(1)).
        write(param.domain);
    group.createAttribute<std::string>(
          std::string{h5geo::detail::spatial_reference},
          h5gt::DataSpace::From(param.spatialReference)).
        write(param.spatialReference);
    group.createAttribute<std::string>(
          std::string{h5geo::detail::length_units},
          h5gt::DataSpace::From(param.lengthUnits)).
        write(param.lengthUnits);
    group.createAttribute<std::string>(
          std::string{h5geo::detail::temporal_units},
          h5gt::DataSpace::From(param.temporalUnits)).
        write(param.temporalUnits);
    group.createAttribute<std::string>(
          std::string{h5geo::detail::data_units},
          h5gt::DataSpace::From(param.dataUnits)).
        write(param.dataUnits);

    h5gt::DataSet dataset =
        group.createDataSet<double>(
          std::string{h5geo::detail::horizon_data},
          dataspace, h5gt::LinkCreateProps(), props);
    for (auto const& [key, val] : param.components){
      dataset.createAttribute<size_t>(key, h5gt::DataSpace(1)).write(val);
    }
    return group;

  } catch (h5gt::Exception& err) {
    return std::nullopt;
  }


}

template <typename TBase>
std::optional<h5gt::Group>
H5BaseImpl<TBase>::createNewMap(h5gt::Group &group, void* p)
{
  H5MapParam param = *(static_cast<H5MapParam*>(p));

  // try-catch can't handle this situation
  if (param.xChunkSize < 1 ||
      param.yChunkSize < 1 ||
      param.nX < 1 ||
      param.nY < 1)
    return std::nullopt;

  std::vector<size_t> count = {param.nY, param.nX};
  std::vector<size_t> max_count = {h5gt::DataSpace::UNLIMITED, h5gt::DataSpace::UNLIMITED};
  std::vector<hsize_t> cdims = {param.yChunkSize, param.xChunkSize};
  h5gt::DataSetCreateProps props;
  props.setChunk(cdims);
  h5gt::DataSpace dataspace(count, max_count);

  std::vector<double> origin({param.X0, param.Y0});
  std::vector<double> point1({param.X1, param.Y1});
  std::vector<double> point2({param.X2, param.Y2});

  try {

    group.createAttribute<double>(
          std::string{h5geo::detail::null_value},
          h5gt::DataSpace(1)).
        write(param.nullValue);
    group.createAttribute<h5geo::Domain>(
          std::string{h5geo::detail::Domain},
          h5gt::DataSpace(1)).
        write(param.domain);
    group.createAttribute<std::string>(
          std::string{h5geo::detail::spatial_reference},
          h5gt::DataSpace::From(param.spatialReference)).
        write(param.spatialReference);
    group.createAttribute<std::string>(
          std::string{h5geo::detail::length_units},
          h5gt::DataSpace::From(param.lengthUnits)).
        write(param.lengthUnits);
    group.createAttribute<std::string>(
          std::string{h5geo::detail::temporal_units},
          h5gt::DataSpace::From(param.temporalUnits)).
        write(param.temporalUnits);
    group.createAttribute<std::string>(
          std::string{h5geo::detail::data_units},
          h5gt::DataSpace::From(param.dataUnits)).
        write(param.dataUnits);
    group.createAttribute<double>(
          std::string{h5geo::detail::origin},
          h5gt::DataSpace({2})).
        write(origin);
    group.createAttribute<double>(
          std::string{h5geo::detail::point1},
          h5gt::DataSpace({2})).
        write(point1);
    group.createAttribute<double>(
          std::string{h5geo::detail::point2},
          h5gt::DataSpace({2})).
        write(point2);

    group.createDataSet<double>(
          std::string{h5geo::detail::map_data},
          dataspace, h5gt::LinkCreateProps(), props);

    return group;

  } catch (h5gt::Exception& err) {
    return std::nullopt;
  }
}

template <typename TBase>
std::optional<h5gt::Group>
H5BaseImpl<TBase>::createNewVol(h5gt::Group &group, void* p)
{
  H5VolParam param = *(static_cast<H5VolParam*>(p));

  // try-catch can't handle this situation
  if (param.xChunkSize < 1 ||
      param.yChunkSize < 1 ||
      param.zChunkSize < 1 ||
      param.nX < 1 ||
      param.nY < 1 ||
      param.nZ < 1)
    return std::nullopt;

  std::vector<size_t> count = {param.nZ, param.nY, param.nX};
  std::vector<size_t> max_count = {h5gt::DataSpace::UNLIMITED, h5gt::DataSpace::UNLIMITED, h5gt::DataSpace::UNLIMITED};
  std::vector<hsize_t> cdims = {param.xChunkSize, param.yChunkSize, param.zChunkSize};
  h5gt::DataSetCreateProps props;
  props.setChunk(cdims);
  props.setDeflate(param.compression_level);
  h5gt::DataSpace dataspace(count, max_count);

  std::vector<double> origin({param.X0, param.Y0, param.Z0});
  std::vector<double> spacings({param.dX, param.dY, param.dZ});

  try {

    group.createAttribute<double>(
          std::string{h5geo::detail::null_value},
          h5gt::DataSpace(1)).
        write(param.nullValue);
    group.createAttribute<h5geo::Domain>(
          std::string{h5geo::detail::Domain},
          h5gt::DataSpace(1)).
        write(param.domain);
    group.createAttribute<std::string>(
          std::string{h5geo::detail::spatial_reference},
          h5gt::DataSpace::From(param.spatialReference)).
        write(param.spatialReference);
    group.createAttribute<std::string>(
          std::string{h5geo::detail::length_units},
          h5gt::DataSpace::From(param.lengthUnits)).
        write(param.lengthUnits);
    group.createAttribute<std::string>(
          std::string{h5geo::detail::temporal_units},
          h5gt::DataSpace::From(param.temporalUnits)).
        write(param.temporalUnits);
    group.createAttribute<std::string>(
          std::string{h5geo::detail::data_units},
          h5gt::DataSpace::From(param.dataUnits)).
        write(param.dataUnits);
    group.createAttribute<double>(
          std::string{h5geo::detail::origin},
          h5gt::DataSpace({3})).
        write(origin);
    group.createAttribute<double>(
          std::string{h5geo::detail::spacings},
          h5gt::DataSpace({3})).
        write(spacings);
    group.createAttribute<double>(
          std::string{h5geo::detail::orientation},
          h5gt::DataSpace(1)).
        write(param.orientation);

    group.createDataSet<float>(
          std::string{h5geo::detail::vol_data},
          dataspace, h5gt::LinkCreateProps(), props);

    return group;

  } catch (h5gt::Exception& err) {
    return std::nullopt;
  }
}

template <typename TBase>
std::optional<h5gt::Group>
H5BaseImpl<TBase>::createNewWell(h5gt::Group &group, void* p)
{
  H5WellParam param = *(static_cast<H5WellParam *>(p));
  std::vector<double> head_coord({param.headX, param.headY});

  try {

    group.createAttribute<double>(
          std::string{h5geo::detail::null_value},
          h5gt::DataSpace(1)).
        write(param.nullValue);
    group.createAttribute<std::string>(
          std::string{h5geo::detail::spatial_reference},
          h5gt::DataSpace::From(param.spatialReference)).
        write(param.spatialReference);
    group.createAttribute<double>(
          std::string{h5geo::detail::head_coord},
          h5gt::DataSpace({2})).
        write(head_coord);
    group.createAttribute<double>(
          std::string{h5geo::detail::KB},
          h5gt::DataSpace(1)).
        write(param.kb);
    group.createAttribute<std::string>(
          std::string{h5geo::detail::UWI},
          h5gt::DataSpace(1)).
        write(param.uwi);
    group.createAttribute<std::string>(
          std::string{h5geo::detail::length_units},
          h5gt::DataSpace::From(param.lengthUnits)).
        write(param.lengthUnits);

    constexpr auto& group_names =
        magic_enum::enum_names<h5geo::detail::WellGroups>();

    for (const auto& name : group_names){
      group.createGroup(std::string{name});
    }

    return group;

  } catch (h5gt::Exception& err) {
    return std::nullopt;
  }
}

template <typename TBase>
std::optional<h5gt::Group>
H5BaseImpl<TBase>::createNewLogCurve(h5gt::Group &group, void* p)
{
  H5LogCurveParam param = *(static_cast<H5LogCurveParam*>(p));

  // try-catch can't handle this situation
  if (param.chunkSize < 1)
    return std::nullopt;

  std::vector<size_t> count = {2, 1};
  std::vector<size_t> max_count = {2, h5gt::DataSpace::UNLIMITED};
  std::vector<hsize_t> cdims = {2, param.chunkSize};
  h5gt::DataSetCreateProps props;
  props.setChunk(cdims);
  h5gt::DataSpace dataspace(count, max_count);

  try {

    group.createAttribute<double>(
          std::string{h5geo::detail::null_value},
          h5gt::DataSpace(1)).
        write(param.nullValue);
    group.createAttribute<std::string>(
          std::string{h5geo::detail::spatial_reference},
          h5gt::DataSpace::From(param.spatialReference)).
        write(param.spatialReference);
    group.createAttribute<std::string>(
          std::string{h5geo::detail::length_units},
          h5gt::DataSpace::From(param.lengthUnits)).
        write(param.lengthUnits);
    group.createAttribute<std::string>(
          std::string{h5geo::detail::data_units},
          h5gt::DataSpace::From(param.dataUnits)).
        write(param.dataUnits);

    h5gt::DataSet dataset =
        group.createDataSet<double>(
          std::string{h5geo::detail::log_data},
          dataspace, h5gt::LinkCreateProps(), props);
    dataset.createAttribute<size_t>(
          std::string{magic_enum::enum_name(h5geo::LogDataType::MD)}, h5gt::DataSpace(1)).write(0);
    dataset.createAttribute<size_t>(
          std::string{magic_enum::enum_name(h5geo::LogDataType::VAL)}, h5gt::DataSpace(1)).write(1);

    return group;

  } catch (h5gt::Exception& err) {
    return std::nullopt;
  }
}

template <typename TBase>
std::optional<h5gt::Group>
H5BaseImpl<TBase>::createNewDevCurve(h5gt::Group &group, void* p)
{
  H5DevCurveParam param = *(static_cast<H5DevCurveParam*>(p));

  // try-catch can't handle this situation
  if (param.chunkSize < 1)
    return std::nullopt;

  std::vector<size_t> count = {7, 1};
  std::vector<size_t> max_count = {7, h5gt::DataSpace::UNLIMITED};
  std::vector<hsize_t> cdims = {7, param.chunkSize};
  h5gt::DataSetCreateProps props;
  props.setChunk(cdims);
  h5gt::DataSpace dataspace(count, max_count);

  try {

    group.createAttribute<double>(
          std::string{h5geo::detail::null_value},
          h5gt::DataSpace(1)).
        write(param.nullValue);
    group.createAttribute<std::string>(
          std::string{h5geo::detail::spatial_reference},
          h5gt::DataSpace::From(param.spatialReference)).
        write(param.spatialReference);
    group.createAttribute<std::string>(
          std::string{h5geo::detail::length_units},
          h5gt::DataSpace::From(param.lengthUnits)).
        write(param.lengthUnits);
    group.createAttribute<std::string>(
          std::string{h5geo::detail::temporal_units},
          h5gt::DataSpace::From(param.temporalUnits)).
        write(param.temporalUnits);
    group.createAttribute<std::string>(
          std::string{h5geo::detail::angular_units},
          h5gt::DataSpace::From(param.angularUnits)).
        write(param.angularUnits);

    h5gt::DataSet dataset =
        group.createDataSet<double>(
          std::string{h5geo::detail::dev_data},
          dataspace, h5gt::LinkCreateProps(), props);
    dataset.createAttribute<size_t>(
          std::string{h5geo::MD},
          h5gt::DataSpace(1)).write(0);
    dataset.createAttribute<size_t>(
          std::string{h5geo::AZIM},
          h5gt::DataSpace(1)).write(1);
    dataset.createAttribute<size_t>(
          std::string{h5geo::INCL},
          h5gt::DataSpace(1)).write(2);
    dataset.createAttribute<size_t>(
          std::string{h5geo::TVD},
          h5gt::DataSpace(1)).write(3);
    dataset.createAttribute<size_t>(
          std::string{h5geo::DX},
          h5gt::DataSpace(1)).write(4);
    dataset.createAttribute<size_t>(
          std::string{h5geo::DY},
          h5gt::DataSpace(1)).write(5);
    dataset.createAttribute<size_t>(
          std::string{h5geo::OWT},
          h5gt::DataSpace(1)).write(6);

    return group;

  } catch (h5gt::Exception& err) {
    return std::nullopt;
  }
}

template <typename TBase>
std::optional<h5gt::Group>
H5BaseImpl<TBase>::createNewSeis(h5gt::Group &group, void* p)
{
  H5SeisParam param = *(static_cast<H5SeisParam*>(p));
  // try-catch can't handle this situation
  if (param.trcChunk < 1 ||
      param.stdChunk < 1)
    return std::nullopt;

  if (param.mapSEGY){
    if (param.segyFiles.size() < 1)
      return std::nullopt;

    h5geo::Endian endian = h5geo::getSEGYEndian(param.segyFiles[0]);
    if (std::string{magic_enum::enum_name(endian)}.empty())
      return std::nullopt;

    param.nSamp = h5geo::getSEGYNSamp(param.segyFiles[0], endian);
    param.nTrc = h5geo::getSEGYNTrc(param.segyFiles[0], param.nSamp, endian);

    if (param.nSamp < 1 || param.nTrc < 1)
      return std::nullopt;

    h5geo::SegyFormat format = h5geo::getSEGYFormat(param.segyFiles[0], endian);
    if (format != h5geo::SegyFormat::FourByte_IEEE)
      return std::nullopt;

    for (size_t i = 1; i < param.segyFiles.size(); i++){
      auto e = h5geo::getSEGYEndian(param.segyFiles[i]);
      auto n = h5geo::getSEGYNTrc(param.segyFiles[i], 0, endian);
      auto f = h5geo::getSEGYFormat(param.segyFiles[0], endian);
      if (endian != h5geo::getSEGYEndian(param.segyFiles[i]) ||
          param.nSamp != h5geo::getSEGYNSamp(param.segyFiles[i], endian) ||
          h5geo::getSEGYFormat(param.segyFiles[i], endian) != h5geo::SegyFormat::FourByte_IEEE)
        return std::nullopt;

      param.nTrc += h5geo::getSEGYNTrc(param.segyFiles[i], 0, endian);
    }

    auto optG = createExternalSEGY(
          group,
          param.nTrc,
          param.nSamp,
          param.trcChunk,
          param.stdChunk,
          param.segyFiles,
          endian);
    if (!optG.has_value())
      return std::nullopt;
  }

  // try-catch can't handle this situation
  if (param.nTrc < 1 ||
      param.nSamp < 1)
    return std::nullopt;

  try {

    group.createAttribute<double>(
          std::string{h5geo::detail::null_value},
          h5gt::DataSpace(1)).
        write(param.nullValue);
    group.createAttribute<h5geo::Domain>(
          std::string{h5geo::detail::Domain},
          h5gt::DataSpace(1)).
        write(param.domain);
    group.createAttribute<h5geo::SeisDataType>(
          std::string{h5geo::detail::SeisDataType},
          h5gt::DataSpace(1)).
        write(param.dataType);
    group.createAttribute<h5geo::SurveyType>(
          std::string{h5geo::detail::SurveyType},
          h5gt::DataSpace(1)).
        write(param.surveyType);
    group.createAttribute<double>(
          std::string{h5geo::detail::SRD},
          h5gt::DataSpace(1)).
        write(param.srd);
    group.createAttribute<std::string>(
          std::string{h5geo::detail::spatial_reference},
          h5gt::DataSpace::From(param.spatialReference)).
        write(param.spatialReference);
    group.createAttribute<std::string>(
          std::string{h5geo::detail::length_units},
          h5gt::DataSpace::From(param.lengthUnits)).
        write(param.lengthUnits);
    group.createAttribute<std::string>(
          std::string{h5geo::detail::temporal_units},
          h5gt::DataSpace::From(param.temporalUnits)).
        write(param.temporalUnits);
    group.createAttribute<std::string>(
          std::string{h5geo::detail::data_units},
          h5gt::DataSpace::From(param.dataUnits)).
        write(param.dataUnits);

    createTextHeader(group, param.mapSEGY);
    createBinHeader(group, 10, param.mapSEGY); // stdChunk may be too big for bin header
    createTrace(group, param.nTrc, param.nSamp, param.trcChunk, param.mapSEGY);
    createTraceHeader(group, param.nTrc, param.trcChunk, param.mapSEGY);
    createSort(group);

    return group;

  } catch (h5gt::Exception& err) {
    return std::nullopt;
  }
}

template <typename TBase>
std::optional<h5gt::Group>
H5BaseImpl<TBase>::createExternalSEGY(
    h5gt::Group &seisGroup,
    const size_t& nTrc,
    const size_t& nSamp,
    const hsize_t& trcChunk,
    const hsize_t& stdChunk,
    const std::vector<std::string>& segyFiles,
    h5geo::Endian endian)
{
  if (segyFiles.size() < 1)
    return std::nullopt;

  // if SEGY endian differs from NATIVE we should take into account that or
  // mapped values will have swapped bytes
  h5gt::Endian endianNum;
  switch (endian) {
  case h5geo::Endian::Little :
    endianNum = h5gt::Endian::Little;
    break;
  case h5geo::Endian::Big :
    endianNum = h5gt::Endian::Big;
    break;
  default:
    endianNum = h5gt::Endian::Native;
  }

  h5gt::DataSetCreateProps txtP, binHdrP, dataP;
  std::vector<std::string> fullHeaderNames, shortHeaderNames;
  h5geo::getBinHeaderNames(fullHeaderNames, shortHeaderNames);
  size_t nBinHeaderNames = fullHeaderNames.size();

  std::vector<size_t> count;
  std::vector<size_t> max_count;
  std::vector<hsize_t> cdims;

  try {
    txtP.addExternalFile(segyFiles[0], 0, 3200);
    binHdrP.addExternalFile(segyFiles[0], 3200, 400);
    for (const auto& segy : segyFiles){
      dataP.addExternalFile(segy, 3600);
    }

    h5gt::Group segyG = seisGroup.createGroup(std::string{h5geo::detail::segy});

    // text header
    char txtHdr[40][80];
    segyG.createDataSet<char[80]>(
          std::string{h5geo::detail::text_header},
          h5gt::DataSpace::FromCharArrayStrings(txtHdr),
          h5gt::LinkCreateProps(), txtP);

    // bin header
    count = {20};
    cdims = {stdChunk};
    h5gt::AtomicType<short> shortType(endianNum);
    segyG.createDataSet(
          std::string{h5geo::detail::bin_header_2bytes},
          h5gt::DataSpace(count),
          shortType,
          h5gt::LinkCreateProps(), binHdrP);
    count = {10};
    h5gt::AtomicType<int> intType(endianNum);
    segyG.createDataSet(
          std::string{h5geo::detail::bin_header_4bytes},
          h5gt::DataSpace(count),
          intType,
          h5gt::LinkCreateProps(), binHdrP);

    // trace header
    count = {nTrc, nSamp*2+120};
    segyG.createDataSet(
          std::string{h5geo::detail::trace_header_2bytes},
          h5gt::DataSpace(count),
          shortType,
          h5gt::LinkCreateProps(), dataP);
    count = {nTrc, nSamp+60};
    segyG.createDataSet(
          std::string{h5geo::detail::trace_header_4bytes},
          h5gt::DataSpace(count),
          intType,
          h5gt::LinkCreateProps(), dataP);

    // trace
    count = {nTrc, nSamp+60};
    h5gt::AtomicType<float> floatType(endianNum);
    segyG.createDataSet(
          std::string{h5geo::detail::trace_float},
          h5gt::DataSpace(count),
          floatType,
          h5gt::LinkCreateProps(), dataP);
    return segyG;
  } catch (h5gt::Exception& err) {
    return std::nullopt;
  }
}

template <typename TBase>
std::optional<h5gt::DataSet>
H5BaseImpl<TBase>::createTextHeader(
    h5gt::Group &seisGroup,
    bool mapSEGY)
{
  char txtHdr[40][80];
  h5gt::DataSetCreateProps props;

  try {

    if (mapSEGY){
      auto space = h5gt::DataSpace::FromCharArrayStrings(txtHdr);
      h5gt::Selection vSel(space);
      auto segyG = seisGroup.getGroup(std::string{h5geo::detail::segy});
      auto srcDset = segyG.getDataSet(std::string{h5geo::detail::text_header});
      h5gt::Selection srcSel(srcDset.getSpace());
      props.addVirtualDataSet(vSel.getSpace(), srcDset, srcSel.getSpace());
    }

    return seisGroup.createDataSet<char[80]>(
        std::string{h5geo::detail::text_header},
        h5gt::DataSpace::FromCharArrayStrings(txtHdr),
            h5gt::LinkCreateProps(), props);

  } catch (h5gt::Exception& err) {
    return std::nullopt;
  }
}

template <typename TBase>
std::optional<h5gt::DataSet>
H5BaseImpl<TBase>::createBinHeader(
    h5gt::Group &seisGroup,
    const hsize_t& stdChunk,
    bool mapSEGY)
{
  std::vector<std::string> fullHeaderNames, shortHeaderNames;
  h5geo::getBinHeaderNames(fullHeaderNames, shortHeaderNames);
  size_t nBinHeaderNames = fullHeaderNames.size();

  std::vector<size_t> count = {size_t(nBinHeaderNames)};
  std::vector<size_t> max_count = {h5gt::DataSpace::UNLIMITED};
  std::vector<hsize_t> cdims = {stdChunk};

  try {

    h5gt::DataSpace space(count, max_count);
    h5gt::DataSetCreateProps props;
    props.setChunk(cdims);
    if (mapSEGY){
      h5gt::Selection vSel(space);
      auto segyG = seisGroup.getGroup(std::string{h5geo::detail::segy});
      auto srcDset2b = segyG.getDataSet(std::string{h5geo::detail::bin_header_2bytes});
      auto srcDset4b = segyG.getDataSet(std::string{h5geo::detail::bin_header_4bytes});

      h5gt::Selection vSel4b(space);
      vSel4b = vSel4b.select({0},{3});
      auto srcSel4b = srcDset4b.select({0},{3});
      props.addVirtualDataSet(vSel4b.getSpace(), srcDset4b, srcSel4b.getSpace());

      h5gt::Selection vSel2b(space);
      vSel2b = vSel2b.select({3},{count[0]-3});
      auto srcSel2b = srcDset2b.select({6},{count[0]-3});
      props.addVirtualDataSet(vSel2b.getSpace(), srcDset2b, srcSel2b.getSpace());
    }

    h5gt::DataSet dataset = seisGroup.createDataSet<double>(
          std::string{h5geo::detail::bin_header},
          space, h5gt::LinkCreateProps(), props);
    for (size_t i = 0; i < nBinHeaderNames; i++){
      h5gt::Attribute attribute = dataset.createAttribute<size_t>(
            shortHeaderNames[i], h5gt::DataSpace(1));
      attribute.write(i);
    }
    return dataset;

  } catch (h5gt::Exception& err) {
    return std::nullopt;
  }
}

template <typename TBase>
std::optional<h5gt::DataSet>
H5BaseImpl<TBase>::createTrace(
    h5gt::Group &seisGroup,
    const size_t& nTrc,
    const size_t& nSamp,
    const hsize_t& trcChunk,
    bool mapSEGY)
{
  std::vector<size_t> count = {nTrc, nSamp};
  std::vector<size_t> max_count = {
    h5gt::DataSpace::UNLIMITED, h5gt::DataSpace::UNLIMITED};
  std::vector<hsize_t> cdims = {trcChunk, nSamp};

  try {

    h5gt::DataSetCreateProps props;
    props.setChunk(cdims);
    h5gt::DataSpace space(count, max_count);
    if (mapSEGY){
      h5gt::Selection vSel(space);
      auto segyG = seisGroup.getGroup(std::string{h5geo::detail::segy});
      auto srcDset4b = segyG.getDataSet(std::string{h5geo::detail::trace_float});

      h5gt::Selection vSel4b(space);
      auto srcSel4b = srcDset4b.select({0,60},{nTrc,nSamp});
      props.addVirtualDataSet(vSel4b.getSpace(), srcDset4b, srcSel4b.getSpace());
    }
    h5gt::DataSet dataset = seisGroup.createDataSet<float>(
          std::string{h5geo::detail::trace},
          space, h5gt::LinkCreateProps(), props);
    return dataset;

  } catch (h5gt::Exception& err) {
    return std::nullopt;
  }
}

template <typename TBase>
std::optional<h5gt::DataSet>
H5BaseImpl<TBase>::createTraceHeader(
    h5gt::Group &seisGroup,
    const size_t& nTrc,
    const hsize_t& trcChunk,
    bool mapSEGY)
{
  std::vector<std::string> fullHeaderNames, shortHeaderNames;
  h5geo::getTraceHeaderNames(fullHeaderNames, shortHeaderNames);
  size_t nTraceHeaderNames = fullHeaderNames.size();

  std::vector<size_t> count = {size_t(nTraceHeaderNames), nTrc};
  std::vector<size_t> max_count = {
    h5gt::DataSpace::UNLIMITED, h5gt::DataSpace::UNLIMITED};
  std::vector<hsize_t> cdims = {size_t(nTraceHeaderNames), trcChunk};

  try {

    h5gt::DataSetCreateProps props;
    props.setChunk(cdims);
    h5gt::DataSpace space(count, max_count);
    if (mapSEGY){
      auto segyG = seisGroup.getGroup(std::string{h5geo::detail::segy});
      auto srcDset2b = segyG.getDataSet(std::string{h5geo::detail::trace_header_2bytes});
      auto srcDset4b = segyG.getDataSet(std::string{h5geo::detail::trace_header_4bytes});

      std::vector<int> bytesStart, nBytes;
      h5geo::getTraceHeaderBytes(bytesStart, nBytes);

      for (size_t i = 0; i < bytesStart.size(); i++){
        h5gt::Selection vSel(space);
        vSel = vSel.select({i,0},{1,nTrc}); // SEGY is mapped as {nTrc,nSamp} while Seis Header is in reverse order {nSamp,nTrc}
        if (nBytes[i] == 4){
          auto srcSel4b = srcDset4b.select({0,size_t(bytesStart[i]/4)},{nTrc,1});
          props.addVirtualDataSet(vSel.getSpace(), srcDset4b, srcSel4b.getSpace());
        } else if (nBytes[i] == 2){
          auto srcSel2b = srcDset2b.select({0,size_t(bytesStart[i]/2)},{nTrc,1});
          props.addVirtualDataSet(vSel.getSpace(), srcDset2b, srcSel2b.getSpace());
        }
      }
    }

    h5gt::DataSet dataset = seisGroup.createDataSet<double>(
          std::string{h5geo::detail::trace_header},
          space, h5gt::LinkCreateProps(), props);
    for (size_t i = 0; i < nTraceHeaderNames; i++){
      h5gt::Attribute attribute = dataset.createAttribute<size_t>(
            shortHeaderNames[i], h5gt::DataSpace(1));
      attribute.write(i);
    }
    return dataset;

  } catch (h5gt::Exception& err) {
    return std::nullopt;
  }
}

template <typename TBase>
std::optional<h5gt::Group>
H5BaseImpl<TBase>::createSort(
    h5gt::Group &seisGroup)
{
  try {

    h5gt::Group sortGroup = seisGroup.createGroup(
          std::string{h5geo::detail::sort});
    h5gt::Group indexesGroup = sortGroup.createGroup(
          std::string{h5geo::detail::indexes});
    h5gt::Group uValGroup = sortGroup.createGroup(
          std::string{h5geo::detail::unique_values});
    return sortGroup;

  } catch (h5gt::Exception& err) {
    return std::nullopt;
  }
}

template <typename TBase>
bool H5BaseImpl<TBase>::isSuccessor(const h5gt::Group &parentG, const h5gt::Group &childG)
{
  return isSuccessor(parentG.getPath(), childG.getPath());
}

template <typename TBase>
bool H5BaseImpl<TBase>::isSuccessor(
    const std::string& parentAbsPath, const std::string& childAbsPath)
{
  if (parentAbsPath.empty() || childAbsPath.empty())
    return false;

  if (parentAbsPath[0] != '/' || childAbsPath[0] != '/')
    return false;

  if (childAbsPath.substr(0, parentAbsPath.size()).find(parentAbsPath) == std::string::npos) {
    return false;
  }

  return true;
}

/*---------------------H5GEO---------------------*/
bool h5geo::isGeoContainer(h5gt::File file){
  constexpr auto& cntTypes = magic_enum::enum_values<h5geo::ContainerType>();
  for (const auto &cntType : cntTypes)
    if (h5geo::isGeoContainerByType(file, cntType))
      return true;

  return false;
}

bool h5geo::isGeoContainerByType(h5gt::File file,
                                 const h5geo::ContainerType& cntType)
{
  h5geo::ContainerType val = h5geo::readEnumAttribute<h5gt::File, h5geo::ContainerType>(
        file, std::string{h5geo::detail::ContainerType});
  switch (cntType) {
  case h5geo::ContainerType::MAP:
    return val == h5geo::ContainerType::MAP;
  case h5geo::ContainerType::WELL:
    return val == h5geo::ContainerType::WELL;
  case h5geo::ContainerType::SEISMIC:
    return val == h5geo::ContainerType::SEISMIC;
  case h5geo::ContainerType::VOLUME:
    return val == h5geo::ContainerType::VOLUME;
  default:
    return false;
  }
}

h5geo::ContainerType h5geo::getGeoContainerType(h5gt::File file)
{
  return h5geo::readEnumAttribute<h5gt::File, h5geo::ContainerType>(
        file, std::string{h5geo::detail::ContainerType});
}

bool h5geo::isGeoObject(const h5gt::Group& group){
  constexpr auto& objTypes = magic_enum::enum_values<h5geo::ObjectType>();
  for (const auto &objType : objTypes)
    if (h5geo::isGeoObjectByType(group, objType))
      return true;

  return false;
}

bool h5geo::isGeoObjectByType(const h5gt::Group& group,
                              const h5geo::ObjectType& objType)
{
  switch (objType) {
  case h5geo::ObjectType::MAP :
    return h5geo::isMap(group);
  case h5geo::ObjectType::WELL :
    return h5geo::isWell(group);
  case h5geo::ObjectType::LOGCURVE :
    return h5geo::isLogCurve(group);
  case h5geo::ObjectType::DEVCURVE :
    return h5geo::isDevCurve(group);
  case h5geo::ObjectType::SEISMIC :
    return h5geo::isSeis(group);
  case h5geo::ObjectType::VOLUME :
    return h5geo::isVol(group);
  case h5geo::ObjectType::POINTS_1 :
    return h5geo::isPoints1(group);
  case h5geo::ObjectType::POINTS_2 :
    return h5geo::isPoints2(group);
  case h5geo::ObjectType::POINTS_3 :
    return h5geo::isPoints3(group);
  case h5geo::ObjectType::POINTS_4 :
    return h5geo::isPoints4(group);
  case h5geo::ObjectType::WELLTOPS :
    return h5geo::isWellTops(group);
  case h5geo::ObjectType::HORIZON :
    return h5geo::isHorizon(group);
  default:
    return false;
  }
}

h5geo::ObjectType h5geo::getGeoObjectType(
    const h5gt::Group& group)
{
  // welltops must go before points as in hdf5 welltops == points1
  if (h5geo::isWellTops(group)){
    return h5geo::ObjectType::WELLTOPS;
  } else if (h5geo::isPoints1(group)){
    return h5geo::ObjectType::POINTS_1;
  } else if (h5geo::isPoints2(group)){
    return h5geo::ObjectType::POINTS_2;
  } else if (h5geo::isPoints3(group)){
    return h5geo::ObjectType::POINTS_3;
  } else if (h5geo::isPoints4(group)){
    return h5geo::ObjectType::POINTS_4;
  } else if (h5geo::isHorizon(group)){
    return h5geo::ObjectType::HORIZON;
  } else if (h5geo::isMap(group)){
    return h5geo::ObjectType::MAP;
  } else if (h5geo::isWell(group)){
    return h5geo::ObjectType::WELL;
  } else if (h5geo::isDevCurve(group)){
    return h5geo::ObjectType::DEVCURVE;
  } else if (h5geo::isLogCurve(group)){
    return h5geo::ObjectType::LOGCURVE;
  } else if (h5geo::isSeis(group)){
    return h5geo::ObjectType::SEISMIC;
  } else if (h5geo::isVol(group)){
    return h5geo::ObjectType::VOLUME;
  }
  return static_cast<h5geo::ObjectType>(0);
}

bool h5geo::isPoints(
    const h5gt::Group &group)
{
  if (isPoints1(group))
    return true;
  if (isPoints2(group))
    return true;
  if (isPoints3(group))
    return true;
  if (isPoints4(group))
    return true;

  return false;
}

bool h5geo::isPoints1(
    const h5gt::Group &group)
{
  for (const auto& name : h5geo::detail::points_attrs){
    if (!group.hasAttribute(std::string{name}))
      return false;
  }

  for (const auto& name : h5geo::detail::points_dsets){
    if (!group.hasObject(std::string{name}, h5gt::ObjectType::Dataset))
      return false;

    h5gt::DataSet dset = group.getDataSet(std::string{name});
    auto dtype = dset.getDataType();
    if (!dtype.isTypeEqual(h5geo::create_compound_Point1())){
      return false;
    }
  }
  return true;
}

bool h5geo::isPoints2(
    const h5gt::Group &group)
{
  for (const auto& name : h5geo::detail::points_attrs){
    if (!group.hasAttribute(std::string{name}))
      return false;
  }

  for (const auto& name : h5geo::detail::points_dsets){
    if (!group.hasObject(std::string{name}, h5gt::ObjectType::Dataset))
      return false;

    h5gt::DataSet dset = group.getDataSet(std::string{name});
    auto dtype = dset.getDataType();
    if (!dtype.isTypeEqual(h5geo::create_compound_Point2())){
      return false;
    }
  }
  return true;
}

bool h5geo::isPoints3(
    const h5gt::Group &group)
{
  for (const auto& name : h5geo::detail::points_attrs){
    if (!group.hasAttribute(std::string{name}))
      return false;
  }

  for (const auto& name : h5geo::detail::points_dsets){
    if (!group.hasObject(std::string{name}, h5gt::ObjectType::Dataset))
      return false;

    h5gt::DataSet dset = group.getDataSet(std::string{name});
    auto dtype = dset.getDataType();
    if (!dtype.isTypeEqual(h5geo::create_compound_Point3())){
      return false;
    }
  }
  return true;
}

bool h5geo::isPoints4(
    const h5gt::Group &group)
{
  for (const auto& name : h5geo::detail::points_attrs){
    if (!group.hasAttribute(std::string{name}))
      return false;
  }

  for (const auto& name : h5geo::detail::points_dsets){
    if (!group.hasObject(std::string{name}, h5gt::ObjectType::Dataset))
      return false;

    h5gt::DataSet dset = group.getDataSet(std::string{name});
    auto dtype = dset.getDataType();
    if (!dtype.isTypeEqual(h5geo::create_compound_Point4())){
      return false;
    }
  }
  return true;
}

bool h5geo::isWellTops(const h5gt::Group &group)
{
  return h5geo::isPoints1(group);
}

bool h5geo::isHorizon(
    const h5gt::Group &group)
{
  for (const auto& name : h5geo::detail::horizon_attrs){
    if (!group.hasAttribute(std::string{name}))
      return false;
  }

  for (const auto& name : h5geo::detail::horizon_dsets){
    if (!group.hasObject(std::string{name}, h5gt::ObjectType::Dataset))
      return false;
  }
  return true;
}

bool h5geo::isMap(
    const h5gt::Group &group)
{
  for (const auto& name : h5geo::detail::map_attrs){
    if (!group.hasAttribute(std::string{name}))
      return false;
  }

  for (const auto& name : h5geo::detail::map_dsets){
    if (!group.hasObject(std::string{name}, h5gt::ObjectType::Dataset))
      return false;
  }
  return true;
}

bool h5geo::isWell(
    const h5gt::Group &group)
{
  for (const auto& name : h5geo::detail::well_attrs){
    if (!group.hasAttribute(std::string{name}))
      return false;
  }

  for (const auto& name : h5geo::detail::well_groups){
    if (!group.hasObject(std::string{name}, h5gt::ObjectType::Group))
      return false;
  }
  return true;
}

bool h5geo::isLogCurve(
    const h5gt::Group &group)
{
  for (const auto& name : h5geo::detail::log_dsets){
    if (!group.hasObject(std::string{name}, h5gt::ObjectType::Dataset))
      return false;
  }
  return true;
}

bool h5geo::isDevCurve(
    const h5gt::Group &group)
{
  for (const auto& name : h5geo::detail::dev_dsets){
    if (!group.hasObject(std::string{name}, h5gt::ObjectType::Dataset))
      return false;
  }
  return true;
}

bool h5geo::isSeis(
    const h5gt::Group &group)
{
  for (const auto& name : h5geo::detail::seis_attrs){
    if (!group.hasAttribute(std::string{name}))
      return false;
  }

  for (const auto& name : h5geo::detail::seis_dsets){
    if (!group.hasObject(std::string{name}, h5gt::ObjectType::Dataset))
      return false;
  }

  std::string sortG_name =
      std::string{h5geo::detail::sort};
  std::string indexesG_name =
      std::string{h5geo::detail::indexes};
  std::string unique_valuesG_name =
      std::string{h5geo::detail::unique_values};

  if (!group.hasObject
      (sortG_name, h5gt::ObjectType::Group))
    return false;

  h5gt::Group sortG = group.getGroup(sortG_name);
  if (!sortG.hasObject(indexesG_name, h5gt::ObjectType::Group))
    return false;

  if (!sortG.hasObject(unique_valuesG_name, h5gt::ObjectType::Group))
    return false;

  return true;
}

bool h5geo::isVol(
    const h5gt::Group &group)
{
  for (const auto& name : h5geo::detail::vol_attrs){
    if (!group.hasAttribute(std::string{name}))
      return false;
  }

  for (const auto& name : h5geo::detail::vol_dsets){
    if (!group.hasObject(std::string{name}, h5gt::ObjectType::Dataset))
      return false;
  }
  return true;
}

// containers
H5BaseContainer*
h5geo::createContainer(
    h5gt::File h5File,
    h5geo::ContainerType cntType,
    h5geo::CreationType createFlag)
{
  switch (static_cast<h5geo::ContainerType>(cntType)) {
  case h5geo::ContainerType::MAP :
    return h5geo::createMapContainer(h5File, createFlag);
  case h5geo::ContainerType::SEISMIC :
    return h5geo::createSeisContainer(h5File, createFlag);
  case h5geo::ContainerType::VOLUME :
    return h5geo::createVolContainer(h5File, createFlag);
  case h5geo::ContainerType::WELL :
    return h5geo::createWellContainer(h5File, createFlag);
  default :
    return nullptr;
  }
}

H5BaseContainer*
h5geo::createContainerByName(
    std::string& fileName,
    h5geo::ContainerType cntType,
    h5geo::CreationType createFlag)
{
  switch (static_cast<h5geo::ContainerType>(cntType)) {
  case h5geo::ContainerType::MAP :
    return h5geo::createMapContainerByName(fileName, createFlag);
  case h5geo::ContainerType::SEISMIC :
    return h5geo::createSeisContainerByName(fileName, createFlag);
  case h5geo::ContainerType::VOLUME :
    return h5geo::createVolContainerByName(fileName, createFlag);
  case h5geo::ContainerType::WELL :
    return h5geo::createWellContainerByName(fileName, createFlag);
  default :
    return nullptr;
  }
}

H5BaseContainer*
h5geo::openBaseContainer(h5gt::File h5File)
{
  return new H5BaseContainerImpl<H5BaseContainer>(h5File);
}

H5BaseContainer*
h5geo::openBaseContainerByName(const std::string& fileName)
{
  if (!fs::exists(fileName) || H5Fis_hdf5(fileName.c_str()) < 1)
    return nullptr;

  try {
    h5gt::File h5File(
          fileName,
          h5gt::File::ReadWrite);
    return h5geo::openBaseContainer(h5File);
  } catch (h5gt::Exception& err) {
    return nullptr;
  }
}

H5BaseContainer*
h5geo::openContainer(h5gt::File h5File)
{
  H5BaseContainer* baseContainer = nullptr;
  baseContainer = h5geo::openMapContainer(h5File);
  if (baseContainer)
    return baseContainer;
  baseContainer = h5geo::openWellContainer(h5File);
  if (baseContainer)
    return baseContainer;
  baseContainer = h5geo::openSeisContainer(h5File);
  if (baseContainer)
    return baseContainer;
  baseContainer = h5geo::openVolContainer(h5File);
  if (baseContainer)
    return baseContainer;

  return h5geo::openBaseContainer(h5File);
}

H5BaseContainer*
h5geo::openContainerByName(const std::string& fileName)
{
  H5BaseContainer* baseContainer = nullptr;
  baseContainer = h5geo::openMapContainerByName(fileName);
  if (baseContainer)
    return baseContainer;
  baseContainer = h5geo::openWellContainerByName(fileName);
  if (baseContainer)
    return baseContainer;
  baseContainer = h5geo::openSeisContainerByName(fileName);
  if (baseContainer)
    return baseContainer;
  baseContainer = h5geo::openVolContainerByName(fileName);
  if (baseContainer)
    return baseContainer;

  return h5geo::openBaseContainerByName(fileName);
}

// Map containers
H5MapContainer*
h5geo::createMapContainer(
    h5gt::File h5File, h5geo::CreationType createFlag)
{
  auto opt = H5MapContainerImpl::createContainer(
        h5File, h5geo::ContainerType::MAP, createFlag);
  if (!opt.has_value())
    return nullptr;

  return new H5MapContainerImpl(opt.value());
}

H5MapContainer*
h5geo::createMapContainerByName(
    std::string& fileName, h5geo::CreationType createFlag)
{
  auto opt = H5MapContainerImpl::createContainer(
        fileName, h5geo::ContainerType::MAP, createFlag);
  if (!opt.has_value())
    return nullptr;

  return new H5MapContainerImpl(opt.value());
}

H5MapContainer*
h5geo::openMapContainer(
    h5gt::File h5File){
  return createMapContainer(h5File, h5geo::CreationType::OPEN);
}

H5MapContainer*
h5geo::openMapContainerByName(
    const std::string& fileName){
  if (!fs::exists(fileName) || H5Fis_hdf5(fileName.c_str()) < 1)
    return nullptr;

  try {
    h5gt::File h5File(
          fileName,
          h5gt::File::ReadWrite);
    return h5geo::openMapContainer(h5File);
  } catch (h5gt::Exception& err) {
    return nullptr;
  }
}

// Seis containers
H5SeisContainer*
h5geo::createSeisContainer(
    h5gt::File h5File, h5geo::CreationType createFlag)
{
  auto opt = H5SeisContainerImpl::createContainer(
        h5File, h5geo::ContainerType::SEISMIC, createFlag);
  if (!opt.has_value())
    return nullptr;

  return new H5SeisContainerImpl(opt.value());
}

H5SeisContainer*
h5geo::createSeisContainerByName(
    std::string& fileName, h5geo::CreationType createFlag)
{
  auto opt = H5SeisContainerImpl::createContainer(
        fileName, h5geo::ContainerType::SEISMIC, createFlag);
  if (!opt.has_value())
    return nullptr;

  return new H5SeisContainerImpl(opt.value());
}

H5SeisContainer* h5geo::openSeisContainer(
    h5gt::File h5File){
  return createSeisContainer(h5File, h5geo::CreationType::OPEN);
}

H5SeisContainer* h5geo::openSeisContainerByName(
    const std::string& fileName){
  if (!fs::exists(fileName) || H5Fis_hdf5(fileName.c_str()) < 1)
    return nullptr;

  try {
    h5gt::File h5File(
          fileName,
          h5gt::File::ReadWrite);
    return h5geo::openSeisContainer(h5File);
  } catch (h5gt::Exception& err) {
    return nullptr;
  }
}

// Vol containers
H5VolContainer*
h5geo::createVolContainer(
    h5gt::File h5File, h5geo::CreationType createFlag)
{
  auto opt = H5VolContainerImpl::createContainer(
        h5File, h5geo::ContainerType::VOLUME, createFlag);
  if (!opt.has_value())
    return nullptr;

  return new H5VolContainerImpl(opt.value());
}

H5VolContainer*
h5geo::createVolContainerByName(
    std::string& fileName, h5geo::CreationType createFlag)
{
  auto opt = H5VolContainerImpl::createContainer(
        fileName, h5geo::ContainerType::VOLUME, createFlag);
  if (!opt.has_value())
    return nullptr;

  return new H5VolContainerImpl(opt.value());
}

H5VolContainer* h5geo::openVolContainer(
    h5gt::File h5File){
  return createVolContainer(h5File, h5geo::CreationType::OPEN);
}

H5VolContainer* h5geo::openVolContainerByName(
    const std::string& fileName){
  if (!fs::exists(fileName) || H5Fis_hdf5(fileName.c_str()) < 1)
    return nullptr;

  try {
    h5gt::File h5File(
          fileName,
          h5gt::File::ReadWrite);
    return h5geo::openVolContainer(h5File);
  } catch (h5gt::Exception& err) {
    return nullptr;
  }
}

// Well containers
H5WellContainer*
h5geo::createWellContainer(
    h5gt::File h5File, h5geo::CreationType createFlag)
{
  auto opt = H5WellContainerImpl::createContainer(
        h5File, h5geo::ContainerType::WELL, createFlag);
  if (!opt.has_value())
    return nullptr;

  return new H5WellContainerImpl(opt.value());
}

H5WellContainer*
h5geo::createWellContainerByName(
    std::string& fileName, h5geo::CreationType createFlag)
{
  auto opt = H5WellContainerImpl::createContainer(
        fileName, h5geo::ContainerType::WELL, createFlag);
  if (!opt.has_value())
    return nullptr;

  return new H5WellContainerImpl(opt.value());
}

H5WellContainer*
h5geo::openWellContainer(h5gt::File h5File){
  return createWellContainer(h5File, h5geo::CreationType::OPEN);
}

H5WellContainer*
h5geo::openWellContainerByName(const std::string& fileName){
  if (!fs::exists(fileName) || H5Fis_hdf5(fileName.c_str()) < 1)
    return nullptr;

  try {
    h5gt::File h5File(
          fileName,
          h5gt::File::ReadWrite);
    return h5geo::openWellContainer(h5File);
  } catch (h5gt::Exception& err) {
    return nullptr;
  }
}

H5BaseObject* h5geo::openObject(h5gt::Group group)
{
  H5BaseObject* obj = nullptr;
  obj = openSeis(group);
  if (obj)
    return obj;
  obj = openVol(group);
  if (obj)
    return obj;
  obj = openMap(group);
  if (obj)
    return obj;
  obj = openWell(group);
  if (obj)
    return obj;
  obj = openDevCurve(group);
  if (obj)
    return obj;
  obj = openLogCurve(group);
  if (obj)
    return obj;
  // well tops must go before points as Points1 and WellTops are equal in hdf5
  obj = openWellTops(group);
  if (obj)
    return obj;
  obj = openPoints(group);
  if (obj)
    return obj;
  obj = openHorizon(group);
  if (obj)
    return obj;
  obj = openBaseObject(group);
  return obj;
}

H5BaseObject* h5geo::openObjectByName(
    const std::string& fileName, const std::string& objName)
{
  if (!fs::exists(fileName) || H5Fis_hdf5(fileName.c_str()) < 1)
    return nullptr;

  try {
    h5gt::File h5File(
          fileName,
          h5gt::File::ReadWrite);
    if (!h5File.hasObject(objName, h5gt::ObjectType::Group))
      return nullptr;

    h5gt::Group group = h5File.getGroup(objName);
    return h5geo::openObject(group);
  } catch (h5gt::Exception& err) {
    return nullptr;
  }
}

H5BaseObject*
h5geo::openBaseObject(h5gt::Group group)
{
  return new H5BaseObjectImpl<H5BaseObject>(group);
}

H5BaseObject*
h5geo::openBaseObjectByName(
    const std::string& fileName, const std::string& objName)
{
  if (!fs::exists(fileName) || H5Fis_hdf5(fileName.c_str()) < 1)
    return nullptr;

  try {
    h5gt::File h5File(
          fileName,
          h5gt::File::ReadWrite);
    if (!h5File.hasObject(objName, h5gt::ObjectType::Group))
      return nullptr;

    h5gt::Group group = h5File.getGroup(objName);
    return h5geo::openBaseObject(group);
  } catch (h5gt::Exception& err) {
    return nullptr;
  }
}

H5Map* h5geo::openMap(h5gt::Group group){
  if (isGeoObjectByType(group, h5geo::ObjectType::MAP))
      return new H5MapImpl(group);

  return nullptr;
}

H5Map* h5geo::openMapByName(
    const std::string& fileName, const std::string& objName)
{
  if (!fs::exists(fileName) || H5Fis_hdf5(fileName.c_str()) < 1)
    return nullptr;

  try {
    h5gt::File h5File(
          fileName,
          h5gt::File::ReadWrite);
    if (!h5File.hasObject(objName, h5gt::ObjectType::Group))
      return nullptr;

    h5gt::Group group = h5File.getGroup(objName);
    return h5geo::openMap(group);
  } catch (h5gt::Exception& err) {
    return nullptr;
  }
}

H5Seis* h5geo::openSeis(h5gt::Group group){
  if (isGeoObjectByType(group, h5geo::ObjectType::SEISMIC))
    return new H5SeisImpl(group);

  return nullptr;
}

H5Seis* h5geo::openSeisByName(
    const std::string& fileName, const std::string& objName)
{
  if (!fs::exists(fileName) || H5Fis_hdf5(fileName.c_str()) < 1)
    return nullptr;

  try {
    h5gt::File h5File(
          fileName,
          h5gt::File::ReadWrite);
    if (!h5File.hasObject(objName, h5gt::ObjectType::Group))
      return nullptr;

    h5gt::Group group = h5File.getGroup(objName);
    return h5geo::openSeis(group);
  } catch (h5gt::Exception& err) {
    return nullptr;
  }
}

H5Vol* h5geo::openVol(h5gt::Group group){
  if (isGeoObjectByType(group, h5geo::ObjectType::VOLUME))
    return new H5VolImpl(group);

  return nullptr;
}

H5Vol* h5geo::openVolByName(
    const std::string& fileName, const std::string& objName)
{
  if (!fs::exists(fileName) || H5Fis_hdf5(fileName.c_str()) < 1)
    return nullptr;

  try {
    h5gt::File h5File(
          fileName,
          h5gt::File::ReadWrite);
    if (!h5File.hasObject(objName, h5gt::ObjectType::Group))
      return nullptr;

    h5gt::Group group = h5File.getGroup(objName);
    return h5geo::openVol(group);
  } catch (h5gt::Exception& err) {
    return nullptr;
  }
}

H5Well* h5geo::openWell(h5gt::Group group){
  if (isGeoObjectByType(group, h5geo::ObjectType::WELL))
    return new H5WellImpl(group);

  return nullptr;
}

H5Well* h5geo::openWellByName(
    const std::string& fileName, const std::string& objName)
{
  if (!fs::exists(fileName) || H5Fis_hdf5(fileName.c_str()) < 1)
    return nullptr;

  try {
    h5gt::File h5File(
          fileName,
          h5gt::File::ReadWrite);
    if (!h5File.hasObject(objName, h5gt::ObjectType::Group))
      return nullptr;

    h5gt::Group group = h5File.getGroup(objName);
    return h5geo::openWell(group);
  } catch (h5gt::Exception& err) {
    return nullptr;
  }
}

H5DevCurve* h5geo::openDevCurve(h5gt::Group group){
  if (isGeoObjectByType(group, h5geo::ObjectType::DEVCURVE))
    return new H5DevCurveImpl(group);

  return nullptr;
}

H5DevCurve* h5geo::openDevCurveByName(
    const std::string& fileName, const std::string& objName)
{
  if (!fs::exists(fileName) || H5Fis_hdf5(fileName.c_str()) < 1)
    return nullptr;

  try {
    h5gt::File h5File(
          fileName,
          h5gt::File::ReadWrite);
    if (!h5File.hasObject(objName, h5gt::ObjectType::Group))
      return nullptr;

    h5gt::Group group = h5File.getGroup(objName);
    return h5geo::openDevCurve(group);
  } catch (h5gt::Exception& err) {
    return nullptr;
  }
}

H5LogCurve* h5geo::openLogCurve(h5gt::Group group){
  if (isGeoObjectByType(group, h5geo::ObjectType::LOGCURVE))
    return new H5LogCurveImpl(group);

  return nullptr;
}

H5LogCurve* h5geo::openLogCurveByName(
    const std::string& fileName, const std::string& objName)
{
  if (!fs::exists(fileName) || H5Fis_hdf5(fileName.c_str()) < 1)
    return nullptr;

  try {
    h5gt::File h5File(
          fileName,
          h5gt::File::ReadWrite);
    if (!h5File.hasObject(objName, h5gt::ObjectType::Group))
      return nullptr;

    h5gt::Group group = h5File.getGroup(objName);
    return h5geo::openLogCurve(group);
  } catch (h5gt::Exception& err) {
    return nullptr;
  }
}

H5BasePoints* h5geo::openPoints(h5gt::Group group)
{
  if (isGeoObjectByType(group, h5geo::ObjectType::POINTS_1))
    return new H5Points1Impl(group);
  else if (isGeoObjectByType(group, h5geo::ObjectType::POINTS_2))
    return new H5Points2Impl(group);
  else if (isGeoObjectByType(group, h5geo::ObjectType::POINTS_3))
    return new H5Points3Impl(group);
  else if (isGeoObjectByType(group, h5geo::ObjectType::POINTS_4))
    return new H5Points4Impl(group);

  return nullptr;
}

H5BasePoints* h5geo::openPointsByName(
    const std::string& fileName, const std::string& objName)
{
  if (!fs::exists(fileName) || H5Fis_hdf5(fileName.c_str()) < 1)
    return nullptr;

  try {
    h5gt::File h5File(
          fileName,
          h5gt::File::ReadWrite);
    if (!h5File.hasObject(objName, h5gt::ObjectType::Group))
      return nullptr;

    h5gt::Group group = h5File.getGroup(objName);
    return h5geo::openPoints(group);
  } catch (h5gt::Exception& err) {
    return nullptr;
  }
}


H5Points1* h5geo::openPoints1(h5gt::Group group){
  if (isGeoObjectByType(group, h5geo::ObjectType::POINTS_1))
    return new H5Points1Impl(group);

  return nullptr;
}

H5Points1* h5geo::openPoints1ByName(
    const std::string& fileName, const std::string& objName)
{
  if (!fs::exists(fileName) || H5Fis_hdf5(fileName.c_str()) < 1)
    return nullptr;

  try {
    h5gt::File h5File(
          fileName,
          h5gt::File::ReadWrite);
    if (!h5File.hasObject(objName, h5gt::ObjectType::Group))
      return nullptr;

    h5gt::Group group = h5File.getGroup(objName);
    return h5geo::openPoints1(group);
  } catch (h5gt::Exception& err) {
    return nullptr;
  }
}

H5Points2* h5geo::openPoints2(h5gt::Group group){
  if (isGeoObjectByType(group, h5geo::ObjectType::POINTS_2))
    return new H5Points2Impl(group);

  return nullptr;
}

H5Points2* h5geo::openPoints2ByName(
    const std::string& fileName, const std::string& objName)
{
  if (!fs::exists(fileName) || H5Fis_hdf5(fileName.c_str()) < 1)
    return nullptr;

  try {
    h5gt::File h5File(
          fileName,
          h5gt::File::ReadWrite);
    if (!h5File.hasObject(objName, h5gt::ObjectType::Group))
      return nullptr;

    h5gt::Group group = h5File.getGroup(objName);
    return h5geo::openPoints2(group);
  } catch (h5gt::Exception& err) {
    return nullptr;
  }
}

H5Points3* h5geo::openPoints3(h5gt::Group group){
  if (isGeoObjectByType(group, h5geo::ObjectType::POINTS_3))
    return new H5Points3Impl(group);

  return nullptr;
}

H5Points3* h5geo::openPoints3ByName(
    const std::string& fileName, const std::string& objName)
{
  if (!fs::exists(fileName) || H5Fis_hdf5(fileName.c_str()) < 1)
    return nullptr;

  try {
    h5gt::File h5File(
          fileName,
          h5gt::File::ReadWrite);
    if (!h5File.hasObject(objName, h5gt::ObjectType::Group))
      return nullptr;

    h5gt::Group group = h5File.getGroup(objName);
    return h5geo::openPoints3(group);
  } catch (h5gt::Exception& err) {
    return nullptr;
  }
}

H5Points4* h5geo::openPoints4(h5gt::Group group){
  if (isGeoObjectByType(group, h5geo::ObjectType::POINTS_4))
    return new H5Points4Impl(group);

  return nullptr;
}

H5Points4* h5geo::openPoints4ByName(
    const std::string& fileName, const std::string& objName)
{
  if (!fs::exists(fileName) || H5Fis_hdf5(fileName.c_str()) < 1)
    return nullptr;

  try {
    h5gt::File h5File(
          fileName,
          h5gt::File::ReadWrite);
    if (!h5File.hasObject(objName, h5gt::ObjectType::Group))
      return nullptr;

    h5gt::Group group = h5File.getGroup(objName);
    return h5geo::openPoints4(group);
  } catch (h5gt::Exception& err) {
    return nullptr;
  }
}

H5WellTops* h5geo::openWellTops(h5gt::Group group){
  if (isGeoObjectByType(group, h5geo::ObjectType::WELLTOPS))
    return new H5WellTopsImpl(group);

  return nullptr;
}

H5WellTops* h5geo::openWellTopsByName(
    const std::string& fileName, const std::string& objName)
{
  if (!fs::exists(fileName) || H5Fis_hdf5(fileName.c_str()) < 1)
    return nullptr;

  try {
    h5gt::File h5File(
          fileName,
          h5gt::File::ReadWrite);
    if (!h5File.hasObject(objName, h5gt::ObjectType::Group))
      return nullptr;

    h5gt::Group group = h5File.getGroup(objName);
    return h5geo::openWellTops(group);
  } catch (h5gt::Exception& err) {
    return nullptr;
  }
}

H5Horizon* h5geo::openHorizon(
    h5gt::Group group)
{
  if (isGeoObjectByType(group, h5geo::ObjectType::HORIZON))
    return new H5HorizonImpl(group);

  return nullptr;
}

H5Horizon* h5geo::openHorizonByName(
    const std::string& fileName, const std::string& objName)
{
  if (!fs::exists(fileName) || H5Fis_hdf5(fileName.c_str()) < 1)
    return nullptr;

  try {
    h5gt::File h5File(
          fileName,
          h5gt::File::ReadWrite);
    if (!h5File.hasObject(objName, h5gt::ObjectType::Group))
      return nullptr;

    h5gt::Group group = h5File.getGroup(objName);
    return h5geo::openHorizon(group);
  } catch (h5gt::Exception& err) {
    return nullptr;
  }
}

// explicit instantiation (requires that corresponding headers are included)
template class H5BaseImpl<H5Base>;
template class H5BaseImpl<H5BaseContainer>;
template class H5BaseImpl<H5MapContainer>;
template class H5BaseImpl<H5SeisContainer>;
template class H5BaseImpl<H5VolContainer>;
template class H5BaseImpl<H5WellContainer>;
template class H5BaseImpl<H5BaseObject>;
template class H5BaseImpl<H5Map>;
template class H5BaseImpl<H5Seis>;
template class H5BaseImpl<H5Vol>;
template class H5BaseImpl<H5Well>;
template class H5BaseImpl<H5DevCurve>;
template class H5BaseImpl<H5LogCurve>;
template class H5BaseImpl<H5BasePoints>;
template class H5BaseImpl<H5Points1>;
template class H5BaseImpl<H5Points2>;
template class H5BaseImpl<H5Points3>;
template class H5BaseImpl<H5Points4>;
template class H5BaseImpl<H5WellTops>;
template class H5BaseImpl<H5Horizon>;
