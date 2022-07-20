#ifndef H5SORT_H
#define H5SORT_H

#include <Eigen/Dense>

namespace h5geo
{

/// \brief sort return indexes such that v_sorted = v(ind).
/// \param v Vector (row/col)
/// \return ind
template <typename T>
Eigen::VectorX<ptrdiff_t> sort(
    const Eigen::VectorX<T> &v);

/// \brief sort also calculates v_sorted_ = v(ind).
/// \param v Vector (row/col)
/// \param v_sorted_ Vector (row/col). Fundamental types of `v_sorted_` and `v` should be the same
/// \return ind
template <typename T>
Eigen::VectorX<ptrdiff_t> sort(
    const Eigen::VectorX<T> &v,
    Eigen::VectorX<T>& v_sorted);


/// \brief sort_rows  sorts the rows of a matrix in ascending order
/// based on the elements in the first column. When the first column
/// contains repeated elements, sortrows sorts according to the values
/// in the next column and repeats this behavior for succeeding equal values.
/// M_sorted = M(ind, Eigen::all)
/// \param M
/// \return ind
template <typename T>
Eigen::VectorX<ptrdiff_t> sort_rows(
    const Eigen::MatrixX<T> &M);

/// \brief sort_rows also calculates M_sorted = M(ind, Eigen::all).
/// \param M Matrix wich rows should be sorted
/// \param M_sorted_ Row-sorted matrix. Fundamental type of
/// M_sorted_ should be same as M type
/// \return ind
template <typename T>
Eigen::VectorX<ptrdiff_t> sort_rows(
    const Eigen::MatrixX<T> &M,
    Eigen::MatrixX<T>& M_sorted);


/// \brief sort_unique find unique elements, sort them, identify unique values
/// start and end indexes and return indexes such that v_sorted = v(ind).
/// \param v vector
/// \param uvals
/// \param uvals_from_size first col - start index, second col - number of elements.
/// Each row can be considered as v_sorted.segment(uvals_from_size.row(n))
/// gives the same unique value uval.
/// Also v_sorted.segment(uvals_from_size.row(n)) = v(ind.segment(uvals_from_size.row(n)))
/// \return ind
template <typename T>
Eigen::VectorX<ptrdiff_t> sort_unique(
    const Eigen::VectorX<T> &v,
    Eigen::VectorX<T> &uvals,
    Eigen::MatrixX2<ptrdiff_t> &uvals_from_size);

/// \brief sort_unique also calculates v_sorted = v(ind).
/// \param v vector
/// \param uvals
/// \param uvals_from_size
/// \param v_sorted_
/// \return ind
template <typename T>
Eigen::VectorX<ptrdiff_t> sort_unique(
    const Eigen::VectorX<T> &v,
    Eigen::VectorX<T> &uvals,
    Eigen::MatrixX2<ptrdiff_t> &uvals_from_size,
    Eigen::VectorX<T>& v_sorted);


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
template <typename T>
Eigen::VectorX<ptrdiff_t> sort_rows_unique(
    const Eigen::MatrixX<T> &M,
    Eigen::MatrixX<T> &urows,
    Eigen::MatrixX2<ptrdiff_t> &urows_from_size);

/// \brief sort_rows_unique also calculates M_sorted = M(ind, Eigen::all).
/// \param M
/// \param urows
/// \param urows_from_size
/// \param M_sorted_
/// \return ind
template <typename T>
Eigen::VectorX<ptrdiff_t> sort_rows_unique(
    const Eigen::MatrixX<T> &M,
    Eigen::MatrixX<T> &urows,
    Eigen::MatrixX2<ptrdiff_t> &urows_from_size,
    Eigen::MatrixX<T>& M_sorted);

}


#endif // H5SORT_H
