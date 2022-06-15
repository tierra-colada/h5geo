#ifndef H5POINT_H
#define H5POINT_H

#include <h5gt/H5DataType.hpp>

#include "h5geo_export.h"

namespace h5geo {

// Compound types must be trivial (POD)
// NO constructors, NO destructors, NO virtual member,
// NO std::string as a var member
// check it using 'std::is_trivial<h5geo::Point3>::value'


//------------------------POINT1------------------------//

/// X maybe length or time (depending on Domain)
typedef struct Point1{
  void setX(const double& x) { p[0] = x; }

  double& getX() { return p[0]; }

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

  double p[1];
  // needs to be public to calculate offset
  char name[H5GEO_CHAR_ARRAY_SIZE];
} Point1;

typedef std::vector<h5geo::Point1> Point1Array;


//------------------------POINT2------------------------//

/// X, Y are always length
typedef struct Point2{
  void setX(const double& x) { p[0] = x; }
  void setY(const double& y) { p[1] = y; }

  double& getX() { return p[0]; }
  double& getY() { return p[1]; }

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

  // members must be public to calculate offset and to provide wider API
  double p[2];
  char name[H5GEO_CHAR_ARRAY_SIZE];
} Point2;

typedef std::vector<h5geo::Point2> Point2Array;


//------------------------POINT3------------------------//

/// X, Y are always length, Z either length or time (depending on Domain)
typedef struct Point3{
  void setX(const double& x) { p[0] = x; }
  void setY(const double& y) { p[1] = y; }
  void setZ(const double& z) { p[2] = z; }

  double& getX() { return p[0]; }
  double& getY() { return p[1]; }
  double& getZ() { return p[2]; }

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
  char name[H5GEO_CHAR_ARRAY_SIZE];
} Point3;

typedef std::vector<h5geo::Point3> Point3Array;

//------------------------POINT4------------------------//

/// X, Y are always length, Z either length or time (depending on Domain),
/// VAL is in data units
typedef struct Point4{
  void setX(const double& x) { p[0] = x; }
  void setY(const double& y) { p[1] = y; }
  void setZ(const double& z) { p[2] = z; }
  void setVal(const double& val) { p[3] = val; }

  double& getX() { return p[0]; }
  double& getY() { return p[1]; }
  double& getZ() { return p[2]; }
  double& getVal() { return p[3]; }

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

  double p[4];
  char name[H5GEO_CHAR_ARRAY_SIZE];
} Point4;

typedef std::vector<h5geo::Point4> Point4Array;



inline h5gt::CompoundType compound_Point1() {
  h5gt::CompoundType t(
        {
          {"x", h5gt::AtomicType<double>{}, offsetof(Point1, p[0])},
          {"name", h5gt::AtomicType<h5gt::FixedLenStringArray<H5GEO_CHAR_ARRAY_SIZE>>{}, offsetof(Point1, name)}
        }, sizeof (Point1));

  return t;
}

inline h5gt::CompoundType compound_Point2() {
  h5gt::CompoundType t(
        {
          {"x", h5gt::AtomicType<double>{}, offsetof(Point2, p[0])},
          {"y", h5gt::AtomicType<double>{}, offsetof(Point2, p[1])},
          {"name", h5gt::AtomicType<h5gt::FixedLenStringArray<H5GEO_CHAR_ARRAY_SIZE>>{}, offsetof(Point2, name)}
        }, sizeof (Point2));

  return t;
}

inline h5gt::CompoundType compound_Point3() {
  h5gt::CompoundType t(
        {
          {"x", h5gt::AtomicType<double>{}, offsetof(Point3, p[0])},
          {"y", h5gt::AtomicType<double>{}, offsetof(Point3, p[1])},
          {"z", h5gt::AtomicType<double>{}, offsetof(Point3, p[2])},
          {"name", h5gt::AtomicType<h5gt::FixedLenStringArray<H5GEO_CHAR_ARRAY_SIZE>>{}, offsetof(Point3, name)}
        }, sizeof (Point3));

  return t;
}

inline h5gt::CompoundType compound_Point4() {
  h5gt::CompoundType t(
        {
          {"x", h5gt::AtomicType<double>{}, offsetof(Point4, p[0])},
          {"y", h5gt::AtomicType<double>{}, offsetof(Point4, p[1])},
          {"z", h5gt::AtomicType<double>{}, offsetof(Point4, p[2])},
          {"val", h5gt::AtomicType<double>{}, offsetof(Point4, p[3])},
          {"name", h5gt::AtomicType<h5gt::FixedLenStringArray<H5GEO_CHAR_ARRAY_SIZE>>{}, offsetof(Point4, name)}
        }, sizeof (Point4));

  return t;
}


} // h5geo


H5GT_REGISTER_TYPE(h5geo::Point1, h5geo::compound_Point1);
H5GT_REGISTER_TYPE(h5geo::Point2, h5geo::compound_Point2);
H5GT_REGISTER_TYPE(h5geo::Point3, h5geo::compound_Point3);
H5GT_REGISTER_TYPE(h5geo::Point4, h5geo::compound_Point4);

#endif // H5CORE_TYPES_H
