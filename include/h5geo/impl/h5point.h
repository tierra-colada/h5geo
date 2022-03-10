#ifndef H5POINT_H
#define H5POINT_H

#include <h5gt/H5DataType.hpp>

#include "h5geo_export.h"

namespace h5geo {

// Compound types must be trivial (POD)
// NO constructors, NO destructors, NO virtual member,
// NO std::string as a var member
// check it using 'std::is_trivial<h5geo::Point>::value'
typedef struct Point{
  void setX(const double& x) { p[0] = x; }
  void setY(const double& y) { p[1] = y; }
  void setZ(const double& z) { p[2] = z; }

  double& x() { return p[0]; }
  double& y() { return p[1]; }
  double& z() { return p[2]; }

  void setName(const std::string& s){
    size_t nChar2copy = std::min(s.size(), size_t(H5GEO_CHAR_ARRAY_SIZE - 1));
    if (nChar2copy < 1){
      this->name[0] = '\0';
    } else {
       s.copy(this->name, nChar2copy);
      this->name[nChar2copy] = '\0';
    }
  }

  std::string getName() { return this->name; }

  double p[3];
  // needs to be public to calculate offset
  char name[H5GEO_CHAR_ARRAY_SIZE];
} Point;

typedef std::vector<h5geo::Point> PointArray;

inline h5gt::CompoundType compound_Point() {
  h5gt::CompoundType t(
        {
          {"x", h5gt::AtomicType<double>{}, offsetof(Point, p[0])},
          {"y", h5gt::AtomicType<double>{}, offsetof(Point, p[1])},
          {"z", h5gt::AtomicType<double>{}, offsetof(Point, p[2])},
          {"name", h5gt::AtomicType<h5gt::FixedLenStringArray<H5GEO_CHAR_ARRAY_SIZE>>{}, offsetof(Point, name)}
        }, sizeof (Point));

  return t;
}


} // h5geo


H5GT_REGISTER_TYPE(h5geo::Point, h5geo::compound_Point);

#endif // H5CORE_TYPES_H
