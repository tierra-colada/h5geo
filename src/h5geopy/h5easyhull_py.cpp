#include "../../include/h5geopy/h5easyhull_py.h"

#include <h5geo/impl/h5easyhull.h>

namespace h5geopy {

namespace ext {

// EASYHULL
template<typename T>
Eigen::MatrixX2<T> quickHull2D(
    const Eigen::MatrixX2<T>& v)
{
  return h5geo::quickHull2D(v);
}


} // ext


void defineEasyHullFunctions(py::module_& m){
  // EASYHULL
  m.def("quickHull2D", &ext::quickHull2D<float>,
        py::arg("M"),
        "Quick Hull 2D algorithm https://en.wikipedia.org/wiki/quickHull. "
"`M` is a two column matrix: first col - X, second - Y. "
"Return two column matrix (XY)");
  m.def("quickHull2D", &ext::quickHull2D<double>,
        py::arg("M"));
}


} // h5geopy
