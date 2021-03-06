#pragma once
#include <iostream>
#include <cmath>
using namespace std;
struct Vector3
{
  double x, y, z;
  Vector3() : x(0), y(0), z(0) {}
  Vector3(double x, double y, double z) : x(x), y(y), z(z) {}

  Vector3 operator+(Vector3 obj)
  {
    return Vector3(x + obj.x, y + obj.y, z + obj.z);
  }
  Vector3 operator+=(Vector3 obj)
  {
    return *this = *this + obj;
  }
  Vector3 operator-(Vector3 obj)
  {
    return Vector3(x - obj.x, y - obj.y, z - obj.z);
  }
  Vector3 operator-=(Vector3 obj)
  {
    return *this = *this - obj;
  }
  Vector3 operator*(double obj)
  {
    return Vector3(obj * x, obj * y, obj * z);
  }
  double operator*(Vector3 obj)
  {
    return x * obj.x + y * obj.y + z * obj.z;
  }
  Vector3 operator*=(double obj)
  {
    return *this = *this * obj;
  }
  Vector3 operator/(double obj)
  {
    return Vector3(x / obj, y / obj, z / obj);
  }
  Vector3 operator/=(double obj)
  {
    return *this = *this / obj;
  }
  double &operator[](int obj)
  {
    if (obj == 0) return x;
    if (obj == 1) return y;
    if (obj == 2) return z;
    // error
    cerr << "out of index on vector3 array access" << endl;
    exit(1);
  }

  double magnitude()
  {
    return sqrt(x * x + y * y + z * z);
  }
  double max()
  {
    return std::max(std::max(x, y), z);
  }
};

ostream &operator<<(ostream &, const Vector3);
Vector3 operator*(double, Vector3);
string to_string(Vector3);