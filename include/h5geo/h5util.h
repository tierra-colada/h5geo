#ifndef H5UTIL_H
#define H5UTIL_H

#ifdef H5GEO_USE_GDAL
#include "impl/h5sr_settings.h"
#endif

#include "impl/h5geo_export.h"
#include "impl/h5enum.h"
#include "impl/h5easyhull.h"
#include "impl/h5interpolation.h"
#include "impl/h5polyfit.h"
#include "impl/h5sort.h"

#include <vector>
#include <string>
#include <limits.h>
#include <stdint.h>

#ifndef H5GT_USE_EIGEN
#define H5GT_USE_EIGEN // should be defined before including h5gt
#endif

#include <Eigen/Dense>

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

namespace h5geo {

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
    const std::string& parentPath,
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

/// \brief unlinkObject Unlink object from container
/// \param object parent object (File or Group) relatively
/// to `objPath`
/// \param objPath path to object from root
///(like: /path/to/object)
/// \return
template<typename Parent,
         typename std::enable_if<
           std::is_same<Parent, h5gt::File>::value ||
           std::is_same<Parent, h5gt::Group>::value>::type* = nullptr>
bool unlinkObject(Parent& parent, const std::string& objPath);

/// \brief unlinkContent Unlink everything in group
/// \return
template<typename Object,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value>::type* = nullptr>
bool unlinkContent(Object& object);

/// \brief find_index find all non-zero elements's indices.
/// Possible usage:
/// Eigen::VectorX<ptrdiff_t> ind = find_index(M.array() > 20)
/// M.array() > 20 returns bool matrix and then we find non-zero indices
/// \param M
/// \return ind indices of nonzero elements
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

}

#include "impl/h5utilimpl.h"

#endif // H5UTIL_H
