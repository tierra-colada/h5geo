#define _USE_MATH_DEFINES   // should be before <cmath>, include 'pi' val

#include "h5core_enum.h"

#include <math.h>
#include <Eigen/Dense>

namespace h5geo
{
/* */
template<typename D, typename T>
Eigen::MatrixX<typename D::Scalar> MdAzIncl2ALL(
    const Eigen::DenseBase<D> &M,
    const T& x0, const T& y0, const T& kb,
    const h5geo::AngleUnits& angleUnits,
    const bool& XNorth);

template<typename D, typename T>
Eigen::MatrixX<typename D::Scalar> TvdXY2ALL(
    const Eigen::DenseBase<D> &M,
    const T& x0, const T& y0, const T& kb,
    const bool& XNorth);

template<typename D, typename T>
Eigen::MatrixX<typename D::Scalar> TvdDxDy2ALL(
    const Eigen::DenseBase<D> &M,
    const T& x0, const T& y0, const T& kb,
    const bool& XNorth);

template<typename D, typename T>
Eigen::MatrixX<typename D::Scalar> TvdssXY2ALL(
    const Eigen::DenseBase<D> &M,
    const T& x0, const T& y0, const T& kb,
    const bool& XNorth);

template<typename D, typename T>
Eigen::MatrixX<typename D::Scalar> TvdssDxDy2ALL(
    const Eigen::DenseBase<D> &M,
    const T& x0, const T& y0, const T& kb,
    const bool& XNorth);

/* */
template<typename D, typename T>
Eigen::MatrixX<typename D::Scalar> MdAzIncl2MdXYTvd(
    const Eigen::DenseBase<D> &M,
    const T& x0, const T& y0,
    const h5geo::AngleUnits& angleUnits,
    const bool& XNorth);

template<typename D, typename T>
Eigen::MatrixX<typename D::Scalar> TvdXY2MdXYTvd(
    const Eigen::DenseBase<D> &M,
    const T& x0, const T& y0,
    const bool& XNorth);

template<typename D, typename T>
Eigen::MatrixX<typename D::Scalar> TvdDxDy2MdXYTvd(
    const Eigen::DenseBase<D> &M,
    const T& x0, const T& y0,
    const bool& XNorth);

template<typename D, typename T>
Eigen::MatrixX<typename D::Scalar> TvdssXY2MdXYTvd(
    const Eigen::DenseBase<D> &M,
    const T& x0, const T& y0, const T& kb,
    const bool& XNorth);

template<typename D, typename T>
Eigen::MatrixX<typename D::Scalar> TvdssDxDy2MdXYTvd(
    const Eigen::DenseBase<D> &M,
    const T& x0, const T& y0, const T& kb,
    const bool& XNorth);

template<typename D, typename T>
Eigen::MatrixX<typename D::Scalar> TvdXY2MdAzIncl(
    const Eigen::DenseBase<D> &M,
    const T& x0, const T& y0, const bool& XNorth);

template<typename D>
Eigen::MatrixX<typename D::Scalar> TvdDxDy2MdAzIncl(
    const Eigen::DenseBase<D> &M, const bool& XNorth);

template<typename D, typename T>
Eigen::MatrixX<typename D::Scalar> TvdssXY2MdAzIncl(
    const Eigen::DenseBase<D> &M,
    const T& x0, const T& y0, const T& kb, const bool& XNorth);

template<typename D, typename T>
Eigen::MatrixX<typename D::Scalar> TvdssDxDy2MdAzIncl(
    const Eigen::DenseBase<D> &M,
    const T& kb, const bool& XNorth);

template<typename T>
/*!
 * \brief angleAndRadius Calculate angle and radius from arc and chord
 * \param arcLen
 * \param chordLen
 * \param eps
 * \param max_iter
 * \return
 */
inline Eigen::VectorX<T> angleAndRadius(
    const T& arcLen,
    const T& chordLen,
    const T& eps,
    const size_t& max_iter);

template<typename T>
inline T _betaAngle(
    T I1,
    T I2,
    T A1,
    T A2,
    const h5geo::AngleUnits& angleUnits)
{
  if (angleUnits == h5geo::AngleUnits::DEGREE){
    I1 = I1/180.0*M_PI;
    I2 = I2/180.0*M_PI;
    A1 = A1/180.0*M_PI;
    A2 = A2/180.0*M_PI;
  }

  return acos(cos(I2-I1) - (sin(I1)*sin(I2)*(1 - cos(A2-A1))));
}

template<typename T>
inline T _ratioFactor(T B, const h5geo::AngleUnits& angleUnits)
{
  if (angleUnits == h5geo::AngleUnits::DEGREE)
    B = B/180.0*M_PI;

  if (B == 0)
    return 1 + pow(B,2)/12 + pow(B,4)/120 + pow(B,6)/20160; // or it is possible to simply return 1
  else
    return 2 / B * tan(B / 2);
}

template<typename T>
inline T _deltaEast(
    const T& dMD,
    T I1,
    T I2,
    T A1,
    T A2,
    const T& RF,
    const h5geo::AngleUnits& angleUnits)
{
  if (angleUnits == h5geo::AngleUnits::DEGREE){
    I1 = I1/180.0*M_PI;
    I2 = I2/180.0*M_PI;
    A1 = A1/180.0*M_PI;
    A2 = A2/180.0*M_PI;
  }

  return dMD/2 * (sin(I1)*sin(A1) + sin(I2)*sin(A2))*RF;
}

template<typename T>
inline T _deltaNorth(
    const T& dMD,
    T I1,
    T I2,
    T A1,
    T A2,
    const T& RF,
    const h5geo::AngleUnits& angleUnits)
{
  if (angleUnits == h5geo::AngleUnits::DEGREE){
    I1 = I1/180.0*M_PI;
    I2 = I2/180.0*M_PI;
    A1 = A1/180.0*M_PI;
    A2 = A2/180.0*M_PI;
  }

  return dMD/2 * (sin(I1)*cos(A1) + sin(I2)*cos(A2))*RF;
}

template<typename T>
inline T _deltaZ(
    const T& dMD,
    T I1,
    T I2,
    const T& RF,
    const h5geo::AngleUnits& angleUnits)
{
  if (angleUnits == h5geo::AngleUnits::DEGREE){
    I1 = I1/180.0*M_PI;
    I2 = I2/180.0*M_PI;
  }

  return dMD/2 * (cos(I1) + cos(I2))*RF;
}

template<typename T>
inline T _dMDFromDeltaEast(
    const T& dE,
    T I1,
    T I2,
    T A1,
    T A2,
    const T& RF,
    const h5geo::AngleUnits& angleUnits)
{
  if (angleUnits == h5geo::AngleUnits::DEGREE){
    I1 = I1/180.0*M_PI;
    I2 = I2/180.0*M_PI;
    A1 = A1/180.0*M_PI;
    A2 = A2/180.0*M_PI;
  }

  return 2*dE / ((sin(I1)*sin(A1) + sin(I2)*sin(A2))*RF);
}

template<typename T>
inline T _dMDFromDeltaNorth(
    const T& dN,
    T I1,
    T I2,
    T A1,
    T A2,
    const T& RF,
    const h5geo::AngleUnits& angleUnits)
{
  if (angleUnits == h5geo::AngleUnits::DEGREE){
    I1 = I1/180.0*M_PI;
    I2 = I2/180.0*M_PI;
    A1 = A1/180.0*M_PI;
    A2 = A2/180.0*M_PI;
  }

  return 2*dN / ((sin(I1)*cos(A1) + sin(I2)*cos(A2))*RF);
}

template<typename T>
inline T _dMDFromDeltaZ(
    const T& dZ,
    T I1,
    T I2,
    const T& RF,
    const h5geo::AngleUnits& angleUnits)
{
  if (angleUnits == h5geo::AngleUnits::DEGREE){
    I1 = I1/180.0*M_PI;
    I2 = I2/180.0*M_PI;
  }

  return 2*dZ / ((cos(I1) + cos(I2))*RF);
}

template<typename T>
inline T _fa(
    const T& arc,
    const T& chord,
    T a,
    const h5geo::AngleUnits& angleUnits)
{
  if (angleUnits == h5geo::AngleUnits::DEGREE){
    a = a/180.0*M_PI;
  }

  return chord*a-2*arc*sin(a/2);
}

template<typename T>
inline T _dfa(
    const T& arc,
    const T& chord,
    T a,
    const h5geo::AngleUnits& angleUnits)
{
  if (angleUnits == h5geo::AngleUnits::DEGREE){
    a = a/180.0*M_PI;
  }

  return chord-arc*cos(a/2);
}

template<typename T>
inline T _ddfa(
    const T& arc,
    T a,
    const h5geo::AngleUnits& angleUnits)
{
  if (angleUnits == h5geo::AngleUnits::DEGREE){
    a = a/180.0*M_PI;
  }

  return arc*sin(a/2)/2;
}

};





template<typename D, typename T>
Eigen::MatrixX<typename D::Scalar> h5geo::MdAzIncl2ALL(
    const Eigen::DenseBase<D> &M,
    const T& x0, const T& y0, const T& kb,
    const h5geo::AngleUnits& angleUnits,
    const bool& XNorth)
{
  if (M.cols() != 3)
    return Eigen::MatrixX<typename D::Scalar>();

  Eigen::MatrixX<typename D::Scalar> M_MdXYTvd =
      MdAzIncl2MdXYTvd(M, x0, y0, angleUnits, XNorth);

  Eigen::MatrixX<typename D::Scalar> M_OUT(M.rows(), 9);
  M_OUT.col(0) = M_MdXYTvd.col(0); // MD
  M_OUT.col(1) = M_MdXYTvd.col(1); // X
  M_OUT.col(2) = M_MdXYTvd.col(2); // Y
  M_OUT.col(3) = M_MdXYTvd.col(3).array() + kb; // TVDSS
  M_OUT.col(4) = M_MdXYTvd.col(3); // TVD
  M_OUT.col(5) = M_MdXYTvd.col(1).array() - x0; // DX
  M_OUT.col(6) = M_MdXYTvd.col(2).array() - y0; // DY
  if (angleUnits == h5geo::AngleUnits::DEGREE){
    M_OUT.col(7) = M.col(1)/180.0*M_PI; // AZ
    M_OUT.col(8) = M.col(2)/180.0*M_PI; // INCL
  } else {
    M_OUT.col(7) = M.col(1); // AZ
    M_OUT.col(8) = M.col(2); // INCL
  }

  return M_OUT;
}

template<typename D, typename T>
Eigen::MatrixX<typename D::Scalar> h5geo::TvdXY2ALL(
    const Eigen::DenseBase<D> &M,
    const T& x0, const T& y0, const T& kb,
    const bool& XNorth)
{
  if (M.cols() != 3)
    return Eigen::MatrixX<typename D::Scalar>();

  Eigen::MatrixX<typename D::Scalar> M_MdAzIncl =
      TvdXY2MdAzIncl(M, x0, y0, XNorth);

  Eigen::MatrixX<typename D::Scalar> M_OUT(M.rows(), 9);
  M_OUT.col(0) = M_MdAzIncl.col(0); // MD
  M_OUT.col(1) = M.col(1); // X
  M_OUT.col(2) = M.col(2); // Y
  M_OUT.col(3) = M.col(0).array() + kb; // TVDSS
  M_OUT.col(4) = M.col(0); // TVD
  M_OUT.col(5) = M.col(1).array() - x0; // DX
  M_OUT.col(6) = M.col(2).array() - y0; // DY
  M_OUT.col(7) = M_MdAzIncl.col(1); // AZ
  M_OUT.col(8) = M_MdAzIncl.col(2); // INCL
  return M_OUT;
}

template<typename D, typename T>
Eigen::MatrixX<typename D::Scalar> h5geo::TvdDxDy2ALL(
    const Eigen::DenseBase<D> &M,
    const T& x0, const T& y0, const T& kb,
    const bool& XNorth)
{
  if (M.cols() != 3)
    return Eigen::MatrixX<typename D::Scalar>();

  Eigen::MatrixX<typename D::Scalar> M_MdAzIncl =
      TvdDxDy2MdAzIncl(M, XNorth);

  Eigen::MatrixX<typename D::Scalar> M_OUT(M.rows(), 9);
  M_OUT.col(0) = M_MdAzIncl.col(0); // MD
  M_OUT.col(1) = M.col(1).array() + x0; // X
  M_OUT.col(2) = M.col(2).array() + y0; // Y
  M_OUT.col(3) = M.col(0).array() + kb; // TVDSS
  M_OUT.col(4) = M.col(0); // TVD
  M_OUT.col(5) = M.col(1); // DX
  M_OUT.col(6) = M.col(2); // DY
  M_OUT.col(7) = M_MdAzIncl.col(1); // AZ
  M_OUT.col(8) = M_MdAzIncl.col(2); // INCL
  return M_OUT;
}

template<typename D, typename T>
Eigen::MatrixX<typename D::Scalar> h5geo::TvdssXY2ALL(
    const Eigen::DenseBase<D> &M,
    const T& x0, const T& y0, const T& kb,
    const bool& XNorth)
{
  if (M.cols() != 3)
    return Eigen::MatrixX<typename D::Scalar>();

  Eigen::MatrixX<typename D::Scalar> M_MdAzIncl =
      TvdssXY2MdAzIncl(M, x0, y0, kb, XNorth);

  Eigen::MatrixX<typename D::Scalar> M_OUT(M.rows(), 9);
  M_OUT.col(0) = M_MdAzIncl.col(0); // MD
  M_OUT.col(1) = M.col(1); // X
  M_OUT.col(2) = M.col(2); // Y
  M_OUT.col(3) = M.col(0); // TVDSS
  M_OUT.col(4) = M.col(0).array() - kb; // TVD
  M_OUT.col(5) = M.col(1).array() - x0; // DX
  M_OUT.col(6) = M.col(2).array() - y0; // DY
  M_OUT.col(7) = M_MdAzIncl.col(1); // AZ
  M_OUT.col(8) = M_MdAzIncl.col(2); // INCL
  return M_OUT;
}

template<typename D, typename T>
Eigen::MatrixX<typename D::Scalar> h5geo::TvdssDxDy2ALL(
    const Eigen::DenseBase<D> &M,
    const T& x0, const T& y0, const T& kb,
    const bool& XNorth)
{
  if (M.cols() != 3)
    return Eigen::MatrixX<typename D::Scalar>();

  Eigen::MatrixX<typename D::Scalar> M_MdAzIncl =
      TvdssDxDy2MdAzIncl(M, kb, XNorth);

  Eigen::MatrixX<typename D::Scalar> M_OUT(M.rows(), 9);
  M_OUT.col(0) = M_MdAzIncl.col(0); // MD
  M_OUT.col(1) = M.col(1).array() + x0; // X
  M_OUT.col(2) = M.col(2).array() + y0; // Y
  M_OUT.col(3) = M.col(0); // TVDSS
  M_OUT.col(4) = M.col(0).array() - kb; // TVD
  M_OUT.col(5) = M.col(1); // DX
  M_OUT.col(6) = M.col(2); // DY
  M_OUT.col(7) = M_MdAzIncl.col(1); // AZ
  M_OUT.col(8) = M_MdAzIncl.col(2); // INCL
  return M_OUT;
}

template<typename D, typename T>
Eigen::MatrixX<typename D::Scalar> h5geo::MdAzIncl2MdXYTvd(
    const Eigen::DenseBase<D> &M,
    const T& x0, const T& y0,
    const h5geo::AngleUnits& angleUnits,
    const bool& XNorth)
{
  if (M.cols() != 3)
    return Eigen::MatrixX<typename D::Scalar>();

  double deg2rad_coef;
  if (angleUnits == h5geo::AngleUnits::DEGREE){
    deg2rad_coef = 1.0/180.0*M_PI;
  } else {
    deg2rad_coef = 1;
  }

  Eigen::MatrixX<typename D::Scalar> M_OUT(M.rows(), 4);
  M_OUT.col(0) = M.col(0);
  T I1, I2, A1, A2, dMD, B, RF, dx, dy, dz;

  for (size_t i = 0; i < M.rows(); i++){
    if (i == 0){
      A1 = M(0, 1) * deg2rad_coef;
      A2 = M(0, 1) * deg2rad_coef;
      I1 = M(0, 2) * deg2rad_coef;
      I2 = M(0, 2) * deg2rad_coef;
      dMD = M(0, 0);
    } else {
      A1 = M(i, 1) * deg2rad_coef;
      A2 = M(i-1, 1) * deg2rad_coef;
      I1 = M(i, 2) * deg2rad_coef;
      I2 = M(i-1, 2) * deg2rad_coef;
      dMD = M(i, 0) - M(i-1, 0);
    }

    B = _betaAngle(I1, I2, A1, A2, h5geo::AngleUnits::RADIAN);
    RF = _ratioFactor(B, h5geo::AngleUnits::RADIAN);

    if (XNorth){
      dx = _deltaNorth(dMD, I1, I2, A1, A2, RF, h5geo::AngleUnits::RADIAN);
      dy = _deltaEast(dMD, I1, I2, A1, A2, RF, h5geo::AngleUnits::RADIAN);
    } else {
      dx = _deltaEast(dMD, I1, I2, A1, A2, RF, h5geo::AngleUnits::RADIAN);
      dy = _deltaNorth(dMD, I1, I2, A1, A2, RF, h5geo::AngleUnits::RADIAN);
    }

    dz = _deltaZ(dMD, I1, I2, RF, h5geo::AngleUnits::RADIAN);

    if (i == 0){
      M_OUT(0, 1) = x0 + dx;
      M_OUT(0, 2) = y0 + dy;
      M_OUT(0, 3) = dz;
    } else {
      M_OUT(i, 1) = M_OUT(i-1, 1) + dx;
      M_OUT(i, 2) = M_OUT(i-1, 2) + dy;
      M_OUT(i, 3) = M_OUT(i-1, 3) + dz;
    }
  }
  return M_OUT;
}

template<typename D, typename T>
Eigen::MatrixX<typename D::Scalar> h5geo::TvdXY2MdXYTvd(
    const Eigen::DenseBase<D> &M,
    const T& x0, const T& y0,
    const bool& XNorth)
{
  if (M.cols() != 3)
    return Eigen::MatrixX<typename D::Scalar>();

  Eigen::MatrixX<typename D::Scalar> M_MdAzIncl =
      TvdXY2MdAzIncl(M, x0, y0, XNorth);

  Eigen::MatrixX<typename D::Scalar> M_MdXYTvd(M.rows(), 4);

  M_MdXYTvd.col(0) = M_MdAzIncl.col(0);
  M_MdXYTvd.col(1) = M.col(1);
  M_MdXYTvd.col(2) = M.col(2);
  M_MdXYTvd.col(3) = M.col(0);

  return M_MdXYTvd;
}

template<typename D, typename T>
Eigen::MatrixX<typename D::Scalar> h5geo::TvdDxDy2MdXYTvd(
    const Eigen::DenseBase<D> &M,
    const T& x0, const T& y0,
    const bool& XNorth)
{
  if (M.cols() != 3)
    return Eigen::MatrixX<typename D::Scalar>();

  Eigen::MatrixX<typename D::Scalar> M_MdAzIncl =
      TvdDxDy2MdAzIncl(M, XNorth);

  Eigen::MatrixX<typename D::Scalar> M_MdXYTvd(M.rows(), 4);

  M_MdXYTvd.col(0) = M_MdAzIncl.col(0);
  M_MdXYTvd.col(1) = M.col(1).array() + x0;
  M_MdXYTvd.col(2) = M.col(2).array() + y0;
  M_MdXYTvd.col(3) = M.col(0);

  return M_MdXYTvd;
}

template<typename D, typename T>
Eigen::MatrixX<typename D::Scalar> h5geo::TvdssXY2MdXYTvd(
    const Eigen::DenseBase<D> &M,
    const T& x0, const T& y0, const T& kb,
    const bool& XNorth)
{
  if (M.cols() != 3)
    return Eigen::MatrixX<typename D::Scalar>();

  Eigen::MatrixX<typename D::Scalar> M_MdAzIncl =
      TvdssXY2MdAzIncl(M, x0, y0, kb, XNorth);

  Eigen::MatrixX<typename D::Scalar> M_MdXYTvd(M.rows(), 4);

  M_MdXYTvd.col(0) = M_MdAzIncl.col(0);
  M_MdXYTvd.col(1) = M.col(1);
  M_MdXYTvd.col(2) = M.col(2);
  M_MdXYTvd.col(3) = M.col(0).array() - kb;

  return M_MdXYTvd;
}

template<typename D, typename T>
Eigen::MatrixX<typename D::Scalar> h5geo::TvdssDxDy2MdXYTvd(
    const Eigen::DenseBase<D> &M,
    const T& x0, const T& y0, const T& kb,
    const bool& XNorth)
{
  if (M.cols() != 3)
    return Eigen::MatrixX<typename D::Scalar>();

  Eigen::MatrixX<typename D::Scalar> M_MdAzIncl =
      TvdssDxDy2MdAzIncl(M, kb, XNorth);

  Eigen::MatrixX<typename D::Scalar> M_MdXYTvd(M.rows(), 4);

  M_MdXYTvd.col(0) = M_MdAzIncl.col(0);
  M_MdXYTvd.col(1) = M.col(1).array() + x0;
  M_MdXYTvd.col(2) = M.col(2).array() + y0;
  M_MdXYTvd.col(3) = M.col(0).array() - kb;

  return M_MdXYTvd;
}

template<typename D, typename T>
Eigen::MatrixX<typename D::Scalar> h5geo::TvdXY2MdAzIncl(
    const Eigen::DenseBase<D> &M,
    const T& x0, const T& y0,
    const bool& XNorth)
{
  if (M.cols() != 3)
    return Eigen::MatrixX<typename D::Scalar>();

  Eigen::MatrixX<typename D::Scalar> MM = M;
  MM.col(1) = MM.col(1).array() - x0;
  MM.col(2) = MM.col(2).array() - y0;

  return TvdDxDy2MdAzIncl(MM, XNorth);
}

template<typename D>
Eigen::MatrixX<typename D::Scalar> h5geo::TvdDxDy2MdAzIncl(
    const Eigen::DenseBase<D> &M, const bool& XNorth)
{
  if (M.cols() != 3)
    return Eigen::MatrixX<typename D::Scalar>();

  Eigen::MatrixX<typename D::Scalar> M_OUT(M.rows(), 3);
  typename D::Scalar I1, I2, A1, A2, dMD, B, RF, dx, dy, dz;

  for (size_t i = 0; i < M.rows(); i++){
    if (i == 0){
      dx = M(0, 1);
      dy = M(0, 2);
      dz = M(0, 0);

      if (XNorth){
        A1 = atan2(dy, dx);
      } else {
        A1 = atan2(dx, dy);
      }

      A2 = A1;
      I1 = atan2(sqrt(pow(dx,2)+pow(dy,2)), dz);
      I2 = I1;
    } else {
      dx = M(i, 1) - M(i-1, 1);
      dy = M(i, 2) - M(i-1, 2);
      dz = M(i, 0) - M(i-1, 0);
      A1 = M_OUT(i-1, 1);

      if (XNorth){
        A2 = atan2(dy, dx);
      } else {
        A2 = atan2(dx, dy);
      }

      I1 = M_OUT(i-1, 2);
      I2 = atan2(sqrt(pow(dx,2)+pow(dy,2)), dz);
    }

    B = _betaAngle(I1, I2, A1, A2, h5geo::AngleUnits::RADIAN);
    RF = _ratioFactor(B, h5geo::AngleUnits::RADIAN);

    dMD = _dMDFromDeltaZ(dz, I1, I2, RF, h5geo::AngleUnits::RADIAN);

    if (i == 0)
      M_OUT(0, 0) = dMD;
    else
      M_OUT(i, 0) = M_OUT(i-1, 0) + dMD;

    M_OUT(i, 1) = A2;
    M_OUT(i, 2) = I2;
  }
  return M_OUT;
}

template<typename D, typename T>
Eigen::MatrixX<typename D::Scalar> h5geo::TvdssXY2MdAzIncl(
    const Eigen::DenseBase<D> &M,
    const T& x0, const T& y0, const T& kb,
    const bool& XNorth)
{
  if (M.cols() != 3)
    return Eigen::MatrixX<typename D::Scalar>();

  Eigen::MatrixX<typename D::Scalar> MM = M;
  MM.col(0) = MM.col(0).array() - kb;
  MM.col(1) = MM.col(1).array() - x0;
  MM.col(2) = MM.col(2).array() - y0;

  return TvdDxDy2MdAzIncl(MM, XNorth);
}

template<typename D, typename T>
Eigen::MatrixX<typename D::Scalar> h5geo::TvdssDxDy2MdAzIncl(
    const Eigen::DenseBase<D> &M,
    const T& kb, const bool& XNorth)
{
  if (M.cols() != 3)
    return Eigen::MatrixX<typename D::Scalar>();

  Eigen::MatrixX<typename D::Scalar> MM = M;
  MM.col(0) = MM.col(0).array() - kb;

  return TvdDxDy2MdAzIncl(MM, XNorth);
}

template<typename T>
Eigen::VectorX<T> h5geo::angleAndRadius(
    const T& arcLen,
    const T& chordLen,
    const T& eps,
    const size_t& max_iter)
{
  if (arcLen <= chordLen ||
      arcLen == 0 ||
      chordLen == 0)
    return Eigen::VectorX<T>();

  T a_min = 0;
  T a_max = 2*M_PI;
  T a0, a1, f, df, ddf;
  for (a0 = a_min; a0 < a_max; a0 = a0+0.1){
    f = _fa(arcLen, chordLen, a0, h5geo::AngleUnits::RADIAN);
    ddf = _ddfa(arcLen, a0, h5geo::AngleUnits::RADIAN);
    if (f*ddf > 0)
      break;
  }

  df = _dfa(arcLen, chordLen, a0, h5geo::AngleUnits::RADIAN);
  if (df == 0)
    return Eigen::VectorX<T>();

  a1 = a0 - f/df;
  size_t i = 1, ii = 0;
  while (abs(f) > eps && i < max_iter){
    f = _fa(arcLen, chordLen, a0, h5geo::AngleUnits::RADIAN);
    df = _dfa(arcLen, chordLen, a0, h5geo::AngleUnits::RADIAN);
    if (df == 0)
      return Eigen::VectorX<T>();

    a0 = a1;
    a1 = a0 - f/df;
    if (a1 < a_min || a1 > a_max){
      while (a1 < a_min || a1 > a_max){
        a1 = (a0+a1)/2;
        ii++;
        if (ii == max_iter)
          return Eigen::VectorX<T>();
      }
      ii = 0;
    }
    i++;
  }
  T radius = arcLen/a1;

  Eigen::Vector2<T> v;
  v(0) = a1;
  v(1) = radius;
  return v;
}
