#ifndef H5UTILIMPL_H
#define H5UTILIMPL_H

#ifndef H5GT_USE_EIGEN
#define H5GT_USE_EIGEN // should be defined before including h5gt
#endif

#include <Eigen/Dense>

#include <h5gt/H5File.hpp>
#include <h5gt/H5Group.hpp>
#include <h5gt/H5DataSet.hpp>
#include <h5gt/H5DataSpace.hpp>
#include <h5gt/H5Attribute.hpp>


namespace h5geo {

template<typename Object,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value>::type*>
inline bool deleteAllAttributes(Object& object){
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
           std::is_same<Parent, h5gt::File>::value ||
           std::is_same<Parent, h5gt::Group>::value>::type*>
inline bool unlinkObject(Parent& parent, const std::string& objPath){
  try {
    parent.unlink(objPath);
  }  catch (h5gt::Exception e) {
    return false;
  }
  return true;
}

template<typename Object,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value>::type*>
inline bool unlinkContent(Object& object){
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

template<typename T>
inline Eigen::VectorX<ptrdiff_t> find_index(
    Eigen::DenseBase<T> const & M){
  Eigen::VectorX<ptrdiff_t> ind(M.size());
  ptrdiff_t ii = 0;
  for (ptrdiff_t i = 0; i < M.size(); i++){
    if (M(i) != 0){
      ind(ii) = i;
      ii++;
    }
  }
  ind.conservativeResize(ii);

  return ind;
}

template<typename D, typename T,
         typename std::enable_if<
           std::is_arithmetic<T>::value>::type*>
inline h5gt::ElementSet rowCols2ElementSet(
    const T& row,
    const Eigen::DenseBase<D>&  cols)
{
  ptrdiff_t I = cols.size();
  std::vector<size_t> v(2*I, row);

  for (ptrdiff_t i = 0; i < I; i++)
    v[2*i+1] = cols(i);

  return h5gt::ElementSet(v);
}

template<typename D, typename T,
         typename std::enable_if<
           std::is_arithmetic<T>::value>::type*>
inline h5gt::ElementSet rowsCol2ElementSet(
    const Eigen::DenseBase<D>& rows,
    const T& col)
{
  ptrdiff_t I = rows.size();
  std::vector<size_t> v(2*I, col);

  for (ptrdiff_t i = 0; i < I; i++)
    v[2*i] = rows(i);

  return h5gt::ElementSet(v);
}

template<typename D>
inline h5gt::ElementSet rowsCols2ElementSet(
    const Eigen::DenseBase<D>& rows,
    const Eigen::DenseBase<D>& cols)
{
  ptrdiff_t I = rows.size();
  ptrdiff_t J = cols.size();

  std::vector<size_t> v(2*I*J);

  for (ptrdiff_t i = 0; i < I; i++){
    for (ptrdiff_t j = 0; j < J; j++){
      v[2*j + 2*i*J] = rows(i);
      v[2*j+1 + 2*i*J] = cols(j);
    }
  }

  return h5gt::ElementSet(v);
}

template<typename T,
         typename std::enable_if<
           std::is_arithmetic<T>::value>::type*>
inline h5gt::ElementSet rowCols2ElementSet(
    const T& row,
    const std::vector<T>& cols)
{
  size_t I = cols.size();
  std::vector<size_t> v(2*I, row);

  for (size_t i = 0; i < I; i++)
    v[2*i+1] = cols[i];

  return h5gt::ElementSet(v);
}

template<typename T,
         typename std::enable_if<
           std::is_arithmetic<T>::value>::type*>
inline h5gt::ElementSet rowsCol2ElementSet(
    const std::vector<T>& rows,
    const T& col)
{
  size_t I = rows.size();
  std::vector<size_t> v(2*I, col);

  for (size_t i = 0; i < I; i++)
    v[2*i] = rows[i];

  return h5gt::ElementSet(v);
}

template<typename T,
         typename std::enable_if<
           std::is_arithmetic<T>::value>::type*>
inline h5gt::ElementSet rowsCols2ElementSet(
    const std::vector<T>& rows,
    const std::vector<T>& cols)
{
  size_t I = rows.size();
  size_t J = cols.size();

  std::vector<size_t> v(2*I*J);

  for (size_t i = 0; i < I; i++){
    for (size_t j = 0; j < J; j++){
      v[2*j + 2*i*J] = rows[i];
      v[2*j+1 + 2*i*J] = cols[j];
    }
  }

  return h5gt::ElementSet(v);
}

}

#endif // H5UTILIMPL_H
