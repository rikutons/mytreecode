#pragma once
#include "vector3.hpp"

class Particle{
  public:
  Vector3 pos;
  Vector3 velocity;
  Vector3 acceralation;
  double mass;
  double phi; // ?
  Particle *next;
  Particle() {}

  void predict(double dt){
    double dt2 = dt*dt*0.5;
    pos = pos + dt * velocity + dt2 * acceralation;
    velocity += (dt * 0.5) * acceralation;
  }
  void correct(double dt){
    velocity +=  (dt*0.5)*acceralation;
  }
  // Particle operator[](int index) &{
  //   Particle* p = this;
  //   for (int i = 0; i < index; i++)
  //   {
  //     p = p->next;
  //     cout << i << endl;
  //   }
  //   return *p;
  // }
};