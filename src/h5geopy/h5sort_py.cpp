#include "../../include/h5geopy/h5sort_py.h"

#include <h5geo/misc/h5sort.h>

namespace h5geopy {

namespace ext {

// SORT
template <typename D>
Eigen::VectorX<ptrdiff_t> sort(const py::EigenDRef<const D> v)
{
  return h5geo::sort(v);
}

template <typename D>
std::tuple<Eigen::VectorX<ptrdiff_t>, D> sort_vsorted(const py::EigenDRef<const D> v)
{
  D v_sorted;
  Eigen::VectorX<ptrdiff_t> ind = h5geo::sort(v, v_sorted);
  return std::make_tuple(std::move(ind), std::move(v_sorted));
}

template <typename D>
Eigen::VectorX<ptrdiff_t> sort_rows(const py::EigenDRef<const D> M)
{
  return h5geo::sort_rows(M);
}

template <typename D>
std::tuple<Eigen::VectorX<ptrdiff_t>, D> sort_rows_msorted(const py::EigenDRef<const D> M)
{
  D M_sorted;
  Eigen::VectorX<ptrdiff_t> ind = h5geo::sort_rows(M, M_sorted);
  return std::make_tuple(std::move(ind), std::move(M_sorted));
}

template <typename D>
std::tuple<
Eigen::VectorX<ptrdiff_t>,
Eigen::VectorX<typename D::Scalar>,
Eigen::MatrixX2<ptrdiff_t>
>
sort_unique(const py::EigenDRef<const D> v)
{
  Eigen::VectorX<typename D::Scalar> uvals;
  Eigen::MatrixX2<ptrdiff_t> uvals_from_size;
  Eigen::VectorX<ptrdiff_t> ind = h5geo::sort_unique(v, uvals, uvals_from_size);

  return std::make_tuple(std::move(ind), std::move(uvals), std::move(uvals_from_size));
}

template <typename D>
std::tuple<
Eigen::VectorX<ptrdiff_t>,
Eigen::VectorX<typename D::Scalar>,
Eigen::MatrixX2<ptrdiff_t>,
D
>
sort_unique_vsorted(const py::EigenDRef<const D> v)
{
  Eigen::VectorX<typename D::Scalar> uvals;
  Eigen::MatrixX2<ptrdiff_t> uvals_from_size;
  D v_sorted;
  Eigen::VectorX<ptrdiff_t> ind = h5geo::sort_unique(v, uvals, uvals_from_size, v_sorted);

  return std::make_tuple(std::move(ind), std::move(uvals), std::move(uvals_from_size), std::move(v_sorted));
}

template <typename D>
std::tuple<
Eigen::VectorX<ptrdiff_t>,
Eigen::MatrixX<typename D::Scalar>,
Eigen::MatrixX2<ptrdiff_t>
>
sort_rows_unique(const py::EigenDRef<const D> M)
{
  Eigen::MatrixX<typename D::Scalar> urows;
  Eigen::MatrixX2<ptrdiff_t> urows_from_size;
  Eigen::VectorX<ptrdiff_t> ind = h5geo::sort_rows_unique(M, urows, urows_from_size);

  return std::make_tuple(std::move(ind), std::move(urows), std::move(urows_from_size));
}

template <typename D>
std::tuple<
Eigen::VectorX<ptrdiff_t>,
Eigen::MatrixX<typename D::Scalar>,
Eigen::MatrixX2<ptrdiff_t>,
D
>
sort_rows_unique_msorted(const py::EigenDRef<const D> M)
{
  Eigen::MatrixX<typename D::Scalar> urows;
  Eigen::MatrixX2<ptrdiff_t> urows_from_size;
  D M_sorted;
  Eigen::VectorX<ptrdiff_t> ind = h5geo::sort_rows_unique(M, urows, urows_from_size, M_sorted);

  return std::make_tuple(std::move(ind), std::move(urows), std::move(urows_from_size), std::move(M_sorted));
}


} // ext


void defineSortFunctions(py::module_& m){
  // SORT
  m.def("sort", &ext::sort<const Eigen::VectorXf>,
        py::arg("v"),
        "return indices such that `v_sorted = v(ind)`. Input parameter `v` is a vector");
  m.def("sort", &ext::sort<const Eigen::VectorXd>,
        py::arg("v"));

  m.def("sortv", &ext::sort_vsorted<Eigen::VectorXf>,
        py::arg("v"),
        "return indices such that `v_sorted = v(ind)` and also return `v_sorted`. "
"Input parameter `v` is a vector");
  m.def("sortv", &ext::sort_vsorted<Eigen::VectorXd>,
        py::arg("v"));

  m.def("sort_rows", &ext::sort_rows<Eigen::MatrixXf>,
        py::arg("M"),
        "sorts the rows of a matrix in ascending order "
"based on the elements in the first column. When the first column "
"contains repeated elements, sortrows sorts according to the values "
"in the next column and repeats this behavior for succeeding equal values. "
"M_sorted = M(ind, Eigen::all)");
  m.def("sort_rows", &ext::sort_rows<Eigen::MatrixXd>,
        py::arg("M"));

  m.def("sort_rowsv", &ext::sort_rows_msorted<Eigen::MatrixXf>,
        py::arg("M"),
        "sorts the rows of a matrix in ascending order "
"based on the elements in the first column. When the first column "
"contains repeated elements, sortrows sorts according to the values "
"in the next column and repeats this behavior for succeeding equal values. "
"also calculates M_sorted = M(ind, Eigen::all)");
  m.def("sort_rowsv", &ext::sort_rows_msorted<Eigen::MatrixXd>,
        py::arg("M"));

  m.def("sort_unique", &ext::sort_unique<Eigen::VectorXf>,
        py::arg("v"),
        "find unique elements, sort them, identify unique values "
"start and end indices and return indices `ind` such that v_sorted = v(ind), "
"`uvals` vector and two column matrix `uvals_from_size` where"
"first col - start index, second col - number of elements. "
"Each row can be considered as v_sorted.segment(uvals_from_size.row(n)) "
"gives the same unique value uval.");
  m.def("sort_unique", &ext::sort_unique<Eigen::VectorXd>,
        py::arg("v"));

  m.def("sort_uniquev", &ext::sort_unique_vsorted<Eigen::VectorXf>,
        py::arg("v"),
        "find unique elements, sort them, identify unique values "
"start and end indices and return indices `ind` such that v_sorted = v(ind), "
"`uvals` vector and two column matrix `uvals_from_size` where"
"first col - start index, second col - number of elements. "
"Each row can be considered as v_sorted.segment(uvals_from_size.row(n)) "
"gives the same unique value uval. "
"Also return `v_sorted`");
  m.def("sort_uniquev", &ext::sort_unique_vsorted<Eigen::VectorXd>,
        py::arg("v"));

  m.def("sort_rows_unique", &ext::sort_rows_unique<Eigen::MatrixXf>,
        py::arg("M"),
        "find unique rows, sort them, identify unique rows "
"start and end row-indices and return row-indices such that M_sorted = M(ind, Eigen::all)."
"Return indices `ind`, `urows` and `urows_from_size`");
  m.def("sort_rows_unique", &ext::sort_rows_unique<Eigen::MatrixXd>,
        py::arg("M"));

  m.def("sort_rows_uniquev", &ext::sort_rows_unique_msorted<Eigen::MatrixXf>,
        py::arg("M"),
        "find unique rows, sort them, identify unique rows "
"start and end row-indices and return row-indices such that M_sorted = M(ind, Eigen::all)."
"Return indices `ind`, `urows` and `urows_from_size`. Also return `M_sorted`");
  m.def("sort_rows_uniquev", &ext::sort_rows_unique_msorted<Eigen::MatrixXd>,
        py::arg("M"));
}


} // h5geopy
