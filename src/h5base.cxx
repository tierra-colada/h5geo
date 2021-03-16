#include "../include/h5base/h5base.h"

H5Base::H5Base()
{

}

H5Base::~H5Base(){

}

std::vector<h5gt::Group>
H5Base::getChildList(h5gt::Group& group,
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

bool H5Base::isObject(
    h5gt::Group &group,
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
  }
}

bool H5Base::isSurf(
    h5gt::Group &group)
{
  constexpr auto& dataset_names =
      magic_enum::enum_names<h5geo::SurfDatasets>();

  for (const auto& name : dataset_names){
    if (!group.hasObject(std::string{name}, h5gt::ObjectType::Dataset))
      return false;
  }
  return true;
}

bool H5Base::isWell(
    h5gt::Group &group)
{
  constexpr auto& group_names =
      magic_enum::enum_names<h5geo::WellGroups>();

  for (const auto& name : group_names){
    if (!group.hasObject(std::string{name}, h5gt::ObjectType::Group))
      return false;
  }
  return true;
}

bool H5Base::isLogCurve(
    h5gt::Group &group)
{
  constexpr auto& dataset_names =
      magic_enum::enum_names<h5geo::LogDatasets>();

  for (const auto& name : dataset_names){
    if (!group.hasObject(std::string{name}, h5gt::ObjectType::Dataset))
      return false;
  }
  return true;
}

bool H5Base::isDevCurve(
    h5gt::Group &group)
{
  constexpr auto& dataset_names =
      magic_enum::enum_names<h5geo::DevDatasets>();

  for (const auto& name : dataset_names){
    if (!group.hasObject(std::string{name}, h5gt::ObjectType::Dataset))
      return false;
  }
  return true;
}

bool H5Base::isSeis(
    h5gt::Group &group)
{
  constexpr auto& attr_names =
      magic_enum::enum_names<h5geo::SeisAttributes>();
  constexpr auto& dataset_names =
      magic_enum::enum_names<h5geo::SeisDatasets>();

  for (const auto& name : attr_names){
    if (!group.hasAttribute(std::string{name}))
      return false;
  }

  for (const auto& name : dataset_names){
    if (!group.hasObject(std::string{name}, h5gt::ObjectType::Dataset))
      return false;
  }

  std::string sortG_name =
      std::string{magic_enum::enum_name(h5geo::SeisGroups::sort)};
  std::string indexesG_name =
      std::string{magic_enum::enum_name(h5geo::SeisGroups::indexes)};
  std::string unique_valuesG_name =
      std::string{magic_enum::enum_name(h5geo::SeisGroups::unique_values)};

  if (!group.hasObject(sortG_name,
        h5gt::ObjectType::Group))
    return false;

  h5gt::Group sortG = group.getGroup(sortG_name);
  if (!sortG.hasObject(indexesG_name, h5gt::ObjectType::Group))
    return false;

  if (!sortG.hasObject(unique_valuesG_name, h5gt::ObjectType::Group))
    return false;

  return true;
}

std::optional<h5gt::Group>
H5Base::createObject(
    std::string& objName,
    h5gt::File& parentFile,
    const h5geo::ObjectType& objType,
    void* p,
    const h5geo::CreationType createFlag)
{
  std::optional<h5gt::Group> opt =
      h5geo::createGroup(parentFile, objName, createFlag);

  if (!opt.has_value())
    return std::nullopt;

  return createObject(opt.value(), objType, p, createFlag);
}

std::optional<h5gt::Group>
H5Base::createObject(
    std::string& objName,
    h5gt::Group& parentGroup,
    const h5geo::ObjectType& objType,
    void* p,
    const h5geo::CreationType createFlag)
{
  std::optional<h5gt::Group> opt =
      h5geo::createGroup(parentGroup, objName, createFlag);

  if (!opt.has_value())
    return std::nullopt;

  return createObject(opt.value(), objType, p, createFlag);
}

std::optional<h5gt::Group>
H5Base::createObject(
    h5gt::Group& group,
    const h5geo::ObjectType& objType,
    void* p,
    const h5geo::CreationType createFlag)
{
  switch (createFlag) {
  case h5geo::CreationType::OPEN_OR_CREATE :
    if (isObject(group, objType))
      return group;
    return createNewObject(group, objType, p);
  case h5geo::CreationType::CREATE_OR_OVERWRITE :
    return createNewObject(group, objType, p);
  case h5geo::CreationType::CREATE_UNDER_NEW_NAME :
    return createNewObject(group, objType, p);
  }
}

std::optional<h5gt::Group>
/*!
 * \brief H5Base::createNewObject This method should work only
 * with clean groups that don't have any attributes or content
 * \param group is going to be object group (objG)
 * \param objType
 * \param p
 */
H5Base::createNewObject(
    h5gt::Group &group,
    const h5geo::ObjectType& objType,
    void* p)
{
  if (!h5geo::unlinkContent(group))
    return std::nullopt;
  if (!h5geo::deleteAllAttributes(group))
    return std::nullopt;

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
  }
}

std::optional<h5gt::Group>
H5Base::createNewSurf(h5gt::Group &group, void* p)
{
  SurfParam param = *(static_cast<SurfParam*>(p));
  std::vector<double> origin({param.X0, param.Y0});
  std::vector<double> spacing({param.dX, param.dY});

  group.createAttribute<double>(
        std::string{magic_enum::enum_name(h5geo::SurfAttributes::origin)},
        h5gt::DataSpace({2})).
      write(origin);

  group.createAttribute<double>(
        std::string{magic_enum::enum_name(h5geo::SurfAttributes::spacing)},
        h5gt::DataSpace({2})).
      write(spacing);

  group.createDataSet<double>(
        std::string{magic_enum::enum_name(h5geo::SurfDatasets::surf_data)},
        h5gt::DataSpace({param.nX, param.nY}));

  return group;
}

std::optional<h5gt::Group>
H5Base::createNewWell(h5gt::Group &group, void* p)
{
  WellParam param = *(static_cast<WellParam *>(p));
  std::vector<double> head_coord({param.headY, param.headX});

  group.createAttribute<double>(
        std::string{magic_enum::enum_name(h5geo::WellAttributes::head_coord)},
        h5gt::DataSpace({2})).
      write(head_coord);
  group.createAttribute<double>(
        std::string{magic_enum::enum_name(h5geo::WellAttributes::KB)},
        h5gt::DataSpace(1)).
      write(param.kb);
  group.createAttribute<std::string>(
        std::string{magic_enum::enum_name(h5geo::WellAttributes::UWI)},
        h5gt::DataSpace(1)).
      write(param.uwi);

  constexpr auto& group_names =
      magic_enum::enum_names<h5geo::WellGroups>();

  for (const auto& name : group_names){
    group.createGroup(std::string{name});
  }

  return group;
}

std::optional<h5gt::Group>
H5Base::createNewLogCurve(h5gt::Group &group, void* p)
{
  LogCurveParam param = *(static_cast<LogCurveParam*>(p));
  std::vector<size_t> count = {2, 1};
  std::vector<hsize_t> cdims = {2, param.chunkSize};
  std::vector<size_t> max_count = {2, h5gt::DataSpace::UNLIMITED};
  h5gt::DataSetCreateProps props;
  props.setChunk(cdims);
  h5gt::DataSpace dataspace(count, max_count);

  h5gt::DataSet dataset =
      group.createDataSet<double>(
        std::string{magic_enum::enum_name(h5geo::LogDatasets::log_data)},
        dataspace, h5gt::LinkCreateProps(), props);
  dataset.createAttribute<size_t>(
        std::string{magic_enum::enum_name(h5geo::LogDataType::MD)}, h5gt::DataSpace(1)).write(0);
  dataset.createAttribute<size_t>(
        std::string{magic_enum::enum_name(h5geo::LogDataType::VAL)}, h5gt::DataSpace(1)).write(1);

  return group;
}

std::optional<h5gt::Group>
H5Base::createNewDevCurve(h5gt::Group &group, void* p)
{
  DevCurveParam param = *(static_cast<DevCurveParam*>(p));
  std::vector<size_t> count = {5, 1};
  std::vector<hsize_t> cdims = {5, param.chunkSize};
  std::vector<size_t> max_count = {5, h5gt::DataSpace::UNLIMITED};
  h5gt::DataSetCreateProps props;
  props.setChunk(cdims);
  h5gt::DataSpace dataspace(count, max_count);

  h5gt::DataSet dataset =
      group.createDataSet<double>(
        std::string{magic_enum::enum_name(h5geo::DevDatasets::dev_data)},
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
H5Base::createNewSeis(h5gt::Group &group, void* p)
{
  SeisParam param = *(static_cast<SeisParam*>(p));

  group.createAttribute<unsigned>(
        std::string{magic_enum::enum_name(h5geo::SeisAttributes::DataType)},
        h5gt::DataSpace(1)).
      write(static_cast<unsigned>(param.dataType));
  group.createAttribute<unsigned>(
        std::string{magic_enum::enum_name(h5geo::SeisAttributes::Domain)},
        h5gt::DataSpace(1)).
      write(static_cast<unsigned>(param.domain));
  group.createAttribute<unsigned>(
        std::string{magic_enum::enum_name(h5geo::SeisAttributes::SurveyType)},
        h5gt::DataSpace(1)).
      write(static_cast<unsigned>(param.surveyType));
  group.createAttribute<double>(
        std::string{magic_enum::enum_name(h5geo::SeisAttributes::SRD)},
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
H5Base::createTextHeader(
    h5gt::Group &seisGroup)
{
  char txtHdr[40][80];
  h5gt::DataSet dataset = seisGroup.createDataSet<char[80]>(
        std::string{magic_enum::enum_name(h5geo::SeisDatasets::text_header)},
        h5gt::DataSpace::FromCharArrayStrings(txtHdr));
  return dataset;
}

std::optional<h5gt::DataSet>
H5Base::createBinHeader(
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
        std::string{magic_enum::enum_name(h5geo::SeisDatasets::bin_header)},
        dataspace, h5gt::LinkCreateProps(), props);
  for (size_t i = 0; i < nBinHeaderNames; i++){
    h5gt::Attribute attribute = dataset.createAttribute<size_t>(
          shortHeaderNameList[i], h5gt::DataSpace(1));
    attribute.write(i);
  }
  return dataset;
}

std::optional<h5gt::DataSet>
H5Base::createTrace(
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
        std::string{magic_enum::enum_name(h5geo::SeisDatasets::trace)},
        dataspace, h5gt::LinkCreateProps(), props);
  return dataset;
}

std::optional<h5gt::DataSet>
H5Base::createTraceHeader(
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
        std::string{magic_enum::enum_name(h5geo::SeisDatasets::trace_header)},
        dataspace, h5gt::LinkCreateProps(), props);
  for (size_t i = 0; i < nTraceHeaderNames; i++){
    h5gt::Attribute attribute = dataset.createAttribute<size_t>(
          shortHeaderNameList[i], h5gt::DataSpace(1));
    attribute.write(i);
  }
  return dataset;
}

std::optional<h5gt::DataSet>
H5Base::createBoundary(
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
        std::string{magic_enum::enum_name(h5geo::SeisDatasets::boundary)},
        dataspace, h5gt::LinkCreateProps(), props);
  return dataset;
}

std::optional<h5gt::Group>
H5Base::createSort(
    h5gt::Group &seisGroup)
{
  h5gt::Group sortGroup = seisGroup.createGroup(
        std::string{magic_enum::enum_name(h5geo::SeisGroups::sort)});
  h5gt::Group indexesGroup = sortGroup.createGroup(
        std::string{magic_enum::enum_name(h5geo::SeisGroups::indexes)});
  h5gt::Group uValGroup = sortGroup.createGroup(
        std::string{magic_enum::enum_name(h5geo::SeisGroups::unique_values)});
  return sortGroup;
}

