#include <iostream>
#include <cmath>
#include "vector3.hpp"
using namespace std;

ostream& operator<<(ostream& os, const Vector3 v){
  return os << v.x << ", " << v.y << ", " << v.z;
}

Vector3 operator*(double lhs, Vector3 rhs){
  return rhs * lhs;
}

Vector3 operator/(Vector3 lhs, double rhs){
  return Vector3(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
}