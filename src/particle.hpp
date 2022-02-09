#pragma once
#include "vector3.hpp"

class Particle
{
public:
  Vector3 pos;
  Vector3 velocity;
  Vector3 acceralation;
  double mass;
  double phi; // 位置エネルギー
  Particle *next;
  int index;
  Particle() {}
  Particle(Vector3 pos, double mass) : pos(pos), mass(mass) {}

  void Predict(double dt)
  {
    pos += dt * velocity + dt * dt * acceralation / 2;
    velocity += dt * acceralation / 2;
  }
  void Correct(double dt)
  {
    velocity += dt * acceralation / 2;
  }
  double CalcKineticEnergy()
  {
    return mass * velocity * velocity / 2;
  }
  double CalcPotentialEnergy()
  {
    return mass * phi / 2;
  }
  int CalcGroupIndex(Vector3 centerPos, double r) { // 64分割前提 centerPosから一辺がrの立方体を分割した際何番目に位置するかを返す
    centerPos = pos - centerPos;
    centerPos /= (r / 4);
    int ret = 0;
    for (int i = 0; i < 3; i++) {
      int a = centerPos[i] + 2;
      if(a < 0)
        a = 0;
      if(a > 3)
        a = 3;
      ret += a << (i * 2);
    }
    return ret;
  }
  string ToDataString()
  {
    return to_string(index) + " " + to_string(mass) + " " + to_string(pos) + " " + to_string(velocity) + " " + to_string(acceralation);
  }
  static Vector3 CalcGroupCenter(int groupIndex, double r) { // 64分割前提 groupIndexからグループの中心が一辺rの立方体のどこに位置するかを返す
    Vector3 ret;
    for (int i = 0; i < 3; i++)
    {
      int x = groupIndex & 0b11;
      ret[i] = (x - 1.5) * r / 4;
      groupIndex >>= 2;
    }
    return ret;
  }
};