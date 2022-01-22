#ifndef EASYHULL_H
#define EASYHULL_H

// Implementations of various convex hull algorithms
// using the C++ Standard Library.
// For clarity, the implementations do not check for
// duplicate or collinear points.

#include <algorithm>
#include <iostream>
#include <vector>

#include <Eigen/Dense>


namespace h5geo{

template<typename T1, typename T2>
/*!
 * \brief _ccw The z-value of the cross product of segments
 * (a, b) and (a, c). Positive means c is _ccw
 * from (a, b), negative cw. Zero means its collinear.
 * \param a
 * \param b
 * \param c
 * \return
 */
T1 _ccw(const Eigen::Vector2<T1>& a,
       const Eigen::Vector2<T1>& b,
       const Eigen::MatrixBase<T2>& c) {
  return (b.x() - a.x()) * (c.y() - a.y()) - (b.y() - a.y()) * (c.x() - a.x());
}

template<typename T>
/*!
 * \brief _isLeftOf Returns true if a is lexicographically before b.
 * \param a
 * \param b
 * \return
 */
bool _isLeftOf(const Eigen::Vector2<T>& a,
              const Eigen::Vector2<T>& b) {
  return (a.x() < b.x() || (a.x() == b.x() && a.y() < b.y()));
}

template<typename T>
/*!
 * \brief The _ccwSorter struct Used to sort points in _ccw order about a pivot.
 */
struct _ccwSorter {
  const Eigen::Vector2<T>& pivot;

  _ccwSorter(const Eigen::Vector2<T>& inPivot) : pivot(inPivot) { }

  bool operator()(const Eigen::Vector2<T>& a, const Eigen::Vector2<T>& b) {
    return _ccw(pivot, a, b) < 0;
  }
};

template<typename T>
/*!
 * \brief _segmentLen The length of segment (a, b).
 * \param a
 * \param b
 * \return
 */
T _segmentLen(const Eigen::Vector2<T>& a,
      const Eigen::Vector2<T>& b) {
  return sqrt((b.x() - a.x()) * (b.x() - a.x()) + (b.y() - a.y()) * (b.y() - a.y()));
}

template<typename T1, typename T2>
/*!
 * \brief _segmentDist The unsigned distance of p from segment (a, b).
 * \param a
 * \param b
 * \param p
 * \return
 */
T1 _segmentDist(const Eigen::Vector2<T1>& a,
        const Eigen::Vector2<T1>& b,
        const Eigen::MatrixBase<T2>& p) {
  return fabs((b.x() - a.x()) * (a.y() - p.y()) - (b.y() - a.y()) * (a.x() - p.x())) / _segmentLen(a, b);
}

template<typename T>
/*!
 * \brief _getFarthestInd Returns the index of the farthest point from segment (a, b).
 * \param a
 * \param b
 * \param v
 * \return
 */
size_t _getFarthestInd(const Eigen::Vector2<T>& a,
                   const Eigen::Vector2<T>& b,
                   const std::vector<Eigen::Vector2<T> >& v) {
  size_t idxMax = 0;
  T distMax = _segmentDist(a, b, v[idxMax]);

  for (size_t i = 1; i < v.size(); ++i) {
    T distCurr = _segmentDist(a, b, v[i]);
    if (distCurr > distMax) {
      idxMax = i;
      distMax = distCurr;
    }
  }

  return idxMax;
}

template<typename T1, typename T2>
size_t _getFarthestInd(const Eigen::Vector2<T1>& a,
                   const Eigen::Vector2<T1>& b,
                   const Eigen::MatrixX2<T2>& v) {
  size_t idxMax = 0;
  T1 distMax = _segmentDist(a, b, v.row(idxMax));

  for (size_t i = 1; i < v.rows(); ++i) {
    T1 distCurr = _segmentDist(a, b, v.row(i));
    if (distCurr > distMax) {
      idxMax = i;
      distMax = distCurr;
    }
  }

  return idxMax;
}

template<typename T>
/*!
 * \brief giftWrapping The gift-wrapping algorithm for convex hull.
 * https://en.wikipedia.org/wiki/Gift_wrapping_algorithm
 * \param v
 * \return
 */
[[deprecated("Use 'quickHull2D' instead")]]
std::vector<Eigen::Vector2<T> > giftWrapping(std::vector<Eigen::Vector2<T> > v) {
  // Move the leftmost point to the beginning of our vector.
  // It will be the first point in our convext hull.
  swap(v[0], *min_element(v.begin(), v.end(), _isLeftOf<T>));

  std::vector<Eigen::Vector2<T> > hull;
  // Repeatedly find the first _ccw point from our last hull point
  // and put it at the front of our array.
  // Stop when we see our first point again.
  do {
    hull.push_back(v[0]);
    swap(v[0], *min_element(v.begin() + 1, v.end(), _ccwSorter(v[0])));
  } while (v[0] != hull[0]);

  return hull;
}

template<typename T>
/*!
 * \brief GrahamScan The Graham scan algorithm for convex hull.
 * https://en.wikipedia.org/wiki/Graham_scan
 * \param v
 * \return
 */
[[deprecated("Use 'quickHull2D' instead")]]
std::vector<Eigen::Vector2<T> > GrahamScan(std::vector<Eigen::Vector2<T> > v) {
  // Put our leftmost point at index 0
  swap(v[0], *min_element(v.begin(), v.end(), _isLeftOf<T>));

  // Sort the rest of the points in counter-clockwise order
  // from our leftmost point.
  sort(v.begin() + 1, v.end(), _ccwSorter(v[0]));

  // Add our first three points to the hull.
  std::vector<Eigen::Vector2<T> > hull;
  auto it = v.begin();
  hull.push_back(*it++);
  hull.push_back(*it++);
  hull.push_back(*it++);

  while (it != v.end()) {
    // Pop off any points that make a convex angle with *it
    while (_ccw(*(hull.rbegin() + 1), *(hull.rbegin()), *it) >= 0) {
      hull.pop_back();
    }
    hull.push_back(*it++);
  }

  return hull;
}


template<typename T>
/*!
 * \brief monotoneChain The monotone chain algorithm for convex hull.
 * \param v
 * \return
 */
[[deprecated("Use 'quickHull2D' instead")]]
std::vector<Eigen::Vector2<T> > monotoneChain(std::vector<Eigen::Vector2<T> > v) {
  // Sort our points in lexicographic order.
  sort(v.begin(), v.end(), _isLeftOf<T>);

  // Find the lower half of the convex hull.
  std::vector<Eigen::Vector2<T> > lower;
  for (auto it = v.begin(); it != v.end(); ++it) {
    // Pop off any points that make a convex angle with *it
    while (lower.size() >= 2 && _ccw(*(lower.rbegin() + 1), *(lower.rbegin()), *it) >= 0) {
      lower.pop_back();
    }
    lower.push_back(*it);
  }

  // Find the upper half of the convex hull.
  std::vector<Eigen::Vector2<T> > upper;
  for (auto it = v.rbegin(); it != v.rend(); ++it) {
    // Pop off any points that make a convex angle with *it
    while (upper.size() >= 2 && _ccw(*(upper.rbegin() + 1), *(upper.rbegin()), *it) >= 0) {
      upper.pop_back();
    }
    upper.push_back(*it);
  }

  std::vector<Eigen::Vector2<T> > hull;
  hull.insert(hull.end(), lower.begin(), lower.end());
  // Both hulls include both endpoints, so leave them out when we
  // append the upper hull.
  hull.insert(hull.end(), upper.begin() + 1, upper.end() - 1);
  return hull;
}


template<typename T>
/*!
 * \brief quickHull2D Recursive call of the quickHull2D algorithm.
 * \param v
 * \param a
 * \param b
 * \param hull
 */
void quickHull2D(
    const std::vector<Eigen::Vector2<T> >& v,
    const Eigen::Vector2<T>& a,
    const Eigen::Vector2<T>& b,
    std::vector<Eigen::Vector2<T> >& hull)
{
  if (v.empty()) {
    return;
  }

  Eigen::Vector2<T> f = v[_getFarthestInd(a, b, v)];

  // Collect points to the left of segment (a, f)
  std::vector<Eigen::Vector2<T> > left;
  for (auto p : v) {
    if (_ccw(a, f, p) > 0) {
      left.push_back(p);
    }
  }
  quickHull2D(left, a, f, hull);

  // Add f to the hull
  hull.push_back(f);

  // Collect points to the left of segment (f, b)
  std::vector<Eigen::Vector2<T> > right;
  for (auto p : v) {
    if (_ccw(f, b, p) > 0) {
      right.push_back(p);
    }
  }
  quickHull2D(right, f, b, hull);
}

template<typename T>
/*!
 * \brief quickHull2D Quick Hull 2D algorithm https://en.wikipedia.org/wiki/quickHull.
 * \param v
 * \return
 */
std::vector<Eigen::Vector2<T> > quickHull2D(
    const std::vector<Eigen::Vector2<T> >& v)
{
  // Start with the leftmost and rightmost points.
  Eigen::Vector2<T> a = *min_element(v.begin(), v.end(), _isLeftOf<T>);
  Eigen::Vector2<T> b = *max_element(v.begin(), v.end(), _isLeftOf<T>);

  // Split the points on either side of segment (a, b)
  std::vector<Eigen::Vector2<T> > left, right;
  for (auto p : v) {
    _ccw(a, b, p) > 0 ? left.push_back(p) : right.push_back(p);
  }

  std::vector<Eigen::Vector2<T> > hull;
  // Be careful to add points to the hull
  // in the correct order. Add our leftmost point.
  hull.push_back(a);

  // Add hull points from the left (top)
  quickHull2D(left, a, b, hull);

  // Add our rightmost point
  hull.push_back(b);

  // Add hull points from the right (bottom)
  quickHull2D(right, b, a, hull);

  hull.push_back(hull[0]);

  return hull;
}

template<typename T>
/*!
 * \brief quickHull2D Recursive call of the quickHull2D algorithm.
 * \param v
 * \param a
 * \param b
 * \param hull
 * \param ih
 */
void quickHull2D(
    const Eigen::MatrixX2<T>& v,
    const Eigen::Vector2<T>& a,
    const Eigen::Vector2<T>& b,
    Eigen::MatrixX2<T>& hull,
    ptrdiff_t& ih)
{
  if (v.size() == 0) {
    return;
  }

  Eigen::Vector2<T> f = v.row(_getFarthestInd(a, b, v));

  // Collect points to the left of segment (a, f)
  Eigen::MatrixX2<T> left(v.rows(), 2);
  ptrdiff_t il = 0;
  for (ptrdiff_t i = 0; i < v.rows(); i++){
    if (_ccw(a, f, v.row(i)) > 0) {
      left.row(il) = v.row(i);
      il++;
    }
  }
  left.conservativeResize(il, 2);
  quickHull2D(left, a, f, hull, ih);

  // Add f to the hull
  hull.row(ih) = f;
  ih++;

  // Collect points to the left of segment (f, b)
  Eigen::MatrixX2<T> right(v.rows(), 2);
  ptrdiff_t ir = 0;
  for (ptrdiff_t i = 0; i < v.rows(); i++){
    if (_ccw(f, b, v.row(i)) > 0) {
      right.row(ir) = v.row(i);
      ir++;
    }
  }
  right.conservativeResize(ir, 2);
  quickHull2D(right, f, b, hull, ih);
};

template<typename T>
/*!
 * \brief quickHull2D Quick Hull 2D algorithm https://en.wikipedia.org/wiki/quickHull.
 * \param v
 * \return
 */
Eigen::MatrixX2<T> quickHull2D(
    const Eigen::MatrixX2<T>& v)
{
  // initialize original index locations
  Eigen::VectorX<ptrdiff_t> idx =
      Eigen::ArrayX<ptrdiff_t>::LinSpaced(
        v.rows(), 0, v.rows()-1);

  // Start with the leftmost and rightmost points.
//  std::function<bool(const ptrdiff_t &, const ptrdiff_t &)> _isLeftOf_fun =
//      [&v](
//      const ptrdiff_t& row1,
//      const ptrdiff_t& row2)->bool
//  {
//    return (v(row1,0) < v(row2,0) || (v(row1,0) == v(row2,0) && v(row1,1) < v(row2,1)));
//  };

//  ptrdiff_t ia = *min_element(idx.begin(), idx.end(), _isLeftOf_fun);
//  ptrdiff_t ib = *max_element(idx.begin(), idx.end(), _isLeftOf_fun);

//  Eigen::Vector2<T> a = v.row(ia);
//  Eigen::Vector2<T> b = v.row(ib);

  // Start with the leftmost and rightmost points.
  // Using Iterators
  // https://stackoverflow.com/questions/65573094/stl-iterators-with-eigen-matrices/65582213#65582213
  Eigen::Vector2<T> a = *std::min_element(
      v.rowwise().begin(), v.rowwise().end(),
      _isLeftOf<T>);

  Eigen::Vector2<T> b = *std::max_element(
      v.rowwise().begin(), v.rowwise().end(),
      _isLeftOf<T>);

  // Split the points on either side of segment (a, b)
  Eigen::MatrixX2<T> left(v.rows(), 2);
  Eigen::MatrixX2<T> right(v.rows(), 2);
  ptrdiff_t il = 0;
  ptrdiff_t ir = 0;
  for (ptrdiff_t i = 0; i < v.rows(); i++){
    if (_ccw(a, b, v.row(i)) > 0){
      left.row(il) = v.row(i);
      il++;
    } else {
      right.row(ir) = v.row(i);
      ir++;
    }
  }
  left.conservativeResize(il, 2);
  right.conservativeResize(ir, 2);


  Eigen::MatrixX2<T> hull(v.rows(), 2);
  ptrdiff_t ih = 0;

  // Be careful to add points to the hull
  // in the correct order. Add our leftmost point.
  hull.row(ih) = a;
  ih++;

  // Add hull points from the left (top)
  quickHull2D(left, a, b, hull, ih);

  // Add our rightmost point
  hull.row(ih) = b;
  ih++;

  // Add hull points from the right (bottom)
  quickHull2D(right, b, a, hull, ih);

  // close hull
  hull.row(ih) = hull.row(0);
  ih++;

  hull.conservativeResize(ih, 2);

  return hull;
}

}


#endif // EASYHULL_H
