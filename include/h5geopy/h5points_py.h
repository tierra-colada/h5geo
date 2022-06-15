#ifndef H5POINTS_PY_H
#define H5POINTS_PY_H

#include "h5geo_py.h"

#include <h5geo/private/h5points1impl.h>
#include <h5geo/private/h5points2impl.h>
#include <h5geo/private/h5points3impl.h>
#include <h5geo/private/h5points4impl.h>

namespace h5geopy {

void H5Points1_py(
    py::class_<
    H5Points1,
    H5Points1Impl<H5Points1>,
    H5BasePoints,
    H5BaseObject,
    H5Base,
    std::unique_ptr<H5Points1, ObjectDeleter>>
    &py_obj);

void H5Points2_py(
    py::class_<
    H5Points2,
    H5Points2Impl<H5Points2>,
    H5BasePoints,
    H5BaseObject,
    H5Base,
    std::unique_ptr<H5Points2, ObjectDeleter>>
    &py_obj);

void H5Points3_py(
    py::class_<
    H5Points3,
    H5Points3Impl<H5Points3>,
    H5BasePoints,
    H5BaseObject,
    H5Base,
    std::unique_ptr<H5Points3, ObjectDeleter>>
    &py_obj);

void H5Points4_py(
    py::class_<
    H5Points4,
    H5Points4Impl<H5Points4>,
    H5BasePoints,
    H5BaseObject,
    H5Base,
    std::unique_ptr<H5Points4, ObjectDeleter>>
    &py_obj);

} // h5geopy

#endif // H5POINTS_PY_H
