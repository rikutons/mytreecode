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
  string ToDataString()
  {
    return to_string(index) + " " + to_string(mass) + " " + to_string(pos) + " " + to_string(velocity) + " " + to_string(acceralation);
  }
};