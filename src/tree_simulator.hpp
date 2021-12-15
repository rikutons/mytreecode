#include "simulator_base.hpp"

class TreeSimulator : public SimulatorBase
{
  void Step();
  void Integrate(BHNode *bn, int nnodes, Particle *particles, int n, double eps_square, double theta, double dt);
  void CalculateGravity(BHNode *bn, int nnodes, Particle *particles, int n, double eps_square, double theta);
  double CalculateSize(Particle *p, int n);
  void ClearAccAndPhi(Particle *parray, int n);
};