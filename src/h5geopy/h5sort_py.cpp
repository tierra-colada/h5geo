#include "../../include/h5geopy/h5sort_py.h"

#include <h5geo/private/h5sort.h>

namespace h5geopy {

namespace ext {

// SORT
template <typename T>
Eigen::VectorX<ptrdiff_t> sort(const Eigen::VectorX<T>& v)
{
  return h5geo::sort(v);
}

template <typename T>
std::tuple<Eigen::VectorX<ptrdiff_t>, Eigen::VectorX<T>> sort_vsorted(const Eigen::VectorX<T>& v)
{
  Eigen::VectorX<T> v_sorted;
  Eigen::VectorX<ptrdiff_t> ind = h5geo::sort(v, v_sorted);
  return std::make_tuple(std::move(ind), std::move(v_sorted));
}

template <typename T>
Eigen::VectorX<ptrdiff_t> sort_rows(const Eigen::MatrixX<T> &M)
{
  return h5geo::sort_rows(M);
}

template <typename T>
std::tuple<Eigen::VectorX<ptrdiff_t>, Eigen::MatrixX<T>> sort_rows_msorted(const Eigen::MatrixX<T> &M)
{
  Eigen::MatrixX<T> M_sorted;
  Eigen::VectorX<ptrdiff_t> ind = h5geo::sort_rows(M, M_sorted);
  return std::make_tuple(std::move(ind), std::move(M_sorted));
}

template <typename T>
std::tuple<
Eigen::VectorX<ptrdiff_t>,
Eigen::VectorX<T>,
Eigen::MatrixX2<ptrdiff_t>
>
sort_unique(const Eigen::VectorX<T> &v)
{
  Eigen::VectorX<T> uvals;
  Eigen::MatrixX2<ptrdiff_t> uvals_from_size;
  Eigen::VectorX<ptrdiff_t> ind = h5geo::sort_unique(v, uvals, uvals_from_size);

  return std::make_tuple(std::move(ind), std::move(uvals), std::move(uvals_from_size));
}

template <typename T>
std::tuple<
Eigen::VectorX<ptrdiff_t>,
Eigen::VectorX<T>,
Eigen::MatrixX2<ptrdiff_t>,
Eigen::VectorX<T>
>
sort_unique_vsorted(const Eigen::VectorX<T> &v)
{
  Eigen::VectorX<T> uvals;
  Eigen::MatrixX2<ptrdiff_t> uvals_from_size;
  Eigen::VectorX<T> v_sorted;
  Eigen::VectorX<ptrdiff_t> ind = h5geo::sort_unique(v, uvals, uvals_from_size, v_sorted);

  return std::make_tuple(std::move(ind), std::move(uvals), std::move(uvals_from_size), std::move(v_sorted));
}

template <typename T>
std::tuple<
Eigen::VectorX<ptrdiff_t>,
Eigen::MatrixX<T>,
Eigen::MatrixX2<ptrdiff_t>
>
sort_rows_unique(const Eigen::MatrixX<T> &M)
{
  Eigen::MatrixX<T> urows;
  Eigen::MatrixX2<ptrdiff_t> urows_from_size;
  Eigen::VectorX<ptrdiff_t> ind = h5geo::sort_rows_unique(M, urows, urows_from_size);

  return std::make_tuple(std::move(ind), std::move(urows), std::move(urows_from_size));
}

template <typename T>
std::tuple<
Eigen::VectorX<ptrdiff_t>,
Eigen::MatrixX<T>,
Eigen::MatrixX2<ptrdiff_t>,
Eigen::MatrixX<T>
>
sort_rows_unique_msorted(const Eigen::MatrixX<T> &M)
{
  Eigen::MatrixX<T> urows;
  Eigen::MatrixX2<ptrdiff_t> urows_from_size;
  Eigen::MatrixX<T> M_sorted;
  Eigen::VectorX<ptrdiff_t> ind = h5geo::sort_rows_unique(M, urows, urows_from_size, M_sorted);

  return std::make_tuple(std::move(ind), std::move(urows), std::move(urows_from_size), std::move(M_sorted));
}


} // ext


void defineSortFunctions(py::module_& m){
  // SORT
  m.def("sort", &ext::sort<float>,
        py::arg("v"),
        "return indexes such that `v_sorted = v(ind)`. Input parameter `v` is a vector");
  m.def("sort", &ext::sort<double>,
        py::arg("v"));

  m.def("sortv", &ext::sort_vsorted<float>,
        py::arg("v"),
        "return indexes such that `v_sorted = v(ind)` and also return `v_sorted`. "
"Input parameter `v` is a vector");
  m.def("sortv", &ext::sort_vsorted<double>,
        py::arg("v"));

  m.def("sort_rows", &ext::sort_rows<float>,
        py::arg("M"),
        "sorts the rows of a matrix in ascending order "
"based on the elements in the first column. When the first column "
"contains repeated elements, sortrows sorts according to the values "
"in the next column and repeats this behavior for succeeding equal values. "
"M_sorted = M(ind, Eigen::all)");
  m.def("sort_rows", &ext::sort_rows<double>,
        py::arg("M"));

  m.def("sort_rowsv", &ext::sort_rows_msorted<float>,
        py::arg("M"),
        "sorts the rows of a matrix in ascending order "
"based on the elements in the first column. When the first column "
"contains repeated elements, sortrows sorts according to the values "
"in the next column and repeats this behavior for succeeding equal values. "
"also calculates M_sorted = M(ind, Eigen::all)");
  m.def("sort_rowsv", &ext::sort_rows_msorted<double>,
        py::arg("M"));

  m.def("sort_unique", &ext::sort_unique<float>,
        py::arg("v"),
        "find unique elements, sort them, identify unique values "
"start and end indexes and return indexes `ind` such that v_sorted = v(ind), "
"`uvals` vector and two column matrix `uvals_from_size` where"
"first col - start index, second col - number of elements. "
"Each row can be considered as v_sorted.segment(uvals_from_size.row(n)) "
"gives the same unique value uval.");
  m.def("sort_unique", &ext::sort_unique<double>,
        py::arg("v"));

  m.def("sort_uniquev", &ext::sort_unique_vsorted<float>,
        py::arg("v"),
        "find unique elements, sort them, identify unique values "
"start and end indexes and return indexes `ind` such that v_sorted = v(ind), "
"`uvals` vector and two column matrix `uvals_from_size` where"
"first col - start index, second col - number of elements. "
"Each row can be considered as v_sorted.segment(uvals_from_size.row(n)) "
"gives the same unique value uval. "
"Also return `v_sorted`");
  m.def("sort_uniquev", &ext::sort_unique_vsorted<double>,
        py::arg("v"));

  m.def("sort_rows_unique", &ext::sort_rows_unique<float>,
        py::arg("M"),
        "find unique rows, sort them, identify unique rows "
"start and end row-indexes and return row-indexes such that M_sorted = M(ind, Eigen::all)."
"Return indexes `ind`, `urows` and `urows_from_size`");
  m.def("sort_rows_unique", &ext::sort_rows_unique<double>,
        py::arg("M"));

  m.def("sort_rows_uniquev", &ext::sort_rows_unique_msorted<float>,
        py::arg("M"),
        "find unique rows, sort them, identify unique rows "
"start and end row-indexes and return row-indexes such that M_sorted = M(ind, Eigen::all)."
"Return indexes `ind`, `urows` and `urows_from_size`. Also return `M_sorted`");
  m.def("sort_rows_uniquev", &ext::sort_rows_unique_msorted<double>,
        py::arg("M"));
}


} // h5geopy
