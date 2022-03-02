#ifndef H5COREIMPL_H
#define H5COREIMPL_H

#include "h5enum.h"
#include "../h5util.h"

#include <type_traits>
#include <string>
#include <vector>
#include <regex>

#ifndef H5GT_USE_EIGEN
#define H5GT_USE_EIGEN // should be defined before including h5gt
#endif

#include <Eigen/Dense>

#include <h5gt/H5File.hpp>
#include <h5gt/H5Group.hpp>
#include <h5gt/H5DataSet.hpp>
#include <h5gt/H5DataSpace.hpp>
#include <h5gt/H5Attribute.hpp>

#include <units/units.hpp>


namespace h5geo
{

template<typename Object, typename T,
         typename std::enable_if<
             std::is_same<Object, h5gt::File>::value ||
             std::is_same<Object, h5gt::Group>::value>::type*>
inline bool _overwriteResizableDataset(
    Object& node,
    const std::string& datasetPath,
    T* M,
    size_t nH5Rows,
    size_t nH5Cols,
    const std::string& unitsFrom,
    const std::string& unitsTo)
{
  if(datasetPath.empty() ||
     !node.hasObject(datasetPath, h5gt::ObjectType::Dataset))
    return false;

  if (nH5Rows == 0 || nH5Cols == 0)
    return false;

  h5gt::DataSet dset = node.getDataSet(datasetPath);
  if (!dset.getCreateProps().isChunked())
    return false;

  auto dtype = dset.getDataType();
  if (!dtype.isTypeEqual(h5gt::AtomicType<T>())){
    return false;
  }

  if (!unitsFrom.empty() && !unitsTo.empty()){
    double coef = units::convert(
        units::unit_from_string(unitsFrom),
        units::unit_from_string(unitsTo));
    for(size_t i = 0; i < nH5Rows*nH5Cols; i++)
      M[i] *= coef;
  }

  try {
    std::vector<size_t> dims = {nH5Rows, nH5Cols};
    dset.resize(dims);
    dset.write_raw(M);
    return true;
  } catch (h5gt::Exception e) {
    return false;
  }
}

template<typename Object, typename D,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value &&
           std::is_arithmetic<typename D::Scalar>::value>::type*>
inline bool overwriteResizableDataset(
    Object& node,
    const std::string& datasetPath,
    Eigen::DenseBase<D>& M,
    const std::string& unitsFrom,
    const std::string& unitsTo)
{
  return _overwriteResizableDataset(
      node, datasetPath, M.derived().data(), M.cols(), M.rows(), unitsFrom, unitsTo);
}

template<typename Object, typename T,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value &&
           std::is_arithmetic<T>::value>::type*>
inline bool overwriteResizableDataset(
    Object& node,
    const std::string& datasetPath,
    std::vector<T>& v,
    const std::string& unitsFrom,
    const std::string& unitsTo)
{
  return _overwriteResizableDataset(
      node, datasetPath, v.data(), 1, v.size(), unitsFrom, unitsTo);
}

template<typename Object, typename T,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value &&
           std::is_arithmetic<T>::value>::type*>
inline bool overwriteResizableDataset(
    Object& node,
    const std::string& datasetPath,
    T& v,
    const std::string& unitsFrom,
    const std::string& unitsTo)
{
  return _overwriteResizableDataset(
      node, datasetPath, &v, 1, 1, unitsFrom, unitsTo);
}

template<typename Object, typename T,
         typename std::enable_if<
             std::is_same<Object, h5gt::File>::value ||
             std::is_same<Object, h5gt::Group>::value>::type*>
inline bool _overwriteDataset(
    Object& node,
    const std::string& datasetPath,
    T* M,
    size_t nH5Rows,
    size_t nH5Cols,
    const std::string& unitsFrom,
    const std::string& unitsTo)
{
  if (datasetPath.empty())
    return false;

  if (nH5Rows == 0 || nH5Cols == 0)
    return false;

  if(node.hasObject(datasetPath, h5gt::ObjectType::Dataset)){
    h5gt::DataSet dset = node.getDataSet(datasetPath);
    auto dtype = dset.getDataType();
    if (!dtype.isTypeEqual(h5gt::AtomicType<T>()) ||
        dset.getMemSpace().getElementCount() != nH5Rows*nH5Cols){
      node.unlink(datasetPath);
      node.template createDataSet<T>(
          datasetPath, h5gt::DataSpace({nH5Rows, nH5Cols}));
    }
  } else {
    node.template createDataSet<T>(
        datasetPath, h5gt::DataSpace({nH5Rows, nH5Cols}));
  }

  if (!unitsFrom.empty() && !unitsTo.empty()){
    double coef = units::convert(
        units::unit_from_string(unitsFrom),
        units::unit_from_string(unitsTo));
    for(size_t i = 0; i < nH5Rows*nH5Cols; i++)
      M[i] *= coef;
  }

  node.getDataSet(datasetPath).write_raw(M);
  return true;
}


template<typename Object, typename D,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value &&
           std::is_arithmetic<typename D::Scalar>::value>::type*>
inline bool overwriteDataset(
    Object& node,
    const std::string& datasetPath,
    Eigen::DenseBase<D>& M,
    const std::string& unitsFrom,
    const std::string& unitsTo)
{
  return _overwriteDataset(
      node, datasetPath, M.derived().data(), M.cols(), M.rows(), unitsFrom, unitsTo);
}

template<typename Object, typename T,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value &&
           std::is_arithmetic<T>::value>::type*>
inline bool overwriteDataset(
    Object& node,
    const std::string& datasetPath,
    std::vector<T>& v,
    const std::string& unitsFrom,
    const std::string& unitsTo)
{
  return _overwriteDataset(
      node, datasetPath, v.data(), 1, v.size(), unitsFrom, unitsTo);
}

template<typename Object, typename T,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value &&
           std::is_arithmetic<T>::value>::type*>
inline bool overwriteDataset(
    Object& node,
    const std::string& datasetPath,
    T& v,
    const std::string& unitsFrom,
    const std::string& unitsTo)
{
  return _overwriteDataset(
      node, datasetPath, &v, 1, 1, unitsFrom, unitsTo);
}

template <typename Object, typename T,
          typename std::enable_if<
            std::is_same<Object, h5gt::File>::value ||
            std::is_same<Object, h5gt::Group>::value>::type*>
inline bool _readDataset(
    Object& node,
    const std::string& datasetPath,
    T* M,
    size_t nElem,
    const std::string& unitsFrom,
    const std::string& unitsTo)
{
  if (datasetPath.empty() ||
      !node.hasObject(datasetPath, h5gt::ObjectType::Dataset))
    return false;

  if (nElem == 0)
    return false;

  h5gt::DataSet dset = node.getDataSet(datasetPath);
  auto dtype = dset.getDataType();
  if (!dtype.isTypeEqual(h5gt::AtomicType<T>()) ||
      dset.getMemSpace().getElementCount() != nElem)
    return false;

  dset.read(M);

  if (!unitsFrom.empty() && !unitsTo.empty()){
    double coef = units::convert(
        units::unit_from_string(unitsFrom),
        units::unit_from_string(unitsTo));
    for(size_t i = 0; i < nElem; i++)
      M[i] *= coef;
  }

  return true;
}

template<typename Object, typename D,
         typename std::enable_if<
           (std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value) &&
           std::is_arithmetic<typename D::Scalar>::value>::type*>
inline bool readDataset(
    Object& node,
    const std::string& datasetPath,
    Eigen::DenseBase<D>& M,
    const std::string& unitsFrom,
    const std::string& unitsTo)
{
  // we don't want to resize vector if no data to read
  if (datasetPath.empty() ||
      !node.hasObject(datasetPath, h5gt::ObjectType::Dataset))
    return false;

  h5gt::DataSet dset = node.getDataSet(datasetPath);
  auto dtype = dset.getDataType();
  if (!dtype.isTypeEqual(h5gt::AtomicType<typename D::Scalar>()))
    return false;

  std::vector<size_t> dims = dset.getDimensions();
  M.derived().resize(dims[1], dims[0]); // resize the derived object
  return _readDataset(node, datasetPath, M.derived().data(), M.size(), unitsFrom, unitsTo);
}

template<typename Object,
         typename std::enable_if<
             std::is_same<Object, h5gt::File>::value ||
             std::is_same<Object, h5gt::Group>::value ||
             std::is_same<Object, h5gt::DataSet>::value>::type*>
inline Eigen::MatrixXf readFloatEigenMtxDataset(
    Object& node,
    const std::string& datasetPath,
    const std::string& unitsFrom,
    const std::string& unitsTo)
{
  Eigen::MatrixXf M;
  readDataset(node, datasetPath, M, unitsFrom, unitsTo);
  return M;
}

template<typename Object,
         typename std::enable_if<
             std::is_same<Object, h5gt::File>::value ||
             std::is_same<Object, h5gt::Group>::value ||
             std::is_same<Object, h5gt::DataSet>::value>::type*>
inline Eigen::MatrixXd readDoubleEigenMtxDataset(
    Object& node,
    const std::string& datasetPath,
    const std::string& unitsFrom,
    const std::string& unitsTo)
{
  Eigen::MatrixXd M;
  readDataset(node, datasetPath, M, unitsFrom, unitsTo);
  return M;
}


template <typename Object, typename T,
          typename std::enable_if<
            std::is_same<Object, h5gt::File>::value ||
            std::is_same<Object, h5gt::Group>::value ||
            std::is_same<Object, h5gt::DataSet>::value>::type*>
inline bool _readAttribute(
    Object& holder,
    const std::string& attrName,
    T *v,
    size_t nElem,
    const std::string& unitsFrom,
    const std::string& unitsTo)
{
  if (attrName.empty() || !holder.hasAttribute(attrName))
    return false;

  if (nElem == 0)
    return false;

  h5gt::Attribute attr = holder.getAttribute(attrName);
  auto dtype = attr.getDataType();
  if (!dtype.isTypeEqual(h5gt::AtomicType<T>()) ||
      attr.getMemSpace().getElementCount() != nElem)
    return false;

  attr.read(v);

  if (!unitsFrom.empty() && !unitsTo.empty()){
    double coef = units::convert(
        units::unit_from_string(unitsFrom),
        units::unit_from_string(unitsTo));
    for(size_t i = 0; i < nElem; i++)
      v[i] *= coef;
  }

  return true;
}

template<typename Object, typename D,
         typename std::enable_if<
           (std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value) &&
           std::is_arithmetic<typename D::Scalar>::value>::type*>
inline bool readAttribute(
    Object& holder,
    const std::string& attrName,
    Eigen::DenseBase<D> &v,
    const std::string& unitsFrom,
    const std::string& unitsTo)
{
  // we don't want to resize vector if no data to read
  if (attrName.empty() || !holder.hasAttribute(attrName))
    return false;

  h5gt::Attribute attr = holder.getAttribute(attrName);
  auto dtype = attr.getDataType();
  if (!dtype.isTypeEqual(h5gt::AtomicType<typename D::Scalar>()))
    return false;

  v.derived().resize(holder.getAttribute(attrName).getMemSpace().getElementCount());
  return _readAttribute(
      holder, attrName, v.derived().data(), v.size(), unitsFrom, unitsTo);
}

template <typename Object, typename T,
          typename std::enable_if<
            (std::is_same<Object, h5gt::File>::value ||
            std::is_same<Object, h5gt::Group>::value ||
            std::is_same<Object, h5gt::DataSet>::value) &&
            std::is_arithmetic<T>::value>::type*>
inline bool readAttribute(
    Object& holder,
    const std::string& attrName,
    std::vector<T> &v,
    const std::string& unitsFrom,
    const std::string& unitsTo)
{
  // we don't want to resize vector if no data to read
  if (attrName.empty() || !holder.hasAttribute(attrName))
    return false;

  h5gt::Attribute attr = holder.getAttribute(attrName);
  auto dtype = attr.getDataType();
  if (!dtype.isTypeEqual(h5gt::AtomicType<T>()))
    return false;

  v.resize(holder.getAttribute(attrName).getMemSpace().getElementCount());
  return _readAttribute(
      holder, attrName, v.data(), v.size(), unitsFrom, unitsTo);
}

template <typename Object, typename T,
          typename std::enable_if<
            (std::is_same<Object, h5gt::File>::value ||
            std::is_same<Object, h5gt::Group>::value ||
            std::is_same<Object, h5gt::DataSet>::value) &&
            std::is_arithmetic<T>::value>::type*>
inline bool readAttribute(
    Object& holder,
    const std::string& attrName,
    T &v,
    const std::string& unitsFrom,
    const std::string& unitsTo)
{
  return _readAttribute(
      holder, attrName, &v, 1, unitsFrom, unitsTo);
}

template <typename Object, typename T,
          typename std::enable_if<
            std::is_same<Object, h5gt::File>::value ||
            std::is_same<Object, h5gt::Group>::value||
            std::is_same<Object, h5gt::DataSet>::value>::type*>
inline bool _overwriteAttribute(
    Object& holder,
    const std::string& attrName,
    T* v,
    size_t nElem,
    const std::string& unitsFrom,
    const std::string& unitsTo)
{
  if (attrName.empty())
    return false;

  if (nElem == 0)
    return false;

  if (!holder.hasAttribute(attrName))
    holder.template createAttribute<T>(
          attrName, h5gt::DataSpace({nElem}));

  h5gt::Attribute attr = holder.getAttribute(attrName);
  auto dtype = attr.getDataType();
  if (!dtype.isTypeEqual(h5gt::AtomicType<T>()) ||
      attr.getMemSpace().getElementCount() != nElem){
    try {
      holder.deleteAttribute(attrName);
      attr = holder.template createAttribute<T>(
            attrName, h5gt::DataSpace({nElem}));
    }  catch (h5gt::Exception e) {
      return false;
    }
  }

  if (!unitsFrom.empty() && !unitsTo.empty()){
    double coef = units::convert(
        units::unit_from_string(unitsFrom),
        units::unit_from_string(unitsTo));
    for(size_t i = 0; i < nElem; i++)
      v[i] *= coef;
  }

  attr.write(v);
  return true;
}

template<typename Object,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value>::type*>
inline bool overwriteAttribute(
    Object& holder,
    const std::string& attrName,
    const std::string& str)
{
  if (attrName.empty())
    return false;

  if (!holder.hasAttribute(attrName))
    holder.template createAttribute<std::string>(
          attrName, h5gt::DataSpace::From(str));

  h5gt::Attribute attr = holder.getAttribute(attrName);
  auto dtype = attr.getDataType();
  if (!dtype.isTypeEqual(h5gt::AtomicType<std::string>()) ||
      attr.getMemSpace().getElementCount() != 1)
    return false;

  attr.write(str);
  return true;
}

template<typename Object, typename D,
         typename std::enable_if<
           (std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value) &&
           std::is_arithmetic<typename D::Scalar>::value>::type*>
inline bool overwriteAttribute(
    Object& holder,
    const std::string& attrName,
    Eigen::DenseBase<D>& v,
    const std::string& unitsFrom,
    const std::string& unitsTo)
{
  return _overwriteAttribute(
      holder, attrName, v.derived().data(), v.size(), unitsFrom, unitsTo);
}

template <typename Object, typename T,
          typename std::enable_if<
            (std::is_same<Object, h5gt::File>::value ||
            std::is_same<Object, h5gt::Group>::value ||
            std::is_same<Object, h5gt::DataSet>::value) &&
            std::is_arithmetic<T>::value>::type*>
inline bool overwriteAttribute(
    Object& holder,
    const std::string& attrName,
    std::vector<T>& v,
    const std::string& unitsFrom,
    const std::string& unitsTo)
{
  return _overwriteAttribute(
      holder, attrName, v.data(), v.size(), unitsFrom, unitsTo);
}

template <typename Object, typename T,
          typename std::enable_if<
            (std::is_same<Object, h5gt::File>::value ||
            std::is_same<Object, h5gt::Group>::value ||
            std::is_same<Object, h5gt::DataSet>::value) &&
            std::is_arithmetic<T>::value>::type*>
inline bool overwriteAttribute(
    Object& holder,
    const std::string& attrName,
    T& v,
    const std::string& unitsFrom,
    const std::string& unitsTo)
{
  return _overwriteAttribute(
      holder, attrName, &v, 1, unitsFrom, unitsTo);
}

template<typename Object,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value>::type*>
inline unsigned readEnumAttribute(Object& object, const std::string& attrName){
  /* as we often use magic_enum to convert enum to string
   * we need to remove `h5geo::` from enum name given
   * from magic_enum () as for example:
   * magic_enum::enum_type_name<h5geo::SurveyType>() */
//  eraseSubStr(attrName, "h5geo::");

  unsigned value = 0;
  readAttribute(object, attrName, value);
  return value;
}

template<typename Object,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value>::type*>
inline std::string readStringAttribute(Object& object, const std::string& attrName){
  std::string str;
  if (attrName.empty() || !object.hasAttribute(attrName))
    return str;

  h5gt::Attribute attr = object.getAttribute(attrName);
  auto dtype = attr.getDataType();
  if (!dtype.isTypeEqual(h5gt::AtomicType<std::string>()) ||
      attr.getMemSpace().getElementCount() != 1)
      return str;

  attr.read(str);
  return str;
}

template<typename Object,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value>::type*>
inline float readFloatAttribute(
    Object& object, const std::string& attrName,
    const std::string& unitsFrom,
    const std::string& unitsTo){
  float value = NAN;
  readAttribute(object, attrName, value, unitsFrom, unitsTo);
  return value;
}

template<typename Object,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value>::type*>
inline double readDoubleAttribute(
    Object& object, const std::string& attrName,
    const std::string& unitsFrom,
    const std::string& unitsTo)
{
  double value = NAN;
  readAttribute(object, attrName, value, unitsFrom, unitsTo);
  return value;
}

template<typename Object,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value>::type*>
inline std::vector<float> readFloatVecAttribute(
    Object& object, const std::string& attrName,
    const std::string& unitsFrom,
    const std::string& unitsTo){
  std::vector<float> value;
  readAttribute(object, attrName, value, unitsFrom, unitsTo);
  return value;
}

template<typename Object,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value>::type*>
inline std::vector<double> readDoubleVecAttribute(
    Object& object, const std::string& attrName,
    const std::string& unitsFrom,
    const std::string& unitsTo){
  std::vector<double> value;
  readAttribute(object, attrName, value, unitsFrom, unitsTo);
  return value;
}

template<typename Object,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value>::type*>
inline Eigen::VectorXf readFloatEigenVecAttribute(
    Object& object, const std::string& attrName,
    const std::string& unitsFrom,
    const std::string& unitsTo){
  Eigen::VectorXf value;
  readAttribute(object, attrName, value, unitsFrom, unitsTo);
  return value;
}

template<typename Object,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value>::type*>
inline Eigen::VectorXd readDoubleEigenVecAttribute(
    Object& object, const std::string& attrName,
    const std::string& unitsFrom,
    const std::string& unitsTo){
  Eigen::VectorXd value;
  readAttribute(object, attrName, value, unitsFrom, unitsTo);
  return value;
}

template<typename D>
inline bool writeData2IndexedDataset(
    h5gt::DataSet& dataset,
    const std::string& attrName,
    const Eigen::DenseBase<D>& v,
    bool resize)
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
inline Eigen::VectorX<T> getDataFromIndexedDataset(
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

} // namespace h5geo

#endif // H5COREIMPL_H
