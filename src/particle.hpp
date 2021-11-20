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
  Particle() {}

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
};