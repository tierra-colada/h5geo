#include "../../include/h5geopy/h5deviation_py.h"

#include <h5geo/misc/h5deviation.h>

namespace h5geopy {

namespace ext {

// DEVIATION
template<typename D>
D MdAzIncl2ALL(
    const py::EigenDRef<const D> M,
    const typename D::Scalar& x0,
    const typename D::Scalar& y0,
    const typename D::Scalar& kb,
    const h5geo::AngleUnits& angleUnits,
    const bool& XNorth)
{
  return h5geo::MdAzIncl2ALL(M, x0, y0, kb, angleUnits, XNorth);
}

template<typename D>
D TvdXY2ALL(
    const py::EigenDRef<const D> M,
    const typename D::Scalar& x0,
    const typename D::Scalar& y0,
    const typename D::Scalar& kb,
    const bool& XNorth)
{
  return h5geo::TvdXY2ALL(M, x0, y0, kb, XNorth);
}

template<typename D>
D TvdDxDy2ALL(
    const py::EigenDRef<const D> M,
    const typename D::Scalar& x0,
    const typename D::Scalar& y0,
    const typename D::Scalar& kb,
    const bool& XNorth)
{
  return h5geo::TvdDxDy2ALL(M, x0, y0, kb, XNorth);
}

template<typename D>
D TvdssXY2ALL(
    const py::EigenDRef<const D> M,
    const typename D::Scalar& x0,
    const typename D::Scalar& y0,
    const typename D::Scalar& kb,
    const bool& XNorth)
{
  return h5geo::TvdssXY2ALL(M, x0, y0, kb, XNorth);
}

template<typename D>
D traj2ALL(
    const py::EigenDRef<const D> M,
    const typename D::Scalar& x0,
    const typename D::Scalar& y0,
    const typename D::Scalar& kb,
    const h5geo::AngleUnits& angleUnits,
    const h5geo::TrajectoryFormat& trajFormat,
    const bool& XNorth)
{
  return h5geo::traj2ALL(M, x0, y0, kb, angleUnits, trajFormat, XNorth);
}

template<typename D>
D TvdssDxDy2ALL(
    const py::EigenDRef<const D> M,
    const typename D::Scalar& x0,
    const typename D::Scalar& y0,
    const typename D::Scalar& kb,
    const bool& XNorth)
{
  return h5geo::TvdssDxDy2ALL(M, x0, y0, kb, XNorth);
}

/* */
template<typename D>
D MdAzIncl2MdXYTvd(
    const py::EigenDRef<const D> M,
    const typename D::Scalar& x0,
    const typename D::Scalar& y0,
    const h5geo::AngleUnits& angleUnits,
    const bool& XNorth)
{
  return h5geo::MdAzIncl2MdXYTvd(M, x0, y0, angleUnits, XNorth);
}

template<typename D>
D TvdXY2MdXYTvd(
    const py::EigenDRef<const D> M,
    const typename D::Scalar& x0,
    const typename D::Scalar& y0,
    const bool& XNorth)
{
  return h5geo::TvdXY2MdXYTvd(M, x0, y0, XNorth);
}

template<typename D>
D TvdDxDy2MdXYTvd(
    const py::EigenDRef<const D> M,
    const typename D::Scalar& x0,
    const typename D::Scalar& y0,
    const bool& XNorth)
{
  return h5geo::TvdDxDy2MdXYTvd(M, x0, y0, XNorth);
}

template<typename D>
D TvdssXY2MdXYTvd(
    const py::EigenDRef<const D> M,
    const typename D::Scalar& x0,
    const typename D::Scalar& y0,
    const typename D::Scalar& kb,
    const bool& XNorth)
{
  return h5geo::TvdssXY2MdXYTvd(M, x0, y0, kb, XNorth);
}

template<typename D>
D TvdssDxDy2MdXYTvd(
    const py::EigenDRef<const D> M,
    const typename D::Scalar& x0,
    const typename D::Scalar& y0,
    const typename D::Scalar& kb,
    const bool& XNorth)
{
  return h5geo::TvdssDxDy2MdXYTvd(M, x0, y0, kb, XNorth);
}

template<typename D>
D TvdXY2MdAzIncl(
    const py::EigenDRef<const D> M,
    const typename D::Scalar& x0,
    const typename D::Scalar& y0,
    const bool& XNorth)
{
  return h5geo::TvdXY2MdAzIncl(M, x0, y0, XNorth);
}

template<typename D>
D TvdDxDy2MdAzIncl(
    const py::EigenDRef<const D> M,
    const bool& XNorth)
{
  return h5geo::TvdDxDy2MdAzIncl(M, XNorth);
}

template<typename D>
D TvdssXY2MdAzIncl(
    const py::EigenDRef<const D> M,
    const typename D::Scalar& x0,
    const typename D::Scalar& y0,
    const typename D::Scalar& kb,
    const bool& XNorth)
{
  return h5geo::TvdssXY2MdAzIncl(M, x0, y0, kb, XNorth);
}

template<typename D>
D TvdssDxDy2MdAzIncl(
    const py::EigenDRef<const D> M,
    const typename D::Scalar& kb,
    const bool& XNorth)
{
  return h5geo::TvdssDxDy2MdAzIncl(M, kb, XNorth);
}


} // ext


void defineDeviationFunctions(py::module_& m){
  // DEVIATION
  m.def("MdAzIncl2ALL", &ext::MdAzIncl2ALL<Eigen::MatrixXf>,
        py::arg("M"), py::arg("x0"), py::arg("y0"), py::arg("kb"),
        py::arg("angleUnits"), py::arg("XNorth"),
        "Convert matrix `M` (whose columns represent MD Az Incl respectively)"
"to `MD_X_Y_Z_TVD_DX_DY_AZ_INCL` (Z the same as TVDSS). Set `XNorth` to `True` "
"if `X` axis points to the North");
  m.def("MdAzIncl2ALL", &ext::MdAzIncl2ALL<Eigen::MatrixXd>,
        py::arg("M"), py::arg("x0"), py::arg("y0"), py::arg("kb"),
        py::arg("angleUnits"), py::arg("XNorth"));

  m.def("TvdXY2ALL", &ext::TvdXY2ALL<Eigen::MatrixXf>,
        py::arg("M"), py::arg("x0"), py::arg("y0"), py::arg("kb"),
        py::arg("XNorth"),
        "Convert matrix `M` (whose columns represent TVD X Y respectively)"
"to `MD_X_Y_Z_TVD_DX_DY_AZ_INCL` (Z the same as TVDSS). Set `XNorth` to `True` "
"if `X` axis points to the North");
  m.def("TvdXY2ALL", &ext::TvdXY2ALL<Eigen::MatrixXd>,
        py::arg("M"), py::arg("x0"), py::arg("y0"), py::arg("kb"),
        py::arg("XNorth"));

  m.def("TvdDxDy2ALL", &ext::TvdDxDy2ALL<Eigen::MatrixXf>,
        py::arg("M"), py::arg("x0"), py::arg("y0"), py::arg("kb"),
        py::arg("XNorth"),
        "Convert matrix `M` (whose columns represent TVD DX DY respectively)"
"to `MD_X_Y_Z_TVD_DX_DY_AZ_INCL` (Z the same as TVDSS). Set `XNorth` to `True` "
"if `X` axis points to the North");
  m.def("TvdDxDy2ALL", &ext::TvdDxDy2ALL<Eigen::MatrixXd>,
        py::arg("M"), py::arg("x0"), py::arg("y0"), py::arg("kb"),
        py::arg("XNorth"));

  m.def("TvdssXY2ALL", &ext::TvdssXY2ALL<Eigen::MatrixXf>,
        py::arg("M"), py::arg("x0"), py::arg("y0"), py::arg("kb"),
        py::arg("XNorth"),
        "Convert matrix `M` (whose columns represent TVDSS X Y respectively)"
"to `MD_X_Y_Z_TVD_DX_DY_AZ_INCL` (Z the same as TVDSS). Set `XNorth` to `True` "
"if `X` axis points to the North");
  m.def("TvdDxDy2ALL", &ext::TvdssXY2ALL<Eigen::MatrixXd>,
        py::arg("M"), py::arg("x0"), py::arg("y0"), py::arg("kb"),
        py::arg("XNorth"));

  m.def("traj2ALL", &ext::traj2ALL<Eigen::MatrixXf>,
        py::arg("M"), py::arg("x0"), py::arg("y0"), py::arg("kb"),
        py::arg("angleUnits"), py::arg("trajFormat"), py::arg("XNorth"),
        "Convert matrix `M` of any trajectory format "
"to `MD_X_Y_Z_TVD_DX_DY_AZ_INCL` (Z the same as TVDSS). Set `XNorth` to `True` "
"if `X` axis points to the North");
  m.def("traj2ALL", &ext::traj2ALL<Eigen::MatrixXd>,
        py::arg("M"), py::arg("x0"), py::arg("y0"), py::arg("kb"),
        py::arg("angleUnits"), py::arg("trajFormat"), py::arg("XNorth"));

  m.def("TvdssDxDy2ALL", &ext::TvdssDxDy2ALL<Eigen::MatrixXf>,
        py::arg("M"), py::arg("x0"), py::arg("y0"), py::arg("kb"),
        py::arg("XNorth"),
        "Convert matrix `M` (whose columns represent TVDSS DX DY respectively)"
"to `MD_X_Y_Z_TVD_DX_DY_AZ_INCL` (Z the same as TVDSS). Set `XNorth` to `True` "
"if `X` axis points to the North");
  m.def("TvdssDxDy2ALL", &ext::TvdssDxDy2ALL<Eigen::MatrixXd>,
        py::arg("M"), py::arg("x0"), py::arg("y0"), py::arg("kb"),
        py::arg("XNorth"));

  /* */
  m.def("MdAzIncl2MdXYTvd", &ext::MdAzIncl2MdXYTvd<Eigen::MatrixXf>,
        py::arg("M"), py::arg("x0"), py::arg("y0"),
        py::arg("angleUnits"), py::arg("XNorth"),
        "Convert matrix `M` (whose columns represent MD Az Incl respectively)"
"to `MD_X_Y_TVD`. Set `XNorth` to `True` "
"if `X` axis points to the North");
  m.def("MdAzIncl2MdXYTvd", &ext::MdAzIncl2MdXYTvd<Eigen::MatrixXd>,
        py::arg("M"), py::arg("x0"), py::arg("y0"),
        py::arg("angleUnits"), py::arg("XNorth"));

  m.def("TvdXY2MdXYTvd", &ext::TvdXY2MdXYTvd<Eigen::MatrixXf>,
        py::arg("M"), py::arg("x0"), py::arg("y0"),
        py::arg("XNorth"),
        "Convert matrix `M` (whose columns represent TVD X Y respectively)"
"to `MD_X_Y_TVD`. Set `XNorth` to `True` "
"if `X` axis points to the North");
  m.def("TvdXY2MdXYTvd", &ext::TvdXY2MdXYTvd<Eigen::MatrixXd>,
        py::arg("M"), py::arg("x0"), py::arg("y0"),
        py::arg("XNorth"));

  m.def("TvdDxDy2MdXYTvd", &ext::TvdDxDy2MdXYTvd<Eigen::MatrixXf>,
        py::arg("M"), py::arg("x0"), py::arg("y0"),
        py::arg("XNorth"),
        "Convert matrix `M` (whose columns represent TVD DX DY respectively)"
"to `MD_X_Y_TVD`. Set `XNorth` to `True` "
"if `X` axis points to the North");
  m.def("TvdDxDy2MdXYTvd", &ext::TvdDxDy2MdXYTvd<Eigen::MatrixXd>,
        py::arg("M"), py::arg("x0"), py::arg("y0"),
        py::arg("XNorth"));

  m.def("TvdssXY2MdXYTvd", &ext::TvdssXY2MdXYTvd<Eigen::MatrixXf>,
        py::arg("M"), py::arg("x0"), py::arg("y0"), py::arg("kb"),
        py::arg("XNorth"),
        "Convert matrix `M` (whose columns represent TVDSS X Y respectively)"
"to `MD_X_Y_TVD`. Set `XNorth` to `True` "
"if `X` axis points to the North");
  m.def("TvdssXY2MdXYTvd", &ext::TvdssXY2MdXYTvd<Eigen::MatrixXd>,
        py::arg("M"), py::arg("x0"), py::arg("y0"), py::arg("kb"),
        py::arg("XNorth"));

  m.def("TvdssDxDy2MdXYTvd", &ext::TvdssDxDy2MdXYTvd<Eigen::MatrixXf>,
        py::arg("M"), py::arg("x0"), py::arg("y0"), py::arg("kb"),
        py::arg("XNorth"),
        "Convert matrix `M` (whose columns represent TVDSS DX DY respectively)"
"to `MD_X_Y_TVD`. Set `XNorth` to `True` "
"if `X` axis points to the North");
  m.def("TvdssDxDy2MdXYTvd", &ext::TvdssDxDy2MdXYTvd<Eigen::MatrixXd>,
        py::arg("M"), py::arg("x0"), py::arg("y0"), py::arg("kb"),
        py::arg("XNorth"));

  m.def("TvdXY2MdAzIncl", &ext::TvdXY2MdAzIncl<Eigen::MatrixXf>,
        py::arg("M"), py::arg("x0"), py::arg("y0"),
        py::arg("XNorth"),
        "Convert matrix `M` (whose columns represent TVD X Y respectively)"
"to `MD_Az_Incl`. Set `XNorth` to `True` "
"if `X` axis points to the North");
  m.def("TvdXY2MdAzIncl", &ext::TvdXY2MdAzIncl<Eigen::MatrixXd>,
        py::arg("M"), py::arg("x0"), py::arg("y0"),
        py::arg("XNorth"));

  m.def("TvdDxDy2MdAzIncl", &ext::TvdDxDy2MdAzIncl<Eigen::MatrixXf>,
        py::arg("M"), py::arg("XNorth"),
        "Convert matrix `M` (whose columns represent TVD DX DY respectively)"
"to `MD_Az_Incl`. Set `XNorth` to `True` "
"if `X` axis points to the North");
  m.def("TvdDxDy2MdAzIncl", &ext::TvdDxDy2MdAzIncl<Eigen::MatrixXd>,
        py::arg("M"), py::arg("XNorth"));

  m.def("TvdssXY2MdAzIncl", &ext::TvdssXY2MdAzIncl<Eigen::MatrixXf>,
        py::arg("M"), py::arg("x0"), py::arg("y0"), py::arg("kb"),
        py::arg("XNorth"),
        "Convert matrix `M` (whose columns represent TVDSS X Y respectively)"
"to `MD_Az_Incl`. Set `XNorth` to `True` "
"if `X` axis points to the North");
  m.def("TvdssXY2MdAzIncl", &ext::TvdssXY2MdAzIncl<Eigen::MatrixXd>,
        py::arg("M"), py::arg("x0"), py::arg("y0"), py::arg("kb"),
        py::arg("XNorth"));

  m.def("TvdssDxDy2MdAzIncl", &ext::TvdssDxDy2MdAzIncl<Eigen::MatrixXf>,
        py::arg("M"), py::arg("kb"),
        py::arg("XNorth"),
        "Convert matrix `M` (whose columns represent TVDSS DX DY respectively)"
"to `MD_Az_Incl`. Set `XNorth` to `True` "
"if `X` axis points to the North");
  m.def("TvdssDxDy2MdAzIncl", &ext::TvdssDxDy2MdAzIncl<Eigen::MatrixXd>,
        py::arg("M"), py::arg("kb"),
        py::arg("XNorth"));
}


} // h5geopy
