#ifndef FIND_H
#define FIND_H

#include <Eigen/Dense>

namespace h5geo
{
template <typename T>
/*!
 * \brief find_index find_index find all non-zero elements's indices.
 * Possible usage:
 * Eigen::VectorX<ptrdiff_t> ind = find_index(M.array() > 20)
 * M.array() > 20 returns bool matrix and then we find non-zero indices
 * \param M
 * \return ind indices of nonzero elements
 */
Eigen::VectorX<ptrdiff_t> find_index(
    Eigen::DenseBase<T> const & M){
  Eigen::VectorX<ptrdiff_t> ind(M.size());
  ptrdiff_t ii = 0;
  for (ptrdiff_t i = 0; i < M.size(); i++){
    if (M(i) != 0){
      ind(ii) = i;
      ii++;
    }
  }
  ind.conservativeResize(ii);

  return ind;
}

}

#endif // FIND_H
