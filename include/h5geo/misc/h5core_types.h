#ifndef H5CORE_TYPES_H
#define H5CORE_TYPES_H

#include <h5gt/H5DataType.hpp>

#include "h5geo_export.h"

namespace h5geo {

typedef struct Point{
  Point() {};
  Point(const double& x,
        const double& y,
        const double& z,
        const std::string& name = "")
  {
    this->p[0] = x;
    this->p[1] = y;
    this->p[2] = z;
    this->setName(name);
  }

  void setX(const double& x) { p[0] = x; }
  void setY(const double& y) { p[0] = y; }
  void setZ(const double& z) { p[0] = z; }

  double& x() { return p[0]; }
  double& y() { return p[1]; }
  double& z() { return p[2]; }

  void setName(const std::string& name) {
    strncpy(this->name, name.c_str(), sizeof(this->name));
    this->name[sizeof(this->name) - 1] = 0;
  }
  std::string getName() { return std::string(name); }

  double p[3];
  char name[H5GEO_CHAR_ARR_SIZE];
} Point;

typedef std::vector<h5geo::Point> PointArray;

inline h5gt::CompoundType compound_Point() {
  h5gt::CompoundType t(
        {
          {"x", h5gt::AtomicType<double>{}},
          {"y", h5gt::AtomicType<double>{}},
          {"z", h5gt::AtomicType<double>{}},
          {"name", h5gt::AtomicType<h5gt::FixedLenStringArray<H5GEO_CHAR_ARR_SIZE>>{}}
        });

  return t;
}


} // h5geo


H5GT_REGISTER_TYPE(h5geo::Point, h5geo::compound_Point);

#endif // H5CORE_TYPES_H
