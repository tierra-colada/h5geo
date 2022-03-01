#ifndef H5POINT_H
#define H5POINT_H

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
  void setY(const double& y) { p[1] = y; }
  void setZ(const double& z) { p[2] = z; }

  double& x() { return p[0]; }
  double& y() { return p[1]; }
  double& z() { return p[2]; }

  void setName(const std::string& name) { this->name = name; }
  std::string getName() { return this->name; }

  double p[3];
  std::string name;
} Point;

typedef std::vector<h5geo::Point> PointArray;

inline h5gt::CompoundType compound_Point() {
  h5gt::CompoundType t(
        {
          {"x", h5gt::AtomicType<double>{}, offsetof(Point, p[0])},
          {"y", h5gt::AtomicType<double>{}, offsetof(Point, p[1])},
          {"z", h5gt::AtomicType<double>{}, offsetof(Point, p[2])},
          {"name", h5gt::AtomicType<std::string>{}, offsetof(Point, name)}
        }, sizeof (Point));

  return t;
}


} // h5geo


H5GT_REGISTER_TYPE(h5geo::Point, h5geo::compound_Point);

#endif // H5CORE_TYPES_H
