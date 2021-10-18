#ifndef H5CORE_H
#define H5CORE_H

#ifndef H5GT_USE_EIGEN
#define H5GT_USE_EIGEN // should be defined before including h5gt
#endif

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

void splitHeaderBytes(
    const std::vector<std::string> &headerNamesToSplit,
    std::vector<int> &bytesStart,
    std::vector<int> &nBytes);

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

void getTraceHeaderBytes(
    std::vector<int> &bytesStart,
    std::vector<int> &nBytes);
void getBinHeaderBytes(
    std::vector<int> &bytesStart,
    std::vector<int> &nBytes);

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

template<typename Object, typename T,
         typename std::enable_if<
             std::is_same<Object, h5gt::File>::value ||
             std::is_same<Object, h5gt::Group>::value>::type* = nullptr>
bool _overwriteResizableDataset(
    Object& node,
    const std::string& datasetPath,
    T* M,
    size_t nH5Rows,
    size_t nH5Cols,
    const std::string& unitsFrom = "",
    const std::string& unitsTo = "");

template<typename Object, typename D,
         typename std::enable_if<
           (std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value) &&
           std::is_arithmetic<typename D::Scalar>::value>::type* = nullptr>
bool overwriteResizableDataset(
    Object& node,
    const std::string& datasetPath,
    Eigen::DenseBase<D>& M,
    const std::string& unitsFrom = "",
    const std::string& unitsTo = "");

template<typename Object, typename T,
         typename std::enable_if<
           (std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value) &&
           std::is_arithmetic<T>::value>::type* = nullptr>
bool overwriteResizableDataset(
    Object& node,
    const std::string& datasetPath,
    std::vector<T>& v,
    const std::string& unitsFrom = "",
    const std::string& unitsTo = "");

template<typename Object, typename T,
         typename std::enable_if<
           (std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value) &&
           std::is_arithmetic<T>::value>::type* = nullptr>
bool overwriteResizableDataset(
    Object& node,
    const std::string& datasetPath,
    T& v,
    const std::string& unitsFrom = "",
    const std::string& unitsTo = "");

template<typename Object, typename T,
         typename std::enable_if<
             std::is_same<Object, h5gt::File>::value ||
             std::is_same<Object, h5gt::Group>::value>::type* = nullptr>
bool _overwriteDataset(
    Object& node,
    const std::string& datasetPath,
    T* M,
    size_t nH5Rows,
    size_t nH5Cols,
    const std::string& unitsFrom = "",
    const std::string& unitsTo = "");

template<typename Object, typename D,
         typename std::enable_if<
           (std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value) &&
           std::is_arithmetic<typename D::Scalar>::value>::type* = nullptr>
bool overwriteDataset(
    Object& node,
    const std::string& datasetPath,
    Eigen::DenseBase<D>& M,
    const std::string& unitsFrom = "",
    const std::string& unitsTo = "");

template<typename Object, typename T,
         typename std::enable_if<
           (std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value) &&
           std::is_arithmetic<T>::value>::type* = nullptr>
bool overwriteDataset(
    Object& node,
    const std::string& datasetPath,
    std::vector<T>& v,
    const std::string& unitsFrom = "",
    const std::string& unitsTo = "");

template<typename Object, typename T,
         typename std::enable_if<
           (std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value) &&
           std::is_arithmetic<T>::value>::type* = nullptr>
bool overwriteDataset(
    Object& node,
    const std::string& datasetPath,
    T& v,
    const std::string& unitsFrom = "",
    const std::string& unitsTo = "");

template <typename Object, typename T>
bool _readDataset(
    Object& node,
    const std::string& datasetPath,
    T* M,
    size_t nElem,
    const std::string& unitsFrom = "",
    const std::string& unitsTo = "");

template<typename Object, typename D,
         typename std::enable_if<
             (std::is_same<Object, h5gt::File>::value ||
              std::is_same<Object, h5gt::Group>::value ||
              std::is_same<Object, h5gt::DataSet>::value) &&
             std::is_arithmetic<typename D::Scalar>::value>::type* = nullptr>
bool readDataset(
    Object& node,
    const std::string& datasetPath,
    Eigen::DenseBase<D>& M,
    const std::string& unitsFrom = "",
    const std::string& unitsTo = "");

template<typename Object,
         typename std::enable_if<
             std::is_same<Object, h5gt::File>::value ||
             std::is_same<Object, h5gt::Group>::value ||
             std::is_same<Object, h5gt::DataSet>::value>::type* = nullptr>
Eigen::MatrixXf readFloatEigenMtxDataset(
    Object& node,
    const std::string& datasetPath,
    const std::string& unitsFrom = "",
    const std::string& unitsTo = "");

template<typename Object,
         typename std::enable_if<
             std::is_same<Object, h5gt::File>::value ||
             std::is_same<Object, h5gt::Group>::value ||
             std::is_same<Object, h5gt::DataSet>::value>::type* = nullptr>
Eigen::MatrixXd readDoubleEigenMtxDataset(
    Object& node,
    const std::string& datasetPath,
    const std::string& unitsFrom = "",
    const std::string& unitsTo = "");

template <typename Object, typename T>
bool _readAttribute(
    Object& holder,
    const std::string& attrName,
    T* v,
    size_t nElem,
    const std::string& unitsFrom = "",
    const std::string& unitsTo = "");

template<typename Object, typename D,
         typename std::enable_if<
           (std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value) &&
           std::is_arithmetic<typename D::Scalar>::value>::type* = nullptr>
/*!
 * \brief readAttribute Read attribute only if it exist and
 * its type is the same as passed buffer
 * \param holder h5gt::Object that contains given attribute
 * \param attrName
 * \param v
 */
bool readAttribute(
    Object& holder,
    const std::string& attrName,
    Eigen::DenseBase<D>& v,
    const std::string& unitsFrom = "",
    const std::string& unitsTo = "");

template <typename Object, typename T,
          typename std::enable_if<
            (std::is_same<Object, h5gt::File>::value ||
            std::is_same<Object, h5gt::Group>::value ||
            std::is_same<Object, h5gt::DataSet>::value) &&
            std::is_arithmetic<T>::value>::type* = nullptr>
bool readAttribute(
    Object& holder,
    const std::string& attrName,
    std::vector<T>& v,
    const std::string& unitsFrom = "",
    const std::string& unitsTo = "");

template <typename Object, typename T,
          typename std::enable_if<
            (std::is_same<Object, h5gt::File>::value ||
            std::is_same<Object, h5gt::Group>::value ||
            std::is_same<Object, h5gt::DataSet>::value) &&
            std::is_arithmetic<T>::value>::type* = nullptr>
bool readAttribute(
    Object& holder,
    const std::string& attrName,
    T& v,
    const std::string& unitsFrom = "",
    const std::string& unitsTo = "");

template <typename Object, typename T>
bool _overwriteAttribute(
    Object& holder,
    const std::string& attrName,
    T* v,
    size_t nElem,
    const std::string& unitsFrom = "",
    const std::string& unitsTo = "");

template<typename Object,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value>::type* = nullptr>
bool overwriteAttribute(
    Object& holder,
    const std::string& attrName,
    const std::string& str);

template<typename Object, typename D,
         typename std::enable_if<
           (std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value) &&
           std::is_arithmetic<typename D::Scalar>::value>::type* = nullptr>
/*!
 * \brief overwriteAttribute Create attribute if not exist.
 *  Delete attribute if type or size is different
 *  and create new one and write data to it
 * \param holder h5gt::Object that contains given attribute
 * \param attrName
 * \param v
 */
bool overwriteAttribute(
    Object& holder,
    const std::string& attrName,
    Eigen::DenseBase<D>& v,
    const std::string& unitsFrom = "",
    const std::string& unitsTo = "");

template <typename Object, typename T,
          typename std::enable_if<
            (std::is_same<Object, h5gt::File>::value ||
            std::is_same<Object, h5gt::Group>::value ||
            std::is_same<Object, h5gt::DataSet>::value) &&
            std::is_arithmetic<T>::value>::type* = nullptr>
bool overwriteAttribute(
    Object& holder,
    const std::string& attrName,
    std::vector<T>& v,
    const std::string& unitsFrom = "",
    const std::string& unitsTo = "");

template <typename Object, typename T,
          typename std::enable_if<
            (std::is_same<Object, h5gt::File>::value ||
            std::is_same<Object, h5gt::Group>::value ||
            std::is_same<Object, h5gt::DataSet>::value) &&
            std::is_arithmetic<T>::value>::type* = nullptr>
bool overwriteAttribute(
    Object& holder,
    const std::string& attrName,
    T& v,
    const std::string& unitsFrom = "",
    const std::string& unitsTo = "");

template<typename Object,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value>::type* = nullptr>
/*!
 * \brief readEnumAttribute Read enum from `Object's`
 * attribute as unsigned value. Return `0` if attribute
 * not exists.
 * \param object
 * \param attrName
 */
unsigned readEnumAttribute(Object& object, const std::string& attrName);

template<typename Object,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value>::type* = nullptr>
/*!
 * \brief readStringAttribute Read string from `Object's`
 * attribute. Return `std::string()` (empty string) if attribute
 * not exists.
 * \param object
 * \param attrName
 * \return
 */
std::string readStringAttribute(Object& object, const std::string& attrName);

template<typename Object,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value>::type* = nullptr>
float readFloatAttribute(
    Object& object, const std::string& attrName,
    const std::string& unitsFrom = "",
    const std::string& unitsTo = "");

template<typename Object,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value>::type* = nullptr>
double readDoubleAttribute(
    Object& object, const std::string& attrName,
    const std::string& unitsFrom = "",
    const std::string& unitsTo = "");

template<typename Object,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value>::type* = nullptr>
std::vector<float> readFloatVecAttribute(
    Object& object, const std::string& attrName,
    const std::string& unitsFrom = "",
    const std::string& unitsTo = "");

template<typename Object,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value>::type* = nullptr>
std::vector<double> readDoubleVecAttribute(
    Object& object, const std::string& attrName,
    const std::string& unitsFrom = "",
    const std::string& unitsTo = "");

template<typename Object,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value>::type* = nullptr>
Eigen::VectorXf readFloatEigenVecAttribute(
    Object& object, const std::string& attrName,
    const std::string& unitsFrom = "",
    const std::string& unitsTo = "");

template<typename Object,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value>::type* = nullptr>
Eigen::VectorXd readDoubleEigenVecAttribute(
    Object& object, const std::string& attrName,
    const std::string& unitsFrom = "",
    const std::string& unitsTo = "");


template<typename D>
/*!
 * \brief writeData2IndexedDataset Try to write vector to dataset with
 * attribute where attribute is a single value reflecting
 * row index of a corresponding dataset
 * \param dataset
 * \param v
 * \param attrName
 * \param resize resize if needed. Use it only for resizable dataset!
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
/*!
 * \brief find_index find all non-zero elements's indices.
 * Possible usage:
 * Eigen::VectorX<ptrdiff_t> ind = find_index(M.array() > 20)
 * M.array() > 20 returns bool matrix and then we find non-zero indices
 * \param M
 * \return ind indices of nonzero elements
 */
Eigen::VectorX<ptrdiff_t> find_index(
    Eigen::DenseBase<T> const & M);

template<typename T,
         typename std::enable_if<
           std::is_arithmetic<T>::value>::type* = nullptr>
h5gt::ElementSet rowCols2ElementSet(
    const T& row,
    const Eigen::VectorX<T>& cols);

template<typename T,
         typename std::enable_if<
           std::is_arithmetic<T>::value>::type* = nullptr>
h5gt::ElementSet rowsCol2ElementSet(
    const Eigen::VectorX<T>& rows,
    const T& col);

template<typename T,
         typename std::enable_if<
           std::is_arithmetic<T>::value>::type* = nullptr>
/*!
 * \brief rowsCols2ElementSet select rectilinear block of elements, i.e.
 * uses double loop to select every possible row-col intersection
 * \param rows
 * \param cols
 * \return
 */
h5gt::ElementSet rowsCols2ElementSet(
    const Eigen::VectorX<T>& rows,
    const Eigen::VectorX<T>& cols);

template<typename T,
         typename std::enable_if<
           std::is_arithmetic<T>::value>::type* = nullptr>
h5gt::ElementSet rowCols2ElementSet(
    const T& row,
    const std::vector<T>& cols);

template<typename T,
         typename std::enable_if<
           std::is_arithmetic<T>::value>::type* = nullptr>
h5gt::ElementSet rowsCol2ElementSet(
    const std::vector<T>& rows,
    const T& col);

template<typename T,
         typename std::enable_if<
           std::is_arithmetic<T>::value>::type* = nullptr>
/*!
 * \brief rowsCols2ElementSet select rectilinear block of elements, i.e.
 * uses double loop to select every possible row-col intersection
 * \param rows
 * \param cols
 * \return
 */
h5gt::ElementSet rowsCols2ElementSet(
    const std::vector<T>& rows,
    const std::vector<T>& cols);

} // namespace h5geo

#include "misc/h5core_misc.h"


#endif // H5CORE_H
