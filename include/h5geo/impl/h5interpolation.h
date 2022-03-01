#ifndef H5INTERPOLATION_H
#define H5INTERPOLATION_H

#include <Eigen/Dense>

#include <iostream>
#include <vector>
#include <algorithm>    // std::sort, std::stable_sort
#ifdef H5GEO_USE_THREADS
#include <execution>
#endif

namespace h5geo
{


template <typename D>
/// \brief 1D interpolation for
/// \param x strictly monotonic (either increasing or decreasing)
/// \param y
/// \param xnew
/// \param extrapolate if not then NAN will be used
/// \return
Eigen::VectorX<typename D::Scalar> interp1Monotonic(
    const Eigen::DenseBase<D> &x,
    const Eigen::DenseBase<D> &y,
    const Eigen::DenseBase<D> &xnew,
    typename D::Scalar extrapVal)
{
  typedef typename D::Scalar S;
  if (x.size() != y.size() ||
      x.size() < 2 ||
      y.size() < 2)
    return Eigen::VectorX<S>();

  int nx = x.size();
  Eigen::VectorX<S> ynew;
  ynew.resize(xnew.size());
  S dx = x(1)-x(0);
  bool isXIncreasing;
  if (dx > 0)
    isXIncreasing = true;
  else if (dx < 0)
    isXIncreasing = false;
  else
    return Eigen::VectorX<S>();

  for (ptrdiff_t i = 0; i < xnew.size(); i++){
    int ind = 0;
    if (isXIncreasing){
      if (xnew(i) >= x(nx - 2))
        ind = nx - 2;
      else
        while (xnew(i) > x(ind+1))
          ind++;
    } else {
      if (xnew(i) <= x(nx - 2))
        ind = nx - 2;
      else
        while (xnew(i) < x(ind+1))
          ind++;
    }

    S xL = x(ind);
    S yL = y(ind);
    S xR = x(ind+1);
    S yR = y(ind+1);
    if (isXIncreasing){
      if (xnew(i) < xL)
        yR = extrapVal; // yR = yL;
      if (xnew(i) > xR)
        yL = extrapVal; // yL = yR;
    } else {
      if (xnew(i) > xL)
        yR = extrapVal;
      if (xnew(i) < xR)
        yL = extrapVal;
    }

    S dydx = (yR - yL) / (xR - xL);

    ynew(i) = yL + dydx * (xnew(i) - xL);
  }

  return ynew;
}


} // h5geo

#endif // H5INTERPOLATION_H
