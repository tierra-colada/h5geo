#ifndef H5GEO_PY_H
#define H5GEO_PY_H

#ifndef H5GT_USE_EIGEN
#define H5GT_USE_EIGEN // should be defined before including h5gt
#endif

#include <h5gt/H5Object.hpp>
#include <h5gt/H5File.hpp>
#include <h5gt/H5DataSet.hpp>
#include <h5gt/H5DataSpace.hpp>

#include <Eigen/Dense>

#ifdef H5GEO_USE_GDAL
#include <gdal.h>
#include <gdal_priv.h>
#endif

#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include <pybind11/complex.h>
#include <pybind11/operators.h>
#include <pybind11/cast.h>

#ifdef H5GEO_USE_GDAL
#include <gdal.h>
#include <gdal_priv.h>
#endif

// DON'T WRITE 'using namespace h5gt' AS h5gt AND h5geo ARE INCOPATIBLE

namespace py = pybind11;

namespace h5geo{};
using namespace h5geo;

// opaque types must be included to every translation unit or ODR falls (warnings is given)
#include <h5geo/h5points3.h>

PYBIND11_MAKE_OPAQUE(h5geo::Point1Array);
PYBIND11_MAKE_OPAQUE(h5geo::Point2Array);
PYBIND11_MAKE_OPAQUE(h5geo::Point3Array);
PYBIND11_MAKE_OPAQUE(h5geo::Point4Array);

/* to generate .pyi the returned type should be declared before it
 * is called. For example `createGroup` returns `Group` so I need to
 * place `py::class_<Group, Object>(m, "Group")`
 * before calling `.def("createGroup", ...` ) */

/* py::arithmetic() -> create an enumeration that also supports
 * rudimentary arithmetic and bit-level operations like
 * comparisons, and, or, xor, negation, etc. */

/* export_values() -> export the enum entries into the parent scope,
 * which should be skipped for newer C++11-style strongly typed enums */

/* py::const_ -> is neccessary when binding overoaded functions */

/* All declarations are here. And all functions are invoked in `h5gt.cpp` */

#include <h5geo/impl/h5base.h>

class H5BasePy : public H5Base {
public:
    /* Inherit the constructors */
    using H5Base::H5Base;

    /* Trampoline (need one for each virtual function) */
    void Delete() override {
        PYBIND11_OVERRIDE_PURE(
            void, /* Return type */
            H5Base,      /* Parent class */
            Delete          /* Name of function in C++ (must match Python name) */
        );
    }
};


#endif // H5GEOPY_H
