#ifndef H5CORE_H
#define H5CORE_H

#include "misc/h5core_enum.h"

#include <type_traits>
#include <string>
#include <vector>
#include <regex>

#include <Eigen/Dense>

#include <h5gt/H5File.hpp>
#include <h5gt/H5Group.hpp>
#include <h5gt/H5DataSet.hpp>
#include <h5gt/H5DataSpace.hpp>
#include <h5gt/H5Attribute.hpp>

namespace h5geo
{

std::vector<std::string> getRawBinHeaderNames();
std::vector<std::string> getRawTraceHeaderNames();

/*!
 * \brief compareStrings Return `true` if strings are equal.
 * \param bigger
 * \param smaller
 * \param caseSensitivity
 * \return
 */
bool compareStrings(
    const std::string& bigger,
    const std::string& smaller,
    const CaseSensitivity& caseSensitivity = CaseSensitivity::CASE_INSENSITIVE);

/*!
 * \brief eraseSubStr First Occurrence of given  substring from main string.
 * \param mainStr
 * \param toErase
 */
void eraseSubStr(
    std::string & mainStr, const std::string & toErase);

/*!
 * \brief eraseAllSubStr Erase all Occurrences of given substring from main string.
 * \param mainStr
 * \param toErase
 */
void eraseAllSubStr(
    std::string & mainStr, const std::string & toErase);

char getDelimiter(
    const Delimiter& delimiter);

/*!
 * \brief generateName generates unique name by adding "_i"
 * \param nameList
 * \param baseName if empty then it will be replaced by "no_name"
 * \return
 */
std::string generateName(
    const std::vector<std::string> &nameList,
    std::string baseName = std::string());

std::vector<std::string> splitString(
    const std::string &s, const std::string delimiter);

void splitHeaderNames(
    const std::vector<std::string> &headerNamesToSplit,
    std::vector<std::string> &fullHeaderNames,
    std::vector<std::string> &shortHeaderNames);

/*!
 * \brief splitPath Split path of type /path///to/where/things/happen//
 * to output vector {"path", "to", "where", "things", "happen"}.
 * Path that starts from `/` is treated as absolute
 * \param path
 * \return
 */
std::vector<std::string> splitPath(
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
std::vector<std::string> splitPath(
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
std::string splitPathToParentAndObj(
    const std::string& path,
    std::string& objName);

std::string getRelativePath(
    const std::string& parentPath,
    const std::string& objPath,
    const CaseSensitivity& caseSensitivity = CaseSensitivity::CASE_INSENSITIVE);

void getTraceHeaderNames(
    std::vector<std::string> &fullHeaderNames,
    std::vector<std::string> &shortHeaderNames);
void getBinHeaderNames(
    std::vector<std::string> &fullHeaderNames,
    std::vector<std::string> &shortHeaderNames);

size_t getTraceHeaderCount();
size_t getBinHeaderCount();

/*!
 * \brief getIndexFromAttribute Get row/col from Datasets with attributes
 * where attribute reflects the row/col index (like tables)
 * \param dataset
 * \param attributeName
 * \return
 */
ptrdiff_t getIndexFromAttribute(
    h5gt::DataSet& dataset,
    const std::string& attributeName);

template<typename Object,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value>::type* = nullptr>
bool setEnumFromObj(
    Object& object,
    const std::string& attrName,
    const unsigned& val);

template<typename Object,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value>::type* = nullptr>
/*!
 * \brief getEnumFromObj Read enum from `Object's`
 * attribute as unsigned value. Return `0` if attribute
 * not exists.
 * \param object
 * \param attrName
 */
unsigned getEnumFromObj(Object& object, const std::string& attrName);

template<typename Object,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value>::type* = nullptr>
bool deleteAllAttributes(Object& object);

template<typename Parent,
         typename std::enable_if<
           std::is_same<Parent, h5gt::File>::value ||
           std::is_same<Parent, h5gt::Group>::value>::type* = nullptr>
/*!
 * \brief unlinkObject Unlink object from container
 * \param object parent object (File or Group) relatively
 * to `objPath`
 * \param objPath path to object from root
 * (like: /path/to/object)
 * \return
 */
bool unlinkObject(Parent& parent, const std::string& objPath);

template<typename Object,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value>::type* = nullptr>
/*!
 * \brief unlinkContent Unlink everything in group
 * \return
 */
bool unlinkContent(Object& object);

template<typename T1, typename T2>
bool _overwriteResizableDataset(
    h5gt::DataSet& dataset,
    const T2* v,
    size_t nH5Rows,
    size_t nH5Cols);

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
    const Eigen::DenseBase<D>& M);

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
    const std::vector<T>& v);

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
    const T& v);

template<typename Object, typename D,
         typename std::enable_if<
           (std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value) &
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
    const Eigen::DenseBase<D>& M);

template<typename Object, typename T,
         typename std::enable_if<
           (std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value) &
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
    const std::vector<T>& v);

template<typename Object, typename T,
         typename std::enable_if<
           (std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value) &
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
    const T& v);

template <typename T1, typename T2>
bool _overwriteAttribute(
    T1& holder,
    const std::string& attrName,
    const T2* v,
    size_t nElem);

template<typename Object, typename D,
         typename std::enable_if<
           (std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value) &
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
    const Eigen::DenseBase<D>& v);

template <typename Object, typename T,
          typename std::enable_if<
            (std::is_same<Object, h5gt::File>::value ||
            std::is_same<Object, h5gt::Group>::value ||
            std::is_same<Object, h5gt::DataSet>::value) &
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
    const std::vector<T>& v);

template <typename Object, typename T,
          typename std::enable_if<
            (std::is_same<Object, h5gt::File>::value ||
            std::is_same<Object, h5gt::Group>::value ||
            std::is_same<Object, h5gt::DataSet>::value) &
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
    const T& v);

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
    bool resize);

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
    const std::string& attrName);

template<typename T>
h5gt::ElementSet
rowCol2ElementSet(
    const T& row,
    const Eigen::VectorX<T>& cols);

template<typename T>
h5gt::ElementSet rowCol2ElementSet(
    const Eigen::VectorX<T>& rows,
    const T& col);

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
    const Eigen::VectorX<T>& cols);

template<typename T>
h5gt::ElementSet
rowCol2ElementSet(
    const T& row,
    const std::vector<T>& cols);

template<typename T>
h5gt::ElementSet
rowCol2ElementSet(
    const std::vector<T>& rows,
    const T& col);

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
    const std::vector<T>& cols);

} // namespace h5geo

#include "misc/h5core_misc.h"


#endif // H5CORE_H
