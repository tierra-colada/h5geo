#include "../../include/h5geo/misc/h5baseimpl.h"
#include "../../include/h5geo/misc/h5basecontainerimpl.h"
#include "../../include/h5geo/misc/h5mapcontainerimpl.h"
#include "../../include/h5geo/misc/h5seiscontainerimpl.h"
#include "../../include/h5geo/misc/h5wellcontainerimpl.h"
#include "../../include/h5geo/misc/h5baseobjectimpl.h"
#include "../../include/h5geo/misc/h5mapimpl.h"
#include "../../include/h5geo/misc/h5seisimpl.h"
#include "../../include/h5geo/misc/h5wellimpl.h"
#include "../../include/h5geo/misc/h5devcurveimpl.h"
#include "../../include/h5geo/misc/h5logcurveimpl.h"
#include "../../include/h5geo/misc/h5pointsimpl.h"
#include "../../include/h5geo/h5core.h"
//#include "../../include/h5geo/misc/h5core_enum.h"
#include "../../include/h5geo/misc/h5core_enum_string.h"

#include <h5gt/H5File.hpp>
#include <h5gt/H5Group.hpp>
#include <h5gt/H5DataSet.hpp>

#include <filesystem>
namespace fs = std::filesystem;

template <typename TBase>
void H5BaseImpl<TBase>::Delete()
{
  delete this;
}

template <typename TBase>
std::vector<h5gt::Group>
H5BaseImpl<TBase>::getChildList(
    h5gt::Group& group,
    const h5geo::ObjectType& objType)
{
  std::vector<h5gt::Group> childList;
  std::vector<std::string> nameList = group.listAttributeNames();
  for (const auto& name : nameList){
    if (group.getObjectType(name) != h5gt::ObjectType::Group)
      continue;

    h5gt::Group childG = group.getGroup(name);
    if (h5geo::isGeoObjectByType(childG, objType)){
      childList.push_back(childG);
    } else {
      std::vector<h5gt::Group> subChildList = getChildList(childG, objType);
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
        h5gt::FileAccessProps fapl;
        if (H5Fis_hdf5(fileName.c_str()) > 0 &&
            H5Fis_accessible(fileName.c_str(), fapl.getId()) > 0 ){
          h5gt::File h5File(
                fileName,
                h5gt::File::ReadWrite |
                h5gt::File::OpenOrCreate, fapl);
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

template <typename TBase>
std::optional<h5gt::Group>
/*!
 * \brief H5BaseImpl::createNewObject This method should work only
 * with clean groups that don't have any attributes or content
 * \param group is going to be object group (objG)
 * \param objType
 * \param p
 */
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
  case h5geo::ObjectType::POINTS :
    return createNewPoints(group, p);
  default:
    return std::nullopt;
  }
}

template <typename TBase>
std::optional<h5gt::Group>
H5BaseImpl<TBase>::createNewPoints(h5gt::Group &group, void* p)
{
  PointsParam param = *(static_cast<PointsParam*>(p));

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

    group.createDataSet(
          std::string{h5geo::detail::points_data},
          dataspace, h5geo::compound_Point(), h5gt::LinkCreateProps(), props);

    return group;

  } catch (h5gt::Exception& err) {
    return std::nullopt;
  }


}

template <typename TBase>
std::optional<h5gt::Group>
H5BaseImpl<TBase>::createNewMap(h5gt::Group &group, void* p)
{
  MapParam param = *(static_cast<MapParam*>(p));

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
  std::vector<size_t> count = {4, 1};
  std::vector<size_t> max_count = {5, h5gt::DataSpace::UNLIMITED};
  std::vector<hsize_t> cdims = {4, param.chunkSize};
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
          std::string{h5geo::OWT},
          h5gt::DataSpace(1)).write(3);

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

    createTextHeader(group);
    createBinHeader(group, param.stdChunk);
    createTrace(group, param.nTrc, param.nSamp, param.trcChunk);
    createTraceHeader(group, param.nTrc, param.trcChunk);
    createBoundary(group, param.stdChunk);
    createSort(group);

    return group;

  } catch (h5gt::Exception& err) {
    return std::nullopt;
  }
}

template <typename TBase>
std::optional<h5gt::DataSet>
H5BaseImpl<TBase>::createTextHeader(
    h5gt::Group &seisGroup)
{
  char txtHdr[40][80];
  h5gt::DataSet dataset = seisGroup.createDataSet<char[80]>(
        std::string{h5geo::detail::text_header},
        h5gt::DataSpace::FromCharArrayStrings(txtHdr));
  return dataset;
}

template <typename TBase>
std::optional<h5gt::DataSet>
H5BaseImpl<TBase>::createBinHeader(
    h5gt::Group &seisGroup,
    const hsize_t& stdChunk)
{
  std::vector<std::string> fullHeaderNameList, shortHeaderNameList;
  h5geo::getBinHeaderNames(fullHeaderNameList, shortHeaderNameList);
  size_t nBinHeaderNames = fullHeaderNameList.size();

  std::vector<size_t> count = {size_t(nBinHeaderNames)};
  std::vector<size_t> max_count = {h5gt::DataSpace::UNLIMITED};
  std::vector<hsize_t> cdims = {stdChunk};

  try {

    h5gt::DataSetCreateProps props;
    props.setChunk(cdims);
    h5gt::DataSpace dataspace(count, max_count);
    h5gt::DataSet dataset = seisGroup.createDataSet<double>(
          std::string{h5geo::detail::bin_header},
          dataspace, h5gt::LinkCreateProps(), props);
    for (size_t i = 0; i < nBinHeaderNames; i++){
      h5gt::Attribute attribute = dataset.createAttribute<size_t>(
            shortHeaderNameList[i], h5gt::DataSpace(1));
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
    const hsize_t& trcChunk)
{
  std::vector<size_t> count = {nTrc, nSamp};
  std::vector<size_t> max_count = {
    h5gt::DataSpace::UNLIMITED, h5gt::DataSpace::UNLIMITED};
  std::vector<hsize_t> cdims = {trcChunk, nSamp};

  try {

    h5gt::DataSetCreateProps props;
    props.setChunk(cdims);
    h5gt::DataSpace dataspace(count, max_count);
    h5gt::DataSet dataset = seisGroup.createDataSet<float>(
          std::string{h5geo::detail::trace},
          dataspace, h5gt::LinkCreateProps(), props);
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
    const hsize_t& trcChunk)
{
  std::vector<std::string> fullHeaderNameList, shortHeaderNameList;
  h5geo::getTraceHeaderNames(fullHeaderNameList, shortHeaderNameList);
  size_t nTraceHeaderNames = fullHeaderNameList.size();

  std::vector<size_t> count = {size_t(nTraceHeaderNames), nTrc};
  std::vector<size_t> max_count = {
    h5gt::DataSpace::UNLIMITED, h5gt::DataSpace::UNLIMITED};
  std::vector<hsize_t> cdims = {size_t(nTraceHeaderNames), trcChunk};

  try {

    h5gt::DataSetCreateProps props;
    props.setChunk(cdims);
    h5gt::DataSpace dataspace(count, max_count);
    h5gt::DataSet dataset = seisGroup.createDataSet<double>(
          std::string{h5geo::detail::trace_header},
          dataspace, h5gt::LinkCreateProps(), props);
    for (size_t i = 0; i < nTraceHeaderNames; i++){
      h5gt::Attribute attribute = dataset.createAttribute<size_t>(
            shortHeaderNameList[i], h5gt::DataSpace(1));
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
bool H5BaseImpl<TBase>::isSuccessor(h5gt::Group &parentG, h5gt::Group &childG)
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

bool h5geo::isGeoObject(h5gt::Group& group){
  constexpr auto& objTypes = magic_enum::enum_values<h5geo::ObjectType>();
  for (const auto &objType : objTypes)
    if (h5geo::isGeoObjectByType(group, objType))
      return true;

  return false;
}

bool h5geo::isGeoObjectByType(h5gt::Group& group,
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
  case h5geo::ObjectType::POINTS :
    return h5geo::isPoints(group);
  default:
    return false;
  }
}

bool h5geo::isPoints(
    h5gt::Group &group)
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
    if (!dtype.isTypeEqual(h5geo::compound_Point())){
      return false;
    }
  }
  return true;
}

bool h5geo::isMap(
    h5gt::Group &group)
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
    h5gt::Group &group)
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
    h5gt::Group &group)
{
  for (const auto& name : h5geo::detail::log_dsets){
    if (!group.hasObject(std::string{name}, h5gt::ObjectType::Dataset))
      return false;
  }
  return true;
}

bool h5geo::isDevCurve(
    h5gt::Group &group)
{
  for (const auto& name : h5geo::detail::dev_dsets){
    if (!group.hasObject(std::string{name}, h5gt::ObjectType::Dataset))
      return false;
  }
  return true;
}

bool h5geo::isSeis(
    h5gt::Group &group)
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
  obj = openPoints(group);
  if (obj)
    return obj;
  obj = openBaseObject(group);
  return obj;
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
template class H5BaseImpl<H5Points>;
