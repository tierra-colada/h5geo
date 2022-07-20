#include "../../include/h5geo/private/h5sort.h"
#include "h5geo_export.h"

#include <algorithm>    // std::sort, std::stable_sort

// std::execution must be hidden in .cpp as it causes errors:
// both TBB and Qt has `emit` macro that makes macro collision (on Linux for sure).
// see:
// https://forum.qt.io/topic/122225/trying-to-use-c-17-parallel-algorithms-with-qt/9
#ifdef H5GEO_USE_THREADS
#include <execution>
#endif


namespace h5geo
{

/// \brief _sort return indexes such that v_sorted = v(ind).
/// \param v Vector (row/col)
/// \return ind
template <typename D>
Eigen::VectorX<ptrdiff_t> _sort(
    const Eigen::DenseBase<D> &v)
{

  // initialize original index locations
  Eigen::VectorX<ptrdiff_t> idx =
      Eigen::ArrayX<ptrdiff_t>::LinSpaced(
        v.size(), 0, v.size()-1);

#ifdef H5GEO_USE_THREADS
  std::stable_sort(std::execution::seq, idx.begin(), idx.end(),
                   [&v](
                   const ptrdiff_t& i1,
                   const ptrdiff_t& i2)
  {return v(i1) < v(i2);});
#else
  std::stable_sort(idx.begin(), idx.end(),
                   [&v](
                   const ptrdiff_t& i1,
                   const ptrdiff_t& i2)
  {return v(i1) < v(i2);});
#endif

  return idx;
}

template <typename T>
Eigen::VectorX<ptrdiff_t> sort(
    const Eigen::VectorX<T> &v)
{
  return _sort(v);
}

template <typename T>
Eigen::VectorX<ptrdiff_t> sort(
    const Eigen::VectorX<T> &v,
    Eigen::VectorX<T>& v_sorted){
  Eigen::VectorX<ptrdiff_t> idx = _sort(v);
  v_sorted = v(idx);
  return idx;
}

/// \brief _sort_rows  sorts the rows of a matrix in ascending order
/// based on the elements in the first column. When the first column
/// contains repeated elements, sortrows sorts according to the values
/// in the next column and repeats this behavior for succeeding equal values.
/// M_sorted = M(ind, Eigen::all)
/// \param M
/// \return ind
template <typename D>
Eigen::VectorX<ptrdiff_t> _sort_rows(
    const Eigen::DenseBase<D> &M){

  // initialize original index locations
  Eigen::VectorX<ptrdiff_t> idx =
      Eigen::ArrayX<ptrdiff_t>::LinSpaced(
        M.rows(), 0, M.rows()-1);

  ptrdiff_t col = 0;

  std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun;
  cmp_fun = [&M, &col, &cmp_fun](
      const ptrdiff_t& row1,
      const ptrdiff_t& row2)->bool
  {
    if (M(row1, col) < M(row2, col)){
      col = 0;
      return true;
    }

    if (M(row1, col) > M(row2, col)){
      col = 0;
      return false;
    }

    if (col == M.cols()-1){
      col = 0;
      return false;
    }

    col++;
    return cmp_fun(row1, row2);
  };

#ifdef H5GEO_USE_THREADS
  stable_sort(std::execution::seq, idx.begin(), idx.end(), cmp_fun);
#else
  stable_sort(idx.begin(), idx.end(), cmp_fun);
#endif
  return idx;
}

template <typename T>
Eigen::VectorX<ptrdiff_t> sort_rows(
    const Eigen::MatrixX<T> &M)
{
  return _sort_rows(M);
}

template <typename T>
Eigen::VectorX<ptrdiff_t> sort_rows(
    const Eigen::MatrixX<T> &M,
    Eigen::MatrixX<T>& M_sorted)
{
  Eigen::VectorX<ptrdiff_t> idx = _sort_rows(M);
  M_sorted = M(idx, Eigen::all);
  return idx;
}

/// \brief _sort_unique find unique elements, sort them, identify unique values
/// start and end indexes and return indexes such that v_sorted = v(ind).
/// \param v vector
/// \param uvals
/// \param uvals_from_size first col - start index, second col - number of elements.
/// Each row can be considered as v_sorted.segment(uvals_from_size.row(n))
/// gives the same unique value uval.
/// Also v_sorted.segment(uvals_from_size.row(n)) = v(ind.segment(uvals_from_size.row(n)))
/// \return ind
template <typename D>
Eigen::VectorX<ptrdiff_t> _sort_unique(
    const Eigen::DenseBase<D> &v,
    Eigen::VectorX<typename D::Scalar> &uvals,
    Eigen::MatrixX2<ptrdiff_t> &uvals_from_size)
{
  if (v.size() < 1)
    return Eigen::VectorX<ptrdiff_t>();

  Eigen::VectorX<ptrdiff_t> idx = _sort(v);

  uvals.resize(idx.size());
  uvals_from_size.resize(idx.size(), Eigen::NoChange);

  // initialize first iteration
  uvals(0) = v(idx(0));
  uvals_from_size(0, 0) = 0;
  uvals_from_size(0, 1) = 1;

  ptrdiff_t ii = 0;
  for (ptrdiff_t i = 1; i < idx.size(); i++){
    if (v(idx(i - 1)) == v(idx(i))){
      uvals_from_size(ii, 1)++;
    } else {
      ii++;
      uvals(ii) = v(idx(i));
      uvals_from_size(ii, 0) = i;
      uvals_from_size(ii, 1) = 1;
    }
  }
  uvals.conservativeResize(ii + 1);
  uvals_from_size.conservativeResize(ii + 1, Eigen::NoChange);
  return idx;
}

template <typename T>
Eigen::VectorX<ptrdiff_t> sort_unique(
    const Eigen::VectorX<T> &v,
    Eigen::VectorX<T> &uvals,
    Eigen::MatrixX2<ptrdiff_t> &uvals_from_size)
{
  return _sort_unique(v, uvals, uvals_from_size);
}

template <typename T>
Eigen::VectorX<ptrdiff_t> sort_unique(
    const Eigen::VectorX<T> &v,
    Eigen::VectorX<T> &uvals,
    Eigen::MatrixX2<ptrdiff_t> &uvals_from_size,
    Eigen::VectorX<T>& v_sorted)
{
  Eigen::VectorX<ptrdiff_t> idx = _sort_unique(v, uvals, uvals_from_size);
  v_sorted = v(idx);
  return idx;
}

/// \brief _sort_rows_unique find unique rows, sort them, identify unique rows
/// start and end row-indexes and return row-indexes such that M_sorted = M(ind, Eigen::all).
/// \param M
/// \param urows
/// \param urows_from_size first col - start index, second col - number of elements.
/// Each row can be considered as:
/// M_sorted.middleRows(urows_from_size.row(n))
/// gives the same unique value uval.
/// Also:
/// M_sorted.middleRows(urows_from_size.row(n)) = M(ind.segment(urows_from_size.row(n)))
/// \return ind
template <typename D>
Eigen::VectorX<ptrdiff_t> _sort_rows_unique(
    const Eigen::DenseBase<D> &M,
    Eigen::MatrixX<typename D::Scalar> &urows,
    Eigen::MatrixX2<ptrdiff_t> &urows_from_size)
{
  if (M.rows() < 1)
    return Eigen::VectorX<ptrdiff_t>();

  Eigen::VectorX<ptrdiff_t> idx = _sort_rows(M);

  urows.resize(idx.size(), M.cols());
  urows_from_size.resize(idx.size(), Eigen::NoChange);

  // initialize first iteration
  urows.row(0) = M.row(idx(0));
  urows_from_size(0, 0) = 0;
  urows_from_size(0, 1) = 1;

  ptrdiff_t ii = 0;
  for (ptrdiff_t i = 1; i < idx.size(); i++){
    if (M.row(idx(i - 1)) == M.row(idx(i))){
      urows_from_size(ii, 1)++;
    } else {
      ii++;
      urows.row(ii) = M.row(idx(i));
      urows_from_size(ii, 0) = i;
      urows_from_size(ii, 1) = 1;
    }
  }
  urows.conservativeResize(ii + 1, Eigen::NoChange);
  urows_from_size.conservativeResize(ii + 1, Eigen::NoChange);
  return idx;
}

template <typename T>
Eigen::VectorX<ptrdiff_t> sort_rows_unique(
    const Eigen::MatrixX<T> &M,
    Eigen::MatrixX<T> &urows,
    Eigen::MatrixX2<ptrdiff_t> &urows_from_size)
{
  return _sort_rows_unique(M, urows, urows_from_size);
}

template <typename T>
Eigen::VectorX<ptrdiff_t> sort_rows_unique(
    const Eigen::MatrixX<T> &M,
    Eigen::MatrixX<T> &urows,
    Eigen::MatrixX2<ptrdiff_t> &urows_from_size,
    Eigen::MatrixX<T>& M_sorted)
{
  Eigen::VectorX<ptrdiff_t> idx = _sort_rows_unique(M, urows, urows_from_size);
  M_sorted = M(idx, Eigen::all);
  return idx;
}


// explicit instantiation
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort(
    const Eigen::VectorX<bool> &v);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort(
    const Eigen::VectorX<char> &v);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort(
    const Eigen::VectorX<unsigned char> &v);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort(
    const Eigen::VectorX<short> &v);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort(
    const Eigen::VectorX<int> &v);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort(
    const Eigen::VectorX<unsigned> &v);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort(
    const Eigen::VectorX<ptrdiff_t> &v);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort(
    const Eigen::VectorX<size_t> &v);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort(
    const Eigen::VectorX<float> &v);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort(
    const Eigen::VectorX<double> &v);


template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort(
    const Eigen::VectorX<bool> &v,
    Eigen::VectorX<bool>& v_sorted);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort(
    const Eigen::VectorX<char> &v,
    Eigen::VectorX<char>& v_sorted);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort(
    const Eigen::VectorX<unsigned char> &v,
    Eigen::VectorX<unsigned char>& v_sorted);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort(
    const Eigen::VectorX<short> &v,
    Eigen::VectorX<short>& v_sorted);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort(
    const Eigen::VectorX<int> &v,
    Eigen::VectorX<int>& v_sorted);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort(
    const Eigen::VectorX<unsigned> &v,
    Eigen::VectorX<unsigned>& v_sorted);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort(
    const Eigen::VectorX<ptrdiff_t> &v,
    Eigen::VectorX<ptrdiff_t>& v_sorted);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort(
    const Eigen::VectorX<size_t> &v,
    Eigen::VectorX<size_t>& v_sorted);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort(
    const Eigen::VectorX<float> &v,
    Eigen::VectorX<float>& v_sorted);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort(
    const Eigen::VectorX<double> &v,
    Eigen::VectorX<double>& v_sorted);


template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_rows(
    const Eigen::MatrixX<bool> &M);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_rows(
    const Eigen::MatrixX<char> &M);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_rows(
    const Eigen::MatrixX<unsigned char> &M);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_rows(
    const Eigen::MatrixX<short> &M);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_rows(
    const Eigen::MatrixX<int> &M);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_rows(
    const Eigen::MatrixX<unsigned> &M);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_rows(
    const Eigen::MatrixX<ptrdiff_t> &M);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_rows(
    const Eigen::MatrixX<size_t> &M);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_rows(
    const Eigen::MatrixX<float> &M);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_rows(
    const Eigen::MatrixX<double> &M);

template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_rows(
    const Eigen::MatrixX<bool> &M,
    Eigen::MatrixX<bool>& M_sorted);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_rows(
    const Eigen::MatrixX<char> &M,
    Eigen::MatrixX<char>& M_sorted);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_rows(
    const Eigen::MatrixX<unsigned char> &M,
    Eigen::MatrixX<unsigned char>& M_sorted);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_rows(
    const Eigen::MatrixX<short> &M,
    Eigen::MatrixX<short>& M_sorted);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_rows(
    const Eigen::MatrixX<int> &M,
    Eigen::MatrixX<int>& M_sorted);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_rows(
    const Eigen::MatrixX<unsigned> &M,
    Eigen::MatrixX<unsigned>& M_sorted);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_rows(
    const Eigen::MatrixX<ptrdiff_t> &M,
    Eigen::MatrixX<ptrdiff_t>& M_sorted);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_rows(
    const Eigen::MatrixX<size_t> &M,
    Eigen::MatrixX<size_t>& M_sorted);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_rows(
    const Eigen::MatrixX<float> &M,
    Eigen::MatrixX<float>& M_sorted);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_rows(
    const Eigen::MatrixX<double> &M,
    Eigen::MatrixX<double>& M_sorted);


template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_unique(
    const Eigen::VectorX<bool> &v,
    Eigen::VectorX<bool> &uvals,
    Eigen::MatrixX2<ptrdiff_t> &uvals_from_size);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_unique(
    const Eigen::VectorX<char> &v,
    Eigen::VectorX<char> &uvals,
    Eigen::MatrixX2<ptrdiff_t> &uvals_from_size);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_unique(
    const Eigen::VectorX<unsigned char> &v,
    Eigen::VectorX<unsigned char> &uvals,
    Eigen::MatrixX2<ptrdiff_t> &uvals_from_size);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_unique(
    const Eigen::VectorX<short> &v,
    Eigen::VectorX<short> &uvals,
    Eigen::MatrixX2<ptrdiff_t> &uvals_from_size);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_unique(
    const Eigen::VectorX<int> &v,
    Eigen::VectorX<int> &uvals,
    Eigen::MatrixX2<ptrdiff_t> &uvals_from_size);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_unique(
    const Eigen::VectorX<unsigned> &v,
    Eigen::VectorX<unsigned> &uvals,
    Eigen::MatrixX2<ptrdiff_t> &uvals_from_size);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_unique(
    const Eigen::VectorX<ptrdiff_t> &v,
    Eigen::VectorX<ptrdiff_t> &uvals,
    Eigen::MatrixX2<ptrdiff_t> &uvals_from_size);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_unique(
    const Eigen::VectorX<size_t> &v,
    Eigen::VectorX<size_t> &uvals,
    Eigen::MatrixX2<ptrdiff_t> &uvals_from_size);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_unique(
    const Eigen::VectorX<float> &v,
    Eigen::VectorX<float> &uvals,
    Eigen::MatrixX2<ptrdiff_t> &uvals_from_size);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_unique(
    const Eigen::VectorX<double> &v,
    Eigen::VectorX<double> &uvals,
    Eigen::MatrixX2<ptrdiff_t> &uvals_from_size);

template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_unique(
    const Eigen::VectorX<bool> &v,
    Eigen::VectorX<bool> &uvals,
    Eigen::MatrixX2<ptrdiff_t> &uvals_from_size,
    Eigen::VectorX<bool>& v_sorted);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_unique(
    const Eigen::VectorX<char> &v,
    Eigen::VectorX<char> &uvals,
    Eigen::MatrixX2<ptrdiff_t> &uvals_from_size,
    Eigen::VectorX<char>& v_sorted);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_unique(
    const Eigen::VectorX<unsigned char> &v,
    Eigen::VectorX<unsigned char> &uvals,
    Eigen::MatrixX2<ptrdiff_t> &uvals_from_size,
    Eigen::VectorX<unsigned char>& v_sorted);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_unique(
    const Eigen::VectorX<short> &v,
    Eigen::VectorX<short> &uvals,
    Eigen::MatrixX2<ptrdiff_t> &uvals_from_size,
    Eigen::VectorX<short>& v_sorted);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_unique(
    const Eigen::VectorX<int> &v,
    Eigen::VectorX<int> &uvals,
    Eigen::MatrixX2<ptrdiff_t> &uvals_from_size,
    Eigen::VectorX<int>& v_sorted);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_unique(
    const Eigen::VectorX<unsigned> &v,
    Eigen::VectorX<unsigned> &uvals,
    Eigen::MatrixX2<ptrdiff_t> &uvals_from_size,
    Eigen::VectorX<unsigned>& v_sorted);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_unique(
    const Eigen::VectorX<ptrdiff_t> &v,
    Eigen::VectorX<ptrdiff_t> &uvals,
    Eigen::MatrixX2<ptrdiff_t> &uvals_from_size,
    Eigen::VectorX<ptrdiff_t>& v_sorted);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_unique(
    const Eigen::VectorX<size_t> &v,
    Eigen::VectorX<size_t> &uvals,
    Eigen::MatrixX2<ptrdiff_t> &uvals_from_size,
    Eigen::VectorX<size_t>& v_sorted);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_unique(
    const Eigen::VectorX<float> &v,
    Eigen::VectorX<float> &uvals,
    Eigen::MatrixX2<ptrdiff_t> &uvals_from_size,
    Eigen::VectorX<float>& v_sorted);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_unique(
    const Eigen::VectorX<double> &v,
    Eigen::VectorX<double> &uvals,
    Eigen::MatrixX2<ptrdiff_t> &uvals_from_size,
    Eigen::VectorX<double>& v_sorted);


template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_rows_unique(
    const Eigen::MatrixX<bool> &M,
    Eigen::MatrixX<bool> &urows,
    Eigen::MatrixX2<ptrdiff_t> &urows_from_size);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_rows_unique(
    const Eigen::MatrixX<char> &M,
    Eigen::MatrixX<char> &urows,
    Eigen::MatrixX2<ptrdiff_t> &urows_from_size);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_rows_unique(
    const Eigen::MatrixX<unsigned char> &M,
    Eigen::MatrixX<unsigned char> &urows,
    Eigen::MatrixX2<ptrdiff_t> &urows_from_size);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_rows_unique(
    const Eigen::MatrixX<short> &M,
    Eigen::MatrixX<short> &urows,
    Eigen::MatrixX2<ptrdiff_t> &urows_from_size);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_rows_unique(
    const Eigen::MatrixX<int> &M,
    Eigen::MatrixX<int> &urows,
    Eigen::MatrixX2<ptrdiff_t> &urows_from_size);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_rows_unique(
    const Eigen::MatrixX<unsigned> &M,
    Eigen::MatrixX<unsigned> &urows,
    Eigen::MatrixX2<ptrdiff_t> &urows_from_size);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_rows_unique(
    const Eigen::MatrixX<ptrdiff_t> &M,
    Eigen::MatrixX<ptrdiff_t> &urows,
    Eigen::MatrixX2<ptrdiff_t> &urows_from_size);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_rows_unique(
    const Eigen::MatrixX<size_t> &M,
    Eigen::MatrixX<size_t> &urows,
    Eigen::MatrixX2<ptrdiff_t> &urows_from_size);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_rows_unique(
    const Eigen::MatrixX<float> &M,
    Eigen::MatrixX<float> &urows,
    Eigen::MatrixX2<ptrdiff_t> &urows_from_size);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_rows_unique(
    const Eigen::MatrixX<double> &M,
    Eigen::MatrixX<double> &urows,
    Eigen::MatrixX2<ptrdiff_t> &urows_from_size);

template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_rows_unique(
    const Eigen::MatrixX<bool> &M,
    Eigen::MatrixX<bool> &urows,
    Eigen::MatrixX2<ptrdiff_t> &urows_from_size,
    Eigen::MatrixX<bool>& M_sorted);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_rows_unique(
    const Eigen::MatrixX<char> &M,
    Eigen::MatrixX<char> &urows,
    Eigen::MatrixX2<ptrdiff_t> &urows_from_size,
    Eigen::MatrixX<char>& M_sorted);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_rows_unique(
    const Eigen::MatrixX<unsigned char> &M,
    Eigen::MatrixX<unsigned char> &urows,
    Eigen::MatrixX2<ptrdiff_t> &urows_from_size,
    Eigen::MatrixX<unsigned char>& M_sorted);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_rows_unique(
    const Eigen::MatrixX<short> &M,
    Eigen::MatrixX<short> &urows,
    Eigen::MatrixX2<ptrdiff_t> &urows_from_size,
    Eigen::MatrixX<short>& M_sorted);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_rows_unique(
    const Eigen::MatrixX<int> &M,
    Eigen::MatrixX<int> &urows,
    Eigen::MatrixX2<ptrdiff_t> &urows_from_size,
    Eigen::MatrixX<int>& M_sorted);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_rows_unique(
    const Eigen::MatrixX<unsigned> &M,
    Eigen::MatrixX<unsigned> &urows,
    Eigen::MatrixX2<ptrdiff_t> &urows_from_size,
    Eigen::MatrixX<unsigned>& M_sorted);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_rows_unique(
    const Eigen::MatrixX<ptrdiff_t> &M,
    Eigen::MatrixX<ptrdiff_t> &urows,
    Eigen::MatrixX2<ptrdiff_t> &urows_from_size,
    Eigen::MatrixX<ptrdiff_t>& M_sorted);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_rows_unique(
    const Eigen::MatrixX<size_t> &M,
    Eigen::MatrixX<size_t> &urows,
    Eigen::MatrixX2<ptrdiff_t> &urows_from_size,
    Eigen::MatrixX<size_t>& M_sorted);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_rows_unique(
    const Eigen::MatrixX<float> &M,
    Eigen::MatrixX<float> &urows,
    Eigen::MatrixX2<ptrdiff_t> &urows_from_size,
    Eigen::MatrixX<float>& M_sorted);
template H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> sort_rows_unique(
    const Eigen::MatrixX<double> &M,
    Eigen::MatrixX<double> &urows,
    Eigen::MatrixX2<ptrdiff_t> &urows_from_size,
    Eigen::MatrixX<double>& M_sorted);

}
