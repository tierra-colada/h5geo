#ifndef H5CORE_H
#define H5CORE_H

#ifndef H5GT_USE_EIGEN
#define H5GT_USE_EIGEN // should be defined before including h5gt
#endif

#include "../impl/h5enum.h"
#include "../impl/h5geo_export.h"

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
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value &&
           std::is_arithmetic<typename D::Scalar>::value>::type* = nullptr>
bool overwriteResizableDataset(
    Object& node,
    const std::string& datasetPath,
    Eigen::DenseBase<D>& M,
    const std::string& unitsFrom = "",
    const std::string& unitsTo = "");

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
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value &&
           std::is_arithmetic<typename D::Scalar>::value>::type* = nullptr>
bool overwriteDataset(
    Object& node,
    const std::string& datasetPath,
    Eigen::DenseBase<D>& M,
    const std::string& unitsFrom = "",
    const std::string& unitsTo = "");

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

/// \brief readAttribute Read attribute only if it exist and
/// its type is the same as passed buffer
/// \param holder h5gt::Object that contains given attribute
/// \param attrName
/// \param v
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

template<typename Object,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value>::type* = nullptr>
bool overwriteAttribute(
    Object& holder,
    const std::string& attrName,
    const std::string& str);

/// \brief overwriteAttribute Create attribute if not exist.
/// Delete attribute if type or size is different
/// and create new one and write data to it
/// \param holder h5gt::Object that contains given attribute
/// \param attrName
/// \param v
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

/// \brief readEnumAttribute Read enum from `Object's`
/// attribute as unsigned value. Return `0` if attribute
/// not exists.
/// \param object
/// \param attrName
template<typename Object,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value>::type* = nullptr>
unsigned readEnumAttribute(Object& object, const std::string& attrName);

/// \brief readStringAttribute Read string from `Object's`
/// attribute. Return `std::string()` (empty string) if attribute
/// not exists.
/// \param object
/// \param attrName
/// \return
template<typename Object,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value>::type* = nullptr>
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

/// \brief writeData2IndexedDataset Try to write vector to dataset with
/// attribute where attribute is a single value reflecting
/// row index of a corresponding dataset
/// \param dataset
/// \param v
/// \param attrName
/// \param resize resize if needed. Use it only for resizable dataset!
/// \return
template<typename D>
bool writeData2IndexedDataset(
    h5gt::DataSet& dataset,
    const std::string& attrName,
    const Eigen::DenseBase<D>& v,
    bool resize);

/// \brief getDataFromIndexedDataset Try to read data to vector
/// from dataset with attribute where attribute is a single
/// value reflecting row index of a corresponding dataset
/// \param dataset
/// \param attrName
/// \return
template<typename T>
Eigen::VectorX<T> getDataFromIndexedDataset(
    h5gt::DataSet& dataset,
    const std::string& attrName);

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

} // namespace h5geo


#include "../impl/h5coreimpl.h"


#endif // H5CORE_H
