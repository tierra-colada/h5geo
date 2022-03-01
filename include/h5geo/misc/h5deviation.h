#ifndef H5DEVIATION_H
#define H5DEVIATION_H

#include "../impl/h5enum.h"

#define _USE_MATH_DEFINES   // should be before <cmath>, include 'pi' val

#include <math.h>
#include <Eigen/Dense>
#include <units/units.hpp>

namespace h5geo
{
/* */
template<typename D>
Eigen::MatrixX<typename D::Scalar> MdAzIncl2ALL(
    const Eigen::DenseBase<D> &M,
    const double& x0, const double& y0, const double& kb,
    const std::string& angularUnitsFrom,
    const std::string& angularUnitsTo,
    const bool& XNorth);

template<typename D>
Eigen::MatrixX<typename D::Scalar> TvdXY2ALL(
    const Eigen::DenseBase<D> &M,
    const double& x0, const double& y0, const double& kb,
    const std::string& angularUnits,
    const bool& XNorth);

template<typename D>
Eigen::MatrixX<typename D::Scalar> TvdDxDy2ALL(
    const Eigen::DenseBase<D> &M,
    const double& x0, const double& y0, const double& kb,
    const std::string& angularUnits,
    const bool& XNorth);

template<typename D>
Eigen::MatrixX<typename D::Scalar> TvdssXY2ALL(
    const Eigen::DenseBase<D> &M,
    const double& x0, const double& y0, const double& kb,
    const std::string& angularUnits,
    const bool& XNorth);

template<typename D>
Eigen::MatrixX<typename D::Scalar> TvdssDxDy2ALL(
    const Eigen::DenseBase<D> &M,
    const double& x0, const double& y0, const double& kb,
    const std::string& angularUnits,
    const bool& XNorth);

template<typename D>
Eigen::MatrixX<typename D::Scalar> traj2ALL(
    const Eigen::DenseBase<D> &M,
    const double& x0, const double& y0, const double& kb,
    const std::string& angularUnitsFrom,
    const std::string& angularUnitsTo,
    const h5geo::TrajectoryFormat& trajFormat,
    const bool& XNorth);

/* */
template<typename D>
Eigen::MatrixX<typename D::Scalar> MdAzIncl2MdXYTvd(
    const Eigen::DenseBase<D> &M,
    const double& x0, const double& y0,
    const std::string& angularUnits,
    const bool& XNorth);

template<typename D>
Eigen::MatrixX<typename D::Scalar> TvdXY2MdXYTvd(
    const Eigen::DenseBase<D> &M,
    const double& x0, const double& y0,
    const bool& XNorth);

template<typename D>
Eigen::MatrixX<typename D::Scalar> TvdDxDy2MdXYTvd(
    const Eigen::DenseBase<D> &M,
    const double& x0, const double& y0,
    const bool& XNorth);

template<typename D>
Eigen::MatrixX<typename D::Scalar> TvdssXY2MdXYTvd(
    const Eigen::DenseBase<D> &M,
    const double& x0, const double& y0, const double& kb,
    const bool& XNorth);

template<typename D>
Eigen::MatrixX<typename D::Scalar> TvdssDxDy2MdXYTvd(
    const Eigen::DenseBase<D> &M,
    const double& x0, const double& y0, const double& kb,
    const bool& XNorth);

template<typename D>
Eigen::MatrixX<typename D::Scalar> TvdXY2MdAzIncl(
    const Eigen::DenseBase<D> &M,
    const double& x0, const double& y0, const std::string& angularUnits, const bool& XNorth);

template<typename D>
Eigen::MatrixX<typename D::Scalar> TvdDxDy2MdAzIncl(
    const Eigen::DenseBase<D> &M, const std::string& angularUnits, const bool& XNorth);

template<typename D>
Eigen::MatrixX<typename D::Scalar> TvdssXY2MdAzIncl(
    const Eigen::DenseBase<D> &M,
    const double& x0, const double& y0, const double& kb, const std::string& angularUnits, const bool& XNorth);

template<typename D>
Eigen::MatrixX<typename D::Scalar> TvdssDxDy2MdAzIncl(
    const Eigen::DenseBase<D> &M,
    const double& kb, const std::string& angularUnits, const bool& XNorth);


///\brief angleAndRadius Calculate angle and radius from arc and chord
///\param arcLen
///\param chordLen
///\param eps
///\param max_iter
///\return
inline Eigen::VectorX<double> angleAndRadius(
    const double& arcLen,
    const double& chordLen,
    const double& eps,
    const size_t& max_iter);


inline double _betaAngle(
    double I1,
    double I2,
    double A1,
    double A2,
    const std::string& angularUnits)
{
  if (!angularUnits.empty()){
    double coef = units::convert(
          units::unit_from_string(angularUnits),
          units::unit_from_string("radian"));
    I1 *= coef;
    I2 *= coef;
    A1 *= coef;
    A2 *= coef;
  }

  return acos(cos(I2-I1) - (sin(I1)*sin(I2)*(1 - cos(A2-A1))));
}


inline double _ratioFactor(double B, const std::string& angularUnits)
{
  if (!angularUnits.empty()){
    double coef = units::convert(
          units::unit_from_string(angularUnits),
          units::unit_from_string("radian"));
    B *= coef;
  }

  if (B == 0)
    return 1 + pow(B,2)/12 + pow(B,4)/120 + pow(B,6)/20160; // or it is possible to simply return 1
  else
    return 2 / B * tan(B / 2);
}


inline double _deltaEast(
    const double& dMD,
    double I1,
    double I2,
    double A1,
    double A2,
    const double& RF,
    const std::string& angularUnits)
{
  if (!angularUnits.empty()){
    double coef = units::convert(
          units::unit_from_string(angularUnits),
          units::unit_from_string("radian"));
    I1 *= coef;
    I2 *= coef;
    A1 *= coef;
    A2 *= coef;
  }

  return dMD/2 * (sin(I1)*sin(A1) + sin(I2)*sin(A2))*RF;
}


inline double _deltaNorth(
    const double& dMD,
    double I1,
    double I2,
    double A1,
    double A2,
    const double& RF,
    const std::string& angularUnits)
{
  if (!angularUnits.empty()){
    double coef = units::convert(
          units::unit_from_string(angularUnits),
          units::unit_from_string("radian"));
    I1 *= coef;
    I2 *= coef;
    A1 *= coef;
    A2 *= coef;
  }

  return dMD/2 * (sin(I1)*cos(A1) + sin(I2)*cos(A2))*RF;
}


inline double _deltaZ(
    const double& dMD,
    double I1,
    double I2,
    const double& RF,
    const std::string& angularUnits)
{
  if (!angularUnits.empty()){
    double coef = units::convert(
          units::unit_from_string(angularUnits),
          units::unit_from_string("radian"));
    I1 *= coef;
    I2 *= coef;
  }

  return dMD/2 * (cos(I1) + cos(I2))*RF;
}


inline double _dMDFromDeltaEast(
    const double& dE,
    double I1,
    double I2,
    double A1,
    double A2,
    const double& RF,
    const std::string& angularUnits)
{
  if (!angularUnits.empty()){
    double coef = units::convert(
          units::unit_from_string(angularUnits),
          units::unit_from_string("radian"));
    I1 *= coef;
    I2 *= coef;
    A1 *= coef;
    A2 *= coef;
  }

  return 2*dE / ((sin(I1)*sin(A1) + sin(I2)*sin(A2))*RF);
}


inline double _dMDFromDeltaNorth(
    const double& dN,
    double I1,
    double I2,
    double A1,
    double A2,
    const double& RF,
    const std::string& angularUnits)
{
  if (!angularUnits.empty()){
    double coef = units::convert(
          units::unit_from_string(angularUnits),
          units::unit_from_string("radian"));
    I1 *= coef;
    I2 *= coef;
    A1 *= coef;
    A2 *= coef;
  }

  return 2*dN / ((sin(I1)*cos(A1) + sin(I2)*cos(A2))*RF);
}


inline double _dMDFromDeltaZ(
    const double& dZ,
    double I1,
    double I2,
    const double& RF,
    const std::string& angularUnits)
{
  if (!angularUnits.empty()){
    double coef = units::convert(
          units::unit_from_string(angularUnits),
          units::unit_from_string("radian"));
    I1 *= coef;
    I2 *= coef;
  }

  return 2*dZ / ((cos(I1) + cos(I2))*RF);
}


inline double _fa(
    const double& arc,
    const double& chord,
    double a,
    const std::string& angularUnits)
{
  if (!angularUnits.empty()){
    double coef = units::convert(
          units::unit_from_string(angularUnits),
          units::unit_from_string("radian"));
    a *= coef;
  }

  return chord*a-2*arc*sin(a/2);
}


inline double _dfa(
    const double& arc,
    const double& chord,
    double a,
    const std::string& angularUnits)
{
  if (!angularUnits.empty()){
    double coef = units::convert(
          units::unit_from_string(angularUnits),
          units::unit_from_string("radian"));
    a *= coef;
  }

  return chord-arc*cos(a/2);
}


inline double _ddfa(
    const double& arc,
    double a,
    const std::string& angularUnits)
{
  if (!angularUnits.empty()){
    double coef = units::convert(
          units::unit_from_string(angularUnits),
          units::unit_from_string("radian"));
    a *= coef;
  }

  return arc*sin(a/2)/2;
}

};





template<typename D>
Eigen::MatrixX<typename D::Scalar> h5geo::MdAzIncl2ALL(
    const Eigen::DenseBase<D> &M,
    const double& x0, const double& y0, const double& kb,
    const std::string& angularUnitsFrom,
    const std::string& angularUnitsTo,
    const bool& XNorth)
{
  if (M.cols() != 3)
    return Eigen::MatrixX<typename D::Scalar>();

  Eigen::MatrixX<typename D::Scalar> M_MdXYTvd =
      MdAzIncl2MdXYTvd(M, x0, y0, angularUnitsFrom, XNorth);

  Eigen::MatrixX<typename D::Scalar> M_OUT(M.rows(), 9);
  M_OUT.col(0) = M_MdXYTvd.col(0); // MD
  M_OUT.col(1) = M_MdXYTvd.col(1); // X
  M_OUT.col(2) = M_MdXYTvd.col(2); // Y
  M_OUT.col(3) = M_MdXYTvd.col(3).array() - kb; // TVDSS
  M_OUT.col(4) = M_MdXYTvd.col(3); // TVD
  M_OUT.col(5) = M_MdXYTvd.col(1).array() - x0; // DX
  M_OUT.col(6) = M_MdXYTvd.col(2).array() - y0; // DY

  if (!angularUnitsTo.empty()){
    double coef = units::convert(
          units::unit_from_string(angularUnitsFrom),
          units::unit_from_string(angularUnitsTo));
    M_OUT.col(7) = M.col(1)*coef; // AZ
    M_OUT.col(8) = M.col(2)*coef; // INCL
  } else {
    M_OUT.col(7) = M.col(1); // AZ
    M_OUT.col(8) = M.col(2); // INCL
  }

  return M_OUT;
}

template<typename D>
Eigen::MatrixX<typename D::Scalar> h5geo::TvdXY2ALL(
    const Eigen::DenseBase<D> &M,
    const double& x0, const double& y0, const double& kb,
    const std::string& angularUnits,
    const bool& XNorth)
{
  if (M.cols() != 3)
    return Eigen::MatrixX<typename D::Scalar>();

  Eigen::MatrixX<typename D::Scalar> M_MdAzIncl =
      TvdXY2MdAzIncl(M, x0, y0, angularUnits, XNorth);

  Eigen::MatrixX<typename D::Scalar> M_OUT(M.rows(), 9);
  M_OUT.col(0) = M_MdAzIncl.col(0); // MD
  M_OUT.col(1) = M.col(1); // X
  M_OUT.col(2) = M.col(2); // Y
  M_OUT.col(3) = M.col(0).array() - kb; // TVDSS
  M_OUT.col(4) = M.col(0); // TVD
  M_OUT.col(5) = M.col(1).array() - x0; // DX
  M_OUT.col(6) = M.col(2).array() - y0; // DY
  M_OUT.col(7) = M_MdAzIncl.col(1); // AZ
  M_OUT.col(8) = M_MdAzIncl.col(2); // INCL
  return M_OUT;
}

template<typename D>
Eigen::MatrixX<typename D::Scalar> h5geo::TvdDxDy2ALL(
    const Eigen::DenseBase<D> &M,
    const double& x0, const double& y0, const double& kb,
    const std::string& angularUnits,
    const bool& XNorth)
{
  if (M.cols() != 3)
    return Eigen::MatrixX<typename D::Scalar>();

  Eigen::MatrixX<typename D::Scalar> M_MdAzIncl =
      TvdDxDy2MdAzIncl(M, angularUnits, XNorth);

  Eigen::MatrixX<typename D::Scalar> M_OUT(M.rows(), 9);
  M_OUT.col(0) = M_MdAzIncl.col(0); // MD
  M_OUT.col(1) = M.col(1).array() + x0; // X
  M_OUT.col(2) = M.col(2).array() + y0; // Y
  M_OUT.col(3) = M.col(0).array() - kb; // TVDSS
  M_OUT.col(4) = M.col(0); // TVD
  M_OUT.col(5) = M.col(1); // DX
  M_OUT.col(6) = M.col(2); // DY
  M_OUT.col(7) = M_MdAzIncl.col(1); // AZ
  M_OUT.col(8) = M_MdAzIncl.col(2); // INCL
  return M_OUT;
}

template<typename D>
Eigen::MatrixX<typename D::Scalar> h5geo::TvdssXY2ALL(
    const Eigen::DenseBase<D> &M,
    const double& x0, const double& y0, const double& kb,
    const std::string& angularUnits,
    const bool& XNorth)
{
  if (M.cols() != 3)
    return Eigen::MatrixX<typename D::Scalar>();

  Eigen::MatrixX<typename D::Scalar> M_MdAzIncl =
      TvdssXY2MdAzIncl(M, x0, y0, kb, angularUnits, XNorth);

  Eigen::MatrixX<typename D::Scalar> M_OUT(M.rows(), 9);
  M_OUT.col(0) = M_MdAzIncl.col(0); // MD
  M_OUT.col(1) = M.col(1); // X
  M_OUT.col(2) = M.col(2); // Y
  M_OUT.col(3) = M.col(0); // TVDSS
  M_OUT.col(4) = M.col(0).array() + kb; // TVD
  M_OUT.col(5) = M.col(1).array() - x0; // DX
  M_OUT.col(6) = M.col(2).array() - y0; // DY
  M_OUT.col(7) = M_MdAzIncl.col(1); // AZ
  M_OUT.col(8) = M_MdAzIncl.col(2); // INCL
  return M_OUT;
}

template<typename D>
Eigen::MatrixX<typename D::Scalar> h5geo::TvdssDxDy2ALL(
    const Eigen::DenseBase<D> &M,
    const double& x0, const double& y0, const double& kb,
    const std::string& angularUnits,
    const bool& XNorth)
{
  if (M.cols() != 3)
    return Eigen::MatrixX<typename D::Scalar>();

  Eigen::MatrixX<typename D::Scalar> M_MdAzIncl =
      TvdssDxDy2MdAzIncl(M, kb, angularUnits, XNorth);

  Eigen::MatrixX<typename D::Scalar> M_OUT(M.rows(), 9);
  M_OUT.col(0) = M_MdAzIncl.col(0); // MD
  M_OUT.col(1) = M.col(1).array() + x0; // X
  M_OUT.col(2) = M.col(2).array() + y0; // Y
  M_OUT.col(3) = M.col(0); // TVDSS
  M_OUT.col(4) = M.col(0).array() + kb; // TVD
  M_OUT.col(5) = M.col(1); // DX
  M_OUT.col(6) = M.col(2); // DY
  M_OUT.col(7) = M_MdAzIncl.col(1); // AZ
  M_OUT.col(8) = M_MdAzIncl.col(2); // INCL
  return M_OUT;
}

template<typename D>
Eigen::MatrixX<typename D::Scalar> h5geo::traj2ALL(
    const Eigen::DenseBase<D> &M,
    const double& x0, const double& y0, const double& kb,
    const std::string& angularUnitsFrom,
    const std::string& angularUnitsTo,
    const h5geo::TrajectoryFormat& trajFormat,
    const bool& XNorth)
{
  switch (trajFormat) {
  case h5geo::TrajectoryFormat::MD_AZIM_INCL:
    return h5geo::MdAzIncl2ALL(M, x0, y0, kb, angularUnitsFrom, angularUnitsTo, XNorth);
  case h5geo::TrajectoryFormat::TVDSS_DX_DY:
    return h5geo::TvdssDxDy2ALL(M, x0, y0, kb, angularUnitsTo, XNorth);
  case h5geo::TrajectoryFormat::TVDSS_X_Y:
    return h5geo::TvdssXY2ALL(M, x0, y0, kb, angularUnitsTo, XNorth);
  case h5geo::TrajectoryFormat::TVD_DX_DY:
    return h5geo::TvdDxDy2ALL(M, x0, y0, kb, angularUnitsTo, XNorth);
  case h5geo::TrajectoryFormat::TVD_X_Y:
    return h5geo::TvdXY2ALL(M, x0, y0, kb, angularUnitsTo, XNorth);
  default:
    return Eigen::MatrixX<typename D::Scalar>();
  }
}

template<typename D>
Eigen::MatrixX<typename D::Scalar> h5geo::MdAzIncl2MdXYTvd(
    const Eigen::DenseBase<D> &M,
    const double& x0, const double& y0,
    const std::string& angularUnits,
    const bool& XNorth)
{
  if (M.cols() != 3)
    return Eigen::MatrixX<typename D::Scalar>();

  double coef = 1;
  if (!angularUnits.empty()){
    coef = units::convert(
          units::unit_from_string(angularUnits),
          units::unit_from_string("radian"));
  }

  Eigen::MatrixX<typename D::Scalar> M_OUT(M.rows(), 4);
  M_OUT.col(0) = M.col(0);
  typename D::Scalar I1, I2, A1, A2, dMD, B, RF, dx, dy, dz;

  for (size_t i = 0; i < M.rows(); i++){
    if (i == 0){
      A1 = M(0, 1) * coef;
      A2 = M(0, 1) * coef;
      I1 = M(0, 2) * coef;
      I2 = M(0, 2) * coef;
      dMD = M(0, 0);
    } else {
      A1 = M(i, 1) * coef;
      A2 = M(i-1, 1) * coef;
      I1 = M(i, 2) * coef;
      I2 = M(i-1, 2) * coef;
      dMD = M(i, 0) - M(i-1, 0);
    }

    B = _betaAngle(I1, I2, A1, A2, "");
    RF = _ratioFactor(B, "");

    if (XNorth){
      dx = _deltaNorth(dMD, I1, I2, A1, A2, RF, "");
      dy = _deltaEast(dMD, I1, I2, A1, A2, RF, "");
    } else {
      dx = _deltaEast(dMD, I1, I2, A1, A2, RF, "");
      dy = _deltaNorth(dMD, I1, I2, A1, A2, RF, "");
    }

    dz = _deltaZ(dMD, I1, I2, RF, "");

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

template<typename D>
Eigen::MatrixX<typename D::Scalar> h5geo::TvdXY2MdXYTvd(
    const Eigen::DenseBase<D> &M,
    const double& x0, const double& y0,
    const bool& XNorth)
{
  if (M.cols() != 3)
    return Eigen::MatrixX<typename D::Scalar>();

  Eigen::MatrixX<typename D::Scalar> M_MdAzIncl =
      TvdXY2MdAzIncl(M, x0, y0, "", XNorth);

  Eigen::MatrixX<typename D::Scalar> M_MdXYTvd(M.rows(), 4);

  M_MdXYTvd.col(0) = M_MdAzIncl.col(0);
  M_MdXYTvd.col(1) = M.col(1);
  M_MdXYTvd.col(2) = M.col(2);
  M_MdXYTvd.col(3) = M.col(0);

  return M_MdXYTvd;
}

template<typename D>
Eigen::MatrixX<typename D::Scalar> h5geo::TvdDxDy2MdXYTvd(
    const Eigen::DenseBase<D> &M,
    const double& x0, const double& y0,
    const bool& XNorth)
{
  if (M.cols() != 3)
    return Eigen::MatrixX<typename D::Scalar>();

  Eigen::MatrixX<typename D::Scalar> M_MdAzIncl =
      TvdDxDy2MdAzIncl(M, "", XNorth);

  Eigen::MatrixX<typename D::Scalar> M_MdXYTvd(M.rows(), 4);

  M_MdXYTvd.col(0) = M_MdAzIncl.col(0);
  M_MdXYTvd.col(1) = M.col(1).array() + x0;
  M_MdXYTvd.col(2) = M.col(2).array() + y0;
  M_MdXYTvd.col(3) = M.col(0);

  return M_MdXYTvd;
}

template<typename D>
Eigen::MatrixX<typename D::Scalar> h5geo::TvdssXY2MdXYTvd(
    const Eigen::DenseBase<D> &M,
    const double& x0, const double& y0, const double& kb,
    const bool& XNorth)
{
  if (M.cols() != 3)
    return Eigen::MatrixX<typename D::Scalar>();

  Eigen::MatrixX<typename D::Scalar> M_MdAzIncl =
      TvdssXY2MdAzIncl(M, x0, y0, kb, "", XNorth);

  Eigen::MatrixX<typename D::Scalar> M_MdXYTvd(M.rows(), 4);

  M_MdXYTvd.col(0) = M_MdAzIncl.col(0);
  M_MdXYTvd.col(1) = M.col(1);
  M_MdXYTvd.col(2) = M.col(2);
  M_MdXYTvd.col(3) = M.col(0).array() + kb;

  return M_MdXYTvd;
}

template<typename D>
Eigen::MatrixX<typename D::Scalar> h5geo::TvdssDxDy2MdXYTvd(
    const Eigen::DenseBase<D> &M,
    const double& x0, const double& y0, const double& kb,
    const bool& XNorth)
{
  if (M.cols() != 3)
    return Eigen::MatrixX<typename D::Scalar>();

  Eigen::MatrixX<typename D::Scalar> M_MdAzIncl =
      TvdssDxDy2MdAzIncl(M, kb, "", XNorth);

  Eigen::MatrixX<typename D::Scalar> M_MdXYTvd(M.rows(), 4);

  M_MdXYTvd.col(0) = M_MdAzIncl.col(0);
  M_MdXYTvd.col(1) = M.col(1).array() + x0;
  M_MdXYTvd.col(2) = M.col(2).array() + y0;
  M_MdXYTvd.col(3) = M.col(0).array() + kb;

  return M_MdXYTvd;
}

template<typename D>
Eigen::MatrixX<typename D::Scalar> h5geo::TvdXY2MdAzIncl(
    const Eigen::DenseBase<D> &M,
    const double& x0, const double& y0,
    const std::string& angularUnits,
    const bool& XNorth)
{
  if (M.cols() != 3)
    return Eigen::MatrixX<typename D::Scalar>();

  Eigen::MatrixX<typename D::Scalar> MM = M;
  MM.col(1) = MM.col(1).array() - x0;
  MM.col(2) = MM.col(2).array() - y0;

  return TvdDxDy2MdAzIncl(MM, angularUnits, XNorth);
}

template<typename D>
Eigen::MatrixX<typename D::Scalar> h5geo::TvdDxDy2MdAzIncl(
    const Eigen::DenseBase<D> &M, const std::string& angularUnits, const bool& XNorth)
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

    B = _betaAngle(I1, I2, A1, A2, "");
    RF = _ratioFactor(B, "");

    dMD = _dMDFromDeltaZ(dz, I1, I2, RF, "");

    if (i == 0)
      M_OUT(0, 0) = dMD;
    else
      M_OUT(i, 0) = M_OUT(i-1, 0) + dMD;

    M_OUT(i, 1) = A2;
    M_OUT(i, 2) = I2;
  }

  if (!angularUnits.empty()){
    double coef = units::convert(
          units::unit_from_string("radian"),
          units::unit_from_string(angularUnits));
    M_OUT.col(1) = M_OUT.col(1).array() * coef;
    M_OUT.col(2) = M_OUT.col(2).array() * coef;
  }

  return M_OUT;
}

template<typename D>
Eigen::MatrixX<typename D::Scalar> h5geo::TvdssXY2MdAzIncl(
    const Eigen::DenseBase<D> &M,
    const double& x0, const double& y0, const double& kb,
    const std::string& angularUnits,
    const bool& XNorth)
{
  if (M.cols() != 3)
    return Eigen::MatrixX<typename D::Scalar>();

  Eigen::MatrixX<typename D::Scalar> MM = M;
  MM.col(0) = MM.col(0).array() + kb;
  MM.col(1) = MM.col(1).array() - x0;
  MM.col(2) = MM.col(2).array() - y0;

  return TvdDxDy2MdAzIncl(MM, angularUnits, XNorth);
}

template<typename D>
Eigen::MatrixX<typename D::Scalar> h5geo::TvdssDxDy2MdAzIncl(
    const Eigen::DenseBase<D> &M,
    const double& kb, const std::string& angularUnits, const bool& XNorth)
{
  if (M.cols() != 3)
    return Eigen::MatrixX<typename D::Scalar>();

  Eigen::MatrixX<typename D::Scalar> MM = M;
  MM.col(0) = MM.col(0).array() + kb;

  return TvdDxDy2MdAzIncl(MM, angularUnits, XNorth);
}


Eigen::VectorX<double> h5geo::angleAndRadius(
    const double& arcLen,
    const double& chordLen,
    const double& eps,
    const size_t& max_iter)
{
  if (arcLen <= chordLen ||
      arcLen == 0 ||
      chordLen == 0)
    return Eigen::VectorX<double>();

  double a_min = 0;
  double a_max = 2*M_PI;
  double a0, a1, f, df, ddf;
  for (a0 = a_min; a0 < a_max; a0 = a0+0.1){
    f = _fa(arcLen, chordLen, a0, "");
    ddf = _ddfa(arcLen, a0, "");
    if (f*ddf > 0)
      break;
  }

  df = _dfa(arcLen, chordLen, a0, "");
  if (df == 0)
    return Eigen::VectorX<double>();

  a1 = a0 - f/df;
  size_t i = 1, ii = 0;
  while (abs(f) > eps && i < max_iter){
    f = _fa(arcLen, chordLen, a0, "");
    df = _dfa(arcLen, chordLen, a0, "");
    if (df == 0)
      return Eigen::VectorX<double>();

    a0 = a1;
    a1 = a0 - f/df;
    if (a1 < a_min || a1 > a_max){
      while (a1 < a_min || a1 > a_max){
        a1 = (a0+a1)/2;
        ii++;
        if (ii == max_iter)
          return Eigen::VectorX<double>();
      }
      ii = 0;
    }
    i++;
  }
  double radius = arcLen/a1;

  Eigen::Vector2<double> v;
  v(0) = a1;
  v(1) = radius;
  return v;
}

#endif // H5DEVIATION_H
