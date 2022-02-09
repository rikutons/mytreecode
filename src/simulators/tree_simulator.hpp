#include "simulator_base.hpp"

class TreeSimulator : public SimulatorBase
{
  int maxDepth = 5;
  void Step();
  void CalculateGravity();
  double CalculateSize();
  void ClearAccAndPhi();

public:
  TreeSimulator(ArgumentInterpreter arguments);
};