#include "simple_simulator.hpp"
void SimpleSimulator::ClearAccAndPhi(Particle *parray, int n)
{
  Particle *p = parray;
  for (int i = 0; i < n; i++)
  {
    p->acceralation = Vector3();
    p->phi = 0;
    p++;
  }
}

void SimpleSimulator::AccumulateMutualGravity(Particle &p1, Particle &p2, double eps_square)
{
  Vector3 dx = p1.pos - p2.pos;
  double r = sqrt(dx * dx + eps_square);
  p1.phi -= p2.mass / r;
  p2.phi -= p1.mass / r;
  p1.acceralation -= p2.mass * dx / powl(r, 3);
  p2.acceralation += p1.mass * dx / powl(r, 3);
}

void SimpleSimulator::Step() 
{
  for (int i = 0; i < n; i++)
    particles[i].Predict(dt);
  ClearAccAndPhi(particles, n);
  int i, j;
  Particle *pi;
  Particle *pj;
  for (i = 0, pi = particles; i < n - 1; i++, pi++)
    for (j = i + 1, pj = pi + 1; j < n; j++, pj++)
      AccumulateMutualGravity(*pi, *pj, eps_square);

  for (int i = 0; i < n; i++)
    particles[i].Correct(dt);
}
