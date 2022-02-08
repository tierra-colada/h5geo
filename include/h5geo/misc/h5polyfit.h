#ifndef H5POLYFIT_H
#define H5POLYFIT_H

#include <Eigen/Dense>

namespace h5geo
{

template <typename D>
Eigen::VectorX<typename D::Scalar> polyfit(const Eigen::DenseBase<D> &x, const Eigen::DenseBase<D> &y,int order)
{
  if (x.size() != y.size())
    return Eigen::VectorX<typename D::Scalar>();

  if (order < 1 || order > x.size() - 1)
    return Eigen::VectorX<typename D::Scalar>();

  Eigen::MatrixX<typename D::Scalar> A(x.size(), order + 1);

  for (ptrdiff_t i = 0; i < x.size(); i++) {
    A(i, 0) = 1.0;
  }

  for (ptrdiff_t j = 0; j < x.size(); j++) {
    for (ptrdiff_t i = 0; i < order; i++) {
      A(j, i + 1) = A(j, i) * x(j);
    }
  }

  auto Q = A.householderQr();
  return Q.solve(y.derived());
}

/// Evaluate a polynomial.
template <typename D>
Eigen::MatrixX<typename D::Scalar> polyval(const Eigen::DenseBase<D> &coeffs, const Eigen::DenseBase<D> &x) {
  Eigen::MatrixX<typename D::Scalar> m(x.rows(), x.cols());
  for (ptrdiff_t i = 0; i < coeffs.size(); i++) {
    m.derived().array() += coeffs(i)*x.derived().array().pow(i);
  }
  return m;
}

} // namespace h5geo

#endif // H5POLYFIT_H
