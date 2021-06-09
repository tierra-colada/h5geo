#include "../../include/h5geo/misc/h5baseimpl.h"
#include "../../include/h5geo/h5core.h"
//#include "../../include/h5geo/misc/h5core_enum.h"

#include <h5gt/H5File.hpp>
#include <h5gt/H5Group.hpp>
#include <h5gt/H5DataSet.hpp>

#include <filesystem>
namespace fs = std::filesystem;

void H5BaseImpl::Delete()
{
  delete this;
}

std::vector<h5gt::Group>
H5BaseImpl::getChildList(
    h5gt::Group& group,
    const h5geo::ObjectType& objType)
{
  std::vector<h5gt::Group> childList;
  std::vector<std::string> nameList = group.listAttributeNames();
  for (const auto& name : nameList){
    if (group.getObjectType(name) != h5gt::ObjectType::Group)
      continue;

    h5gt::Group childG = group.getGroup(name);
    if (isObject(childG, objType)){
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

bool H5BaseImpl::isContainer(h5gt::File file,
    const h5geo::ContainerType& cntType)
{
  unsigned val = h5geo::getEnumFromObj(
        file,
        std::string{magic_enum::enum_name(
                    h5geo::detail::ContainerAttributes::ContainerType)});
  switch (cntType) {
  case h5geo::ContainerType::SURFACE :
    return static_cast<h5geo::ContainerType>(val) == h5geo::ContainerType::SURFACE;
  case h5geo::ContainerType::WELL :
    return static_cast<h5geo::ContainerType>(val) == h5geo::ContainerType::WELL;
  case h5geo::ContainerType::SEISMIC :
    return static_cast<h5geo::ContainerType>(val) == h5geo::ContainerType::SEISMIC;
  default:
    return false;
  }
}

bool H5BaseImpl::isObject(h5gt::Group group,
    const h5geo::ObjectType& objType)
{
  switch (objType) {
  case h5geo::ObjectType::SURFACE :
    return isSurf(group);
  case h5geo::ObjectType::WELL :
    return isWell(group);
  case h5geo::ObjectType::LOGCURVE :
    return isLogCurve(group);
  case h5geo::ObjectType::DEVCURVE :
    return isDevCurve(group);
  case h5geo::ObjectType::SEISMIC :
    return isSeis(group);
  default:
    return false;
  }
}

bool H5BaseImpl::isSurf(
    h5gt::Group &group)
{
  constexpr auto& attr_names =
      magic_enum::enum_names<h5geo::detail::SurfAttributes>();
  constexpr auto& dataset_names =
      magic_enum::enum_names<h5geo::detail::SurfDatasets>();

  for (const auto& name : attr_names){
    if (!group.hasAttribute(std::string{name}))
      return false;
  }

  for (const auto& name : dataset_names){
    if (!group.hasObject(std::string{name}, h5gt::ObjectType::Dataset))
      return false;
  }
  return true;
}

bool H5BaseImpl::isWell(
    h5gt::Group &group)
{
  constexpr auto& attr_names =
      magic_enum::enum_names<h5geo::detail::WellAttributes>();
  constexpr auto& group_names =
      magic_enum::enum_names<h5geo::detail::WellGroups>();

  for (const auto& name : attr_names){
    if (!group.hasAttribute(std::string{name}))
      return false;
  }

  for (const auto& name : group_names){
    if (!group.hasObject(std::string{name}, h5gt::ObjectType::Group))
      return false;
  }
  return true;
}

bool H5BaseImpl::isLogCurve(
    h5gt::Group &group)
{
  constexpr auto& attr_names =
      magic_enum::enum_names<h5geo::detail::LogAttributes>();
  constexpr auto& dataset_names =
      magic_enum::enum_names<h5geo::detail::LogDatasets>();

  for (const auto& name : attr_names){
    if (!group.hasAttribute(std::string{name}))
      return false;
  }

  for (const auto& name : dataset_names){
    if (!group.hasObject(std::string{name}, h5gt::ObjectType::Dataset))
      return false;
  }
  return true;
}

bool H5BaseImpl::isDevCurve(
    h5gt::Group &group)
{
  constexpr auto& attr_names =
      magic_enum::enum_names<h5geo::detail::DevAttributes>();
  constexpr auto& dataset_names =
      magic_enum::enum_names<h5geo::detail::DevDatasets>();

  for (const auto& name : attr_names){
    if (!group.hasAttribute(std::string{name}))
      return false;
  }

  for (const auto& name : dataset_names){
    if (!group.hasObject(std::string{name}, h5gt::ObjectType::Dataset))
      return false;
  }
  return true;
}

bool H5BaseImpl::isSeis(
    h5gt::Group &group)
{
  constexpr auto& attr_names =
      magic_enum::enum_names<h5geo::detail::SeisAttributes>();
  constexpr auto& dataset_names =
      magic_enum::enum_names<h5geo::detail::SeisDatasets>();

  for (const auto& name : attr_names){
    if (!group.hasAttribute(std::string{name}))
      return false;
  }

  for (const auto& name : dataset_names){
    if (!group.hasObject(std::string{name}, h5gt::ObjectType::Dataset))
      return false;
  }

  std::string sortG_name =
      std::string{magic_enum::enum_name(h5geo::detail::SeisGroups::sort)};
  std::string indexesG_name =
      std::string{magic_enum::enum_name(h5geo::detail::SeisGroups::indexes)};
  std::string unique_valuesG_name =
      std::string{magic_enum::enum_name(h5geo::detail::SeisGroups::unique_values)};

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

std::optional<h5gt::File>
H5BaseImpl::createContainer(
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

  if (createFlag == h5geo::CreationType::CREATE_UNDER_NEW_NAME){
    if (fileExist){
      fs::path p_abs = fs::absolute(fileName);
      fs::path parent_p_abs = p_abs.parent_path();
      std::vector<std::string> fileNames;
      for (const auto & file : std::filesystem::directory_iterator(p_abs.parent_path()))
        fileNames.push_back(file.path().stem().generic_string());

      fileName = h5geo::generateName(
            fileNames, p_abs.stem().generic_string());

      fileName = p_abs.parent_path().generic_string() + "/" +
          fileName + p_abs.extension().generic_string();
    }
    createFlag = h5geo::CreationType::OPEN_OR_CREATE;
  }

  if (fileExist){
    if (createFlag == h5geo::CreationType::OPEN_OR_CREATE){
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
}

std::optional<h5gt::File>
H5BaseImpl::createContainer(h5gt::File h5File,
    const h5geo::ContainerType& containerType,
    h5geo::CreationType createFlag)
{
  unsigned val = h5geo::getEnumFromObj(
        h5File,
        std::string{magic_enum::enum_name(
                    h5geo::detail::ContainerAttributes::ContainerType)});
  switch (createFlag) {
  case h5geo::CreationType::OPEN_OR_CREATE: {
    if (isContainer(h5File, containerType))
      return h5File;

    if (val != 0)
      return std::nullopt;

    return createNewContainer(h5File, containerType);
  } case h5geo::CreationType::CREATE_OR_OVERWRITE: {
    if (isContainer(h5File, containerType))
      return h5File;

    return createNewContainer(h5File, containerType);
  } case h5geo::CreationType::CREATE_UNDER_NEW_NAME: {
    return std::nullopt;
  } default :
    return std::nullopt;
  }
}

std::optional<h5gt::Group>
H5BaseImpl::createObject(std::string& objName,
    h5gt::File parentFile,
    const h5geo::ObjectType& objType,
    void* p,
    h5geo::CreationType createFlag)
{
  h5gt::Group parentGroup = parentFile.getGroup("/");
  return createObject(
        objName, parentGroup, objType, p, createFlag);
}

std::optional<h5gt::Group>
H5BaseImpl::createObject(std::string& objName,
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
      objName = h5geo::generateName(nameList, objName);
      objName = closestParent.getPath() + "/" + objName;
    }
    createFlag = h5geo::CreationType::OPEN_OR_CREATE;
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

std::optional<h5gt::Group>
H5BaseImpl::createObject(
    h5gt::Group objG,
    const h5geo::ObjectType& objType,
    void* p,
    h5geo::CreationType createFlag)
{
  switch (createFlag) {
  case h5geo::CreationType::OPEN_OR_CREATE: {
    if (isObject(objG, objType))
      return objG;

    if (objG.getNumberAttributes() > 0 ||
        objG.getNumberObjects() > 0)
      return std::nullopt;

    return createNewObject(objG, objType, p);
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

std::optional<h5gt::File>
H5BaseImpl::createNewContainer(
    h5gt::File &file,
    const h5geo::ContainerType& containerType)
{
  std::string attrName = std::string{magic_enum::enum_name(
              h5geo::detail::ContainerAttributes::ContainerType)};
  if (file.hasAttribute(attrName)){
    file.getAttribute(attrName).
        write(static_cast<unsigned>(containerType));
  } else {
    file.createAttribute<unsigned>(
          attrName, h5gt::DataSpace(1)).
        write(static_cast<unsigned>(containerType));
  }

  return file;
}

std::optional<h5gt::Group>
/*!
 * \brief H5BaseImpl::createNewObject This method should work only
 * with clean groups that don't have any attributes or content
 * \param group is going to be object group (objG)
 * \param objType
 * \param p
 */
H5BaseImpl::createNewObject(
    h5gt::Group &group,
    const h5geo::ObjectType& objType,
    void* p)
{
  switch (objType) {
  case h5geo::ObjectType::SURFACE :
    return createNewSurf(group, p);
  case h5geo::ObjectType::WELL :
    return createNewWell(group, p);
  case h5geo::ObjectType::LOGCURVE :
    return createNewLogCurve(group, p);
  case h5geo::ObjectType::DEVCURVE :
    return createNewDevCurve(group, p);
  case h5geo::ObjectType::SEISMIC :
    return createNewSeis(group, p);
  default:
    return std::nullopt;
  }
}

std::optional<h5gt::Group>
H5BaseImpl::createNewSurf(h5gt::Group &group, void* p)
{
  SurfParam param = *(static_cast<SurfParam*>(p));
  std::vector<double> origin({param.X0, param.Y0});
  std::vector<double> spacing({param.dX, param.dY});

  group.createAttribute<unsigned>(
        std::string{magic_enum::enum_name(h5geo::detail::SurfAttributes::Domain)},
        h5gt::DataSpace(1)).
      write(static_cast<unsigned>(param.domain));
  group.createAttribute<unsigned>(
        std::string{magic_enum::enum_name(h5geo::detail::SurfAttributes::SpatialUnits)},
        h5gt::DataSpace(1)).
      write(static_cast<unsigned>(param.spatialUnits));
  group.createAttribute<unsigned>(
        std::string{magic_enum::enum_name(h5geo::detail::SurfAttributes::TemporalUnits)},
        h5gt::DataSpace(1)).
      write(static_cast<unsigned>(param.temporalUnits));
  group.createAttribute<std::string>(
        std::string{magic_enum::enum_name(h5geo::detail::SurfAttributes::data_units)},
        h5gt::DataSpace::From(param.dataUnits)).
      write(param.dataUnits);
  group.createAttribute<double>(
        std::string{magic_enum::enum_name(h5geo::detail::SurfAttributes::origin)},
        h5gt::DataSpace({2})).
      write(origin);

  group.createAttribute<double>(
        std::string{magic_enum::enum_name(h5geo::detail::SurfAttributes::spacing)},
        h5gt::DataSpace({2})).
      write(spacing);

  group.createDataSet<double>(
        std::string{magic_enum::enum_name(h5geo::detail::SurfDatasets::surf_data)},
        h5gt::DataSpace({param.nX, param.nY}));

  return group;
}

std::optional<h5gt::Group>
H5BaseImpl::createNewWell(h5gt::Group &group, void* p)
{
  WellParam param = *(static_cast<WellParam *>(p));
  std::vector<double> head_coord({param.headY, param.headX});

  group.createAttribute<double>(
        std::string{magic_enum::enum_name(h5geo::detail::WellAttributes::head_coord)},
        h5gt::DataSpace({2})).
      write(head_coord);
  group.createAttribute<double>(
        std::string{magic_enum::enum_name(h5geo::detail::WellAttributes::KB)},
        h5gt::DataSpace(1)).
      write(param.kb);
  group.createAttribute<std::string>(
        std::string{magic_enum::enum_name(h5geo::detail::WellAttributes::UWI)},
        h5gt::DataSpace(1)).
      write(param.uwi);
  group.createAttribute<unsigned>(
        std::string{magic_enum::enum_name(h5geo::detail::WellAttributes::SpatialUnits)},
        h5gt::DataSpace(1)).
      write(static_cast<unsigned>(param.spatialUnits));

  constexpr auto& group_names =
      magic_enum::enum_names<h5geo::detail::WellGroups>();

  for (const auto& name : group_names){
    group.createGroup(std::string{name});
  }

  return group;
}

std::optional<h5gt::Group>
H5BaseImpl::createNewLogCurve(h5gt::Group &group, void* p)
{
  LogCurveParam param = *(static_cast<LogCurveParam*>(p));
  std::vector<size_t> count = {2, 1};
  std::vector<hsize_t> cdims = {2, param.chunkSize};
  std::vector<size_t> max_count = {2, h5gt::DataSpace::UNLIMITED};
  h5gt::DataSetCreateProps props;
  props.setChunk(cdims);
  h5gt::DataSpace dataspace(count, max_count);

  group.createAttribute<unsigned>(
        std::string{magic_enum::enum_name(h5geo::detail::LogAttributes::SpatialUnits)},
        h5gt::DataSpace(1)).
      write(static_cast<unsigned>(param.spatialUnits));
  group.createAttribute<std::string>(
        std::string{magic_enum::enum_name(h5geo::detail::LogAttributes::data_units)},
        h5gt::DataSpace::From(param.dataUnits)).
      write(param.dataUnits);

  h5gt::DataSet dataset =
      group.createDataSet<double>(
        std::string{magic_enum::enum_name(h5geo::detail::LogDatasets::log_data)},
        dataspace, h5gt::LinkCreateProps(), props);
  dataset.createAttribute<size_t>(
        std::string{magic_enum::enum_name(h5geo::LogDataType::MD)}, h5gt::DataSpace(1)).write(0);
  dataset.createAttribute<size_t>(
        std::string{magic_enum::enum_name(h5geo::LogDataType::VAL)}, h5gt::DataSpace(1)).write(1);

  return group;
}

std::optional<h5gt::Group>
H5BaseImpl::createNewDevCurve(h5gt::Group &group, void* p)
{
  DevCurveParam param = *(static_cast<DevCurveParam*>(p));
  std::vector<size_t> count = {5, 1};
  std::vector<hsize_t> cdims = {5, param.chunkSize};
  std::vector<size_t> max_count = {5, h5gt::DataSpace::UNLIMITED};
  h5gt::DataSetCreateProps props;
  props.setChunk(cdims);
  h5gt::DataSpace dataspace(count, max_count);

  group.createAttribute<unsigned>(
        std::string{magic_enum::enum_name(h5geo::detail::DevAttributes::SpatialUnits)},
        h5gt::DataSpace(1)).
      write(static_cast<unsigned>(param.spatialUnits));
  group.createAttribute<unsigned>(
        std::string{magic_enum::enum_name(h5geo::detail::DevAttributes::TemporalUnits)},
        h5gt::DataSpace(1)).
      write(static_cast<unsigned>(param.temporalUnits));
  group.createAttribute<unsigned>(
        std::string{magic_enum::enum_name(h5geo::detail::DevAttributes::AngleUnits)},
        h5gt::DataSpace(1)).
      write(static_cast<unsigned>(param.angleUnits));

  h5gt::DataSet dataset =
      group.createDataSet<double>(
        std::string{magic_enum::enum_name(h5geo::detail::DevDatasets::dev_data)},
        dataspace, h5gt::LinkCreateProps(), props);
  dataset.createAttribute<size_t>(
        std::string{magic_enum::enum_name(h5geo::DevDataType::MD)},
        h5gt::DataSpace(1)).write(0);
  dataset.createAttribute<size_t>(
        std::string{magic_enum::enum_name(h5geo::DevDataType::X)},
        h5gt::DataSpace(1)).write(1);
  dataset.createAttribute<size_t>(
        std::string{magic_enum::enum_name(h5geo::DevDataType::Y)},
        h5gt::DataSpace(1)).write(2);
  dataset.createAttribute<size_t>(
        std::string{magic_enum::enum_name(h5geo::DevDataType::TVD)},
        h5gt::DataSpace(1)).write(3);
  dataset.createAttribute<size_t>(
        std::string{magic_enum::enum_name(h5geo::DevDataType::OWT)},
        h5gt::DataSpace(1)).write(4);

  return group;
}

std::optional<h5gt::Group>
H5BaseImpl::createNewSeis(h5gt::Group &group, void* p)
{
  SeisParam param = *(static_cast<SeisParam*>(p));

  group.createAttribute<unsigned>(
        std::string{magic_enum::enum_name(h5geo::detail::SeisAttributes::Domain)},
        h5gt::DataSpace(1)).
      write(static_cast<unsigned>(param.domain));
  group.createAttribute<unsigned>(
        std::string{magic_enum::enum_name(h5geo::detail::SeisAttributes::SpatialUnits)},
        h5gt::DataSpace(1)).
      write(static_cast<unsigned>(param.spatialUnits));
  group.createAttribute<unsigned>(
        std::string{magic_enum::enum_name(h5geo::detail::SeisAttributes::TemporalUnits)},
        h5gt::DataSpace(1)).
      write(static_cast<unsigned>(param.temporalUnits));
  group.createAttribute<std::string>(
        std::string{magic_enum::enum_name(h5geo::detail::SeisAttributes::data_units)},
        h5gt::DataSpace::From(param.dataUnits)).
      write(param.dataUnits);
  group.createAttribute<unsigned>(
        std::string{magic_enum::enum_name(h5geo::detail::SeisAttributes::SeisDataType)},
        h5gt::DataSpace(1)).
      write(static_cast<unsigned>(param.dataType));
  group.createAttribute<unsigned>(
        std::string{magic_enum::enum_name(h5geo::detail::SeisAttributes::SurveyType)},
        h5gt::DataSpace(1)).
      write(static_cast<unsigned>(param.surveyType));
  group.createAttribute<double>(
        std::string{magic_enum::enum_name(h5geo::detail::SeisAttributes::SRD)},
        h5gt::DataSpace(1)).
      write(param.srd);

  createTextHeader(group);
  createBinHeader(group, param.stdChunk);
  createTrace(group, param.nTrc, param.nSamp, param.trcChunk);
  createTraceHeader(group, param.nTrc, param.trcChunk);
  createBoundary(group, param.stdChunk);
  createSort(group);

  return group;
}

std::optional<h5gt::DataSet>
H5BaseImpl::createTextHeader(
    h5gt::Group &seisGroup)
{
  char txtHdr[40][80];
  h5gt::DataSet dataset = seisGroup.createDataSet<char[80]>(
        std::string{magic_enum::enum_name(h5geo::detail::SeisDatasets::text_header)},
        h5gt::DataSpace::FromCharArrayStrings(txtHdr));
  return dataset;
}

std::optional<h5gt::DataSet>
H5BaseImpl::createBinHeader(
    h5gt::Group &seisGroup,
    const hsize_t& stdChunk)
{
  std::vector<std::string> fullHeaderNameList, shortHeaderNameList;
  h5geo::getBinHeaderNames(fullHeaderNameList, shortHeaderNameList);
  size_t nBinHeaderNames = fullHeaderNameList.size();

  std::vector<size_t> count = {size_t(nBinHeaderNames)};
  std::vector<hsize_t> cdims = {stdChunk};
  std::vector<size_t> max_count = {h5gt::DataSpace::UNLIMITED};

  h5gt::DataSetCreateProps props;
  props.setChunk(cdims);
  h5gt::DataSpace dataspace(count, max_count);
  h5gt::DataSet dataset = seisGroup.createDataSet<double>(
        std::string{magic_enum::enum_name(h5geo::detail::SeisDatasets::bin_header)},
        dataspace, h5gt::LinkCreateProps(), props);
  for (size_t i = 0; i < nBinHeaderNames; i++){
    h5gt::Attribute attribute = dataset.createAttribute<size_t>(
          shortHeaderNameList[i], h5gt::DataSpace(1));
    attribute.write(i);
  }
  return dataset;
}

std::optional<h5gt::DataSet>
H5BaseImpl::createTrace(
    h5gt::Group &seisGroup,
    const size_t& nTrc,
    const size_t& nSamp,
    const hsize_t& trcChunk)
{
  std::vector<size_t> count = {nTrc, nSamp};
  std::vector<hsize_t> cdims = {trcChunk, nSamp};
  std::vector<size_t> max_count = {
    h5gt::DataSpace::UNLIMITED, h5gt::DataSpace::UNLIMITED};

  h5gt::DataSetCreateProps props;
  props.setChunk(cdims);
  h5gt::DataSpace dataspace(count, max_count);
  h5gt::DataSet dataset = seisGroup.createDataSet<float>(
        std::string{magic_enum::enum_name(h5geo::detail::SeisDatasets::trace)},
        dataspace, h5gt::LinkCreateProps(), props);
  return dataset;
}

std::optional<h5gt::DataSet>
H5BaseImpl::createTraceHeader(
    h5gt::Group &seisGroup,
    const size_t& nTrc,
    const hsize_t& trcChunk)
{
  std::vector<std::string> fullHeaderNameList, shortHeaderNameList;
  h5geo::getTraceHeaderNames(fullHeaderNameList, shortHeaderNameList);
  size_t nTraceHeaderNames = fullHeaderNameList.size();

  std::vector<size_t> count = {size_t(nTraceHeaderNames), nTrc};
  std::vector<hsize_t> cdims = {size_t(nTraceHeaderNames), trcChunk};
  std::vector<size_t> max_count = {
    h5gt::DataSpace::UNLIMITED, h5gt::DataSpace::UNLIMITED};

  h5gt::DataSetCreateProps props;
  props.setChunk(cdims);
  h5gt::DataSpace dataspace(count, max_count);
  h5gt::DataSet dataset = seisGroup.createDataSet<double>(
        std::string{magic_enum::enum_name(h5geo::detail::SeisDatasets::trace_header)},
        dataspace, h5gt::LinkCreateProps(), props);
  for (size_t i = 0; i < nTraceHeaderNames; i++){
    h5gt::Attribute attribute = dataset.createAttribute<size_t>(
          shortHeaderNameList[i], h5gt::DataSpace(1));
    attribute.write(i);
  }
  return dataset;
}

std::optional<h5gt::DataSet>
H5BaseImpl::createBoundary(
    h5gt::Group &seisGroup,
    const hsize_t& stdChunk)
{
  std::vector<size_t> count = {2, 1};
  std::vector<hsize_t> cdims = {2, stdChunk};
  std::vector<size_t> max_count = {2, h5gt::DataSpace::UNLIMITED};

  h5gt::DataSetCreateProps props;
  props.setChunk(cdims);
  h5gt::DataSpace dataspace(count, max_count);
  h5gt::DataSet dataset = seisGroup.createDataSet<double>(
        std::string{magic_enum::enum_name(h5geo::detail::SeisDatasets::boundary)},
        dataspace, h5gt::LinkCreateProps(), props);
  return dataset;
}

std::optional<h5gt::Group>
H5BaseImpl::createSort(
    h5gt::Group &seisGroup)
{
  h5gt::Group sortGroup = seisGroup.createGroup(
        std::string{magic_enum::enum_name(h5geo::detail::SeisGroups::sort)});
  h5gt::Group indexesGroup = sortGroup.createGroup(
        std::string{magic_enum::enum_name(h5geo::detail::SeisGroups::indexes)});
  h5gt::Group uValGroup = sortGroup.createGroup(
        std::string{magic_enum::enum_name(h5geo::detail::SeisGroups::unique_values)});
  return sortGroup;
}

bool H5BaseImpl::isSuccessor(
    h5gt::Group parentG, h5gt::Group childG)
{
  return isSuccessor(parentG.getPath(), childG.getPath());
}

bool H5BaseImpl::isSuccessor(
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
