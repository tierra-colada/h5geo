#ifndef H5CORE_H
#define H5CORE_H

#ifndef H5GT_USE_EIGEN
#define H5GT_USE_EIGEN // should be defined before including h5gt
#endif

#include "h5geo_export.h"
#include "private/h5enum.h"
#include "private/h5easyhull.h"
#include "private/h5interpolation.h"
#include "private/h5polyfit.h"
#include "private/h5sort.h"

#include <type_traits>
#include <string>
#include <vector>
#include <regex>
#include <optional>
#include <limits.h>
#include <stdint.h>

#include <Eigen/Dense>

#ifndef H5GT_USE_EIGEN
#define H5GT_USE_EIGEN // should be defined before including h5gt
#endif

#include <h5gt/H5File.hpp>
#include <h5gt/H5Group.hpp>
#include <h5gt/H5DataSet.hpp>
#include <h5gt/H5DataSpace.hpp>
#include <h5gt/H5Attribute.hpp>

#if CHAR_BIT != 8
#error "unsupported char size"
#endif

enum {
  O32_LITTLE_ENDIAN = 0x03020100ul,
  O32_BIG_ENDIAN = 0x00010203ul,
  O32_PDP_ENDIAN = 0x01000302ul,      /* DEC PDP-11 (aka ENDIAN_LITTLE_WORD) */
  O32_HONEYWELL_ENDIAN = 0x02030001ul /* Honeywell 316 (aka ENDIAN_BIG_WORD) */
};

static const union {
  unsigned char bytes[4];
  uint32_t value;
} o32_host_order = { { 0, 1, 2, 3 } };

#define O32_HOST_ORDER (o32_host_order.value)

namespace h5geo
{

/// \brief Resize and overwrite DataSet or create it if not exists
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

/// \brief Resize and overwrite DataSet or create it if not exists
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

/// \brief Resize and overwrite DataSet or create it if not exists
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

/// \brief Resize and overwrite DataSet or create it if not exists
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

/// \brief Create or overwrite DataSet
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

/// \brief Create or overwrite DataSet
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

/// \brief Create or overwrite DataSet
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

/// \brief Create or overwrite DataSet
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

/// \brief Read data from DataSet to buffer
template <typename Object, typename T,
          typename std::enable_if<
            std::is_same<Object, h5gt::File>::value ||
            std::is_same<Object, h5gt::Group>::value>::type* = nullptr>
bool _readDataset(
    Object& node,
    const std::string& datasetPath,
    T* M,
    size_t nElem,
    const std::string& unitsFrom = "",
    const std::string& unitsTo = "");

/// \brief Read data from DataSet
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

/// \brief Read data from DataSet
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

/// \brief Read data from DataSet
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

/// \brief Read data from Attribute
template <typename Object, typename T,
          typename std::enable_if<
            std::is_same<Object, h5gt::File>::value ||
            std::is_same<Object, h5gt::Group>::value ||
            std::is_same<Object, h5gt::DataSet>::value>::type* = nullptr>
bool _readAttribute(
    Object& holder,
    const std::string& attrName,
    T* v,
    size_t nElem,
    const std::string& unitsFrom = "",
    const std::string& unitsTo = "");

/// \brief Read data from Attribute
template<typename Object, typename D,
         typename std::enable_if<
           (std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value) &&
           std::is_arithmetic<typename D::Scalar>::value>::type* = nullptr>
bool readAttribute(
    Object& holder,
    const std::string& attrName,
    Eigen::DenseBase<D>& v,
    const std::string& unitsFrom = "",
    const std::string& unitsTo = "");

/// \brief Read data from Attribute
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

/// \brief Read data from Attribute
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

/// \brief Create or overwrite Attribute
template <typename Object, typename T,
          typename std::enable_if<
            std::is_same<Object, h5gt::File>::value ||
            std::is_same<Object, h5gt::Group>::value||
            std::is_same<Object, h5gt::DataSet>::value>::type* = nullptr>
bool _overwriteAttribute(
    Object& holder,
    const std::string& attrName,
    T* v,
    size_t nElem,
    const std::string& unitsFrom = "",
    const std::string& unitsTo = "");

/// \brief Create or overwrite Attribute
template<typename Object,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value>::type* = nullptr>
bool overwriteAttribute(
    Object& holder,
    const std::string& attrName,
    const std::string& str);

/// \brief Create or overwrite Attribute
template<typename Object, typename D,
         typename std::enable_if<
           (std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value) &&
           std::is_arithmetic<typename D::Scalar>::value>::type* = nullptr>
bool overwriteAttribute(
    Object& holder,
    const std::string& attrName,
    Eigen::DenseBase<D>& v,
    const std::string& unitsFrom = "",
    const std::string& unitsTo = "");

/// \brief Create or overwrite Attribute
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

/// \brief Create or overwrite Attribute
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

/// \brief Create or overwrite registered enum Attribute
template <typename Object, typename T,
          typename std::enable_if<
            std::is_same<Object, h5gt::File>::value ||
            std::is_same<Object, h5gt::Group>::value||
            std::is_same<Object, h5gt::DataSet>::value>::type* = nullptr>
bool _overwriteEnumAttribute(
    Object& holder,
    const std::string& attrName,
    T* v,
    size_t nElem);

/// \brief Create or overwrite registered enum Attribute
template <typename Object, typename T,
          typename std::enable_if<
            (std::is_same<Object, h5gt::File>::value ||
            std::is_same<Object, h5gt::Group>::value ||
            std::is_same<Object, h5gt::DataSet>::value) &&
            std::is_enum<T>::value>::type* = nullptr>
bool overwriteEnumAttribute(
    Object& holder,
    const std::string& attrName,
    T& v);

/// \brief Create or overwrite registered enum Attribute
template <typename Object, typename T,
          typename std::enable_if<
            (std::is_same<Object, h5gt::File>::value ||
            std::is_same<Object, h5gt::Group>::value ||
            std::is_same<Object, h5gt::DataSet>::value) &&
            std::is_enum<T>::value>::type* = nullptr>
bool overwriteEnumAttribute(
    Object& holder,
    const std::string& attrName,
    std::vector<T>& v);

/// \brief Read enum data from Attribute
template <typename Object, typename T,
          typename std::enable_if<
            std::is_same<Object, h5gt::File>::value ||
            std::is_same<Object, h5gt::Group>::value ||
            std::is_same<Object, h5gt::DataSet>::value>::type* = nullptr>
inline bool _readEnumAttribute(
    Object& holder,
    const std::string& attrName,
    T *v,
    size_t nElem);

/// \brief Read data from Attribute
template<typename Object, typename T,
         typename std::enable_if<
           (std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value) &&
           std::is_enum<T>::value>::type* = nullptr>
T readEnumAttribute(Object& object, const std::string& attrName);

/// \brief Read data from Attribute
template<typename Object, typename T,
         typename std::enable_if<
           (std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value) &&
           std::is_enum<T>::value>::type* = nullptr>
std::vector<T> readEnumVecAttribute(Object& object, const std::string& attrName);

/// \brief Read enum data from Attribute
template<typename Object,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value>::type* = nullptr>
std::string readStringAttribute(Object& object, const std::string& attrName);

/// \brief Read data from Attribute
template<typename Object,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value>::type* = nullptr>
float readFloatAttribute(
    Object& object, const std::string& attrName,
    const std::string& unitsFrom = "",
    const std::string& unitsTo = "");

/// \brief Read data from Attribute
template<typename Object,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value>::type* = nullptr>
double readDoubleAttribute(
    Object& object, const std::string& attrName,
    const std::string& unitsFrom = "",
    const std::string& unitsTo = "");

/// \brief Read data from Attribute
template<typename Object,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value>::type* = nullptr>
std::vector<float> readFloatVecAttribute(
    Object& object, const std::string& attrName,
    const std::string& unitsFrom = "",
    const std::string& unitsTo = "");

/// \brief Read data from Attribute
template<typename Object,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value>::type* = nullptr>
std::vector<double> readDoubleVecAttribute(
    Object& object, const std::string& attrName,
    const std::string& unitsFrom = "",
    const std::string& unitsTo = "");

/// \brief Read data from Attribute
template<typename Object,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value>::type* = nullptr>
Eigen::VectorXf readFloatEigenVecAttribute(
    Object& object, const std::string& attrName,
    const std::string& unitsFrom = "",
    const std::string& unitsTo = "");

/// \brief Read data from Attribute
template<typename Object,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value>::type* = nullptr>
Eigen::VectorXd readDoubleEigenVecAttribute(
    Object& object, const std::string& attrName,
    const std::string& unitsFrom = "",
    const std::string& unitsTo = "");

/// \brief writeDataToIndexedDataset Try to write vector to dataset with
/// attribute where attribute is a single value reflecting
/// row index of a corresponding dataset
/// \param dataset
/// \param v
/// \param attrName
/// \param resize resize if needed. Use it only for resizable dataset!
/// \return

/// \brief Write data to indexed DataSet
///
/// Indexed DataSet has attributes with names and values telling what \n
/// rows/cols are responsible for storing data (like tables)
template<typename D>
bool writeDataToIndexedDataset(
    h5gt::DataSet& dataset,
    const std::string& attrName,
    const Eigen::DenseBase<D>& v,
    bool resize);

/// \brief Get data from indexed DataSet
///
/// Indexed DataSet has attributes with names and values telling what \n
/// rows/cols are responsible for storing data (like tables)
template<typename T>
Eigen::VectorX<T> getDataFromIndexedDataset(
    h5gt::DataSet& dataset,
    const std::string& attrName);

// UTIL

H5GEO_EXPORT std::optional<h5gt::File> openFile(
    const std::string& fileName);
H5GEO_EXPORT std::optional<h5gt::Group> openGroup(
    const std::string& fileName,
    const std::string& groupName);
H5GEO_EXPORT std::optional<h5gt::DataSet> openDataSet(
    const std::string& fileName,
    const std::string& dsetName);

H5GEO_EXPORT std::vector<std::string> getRawBinHeaderNames();
H5GEO_EXPORT std::vector<std::string> getRawTraceHeaderNames();

/// \brief compareStrings Return `true` if strings are equal.
/// \param bigger
/// \param smaller
/// \param caseSensitivity
/// \return
H5GEO_EXPORT bool compareStrings(
    const std::string& bigger,
    const std::string& smaller,
    const CaseSensitivity& caseSensitivity = CaseSensitivity::CASE_INSENSITIVE);

/// \brief eraseSubStr First Occurrence of given  substring from main string.
/// \param mainStr
/// \param toErase
H5GEO_EXPORT void eraseSubStr(
    std::string & mainStr, const std::string & toErase);

/// \brief eraseAllSubStr Erase all Occurrences of given substring from main string.
/// \param mainStr
/// \param toErase
H5GEO_EXPORT void eraseAllSubStr(
    std::string & mainStr, const std::string & toErase);

H5GEO_EXPORT char getDelimiter(
    const Delimiter& delimiter);

/// \brief generateName generates unique name by adding "_i"
/// \param nameList
/// \param baseName if empty then it will be replaced by "no_name"
/// \return
H5GEO_EXPORT std::string generateName(
    const std::vector<std::string> &nameList,
    std::string baseName = std::string());

H5GEO_EXPORT std::vector<std::string> splitString(
    const std::string &s, const std::string delimiter);

H5GEO_EXPORT void splitHeaderNames(
    const std::vector<std::string> &headerNamesToSplit,
    std::vector<std::string> &fullHeaderNames,
    std::vector<std::string> &shortHeaderNames);

H5GEO_EXPORT void splitHeaderBytes(
    const std::vector<std::string> &headerNamesToSplit,
    std::vector<int> &bytesStart,
    std::vector<int> &nBytes);

/// \brief splitPath Split path of type /path///to/where/things/happen//
/// to output vector {"path", "to", "where", "things", "happen"}.
/// Path that starts from `/` is treated as absolute
/// \param path
/// \return
H5GEO_EXPORT std::vector<std::string> splitPath(
    std::string path);

/// \brief splitPath Split path of type /path///to/where/things/happen//
/// to output vector {"path", "to", "where", "things", "happen"}
/// and to filtered path `/path/to/where/things/happen`.
/// Path that starts from `/` is treated as absolute
/// \param path
/// \param filteredPath
/// \return
H5GEO_EXPORT std::vector<std::string> splitPath(
    std::string path, std::string& filteredPath);

/// \brief splitPathToParentAndObj Return path to parent
/// and object name. E.g. if `path = /a/s` then it returns
/// `/a` as path and `s` as object name.
/// If path is empty or `path = /` then both output path
/// and object name are empty strings.
/// \param path
/// \param objName
/// \return
H5GEO_EXPORT std::string splitPathToParentAndObj(
    const std::string& path,
    std::string& objName);

H5GEO_EXPORT std::string getRelativePath(
    const std::string& referencePath,
    const std::string& objPath,
    const CaseSensitivity& caseSensitivity = CaseSensitivity::CASE_INSENSITIVE);

H5GEO_EXPORT void getTraceHeaderNames(
    std::vector<std::string> &fullHeaderNames,
    std::vector<std::string> &shortHeaderNames);
H5GEO_EXPORT std::vector<std::string> getTraceHeaderShortNames();
H5GEO_EXPORT std::vector<std::string> getTraceHeaderFullNames();

H5GEO_EXPORT void getBinHeaderNames(
    std::vector<std::string> &fullHeaderNames,
    std::vector<std::string> &shortHeaderNames);
H5GEO_EXPORT std::vector<std::string> getBinHeaderShortNames();
H5GEO_EXPORT std::vector<std::string> getBinHeaderFullNames();

H5GEO_EXPORT void getTraceHeaderBytes(
    std::vector<int> &bytesStart,
    std::vector<int> &nBytes);
H5GEO_EXPORT void getBinHeaderBytes(
    std::vector<int> &bytesStart,
    std::vector<int> &nBytes);

H5GEO_EXPORT size_t getTraceHeaderCount();
H5GEO_EXPORT size_t getBinHeaderCount();

/// \brief getIndexFromAttribute Get row/col from Datasets with attributes
/// where attribute reflects the row/col index (like tables)
/// \param dataset
/// \param attrName
/// \return
H5GEO_EXPORT ptrdiff_t getIndexFromAttribute(
    h5gt::DataSet& dataset,
    const std::string& attrName);


template<typename Object,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value>::type* = nullptr>
bool deleteAllAttributes(Object& object);

/// \brief unlinkContent Unlink everything in group
/// \return
template<typename Object,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value>::type* = nullptr>
bool unlinkContent(Object& object);

/// \brief find_index find all non-zero elements's indexes.
/// Possible usage:
/// Eigen::VectorX<ptrdiff_t> ind = find_index(M.array() > 20)
/// M.array() > 20 returns bool matrix and then we find non-zero indexes
/// \param M
/// \return ind indexes of nonzero elements
template<typename T>
Eigen::VectorX<ptrdiff_t> find_index(
    Eigen::DenseBase<T> const & M);

template<typename D, typename T,
         typename std::enable_if<
           std::is_arithmetic<T>::value>::type* = nullptr>
h5gt::ElementSet rowCols2ElementSet(
    const T& row,
    const Eigen::DenseBase<D>& cols);

template<typename D, typename T,
         typename std::enable_if<
           std::is_arithmetic<T>::value>::type* = nullptr>
h5gt::ElementSet rowsCol2ElementSet(
    const Eigen::DenseBase<D>& rows,
    const T& col);

/// \brief rowsCols2ElementSet select rectilinear block of elements, i.e.
/// uses double loop to select every possible row-col intersection
/// \param rows vector
/// \param cols vector
/// \return
template<typename D>
h5gt::ElementSet rowsCols2ElementSet(
    const Eigen::DenseBase<D>& rows,
    const Eigen::DenseBase<D>& cols);

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

/// \brief rowsCols2ElementSet select rectilinear block of elements, i.e.
/// uses double loop to select every possible row-col intersection
/// \param rows
/// \param cols
/// \return
template<typename T,
         typename std::enable_if<
           std::is_arithmetic<T>::value>::type* = nullptr>
h5gt::ElementSet rowsCols2ElementSet(
    const std::vector<T>& rows,
    const std::vector<T>& cols);

} // namespace h5geo


#ifdef H5GEO_USE_GDAL
#include "private/h5core_sr_settings.h"
#endif

#include "private/h5coreimpl.h"
#include "private/h5core_segy.h"


#endif // H5CORE_H
