#ifndef H5CORE_H
#define H5CORE_H

#include "../h5geo_export.h"
#include "h5core_enum.h"
#include "../h5algo/h5deviation.h"
#include "../h5algo/h5easyhull.h"
#include "../h5algo/h5find.h"
#include "../h5algo/h5sort.h"

#include <type_traits>
#include <string>
#include <vector>
#include <regex>
#include <optional>

#include <Eigen/Dense>

#include <H5File.hpp>
#include <H5Group.hpp>
#include <H5DataSet.hpp>
#include <H5DataSpace.hpp>
#include <H5Attribute.hpp>

namespace h5geo
{
std::regex H5GEO_EXPORT fileNameRegExp();
std::regex H5GEO_EXPORT fileNameNoSpaceRegExp();
std::regex H5GEO_EXPORT floatRegExp();

std::string H5GEO_EXPORT fileNameToolTipText();
std::string H5GEO_EXPORT fileNameNoSpaceToolTipText();

void H5GEO_EXPORT splitHeaderNames(
    const std::vector<std::string> &headerNamesToSplit,
    std::vector<std::string> &fullHeaderNames, std::vector<std::string> &shortHeaderNames);

void H5GEO_EXPORT getTraceHeaderNames(
    std::vector<std::string> &fullHeaderNames,
    std::vector<std::string> &shortHeaderNames);
void H5GEO_EXPORT getBinHeaderNames(
    std::vector<std::string> &fullHeaderNames,
    std::vector<std::string> &shortHeaderNames);

size_t H5GEO_EXPORT getTraceHeaderCount();
size_t H5GEO_EXPORT getBinHeaderCount();

/*!
 * \brief generateName generates unique name by adding "_i"
 * \param nameList
 * \param baseName if empty then it will be replaced by "no_name"
 * \return
 */
std::string H5GEO_EXPORT generateName(
    const std::vector<std::string> &nameList,
    std::string baseName = std::string());

std::vector<std::string> H5GEO_EXPORT splitString(
    const std::string &s, const std::string delimiter);

/*!
 * \brief isFileExist Check is file exist in filesystem
 * \param name
 * \return
 */
bool H5GEO_EXPORT isFileExist (const std::string& name);

/*!
 * \brief getIndexFromAttribute Get row/col from Datasets with attributes
 * where attribute reflects the row/col index (like tables)
 * \param dataset
 * \param attributeName
 * \return
   */
ptrdiff_t H5GEO_EXPORT getIndexFromAttribute(
    h5gt::DataSet& dataset,
    const std::string& attributeName);

/*!
 * \brief splitPathSplit path of type /path///to/where/things/happen//
 * to output vector {"path", "to", "where", "things", "happen"}.
 * Path that starts from `/` is treated as absolute
 * \param path
 * \return
 */
std::vector<std::string> H5GEO_EXPORT splitPath(
    std::string path);

/*!
 * \brief splitPath Split path of type /path///to/where/things/happen//
 * to output vector {"path", "to", "where", "things", "happen"}
 * and to filtered path `/path/to/where/things/happen`.
 * Path that starts from `/` is treated as absolute
 * \param path
 * \param filteredPath
 * \return
 */
std::vector<std::string> H5GEO_EXPORT splitPath(
    std::string path, std::string& filteredPath);

/*!
 * \brief splitPathToParentAndObj Return path to parent
 * and object name. E.g. if `path = /a/s` then it returns
 * `/a` as path and `s` as object name.
 * If path is empty or `path = /` then both output path
 * and object name are empty strings.
 * \param path
 * \param objName
 * \return
 */
std::string H5GEO_EXPORT splitPathToParentAndObj(
    const std::string& path,
    std::string& objName);

/*!
 * \brief compareStrings Return `true` if strings are equal.
 * \param bigger
 * \param smaller
 * \param caseSensitivity
 * \return
 */
bool H5GEO_EXPORT compareStrings(
    const std::string& bigger,
    const std::string& smaller,
    const CaseSensitivity& caseSensitivity = CaseSensitivity::CASE_INSENSITIVE);

std::string H5GEO_EXPORT getRelativePath(
    const std::string& parentPath,
    const std::string& objPath,
    const CaseSensitivity& caseSensitivity = CaseSensitivity::CASE_INSENSITIVE);

/*!
 * \brief eraseSubStr First Occurrence of given  substring from main string.
 * \param mainStr
 * \param toErase
 */
void H5GEO_EXPORT eraseSubStr(
    std::string & mainStr, const std::string & toErase);

/*!
 * \brief eraseAllSubStr Erase all Occurrences of given substring from main string.
 * \param mainStr
 * \param toErase
 */
void H5GEO_EXPORT eraseAllSubStr(
    std::string & mainStr, const std::string & toErase);

/*!
 * \brief ibm2ieee Convert IBM to IEEE float format.
 * But you need to read the value as INT (not as a float).
 * \param from
 * \return
 */
float H5GEO_EXPORT ibm2ieee(const int& from);

/*!
 * \brief ebc_to_ascii_table Convert EBCDIC to ASCII.
 * \param ascii
 * \return
 */
unsigned char H5GEO_EXPORT ebc_to_ascii(
    const unsigned char& ascii);


template <class To, class From>
typename std::enable_if<
(sizeof(To) == sizeof(From)) &&
std::is_trivially_copyable<From>::value &&
std::is_trivial<To>::value,
// this implementation requires that To is trivially default constructible
To>::type
// constexpr support needs compiler magic
inline bit_cast(const From &src) noexcept
{
  To dst;
  std::memcpy(&dst, &src, sizeof(To));
  return dst;
}

template<typename Object,
         typename std::enable_if<
           std::is_base_of<Object, h5gt::File>::value |
           std::is_base_of<Object, h5gt::Group>::value |
           std::is_base_of<Object, h5gt::DataSet>::value>::type* = nullptr>
bool setEnumFromObj(
    Object& object,
    const std::string& attrName,
    const unsigned& val)
{
  try {
    h5gt::Attribute attr = object.getAttribute(attrName);
    attr.write(val);
  } catch (h5gt::Exception e) {
    return false;
  }
  return true;
}

template<typename Object,
         typename std::enable_if<
           std::is_base_of<Object, h5gt::File>::value |
           std::is_base_of<Object, h5gt::Group>::value |
           std::is_base_of<Object, h5gt::DataSet>::value>::type* = nullptr>
/*!
 * \brief getEnumFromObj Read enum from `Object's`
 * attribute as unsigned value.
 * \param object
 * \param attrName
 */
unsigned getEnumFromObj(Object& object, const std::string& attrName){
  /* as we often use magic_enum to convert enum to string
   * we need to remove `h5geo::` from enum name given
   * from magic_enum () as for example:
   * magic_enum::enum_type_name<h5geo::SurveyType>() */
//  eraseSubStr(attrName, "h5geo::");

  unsigned value;
  try {
    h5gt::Attribute attr = object.getAttribute(attrName);
    attr.read(value);
  } catch (h5gt::Exception e) {
    return 0;
  }
  return value;
}

template<typename Object,
         typename std::enable_if<
           std::is_base_of<Object, h5gt::File>::value |
           std::is_base_of<Object, h5gt::Group>::value |
           std::is_base_of<Object, h5gt::DataSet>::value>::type* = nullptr>
bool deleteAllAttributes(Object& object){
  try {
    std::vector<std::string> attrNameList =
        object.listAttributeNames();
    for (const auto& name : attrNameList)
      object.deleteAttribute(name);
  } catch (h5gt::Exception e) {
    return false;
  }
  return true;
}

template<typename Parent,
         typename std::enable_if<
           std::is_base_of<Parent, h5gt::File>::value |
           std::is_base_of<Parent, h5gt::Group>::value>::type* = nullptr>
/*!
 * \brief unlinkObject Unlink object from container
 * \param object parent object (File or Group) relatively
 * to `objPath`
 * \param objPath path to object from root
 * (like: /path/to/object)
 * \return
 */
bool unlinkObject(Parent& parent, const std::string& objPath){
  try {
    parent.unlink(objPath);
  }  catch (h5gt::Exception e) {
    return false;
  }
  return true;
}

template<typename Object,
         typename std::enable_if<
           std::is_base_of<Object, h5gt::File>::value |
           std::is_base_of<Object, h5gt::Group>::value>::type* = nullptr>
/*!
 * \brief unlinkContent Unlink everything in group
 * \return
 */
bool unlinkContent(Object& object){
  try {
    std::vector<std::string> objNames =
        object.listObjectNames();
    for (const auto& name : objNames)
      object.unlink(name);
  } catch (h5gt::Exception e) {
    return false;
  }
  return true;
}

template<typename T1, typename T2>
bool _overwriteResizableDataset(
    h5gt::DataSet& dataset,
    const T2* v,
    size_t nH5Rows,
    size_t nH5Cols)
{
  try {
    std::vector<size_t> dims = {nH5Rows, nH5Cols};
    dataset.resize(dims);
    dataset.write_raw(v);
    return true;
  } catch (h5gt::Exception e) {
    return false;
  }
}

template<typename D,
         typename std::enable_if<
           std::is_fundamental<typename D::Scalar>::value>::type* = nullptr>
/*!
 * \brief overwriteResizableDataset Try to resize and write matrix to dataset.
 * Matrix internally cast its type to DataSet's type
 * \param dataset
 * \param M
 * \return
 */
bool overwriteResizableDataset(
    h5gt::DataSet& dataset,
    const Eigen::DenseBase<D>& M)
{
  return _overwriteResizableDataset(dataset, M.derived().data(), M.cols, M.rows());
}

template<typename T,
         typename std::enable_if<
           std::is_fundamental<T>::value>::type* = nullptr>
/*!
 * \brief overwriteResizableDataset Try to resize and write matrix to dataset.
 * Matrix internally cast its type to DataSet's type
 * \param dataset
 * \param M
 * \return
 */
bool overwriteResizableDataset(
    h5gt::DataSet& dataset,
    const std::vector<T>& v)
{
  return _overwriteResizableDataset(dataset, v.data(), 1, v.size());
}

template<typename T,
         typename std::enable_if<
           std::is_fundamental<T>::value>::type* = nullptr>
/*!
 * \brief overwriteResizableDataset Try to resize and write matrix to dataset.
 * Matrix internally cast its type to DataSet's type
 * \param dataset
 * \param M
 * \return
 */
bool overwriteResizableDataset(
    h5gt::DataSet& dataset,
    const T& v)
{
  return _overwriteResizableDataset(dataset, &v, 1, 1);
}

template<typename Object, typename D,
         typename std::enable_if<
           (std::is_base_of<Object, h5gt::File>::value |
           std::is_base_of<Object, h5gt::Group>::value) &
           std::is_fundamental<typename D::Scalar>::value>::type* = nullptr>
/*!
 * \brief overwriteDataset If dataset exists then it will be unlinked
 * and then created again.
 * If path to dataset doesn't exist then it will create it.
 * Write matrix to a created dataset.
 * \param node Reference node for dataset
 * \param datasetPath May contain / symbol wich is treated as
 * path/to/datasetPath
 * \param M
 * \return
 */
bool overwriteDataset(
    Object& node,
    std::string& datasetPath,
    const Eigen::DenseBase<D>& M)
{
  return _overwriteDataset(node, datasetPath, M.derived().data(), M.cols(), M.rows());
}

template<typename Object, typename T,
         typename std::enable_if<
           (std::is_base_of<Object, h5gt::File>::value |
           std::is_base_of<Object, h5gt::Group>::value) &
           std::is_fundamental<T>::value>::type* = nullptr>
/*!
 * \brief overwriteDataset If dataset exists then it will be unlinked
 * and then created again.
 * If path to dataset doesn't exist then it will create it.
 * Write matrix to a created dataset.
 * \param node Reference node for dataset
 * \param datasetPath May contain / symbol wich is treated as
 * path/to/datasetPath
 * \param M
 * \return
 */
bool overwriteDataset(
    Object& node,
    std::string& datasetPath,
    const std::vector<T>& v)
{
  return _overwriteDataset(node, datasetPath, v.data(), 1, v.size());
}

template<typename Object, typename T,
         typename std::enable_if<
           (std::is_base_of<Object, h5gt::File>::value |
           std::is_base_of<Object, h5gt::Group>::value) &
           std::is_fundamental<T>::value>::type* = nullptr>
/*!
 * \brief overwriteDataset If dataset exists then it will be unlinked
 * and then created again.
 * If path to dataset doesn't exist then it will create it.
 * Write matrix to a created dataset.
 * \param node Reference node for dataset
 * \param datasetPath May contain / symbol wich is treated as
 * path/to/datasetPath
 * \param M
 * \return
 */
bool overwriteDataset(
    Object& node,
    std::string& datasetPath,
    const T& v)
{
  return _overwriteDataset(node, datasetPath, &v, 1, 1);
}

template <typename T1, typename T2>
bool _overwriteAttribute(
    T1& holder,
    const std::string& attrName,
    const T2* v,
    size_t nElem)
{
  if (holder.hasAttribute(attrName))
    holder.deleteAttribute(attrName);

  try {
    holder.template createAttribute<T2>(
          attrName, h5gt::DataSpace({nElem})).
        write_raw(v);
  }  catch (h5gt::Exception e) {
    return false;
  }

  return true;
}

template<typename Object, typename D,
         typename std::enable_if<
           (std::is_base_of<Object, h5gt::File>::value |
           std::is_base_of<Object, h5gt::Group>::value |
           std::is_base_of<Object, h5gt::DataSet>::value) &
           std::is_fundamental<typename D::Scalar>::value>::type* = nullptr>
/*!
 * \brief overwriteAttribute Delete attribute if already exist and create
 * new one. Write data to it
 * \param holder h5gt::Object that contains given attribute
 * \param attrName
 * \param v
 */
bool overwriteAttribute(
    Object& holder,
    const std::string& attrName,
    const Eigen::DenseBase<D>& v)
{
  return _overwriteAttribute(holder, attrName, v.derived().data(), v.size());
}

template <typename Object, typename T,
          typename std::enable_if<
            (std::is_base_of<Object, h5gt::File>::value |
            std::is_base_of<Object, h5gt::Group>::value |
            std::is_base_of<Object, h5gt::DataSet>::value) &
            std::is_fundamental<T>::value>::type* = nullptr>
/*!
 * \brief overwriteAttribute Delete attribute if already exist and create
 * new one. Write data to it
 * \param holder h5gt::Object that contains given attribute
 * \param attrName
 * \param v
 */
bool overwriteAttribute(
    Object& holder,
    const std::string& attrName,
    const std::vector<T>& v)
{
  return _overwriteAttribute(holder, attrName, v.data(), v.size());
}

template <typename Object, typename T,
          typename std::enable_if<
            (std::is_base_of<Object, h5gt::File>::value |
            std::is_base_of<Object, h5gt::Group>::value |
            std::is_base_of<Object, h5gt::DataSet>::value) &
            std::is_fundamental<T>::value>::type* = nullptr>
/*!
 * \brief overwriteAttribute Delete attribute if already exist and create
 * new one. Write data to it
 * \param holder h5gt::Object that contains given attribute
 * \param attrName
 * \param v
 */
bool overwriteAttribute(
    Object& holder,
    const std::string& attrName,
    const T& v)
{
  return _overwriteAttribute(holder, attrName, &v, 1);
}

template<typename D>
/*!
 * \brief writeData2IndexedDataset Try to write vector to dataset with
 * attribute where attribute is a single value reflecting
 * row index of a corresponding dataset
 * \param dataset
 * \param v
 * \param attrName
 * \param resize
 * \return
 */
bool writeData2IndexedDataset(
    h5gt::DataSet& dataset,
    const std::string& attrName,
    const Eigen::DenseBase<D>& v,
    bool resize = false)
{
  if (v.size() == 0)
    return false;

  ptrdiff_t ind = h5geo::getIndexFromAttribute(
        dataset, attrName);

  if (ind < 0)
    return false;

  std::vector dims = dataset.getDimensions();

  if (resize == false &&
      dims[1] < v.size())
    return false;

  if (resize == true  &&
      dims[1] < v.size())
    dataset.resize({dims[0], size_t(v.size())});

  try {
    dataset.select({size_t(ind), 0}, {1, size_t(v.size())}).
        write_raw(v.derived().data());
  } catch (h5gt::Exception e) {
    return false;
  }

  return true;
}

template<typename T>
/*!
 * \brief getDataFromIndexedDataset Try to read data to vector
 * from dataset with attribute where attribute is a single
 * value reflecting row index of a corresponding dataset
 * \param dataset
 * \param attrName
 * \return
 */
Eigen::VectorX<T> getDataFromIndexedDataset(
    h5gt::DataSet& dataset,
    const std::string& attrName)
{
  ptrdiff_t ind = h5geo::getIndexFromAttribute(
        dataset, attrName);

  if (ind < 0)
    return Eigen::VectorXd();

  std::vector dims = dataset.getDimensions();
  Eigen::VectorX<T> v(dims[1]);

  try {
    dataset.select({size_t(ind), 0}, {1, dims[1]}).
        read(v.derived().data());
  } catch (h5gt::Exception e) {
    return Eigen::VectorX<T>();
  }

  return v;
}

template <typename T>
std::string _preparePath(
    T& node,
    const std::string& path,
    bool isPathToDataset)
{
  if (path.empty())
    return std::string();

  std::vector<std::string> splittedPath = splitPath(path);

  if (splittedPath.empty())
    return std::string();

  size_t I;
  if (isPathToDataset)
    I = splittedPath.size()-1;
  else
    I = splittedPath.size();

  if (I == 0)
    return std::string();

  std::string groupPath;
  if (path[0] == '/')
    groupPath = '/';

  for (size_t i = 0; i < I; i++){
    if (i < I-1)
      groupPath += splittedPath[i] + '/';
    else
      groupPath += splittedPath[i];
  }

  if (!node.exist(groupPath) ||
      node.getObjectType(groupPath) != h5gt::ObjectType::Group){
    node.createGroup(groupPath);
  }

  return groupPath;
}

template<typename Object,
         typename std::enable_if<
           std::is_base_of<Object, h5gt::File>::value |
           std::is_base_of<Object, h5gt::Group>::value>::type* = nullptr>
/*!
 * \brief preparePath Creates all inetermediate groups to the
 * destination path.
 * Path that starts from `/` is treated as absolute
 * \param node
 * \param path
 * \param isPathToDataset
 * \return path to parent Group. If there is no Group parent
 * then it returns empty string
 */
std::string preparePath(
    Object& node,
    const std::string& path,
    bool isPathToDataset)
{
  return _preparePath(node, path, isPathToDataset);
}

template<typename Parent,
         typename std::enable_if<
           std::is_base_of<Parent, h5gt::File>::value |
           std::is_base_of<Parent, h5gt::Group>::value>::type* = nullptr>
/*!
 * \brief createGroup Create new group or get already existent
 * one based on the given `createFlag`
 * \param parent
 * \param objPath
 * \param createFlag
 * \return
 */
std::optional<h5gt::Group>
createGroup(
    Parent& parent,
    std::string& objPath,
    const h5geo::CreationType& createFlag)
{
  if (objPath.empty() &&
      createFlag != h5geo::CreationType::CREATE_UNDER_NEW_NAME)
    return std::nullopt;

  if (createFlag == h5geo::CreationType::CREATE_UNDER_NEW_NAME){
    std::string objName;
    std::string closestParentPath =
        h5geo::splitPathToParentAndObj(parent.getPath() + "/" + objPath, objName);
    // create all intermediate groups
    closestParentPath = preparePath(parent, closestParentPath, false);

    if (closestParentPath.empty()){
      objName = h5geo::generateName(parent.listObjectNames(), objName);
    } else {
      objName = h5geo::generateName(
            parent.getGroup(closestParentPath).listObjectNames(), objName);
    }

    objPath = closestParentPath + "/" + objName;
  }

  switch (createFlag) {
  case h5geo::CreationType::OPEN_OR_CREATE :
    if (parent.hasObject(objPath, h5gt::ObjectType::Group))
      return parent.getGroup(objPath);
    return parent.createGroup(objPath);
  case h5geo::CreationType::CREATE_OR_OVERWRITE :
    if (parent.hasObject(objPath, h5gt::ObjectType::Group)){
      h5gt::Group group = parent.getGroup(objPath);
      h5geo::deleteAllAttributes(group);
      h5geo::unlinkContent(group);
      return group;
    }
    return parent.createGroup(objPath);
  case h5geo::CreationType::CREATE_UNDER_NEW_NAME :
    return parent.createGroup(objPath);
  }
}

template<typename T1, typename T2>
bool _overwriteDataset(
    T1& node,
    std::string& datasetPath,
    const T2* v,
    size_t nH5Rows,
    size_t nH5Cols)
{
  if (node.exist(datasetPath) &&
      node.getObjectType(datasetPath) == h5gt::ObjectType::Dataset)
    node.unlink(datasetPath);

  datasetPath = preparePath(
        node, datasetPath, true);

  if (datasetPath.empty())
    return false;

  try {
    node.template createDataSet<T2>(
          datasetPath, h5gt::DataSpace({nH5Rows, nH5Cols})).
        write_raw(v);
  }  catch (h5gt::Exception e) {
    return false;
  }

  return true;
}

template<typename T>
h5gt::ElementSet
rowCol2ElementSet(
    const T& row,
    const Eigen::VectorX<T>& cols)
{
  ptrdiff_t I = cols.size();
  std::vector<size_t> v(2*I, row);

  for (ptrdiff_t i = 0; i < I; i++)
    v[2*i+1] = cols(i);

  return h5gt::ElementSet(v);
}

template<typename T>
h5gt::ElementSet rowCol2ElementSet(
    const Eigen::VectorX<T>& rows,
    const T& col)
{
  ptrdiff_t I = rows.size();
  std::vector<size_t> v(2*I, col);

  for (ptrdiff_t i = 0; i < I; i++)
    v[2*i] = rows(i);

  return h5gt::ElementSet(v);
}

template<typename T>
/*!
 * \brief rowCol2ElementSet select rectilinear block of elements, i.e.
 * uses double loop to select every possible row-col intersection
 * \param rows
 * \param cols
 * \return
 */
h5gt::ElementSet rowCol2ElementSet(
    const Eigen::VectorX<T>& rows,
    const Eigen::VectorX<T>& cols)
{
  ptrdiff_t I = rows.size();
  ptrdiff_t J = cols.size();

  std::vector<size_t> v(2*I*J);

  for (ptrdiff_t j = 0; j < J; j++){
    for (ptrdiff_t i = 0; i < I; i++){
      v[2*i + 2*j*I] = rows(i);
      v[2*i+1 + 2*j*I] = cols(j);
    }
  }

  return h5gt::ElementSet(v);
}

template<typename T>
h5gt::ElementSet
rowCol2ElementSet(
    const T& row,
    const std::vector<T>& cols)
{
  size_t I = cols.size();
  std::vector<size_t> v(2*I, row);

  for (size_t i = 0; i < I; i++)
    v[2*i+1] = cols[i];

  return h5gt::ElementSet(v);
}

template<typename T>
h5gt::ElementSet
rowCol2ElementSet(
    const std::vector<T>& rows,
    const T& col)
{
  size_t I = rows.size();
  std::vector<size_t> v(2*I, col);

  for (size_t i = 0; i < I; i++)
    v[2*i] = rows[i];

  return h5gt::ElementSet(v);
}

template<typename T>
/*!
 * \brief rowCol2ElementSet select rectilinear block of elements, i.e.
 * uses double loop to select every possible row-col intersection
 * \param rows
 * \param cols
 * \return
 */
h5gt::ElementSet
rowCol2ElementSet(
    const std::vector<T>& rows,
    const std::vector<T>& cols)
{
  size_t I = rows.size();
  size_t J = cols.size();

  std::vector<size_t> v(2*I*J);

  for (size_t j = 0; j < J; j++){
    for (size_t i = 0; i < I; i++){
      v[2*i + 2*j*I] = rows[i];
      v[2*i+1 + 2*j*I] = cols[i];
    }
  }

  return h5gt::ElementSet(v);
}

} // namespace h5geo


#endif // H5CORE_H
