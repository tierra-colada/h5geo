#ifndef H5SORT_H
#define H5SORT_H

#include <Eigen/Dense>

namespace h5geo
{

namespace detail
{

/// \brief sort implements sorting while hiding std::execution within `.cpp`
/// \param M matrix or vector to sort
/// \param idx empty vector passed by referece to improve perfomance
/// \param cmp_fun compare function
template <typename D>
void _sort(
    const D &M,
    Eigen::VectorX<ptrdiff_t> &idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);

} // detail

/// \brief sort return indexes such that v_sorted = v(ind).
/// \param v Vector (row/col)
/// \return ind
template <typename D>
Eigen::VectorX<ptrdiff_t> sort(const Eigen::DenseBase<D> &v){

  Eigen::VectorX<ptrdiff_t> idx;
  std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun;
  cmp_fun = [&v](
      const ptrdiff_t& i1,
      const ptrdiff_t& i2)->bool {return v(i1,0) < v(i2,0);};
  detail::_sort(v.derived(), idx, cmp_fun);

  return idx;
}

/// \brief sort also calculates v_sorted_ = v(ind).
/// \param v Vector (row/col)
/// \param v_sorted_ Vector (row/col). Fundamental types of `v_sorted_` and `v` should be the same
/// \return ind
template <typename DRaw, typename DSort>
Eigen::VectorX<ptrdiff_t> sort(
    const Eigen::DenseBase<DRaw> &v,
    Eigen::DenseBase<DSort> const &v_sorted_)
{
  Eigen::VectorX<ptrdiff_t> idx = sort(v);

  Eigen::DenseBase<DSort>& v_sorted =
      const_cast< Eigen::DenseBase<DSort>& >(v_sorted_);
  v_sorted = v(idx, Eigen::all);
  return idx;
}

/// \brief sort_rows  sorts the rows of a matrix in ascending order
/// based on the elements in the first column. When the first column
/// contains repeated elements, sortrows sorts according to the values
/// in the next column and repeats this behavior for succeeding equal values.
/// M_sorted = M(ind, Eigen::all)
/// \param M
/// \return ind
template <typename D>
Eigen::VectorX<ptrdiff_t> sort_rows(const Eigen::DenseBase<D> &M){

  // initialize original index locations
  Eigen::VectorX<ptrdiff_t> idx;

  std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun;
  cmp_fun = [&M, &cmp_fun](
      const ptrdiff_t& row1,
      const ptrdiff_t& row2)->bool
  {
    ptrdiff_t N = M.cols()-1;
    for (ptrdiff_t col = 0; col < N; col++){
      if (M(row1, col) < M(row2, col))
        return true;

      if (M(row1, col) > M(row2, col))
        return false;
    }

    // notice the operator is '<=' as it is the last column check
    // i.e. when all other columns are equal at these rows
    if (M(row1, Eigen::last) <= M(row2, Eigen::last))
      return true;

    return false;
  };

  detail::_sort(M.derived(), idx, cmp_fun);

  return idx;
}

/// \brief sort_rows also calculates M_sorted = M(ind, Eigen::all).
/// \param M Matrix wich rows should be sorted
/// \param M_sorted_ Row-sorted matrix. Fundamental type of
/// M_sorted_ should be same as M type
/// \return ind
template <typename DRaw, typename DSort>
Eigen::VectorX<ptrdiff_t> sort_rows(
    const Eigen::DenseBase<DRaw> &M,
    Eigen::DenseBase<DSort> &M_sorted_)
{
  Eigen::VectorX<ptrdiff_t> idx = sort_rows(M);
  Eigen::DenseBase<DSort>& M_sorted =
      const_cast< Eigen::DenseBase<DSort>& >(M_sorted_);
  M_sorted = M(idx, Eigen::all);
  return idx;
}

/// \brief sort_unique find unique elements, sort them, identify unique values
/// start and end indexes and return indexes such that v_sorted = v(ind).
/// \param v vector
/// \param uvals
/// \param uvals_from_size first col - start index, second col - number of elements.
/// Each row can be considered as v_sorted.segment(uvals_from_size.row(n))
/// gives the same unique value uval.
/// Also v_sorted.segment(uvals_from_size.row(n)) = v(ind.segment(uvals_from_size.row(n)))
/// \return ind
template <typename DRaw, typename TUval>
Eigen::VectorX<ptrdiff_t> sort_unique(
    const Eigen::DenseBase<DRaw> &v,
    Eigen::VectorX<TUval> &uvals,
    Eigen::MatrixX2<ptrdiff_t> &uvals_from_size)
{
  if (v.size() < 1)
    return Eigen::VectorX<ptrdiff_t>();

  Eigen::VectorX<ptrdiff_t> idx = sort(v);

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

/// \brief sort_unique also calculates v_sorted = v(ind).
/// \param v vector
/// \param uvals
/// \param uvals_from_size
/// \param v_sorted_
/// \return ind
template <typename DRaw, typename TUval, typename DSort>
Eigen::VectorX<ptrdiff_t> sort_unique(
    const Eigen::DenseBase<DRaw> &v,
    Eigen::VectorX<TUval> &uvals,
    Eigen::MatrixX2<ptrdiff_t> &uvals_from_size,
    Eigen::DenseBase<DSort> const &v_sorted_)
{
  Eigen::VectorX<ptrdiff_t> idx = sort_unique(v, uvals, uvals_from_size);
  Eigen::DenseBase<DSort>& v_sorted =
      const_cast< Eigen::DenseBase<DSort>& >(v_sorted_);
  v_sorted = v(idx, Eigen::all);
  return idx;
}

/// \brief sort_rows_unique find unique rows, sort them, identify unique rows
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
template <typename DRaw, typename TUval>
Eigen::VectorX<ptrdiff_t> sort_rows_unique(
    const Eigen::DenseBase<DRaw> &M,
    Eigen::MatrixX<TUval> &urows,
    Eigen::MatrixX2<ptrdiff_t> &urows_from_size)
{
  if (M.rows() < 1)
    return Eigen::VectorX<ptrdiff_t>();

  Eigen::VectorX<ptrdiff_t> idx = sort_rows(M);

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

/// \brief sort_rows_unique also calculates M_sorted = M(ind, Eigen::all).
/// \param M
/// \param urows
/// \param urows_from_size
/// \param M_sorted_
/// \return ind
template <typename DRaw, typename TUval, typename DSort>
Eigen::VectorX<ptrdiff_t> sort_rows_unique(
    const Eigen::DenseBase<DRaw> &M,
    Eigen::MatrixX<TUval> &urows,
    Eigen::MatrixX2<ptrdiff_t> &urows_from_size,
    const Eigen::DenseBase<DSort> &M_sorted_)
{
  Eigen::VectorX<ptrdiff_t> idx = sort_rows_unique(M, urows, urows_from_size);
  Eigen::DenseBase<DSort>& M_sorted =
      const_cast< Eigen::DenseBase<DSort>& >(M_sorted_);
  M_sorted = M(idx, Eigen::all);
  return idx;
}

}


#endif // H5SORT_H
