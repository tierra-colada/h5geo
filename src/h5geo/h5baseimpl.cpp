#include "../../include/h5geo/impl/h5baseimpl.h"
#include "../../include/h5geo/impl/h5basecontainerimpl.h"
#include "../../include/h5geo/impl/h5mapcontainerimpl.h"
#include "../../include/h5geo/impl/h5seiscontainerimpl.h"
#include "../../include/h5geo/impl/h5wellcontainerimpl.h"
#include "../../include/h5geo/impl/h5baseobjectimpl.h"
#include "../../include/h5geo/impl/h5mapimpl.h"
#include "../../include/h5geo/impl/h5seisimpl.h"
#include "../../include/h5geo/impl/h5wellimpl.h"
#include "../../include/h5geo/impl/h5devcurveimpl.h"
#include "../../include/h5geo/impl/h5logcurveimpl.h"
#include "../../include/h5geo/impl/h5basepointsimpl.h"
#include "../../include/h5geo/impl/h5points1impl.h"
#include "../../include/h5geo/impl/h5points2impl.h"
#include "../../include/h5geo/impl/h5points3impl.h"
#include "../../include/h5geo/impl/h5points4impl.h"
#include "../../include/h5geo/impl/h5welltopsimpl.h"
#include "../../include/h5geo/misc/h5core.h"
#include "../../include/h5geo/misc/h5enum_string.h"
#include "../../include/h5geo/misc/h5segy.h"

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
  std::string attrName = std::string{h5geo::detail::ContainerType};
  if (file.hasAttribute(attrName)){
    auto attr = file.getAttribute(attrName);
    auto dtype = attr.getDataType();
    if ((!dtype.isTypeEqual(h5gt::AtomicType<unsigned>()) &&
         !dtype.isTypeEqual(h5gt::AtomicType<int>())) ||
        attr.getMemSpace().getElementCount() != 1){
      file.deleteAttribute(attrName); // after deletion `attr` is unavailable!
      file.createAttribute<unsigned>(
            attrName, h5gt::DataSpace(1)).
          write(static_cast<unsigned>(containerType));
      return file;
    } else {
      attr.write(static_cast<unsigned>(containerType));
      return file;
    }
  }

  file.createAttribute<unsigned>(
        attrName, h5gt::DataSpace(1)).
      write(static_cast<unsigned>(containerType));
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
  default:
    return std::nullopt;
  }
}

template <typename TBase>
std::optional<h5gt::Group>
H5BaseImpl<TBase>::createNewPoints(h5gt::Group &group, void* p, h5geo::ObjectType pointsType)
{
  PointsParam param = *(static_cast<PointsParam*>(p));

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

    group.createAttribute<unsigned>(
          std::string{h5geo::detail::Domain},
          h5gt::DataSpace(1)).
        write(static_cast<unsigned>(param.domain));
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
            dataspace, h5geo::compound_Point1(), h5gt::LinkCreateProps(), props);
      break;
    case h5geo::ObjectType::POINTS_2 :
      group.createDataSet(
            std::string{h5geo::detail::points_data},
            dataspace, h5geo::compound_Point2(), h5gt::LinkCreateProps(), props);
      break;
    case h5geo::ObjectType::POINTS_3 :
      group.createDataSet(
            std::string{h5geo::detail::points_data},
            dataspace, h5geo::compound_Point3(), h5gt::LinkCreateProps(), props);
      break;
    case h5geo::ObjectType::POINTS_4 :
      group.createDataSet(
            std::string{h5geo::detail::points_data},
            dataspace, h5geo::compound_Point4(), h5gt::LinkCreateProps(), props);
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
H5BaseImpl<TBase>::createNewMap(h5gt::Group &group, void* p)
{
  MapParam param = *(static_cast<MapParam*>(p));

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

    group.createAttribute<unsigned>(
          std::string{h5geo::detail::Domain},
          h5gt::DataSpace(1)).
        write(static_cast<unsigned>(param.domain));
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
H5BaseImpl<TBase>::createNewWell(h5gt::Group &group, void* p)
{
  WellParam param = *(static_cast<WellParam *>(p));
  std::vector<double> head_coord({param.headY, param.headX});

  try {

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
  LogCurveParam param = *(static_cast<LogCurveParam*>(p));

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
  DevCurveParam param = *(static_cast<DevCurveParam*>(p));

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
  SeisParam param = *(static_cast<SeisParam*>(p));
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

    group.createAttribute<unsigned>(
          std::string{h5geo::detail::Domain},
          h5gt::DataSpace(1)).
        write(static_cast<unsigned>(param.domain));
    group.createAttribute<unsigned>(
          std::string{h5geo::detail::SeisDataType},
          h5gt::DataSpace(1)).
        write(static_cast<unsigned>(param.dataType));
    group.createAttribute<unsigned>(
          std::string{h5geo::detail::SurveyType},
          h5gt::DataSpace(1)).
        write(static_cast<unsigned>(param.surveyType));
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
    createBoundary(group, param.stdChunk);
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
      h5gt::Selection vSel(space);
      auto segyG = seisGroup.getGroup(std::string{h5geo::detail::segy});
      auto srcDset2b = segyG.getDataSet(std::string{h5geo::detail::trace_header_2bytes});
      auto srcDset4b = segyG.getDataSet(std::string{h5geo::detail::trace_header_4bytes});

      std::vector<size_t> srcCols_from = {0,12,8,32,17,42,44,98};
      std::vector<size_t> srcCols_to = {6,16,16,34,21,88,49,100};

      size_t I = srcCols_from.size();
      size_t srcOffset = 0;
      bool is4b = true;
      for (size_t i = 0; i < I; i++){
        size_t nCols = srcCols_to[i] - srcCols_from[i];
        h5gt::Selection vSel(space);
        // should work only line by line or incorrect result will be gotten
        for (size_t ii = 0; ii < nCols; ii++){
          vSel = vSel.select({srcOffset,0},{1,nTrc}); // SEGY is mapped as {nTrc,nSamp} while Seis Header is in reverse order {nSamp,nTrc}
          if (is4b){
            auto srcSel4b = srcDset4b.select({0,srcCols_from[i]+ii},{nTrc,1});
            props.addVirtualDataSet(vSel.getSpace(), srcDset4b, srcSel4b.getSpace());
          } else {
            auto srcSel2b = srcDset2b.select({0,srcCols_from[i]+ii},{nTrc,1});
            props.addVirtualDataSet(vSel.getSpace(), srcDset2b, srcSel2b.getSpace());
          }
          srcOffset++;
        }
        // change 4 bytes to 2 bytes
        is4b = !is4b;
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
std::optional<h5gt::DataSet>
H5BaseImpl<TBase>::createBoundary(
    h5gt::Group &seisGroup,
    const hsize_t& stdChunk)
{
  std::vector<size_t> count = {2, 1};
  std::vector<size_t> max_count = {2, h5gt::DataSpace::UNLIMITED};
  std::vector<hsize_t> cdims = {2, stdChunk};

  try {

    h5gt::DataSetCreateProps props;
    props.setChunk(cdims);
    h5gt::DataSpace dataspace(count, max_count);
    h5gt::DataSet dataset = seisGroup.createDataSet<double>(
          std::string{h5geo::detail::boundary},
          dataspace, h5gt::LinkCreateProps(), props);
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
  unsigned val = h5geo::readEnumAttribute(
        file, std::string{h5geo::detail::ContainerType});
  switch (cntType) {
  case h5geo::ContainerType::MAP:
    return static_cast<h5geo::ContainerType>(val) == h5geo::ContainerType::MAP;
  case h5geo::ContainerType::WELL:
    return static_cast<h5geo::ContainerType>(val) == h5geo::ContainerType::WELL;
  case h5geo::ContainerType::SEISMIC:
    return static_cast<h5geo::ContainerType>(val) == h5geo::ContainerType::SEISMIC;
  default:
    return false;
  }
}

h5geo::ContainerType h5geo::getGeoContainerType(h5gt::File file)
{
  unsigned val = h5geo::readEnumAttribute(
        file, std::string{h5geo::detail::ContainerType});
  return static_cast<h5geo::ContainerType>(val);
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
    if (!dtype.isTypeEqual(h5geo::compound_Point1())){
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
    if (!dtype.isTypeEqual(h5geo::compound_Point2())){
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
    if (!dtype.isTypeEqual(h5geo::compound_Point3())){
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
    if (!dtype.isTypeEqual(h5geo::compound_Point4())){
      return false;
    }
  }
  return true;
}

bool h5geo::isWellTops(const h5gt::Group &group)
{
  return h5geo::isPoints1(group);
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

H5BaseObject* h5geo::openObject(h5gt::Group group)
{
  H5BaseObject* obj = nullptr;
  obj = openSeis(group);
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

// explicit instantiation (requires that corresponding headers are included)
template class H5BaseImpl<H5Base>;
template class H5BaseImpl<H5BaseContainer>;
template class H5BaseImpl<H5MapContainer>;
template class H5BaseImpl<H5SeisContainer>;
template class H5BaseImpl<H5WellContainer>;
template class H5BaseImpl<H5BaseObject>;
template class H5BaseImpl<H5Map>;
template class H5BaseImpl<H5Seis>;
template class H5BaseImpl<H5Well>;
template class H5BaseImpl<H5DevCurve>;
template class H5BaseImpl<H5LogCurve>;
template class H5BaseImpl<H5BasePoints>;
template class H5BaseImpl<H5Points1>;
template class H5BaseImpl<H5Points2>;
template class H5BaseImpl<H5Points3>;
template class H5BaseImpl<H5Points4>;
template class H5BaseImpl<H5WellTops>;
