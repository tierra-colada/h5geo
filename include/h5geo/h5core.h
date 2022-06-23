#ifndef H5CORE_H
#define H5CORE_H

#ifndef H5GT_USE_EIGEN
#define H5GT_USE_EIGEN // should be defined before including h5gt
#endif

#include "h5geo_export.h"
#include "private/h5enum.h"

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
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value &&
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
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value &&
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
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value &&
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
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value &&
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
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value &&
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
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value &&
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

/// \brief Read data from Attribute
template<typename Object,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value>::type* = nullptr>
unsigned readEnumAttribute(Object& object, const std::string& attrName);

/// \brief Read data from Attribute
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

/// \brief writeData2IndexedDataset Try to write vector to dataset with
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
bool writeData2IndexedDataset(
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

} // namespace h5geo


#include "private/h5core_util.h"
#ifdef H5GEO_USE_GDAL
#include "private/h5core_sr_settings.h"
#endif

#include "private/h5coreimpl.h"


#endif // H5CORE_H
