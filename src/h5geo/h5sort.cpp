#include "../../include/h5geo/private/h5sort.h"
#include "h5geo_export.h"

#include <algorithm>    // std::sort, std::stable_sort

// std::execution must be hidden in .cpp as it causes errors:
// both TBB and Qt has `emit` macro that makes macro collision (on Linux for sure).
// see:
// https://forum.qt.io/topic/122225/trying-to-use-c-17-parallel-algorithms-with-qt/9
#ifdef H5GEO_USE_THREADS
#include <execution>
#endif


namespace h5geo
{

namespace detail
{

template <typename D>
void _sort(
    D M,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun)
{
  // initialize original index locations
  idx = Eigen::ArrayX<ptrdiff_t>::LinSpaced(
        M.rows(), 0, M.rows()-1);

#ifdef H5GEO_USE_THREADS
  std::stable_sort(std::execution::par, idx.begin(), idx.end(), cmp_fun);
#else
  std::stable_sort(idx.begin(), idx.end(), cmp_fun);
#endif
}

//==================================================================
// explicit instantiation to avoid TBB and Qt `emit` macro collision
//==================================================================

////=======
//// MATRIX
////=======
//template H5GEO_EXPORT void _sort(
//   Eigen::MatrixX<bool> m,
//   Eigen::VectorX<ptrdiff_t>& idx,
//   std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
//template H5GEO_EXPORT void _sort(
//   Eigen::MatrixX<char> m,
//   Eigen::VectorX<ptrdiff_t>& idx,
//   std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
//template H5GEO_EXPORT void _sort(
//   Eigen::MatrixX<unsigned char> m,
//   Eigen::VectorX<ptrdiff_t>& idx,
//   std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
//template H5GEO_EXPORT void _sort(
//   Eigen::MatrixX<short> m,
//   Eigen::VectorX<ptrdiff_t>& idx,
//   std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
//template H5GEO_EXPORT void _sort(
//   Eigen::MatrixX<unsigned short> m,
//   Eigen::VectorX<ptrdiff_t>& idx,
//   std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
//template H5GEO_EXPORT void _sort(
//   Eigen::MatrixX<int> m,
//   Eigen::VectorX<ptrdiff_t>& idx,
//   std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
//template H5GEO_EXPORT void _sort(
//   Eigen::MatrixX<unsigned> m,
//   Eigen::VectorX<ptrdiff_t>& idx,
//   std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
//template H5GEO_EXPORT void _sort(
//   Eigen::MatrixX<ptrdiff_t> m,
//   Eigen::VectorX<ptrdiff_t>& idx,
//   std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
//template H5GEO_EXPORT void _sort(
//   Eigen::MatrixX<size_t> m,
//   Eigen::VectorX<ptrdiff_t>& idx,
//   std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
//template H5GEO_EXPORT void _sort(
//   Eigen::MatrixX<float> m,
//   Eigen::VectorX<ptrdiff_t>& idx,
//   std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
//template H5GEO_EXPORT void _sort(
//   Eigen::MatrixX<double> m,
//   Eigen::VectorX<ptrdiff_t>& idx,
//   std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);

//=================
// MATRIX REFERENCE
//=================
template H5GEO_EXPORT void _sort(
    Eigen::MatrixX<bool> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::MatrixX<char> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::MatrixX<unsigned char> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::MatrixX<short> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::MatrixX<unsigned short> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::MatrixX<int> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::MatrixX<unsigned> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::MatrixX<ptrdiff_t> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::MatrixX<size_t> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::MatrixX<float> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::MatrixX<double> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);

////=======================
//// MATRIX CONST REFERENCE
////=======================
//template H5GEO_EXPORT void _sort(
//    const Eigen::MatrixX<bool> m,
//    Eigen::VectorX<ptrdiff_t>& idx,
//    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
//template H5GEO_EXPORT void _sort(
//    const Eigen::MatrixX<char> m,
//    Eigen::VectorX<ptrdiff_t>& idx,
//    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
//template H5GEO_EXPORT void _sort(
//    const Eigen::MatrixX<unsigned char> m,
//    Eigen::VectorX<ptrdiff_t>& idx,
//    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
//template H5GEO_EXPORT void _sort(
//    const Eigen::MatrixX<short> m,
//    Eigen::VectorX<ptrdiff_t>& idx,
//    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
//template H5GEO_EXPORT void _sort(
//    const Eigen::MatrixX<unsigned short> m,
//    Eigen::VectorX<ptrdiff_t>& idx,
//    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
//template H5GEO_EXPORT void _sort(
//    const Eigen::MatrixX<int> m,
//    Eigen::VectorX<ptrdiff_t>& idx,
//    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
//template H5GEO_EXPORT void _sort(
//    const Eigen::MatrixX<unsigned> m,
//    Eigen::VectorX<ptrdiff_t>& idx,
//    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
//template H5GEO_EXPORT void _sort(
//    const Eigen::MatrixX<ptrdiff_t> m,
//    Eigen::VectorX<ptrdiff_t>& idx,
//    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
//template H5GEO_EXPORT void _sort(
//    const Eigen::MatrixX<size_t> m,
//    Eigen::VectorX<ptrdiff_t>& idx,
//    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
//template H5GEO_EXPORT void _sort(
//    const Eigen::MatrixX<float> m,
//    Eigen::VectorX<ptrdiff_t>& idx,
//    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
//template H5GEO_EXPORT void _sort(
//    const Eigen::MatrixX<double> m,
//    Eigen::VectorX<ptrdiff_t>& idx,
//    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);

//==============
// MATRIX REF IN
//==============
template H5GEO_EXPORT void _sort(
    const Eigen::Ref<const Eigen::MatrixX<bool>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    const Eigen::Ref<const Eigen::MatrixX<char>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    const Eigen::Ref<const Eigen::MatrixX<unsigned char>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    const Eigen::Ref<const Eigen::MatrixX<short>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    const Eigen::Ref<const Eigen::MatrixX<unsigned short>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    const Eigen::Ref<const Eigen::MatrixX<int>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    const Eigen::Ref<const Eigen::MatrixX<unsigned>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    const Eigen::Ref<const Eigen::MatrixX<ptrdiff_t>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    const Eigen::Ref<const Eigen::MatrixX<size_t>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    const Eigen::Ref<const Eigen::MatrixX<float>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    const Eigen::Ref<const Eigen::MatrixX<double>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);

//==================
// MATRIX REF IN-OUT
//==================
template H5GEO_EXPORT void _sort(
    Eigen::Ref<Eigen::MatrixX<bool>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Ref<Eigen::MatrixX<char>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Ref<Eigen::MatrixX<unsigned char>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Ref<Eigen::MatrixX<short>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Ref<Eigen::MatrixX<unsigned short>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Ref<Eigen::MatrixX<int>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Ref<Eigen::MatrixX<unsigned>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Ref<Eigen::MatrixX<ptrdiff_t>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Ref<Eigen::MatrixX<size_t>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Ref<Eigen::MatrixX<float>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Ref<Eigen::MatrixX<double>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);


////=======
//// VECTOR
////=======
//template H5GEO_EXPORT void _sort(
//   Eigen::VectorX<bool> m,
//   Eigen::VectorX<ptrdiff_t>& idx,
//   std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
//template H5GEO_EXPORT void _sort(
//   Eigen::VectorX<char> m,
//   Eigen::VectorX<ptrdiff_t>& idx,
//   std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
//template H5GEO_EXPORT void _sort(
//   Eigen::VectorX<unsigned char> m,
//   Eigen::VectorX<ptrdiff_t>& idx,
//   std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
//template H5GEO_EXPORT void _sort(
//   Eigen::VectorX<short> m,
//   Eigen::VectorX<ptrdiff_t>& idx,
//   std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
//template H5GEO_EXPORT void _sort(
//   Eigen::VectorX<unsigned short> m,
//   Eigen::VectorX<ptrdiff_t>& idx,
//   std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
//template H5GEO_EXPORT void _sort(
//   Eigen::VectorX<int> m,
//   Eigen::VectorX<ptrdiff_t>& idx,
//   std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
//template H5GEO_EXPORT void _sort(
//   Eigen::VectorX<unsigned> m,
//   Eigen::VectorX<ptrdiff_t>& idx,
//   std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
//template H5GEO_EXPORT void _sort(
//   Eigen::VectorX<ptrdiff_t> m,
//   Eigen::VectorX<ptrdiff_t>& idx,
//   std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
//template H5GEO_EXPORT void _sort(
//   Eigen::VectorX<size_t> m,
//   Eigen::VectorX<ptrdiff_t>& idx,
//   std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
//template H5GEO_EXPORT void _sort(
//   Eigen::VectorX<float> m,
//   Eigen::VectorX<ptrdiff_t>& idx,
//   std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
//template H5GEO_EXPORT void _sort(
//   Eigen::VectorX<double> m,
//   Eigen::VectorX<ptrdiff_t>& idx,
//   std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);

//=================
// VECTOR REFERENCE
//=================
template H5GEO_EXPORT void _sort(
    Eigen::VectorX<bool> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::VectorX<char> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::VectorX<unsigned char> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::VectorX<short> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::VectorX<unsigned short> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::VectorX<int> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::VectorX<unsigned> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::VectorX<ptrdiff_t> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::VectorX<size_t> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::VectorX<float> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::VectorX<double> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);

////=======================
//// VECTOR CONST REFERENCE
////=======================
//template H5GEO_EXPORT void _sort(
//    const Eigen::VectorX<bool> m,
//    Eigen::VectorX<ptrdiff_t>& idx,
//    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
//template H5GEO_EXPORT void _sort(
//    const Eigen::VectorX<char> m,
//    Eigen::VectorX<ptrdiff_t>& idx,
//    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
//template H5GEO_EXPORT void _sort(
//    const Eigen::VectorX<unsigned char> m,
//    Eigen::VectorX<ptrdiff_t>& idx,
//    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
//template H5GEO_EXPORT void _sort(
//    const Eigen::VectorX<short> m,
//    Eigen::VectorX<ptrdiff_t>& idx,
//    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
//template H5GEO_EXPORT void _sort(
//    const Eigen::VectorX<unsigned short> m,
//    Eigen::VectorX<ptrdiff_t>& idx,
//    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
//template H5GEO_EXPORT void _sort(
//    const Eigen::VectorX<int> m,
//    Eigen::VectorX<ptrdiff_t>& idx,
//    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
//template H5GEO_EXPORT void _sort(
//    const Eigen::VectorX<unsigned> m,
//    Eigen::VectorX<ptrdiff_t>& idx,
//    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
//template H5GEO_EXPORT void _sort(
//    const Eigen::VectorX<ptrdiff_t> m,
//    Eigen::VectorX<ptrdiff_t>& idx,
//    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
//template H5GEO_EXPORT void _sort(
//    const Eigen::VectorX<size_t> m,
//    Eigen::VectorX<ptrdiff_t>& idx,
//    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
//template H5GEO_EXPORT void _sort(
//    const Eigen::VectorX<float> m,
//    Eigen::VectorX<ptrdiff_t>& idx,
//    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
//template H5GEO_EXPORT void _sort(
//    const Eigen::VectorX<double> m,
//    Eigen::VectorX<ptrdiff_t>& idx,
//    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);

//==============
// VECTOR REF IN
//==============
template H5GEO_EXPORT void _sort(
    const Eigen::Ref<const Eigen::VectorX<bool>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    const Eigen::Ref<const Eigen::VectorX<char>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    const Eigen::Ref<const Eigen::VectorX<unsigned char>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    const Eigen::Ref<const Eigen::VectorX<short>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    const Eigen::Ref<const Eigen::VectorX<unsigned short>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    const Eigen::Ref<const Eigen::VectorX<int>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    const Eigen::Ref<const Eigen::VectorX<unsigned>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    const Eigen::Ref<const Eigen::VectorX<ptrdiff_t>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    const Eigen::Ref<const Eigen::VectorX<size_t>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    const Eigen::Ref<const Eigen::VectorX<float>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    const Eigen::Ref<const Eigen::VectorX<double>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);

//==================
// VECTOR REF IN-OUT
//==================
template H5GEO_EXPORT void _sort(
    Eigen::Ref<Eigen::VectorX<bool>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Ref<Eigen::VectorX<char>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Ref<Eigen::VectorX<unsigned char>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Ref<Eigen::VectorX<short>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Ref<Eigen::VectorX<unsigned short>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Ref<Eigen::VectorX<int>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Ref<Eigen::VectorX<unsigned>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Ref<Eigen::VectorX<ptrdiff_t>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Ref<Eigen::VectorX<size_t>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Ref<Eigen::VectorX<float>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Ref<Eigen::VectorX<double>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);

//========================
// MATRIX BLOCK OF COLUMNS
//========================
// non contigous
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::MatrixX<bool>,-1,1,0> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::MatrixX<char>,-1,1,0> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::MatrixX<unsigned char>,-1,1,0> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::MatrixX<short>,-1,1,0> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::MatrixX<unsigned short>,-1,1,0> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::MatrixX<int>,-1,1,0> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::MatrixX<unsigned>,-1,1,0> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::MatrixX<ptrdiff_t>,-1,1,0> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::MatrixX<size_t>,-1,1,0> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::MatrixX<float>,-1,1,0> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::MatrixX<double>,-1,1,0> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);

// ref in-out
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::MatrixX<bool>>,-1,1,0> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::MatrixX<char>>,-1,1,0> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::MatrixX<unsigned char>>,-1,1,0> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::MatrixX<short>>,-1,1,0> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::MatrixX<unsigned short>>,-1,1,0> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::MatrixX<int>>,-1,1,0> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::MatrixX<unsigned>>,-1,1,0> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::MatrixX<ptrdiff_t>>,-1,1,0> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::MatrixX<size_t>>,-1,1,0> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::MatrixX<float>>,-1,1,0> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::MatrixX<double>>,-1,1,0> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);

// contigous
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::MatrixX<bool>,-1,1,1> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::MatrixX<char>,-1,1,1> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::MatrixX<unsigned char>,-1,1,1> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::MatrixX<short>,-1,1,1> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::MatrixX<unsigned short>,-1,1,1> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::MatrixX<int>,-1,1,1> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::MatrixX<unsigned>,-1,1,1> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::MatrixX<ptrdiff_t>,-1,1,1> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::MatrixX<size_t>,-1,1,1> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::MatrixX<float>,-1,1,1> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::MatrixX<double>,-1,1,1> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);

// ref in-out
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::MatrixX<bool>>,-1,1,1> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::MatrixX<char>>,-1,1,1> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::MatrixX<unsigned char>>,-1,1,1> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::MatrixX<short>>,-1,1,1> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::MatrixX<unsigned short>>,-1,1,1> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::MatrixX<int>>,-1,1,1> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::MatrixX<unsigned>>,-1,1,1> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::MatrixX<ptrdiff_t>>,-1,1,1> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::MatrixX<size_t>>,-1,1,1> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::MatrixX<float>>,-1,1,1> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::MatrixX<double>>,-1,1,1> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);

//=====================
// MATRIX BLOCK OF ROWS
//=====================
// non contigous
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::MatrixX<bool>,1,-1,0> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::MatrixX<char>,1,-1,0> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::MatrixX<unsigned char>,1,-1,0> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::MatrixX<short>,1,-1,0> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::MatrixX<unsigned short>,1,-1,0> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::MatrixX<int>,1,-1,0> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::MatrixX<unsigned>,1,-1,0> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::MatrixX<ptrdiff_t>,1,-1,0> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::MatrixX<size_t>,1,-1,0> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::MatrixX<float>,1,-1,0> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::MatrixX<double>,1,-1,0> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);

// ref in-out
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::MatrixX<bool>>,1,-1,0> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::MatrixX<char>>,1,-1,0> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::MatrixX<unsigned char>>,1,-1,0> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::MatrixX<short>>,1,-1,0> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::MatrixX<unsigned short>>,1,-1,0> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::MatrixX<int>>,1,-1,0> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::MatrixX<unsigned>>,1,-1,0> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::MatrixX<ptrdiff_t>>,1,-1,0> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::MatrixX<size_t>>,1,-1,0> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::MatrixX<float>>,1,-1,0> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::MatrixX<double>>,1,-1,0> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);

// contigous
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::MatrixX<bool>,1,-1,1> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::MatrixX<char>,1,-1,1> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::MatrixX<unsigned char>,1,-1,1> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::MatrixX<short>,1,-1,1> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::MatrixX<unsigned short>,1,-1,1> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::MatrixX<int>,1,-1,1> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::MatrixX<unsigned>,1,-1,1> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::MatrixX<ptrdiff_t>,1,-1,1> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::MatrixX<size_t>,1,-1,1> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::MatrixX<float>,1,-1,1> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::MatrixX<double>,1,-1,1> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);

// ref in-out
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::MatrixX<bool>>,1,-1,1> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::MatrixX<char>>,1,-1,1> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::MatrixX<unsigned char>>,1,-1,1> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::MatrixX<short>>,1,-1,1> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::MatrixX<unsigned short>>,1,-1,1> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::MatrixX<int>>,1,-1,1> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::MatrixX<unsigned>>,1,-1,1> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::MatrixX<ptrdiff_t>>,1,-1,1> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::MatrixX<size_t>>,1,-1,1> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::MatrixX<float>>,1,-1,1> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::MatrixX<double>>,1,-1,1> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);

//========================
// VECTOR BLOCK OF COLUMNS
//========================
// non contigous
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::VectorX<bool>,-1,1,0> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::VectorX<char>,-1,1,0> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::VectorX<unsigned char>,-1,1,0> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::VectorX<short>,-1,1,0> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::VectorX<unsigned short>,-1,1,0> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::VectorX<int>,-1,1,0> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::VectorX<unsigned>,-1,1,0> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::VectorX<ptrdiff_t>,-1,1,0> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::VectorX<size_t>,-1,1,0> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::VectorX<float>,-1,1,0> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::VectorX<double>,-1,1,0> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);

// ref in-out
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::VectorX<bool>>,-1,1,0> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::VectorX<char>>,-1,1,0> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::VectorX<unsigned char>>,-1,1,0> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::VectorX<short>>,-1,1,0> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::VectorX<unsigned short>>,-1,1,0> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::VectorX<int>>,-1,1,0> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::VectorX<unsigned>>,-1,1,0> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::VectorX<ptrdiff_t>>,-1,1,0> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::VectorX<size_t>>,-1,1,0> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::VectorX<float>>,-1,1,0> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::VectorX<double>>,-1,1,0> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);

// contigous
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::VectorX<bool>,-1,1,1> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::VectorX<char>,-1,1,1> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::VectorX<unsigned char>,-1,1,1> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::VectorX<short>,-1,1,1> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::VectorX<unsigned short>,-1,1,1> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::VectorX<int>,-1,1,1> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::VectorX<unsigned>,-1,1,1> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::VectorX<ptrdiff_t>,-1,1,1> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::VectorX<size_t>,-1,1,1> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::VectorX<float>,-1,1,1> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::VectorX<double>,-1,1,1> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);

// ref in-out
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::VectorX<bool>>,-1,1,1> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::VectorX<char>>,-1,1,1> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::VectorX<unsigned char>>,-1,1,1> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::VectorX<short>>,-1,1,1> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::VectorX<unsigned short>>,-1,1,1> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::VectorX<int>>,-1,1,1> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::VectorX<unsigned>>,-1,1,1> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::VectorX<ptrdiff_t>>,-1,1,1> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::VectorX<size_t>>,-1,1,1> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::VectorX<float>>,-1,1,1> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::VectorX<double>>,-1,1,1> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);


//=====================
// VECTOR BLOCK OF ROWS
//=====================
// non contigous
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::VectorX<bool>,1,-1,0> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::VectorX<char>,1,-1,0> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::VectorX<unsigned char>,1,-1,0> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::VectorX<short>,1,-1,0> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::VectorX<unsigned short>,1,-1,0> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::VectorX<int>,1,-1,0> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::VectorX<unsigned>,1,-1,0> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::VectorX<ptrdiff_t>,1,-1,0> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::VectorX<size_t>,1,-1,0> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::VectorX<float>,1,-1,0> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::VectorX<double>,1,-1,0> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);

// ref in-out
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::VectorX<bool>>,1,-1,0> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::VectorX<char>>,1,-1,0> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::VectorX<unsigned char>>,1,-1,0> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::VectorX<short>>,1,-1,0> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::VectorX<unsigned short>>,1,-1,0> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::VectorX<int>>,1,-1,0> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::VectorX<unsigned>>,1,-1,0> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::VectorX<ptrdiff_t>>,1,-1,0> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::VectorX<size_t>>,1,-1,0> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::VectorX<float>>,1,-1,0> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::VectorX<double>>,1,-1,0> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);

// contigous
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::VectorX<bool>,1,-1,1> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::VectorX<char>,1,-1,1> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::VectorX<unsigned char>,1,-1,1> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::VectorX<short>,1,-1,1> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::VectorX<unsigned short>,1,-1,1> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::VectorX<int>,1,-1,1> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::VectorX<unsigned>,1,-1,1> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::VectorX<ptrdiff_t>,1,-1,1> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::VectorX<size_t>,1,-1,1> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::VectorX<float>,1,-1,1> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::VectorX<double>,1,-1,1> const m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);

// ref in-out
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::VectorX<bool>>,1,-1,1> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::VectorX<char>>,1,-1,1> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::VectorX<unsigned char>>,1,-1,1> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::VectorX<short>>,1,-1,1> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::VectorX<unsigned short>>,1,-1,1> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::VectorX<int>>,1,-1,1> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::VectorX<unsigned>>,1,-1,1> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::VectorX<ptrdiff_t>>,1,-1,1> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::VectorX<size_t>>,1,-1,1> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::VectorX<float>>,1,-1,1> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
    Eigen::Block<Eigen::Ref<Eigen::VectorX<double>>,1,-1,1> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);


// From pybind11 to be able to bind sort functions
using EigenDStride = Eigen::Stride<Eigen::Dynamic, Eigen::Dynamic>;
template <typename MatrixType> using EigenDRef = Eigen::Ref<MatrixType, 0, EigenDStride>;
template <typename MatrixType> using EigenDMap = Eigen::Map<MatrixType, 0, EigenDStride>;


////===================
//// MATRIX PYBIND DREF
////===================
//template H5GEO_EXPORT void _sort(
//   EigenDRef<Eigen::Matrix<bool,-1,1,0,-1,1>> m,
//   Eigen::VectorX<ptrdiff_t>& idx,
//   std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
//template H5GEO_EXPORT void _sort(
//   EigenDRef<Eigen::Matrix<char,-1,1,0,-1,1>> m,
//   Eigen::VectorX<ptrdiff_t>& idx,
//   std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
//template H5GEO_EXPORT void _sort(
//   EigenDRef<Eigen::Matrix<unsigned char,-1,1,0,-1,1>> m,
//   Eigen::VectorX<ptrdiff_t>& idx,
//   std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
//template H5GEO_EXPORT void _sort(
//   EigenDRef<Eigen::Matrix<short,-1,1,0,-1,1>> m,
//   Eigen::VectorX<ptrdiff_t>& idx,
//   std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
//template H5GEO_EXPORT void _sort(
//   EigenDRef<Eigen::Matrix<unsigned short,-1,1,0,-1,1>> m,
//   Eigen::VectorX<ptrdiff_t>& idx,
//   std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
//template H5GEO_EXPORT void _sort(
//   EigenDRef<Eigen::Matrix<int,-1,1,0,-1,1>> m,
//   Eigen::VectorX<ptrdiff_t>& idx,
//   std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
//template H5GEO_EXPORT void _sort(
//   EigenDRef<Eigen::Matrix<unsigned,-1,1,0,-1,1>> m,
//   Eigen::VectorX<ptrdiff_t>& idx,
//   std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
//template H5GEO_EXPORT void _sort(
//   EigenDRef<Eigen::Matrix<ptrdiff_t,-1,1,0,-1,1>> m,
//   Eigen::VectorX<ptrdiff_t>& idx,
//   std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
//template H5GEO_EXPORT void _sort(
//   EigenDRef<Eigen::Matrix<size_t,-1,1,0,-1,1>> m,
//   Eigen::VectorX<ptrdiff_t>& idx,
//   std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
//template H5GEO_EXPORT void _sort(
//   EigenDRef<Eigen::Matrix<float,-1,1,0,-1,1>> m,
//   Eigen::VectorX<ptrdiff_t>& idx,
//   std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
//template H5GEO_EXPORT void _sort(
//   EigenDRef<Eigen::Matrix<double,-1,1,0,-1,1>> m,
//   Eigen::VectorX<ptrdiff_t>& idx,
//   std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);

//==============================
// MATRIX PYTBIND DREF REF
//==============================
template H5GEO_EXPORT void _sort(
   EigenDRef<Eigen::Matrix<bool,-1,1,0,-1,1>> m,
   Eigen::VectorX<ptrdiff_t>& idx,
   std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
   EigenDRef<Eigen::Matrix<char,-1,1,0,-1,1>> m,
   Eigen::VectorX<ptrdiff_t>& idx,
   std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
   EigenDRef<Eigen::Matrix<unsigned char,-1,1,0,-1,1>> m,
   Eigen::VectorX<ptrdiff_t>& idx,
   std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
   EigenDRef<Eigen::Matrix<short,-1,1,0,-1,1>> m,
   Eigen::VectorX<ptrdiff_t>& idx,
   std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
   EigenDRef<Eigen::Matrix<unsigned short,-1,1,0,-1,1>> m,
   Eigen::VectorX<ptrdiff_t>& idx,
   std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
   EigenDRef<Eigen::Matrix<int,-1,1,0,-1,1>> m,
   Eigen::VectorX<ptrdiff_t>& idx,
   std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
   EigenDRef<Eigen::Matrix<unsigned,-1,1,0,-1,1>> m,
   Eigen::VectorX<ptrdiff_t>& idx,
   std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
   EigenDRef<Eigen::Matrix<ptrdiff_t,-1,1,0,-1,1>> m,
   Eigen::VectorX<ptrdiff_t>& idx,
   std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
   EigenDRef<Eigen::Matrix<size_t,-1,1,0,-1,1>> m,
   Eigen::VectorX<ptrdiff_t>& idx,
   std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
   EigenDRef<Eigen::Matrix<float,-1,1,0,-1,1>> m,
   Eigen::VectorX<ptrdiff_t>& idx,
   std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
   EigenDRef<Eigen::Matrix<double,-1,1,0,-1,1>> m,
   Eigen::VectorX<ptrdiff_t>& idx,
   std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);

//==============================
// MATRIX PYTBIND DREF CONST REF
//==============================
//template<typename Scalar_, int Rows_, int Cols_, int Options_, int MaxRows_, int MaxCols_>
//class Eigen::Matrix< Scalar_, Rows_, Cols_, Options_, MaxRows_, MaxCols_ >

// VECTOR X: COLUMN
template H5GEO_EXPORT void _sort(
   const EigenDRef<const Eigen::Matrix<bool,-1,1,0,-1,1>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
   const EigenDRef<const Eigen::Matrix<char,-1,1,0,-1,1>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
   const EigenDRef<const Eigen::Matrix<unsigned char,-1,1,0,-1,1>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
   const EigenDRef<const Eigen::Matrix<short,-1,1,0,-1,1>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
   const EigenDRef<const Eigen::Matrix<unsigned short,-1,1,0,-1,1>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
   const EigenDRef<const Eigen::Matrix<int,-1,1,0,-1,1>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
   const EigenDRef<const Eigen::Matrix<unsigned,-1,1,0,-1,1>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
   const EigenDRef<const Eigen::Matrix<ptrdiff_t,-1,1,0,-1,1>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
   const EigenDRef<const Eigen::Matrix<size_t,-1,1,0,-1,1>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
   const EigenDRef<const Eigen::Matrix<float,-1,1,0,-1,1>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
   const EigenDRef<const Eigen::Matrix<double,-1,1,0,-1,1>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);


template H5GEO_EXPORT void _sort(
   const EigenDRef<const Eigen::Matrix<bool,1,-1,0,1,-1>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
   const EigenDRef<const Eigen::Matrix<char,1,-1,0,1,-1>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
   const EigenDRef<const Eigen::Matrix<unsigned char,1,-1,0,1,-1>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
   const EigenDRef<const Eigen::Matrix<short,1,-1,0,1,-1>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
   const EigenDRef<const Eigen::Matrix<unsigned short,1,-1,0,1,-1>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
   const EigenDRef<const Eigen::Matrix<int,1,-1,0,1,-1>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
   const EigenDRef<const Eigen::Matrix<unsigned,1,-1,0,1,-1>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
   const EigenDRef<const Eigen::Matrix<ptrdiff_t,1,-1,0,1,-1>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
   const EigenDRef<const Eigen::Matrix<size_t,1,-1,0,1,-1>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
   const EigenDRef<const Eigen::Matrix<float,1,-1,0,1,-1>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
   const EigenDRef<const Eigen::Matrix<double,1,-1,0,1,-1>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);

// MATRIX XX
template H5GEO_EXPORT void _sort(
   const EigenDRef<const Eigen::Matrix<bool,-1,-1,0,-1,-1>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
   const EigenDRef<const Eigen::Matrix<char,-1,-1,0,-1,-1>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
   const EigenDRef<const Eigen::Matrix<unsigned char,-1,-1,0,-1,-1>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
   const EigenDRef<const Eigen::Matrix<short,-1,-1,0,-1,-1>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
   const EigenDRef<const Eigen::Matrix<unsigned short,-1,-1,0,-1,-1>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
   const EigenDRef<const Eigen::Matrix<int,-1,-1,0,-1,-1>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
   const EigenDRef<const Eigen::Matrix<unsigned,-1,-1,0,-1,-1>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
   const EigenDRef<const Eigen::Matrix<ptrdiff_t,-1,-1,0,-1,-1>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
   const EigenDRef<const Eigen::Matrix<size_t,-1,-1,0,-1,-1>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
   const EigenDRef<const Eigen::Matrix<float,-1,-1,0,-1,-1>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);
template H5GEO_EXPORT void _sort(
   const EigenDRef<const Eigen::Matrix<double,-1,-1,0,-1,-1>> m,
    Eigen::VectorX<ptrdiff_t>& idx,
    std::function<bool(ptrdiff_t, ptrdiff_t)> cmp_fun);


} // detail


} // h5geo
