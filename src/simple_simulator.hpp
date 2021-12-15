#include "simulator_base.hpp"

class SimpleSimulator : public SimulatorBase
{
  void Step();
  void ClearAccAndPhi(Particle *parray, int n);
  void AccumulateMutualGravity(Particle &p1, Particle &p2, double eps_square);
};