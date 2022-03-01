#include "../../include/h5geopy/h5interpolation_py.h"

#include <h5geo/impl/h5interpolation.h>

namespace h5geopy {

namespace ext {

template <typename D>
Eigen::VectorX<typename D::Scalar> interp1Monotonic(
    const py::EigenDRef<const D> x,
    const py::EigenDRef<const D> y,
    const py::EigenDRef<const D> xnew,
    typename D::Scalar extrapVal)
{
  return h5geo::interp1Monotonic(x, y, xnew, extrapVal);
}

} // ext

void defineInterpolationFunctions(py::module_& m){
  m.def("interp1Monotonic", &ext::interp1Monotonic<const Eigen::VectorXf>,
        py::arg("x"),
        py::arg("y"),
        py::arg("xnew"),
        py::arg("extrapVal"),
        "assume that x is strictly monotonic (either increasing or decreasing)");

  m.def("interp1Monotonic", &ext::interp1Monotonic<const Eigen::VectorXd>,
        py::arg("x"),
        py::arg("y"),
        py::arg("xnew"),
        py::arg("extrapVal"),
        "assume that x is strictly monotonic (either increasing or decreasing)");
}

} // h5geopy
