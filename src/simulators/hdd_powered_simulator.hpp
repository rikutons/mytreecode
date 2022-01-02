#include <string>
#include "simulator_base.hpp"
#include "../sub_area.hpp"

class HDDPoweredSimulator : public SimulatorBase
{
  int div_num; // 分割数 8以外考えたくない！
  Particle *next_particles;
  SubArea *sub_areas;
  double energy;

private:
  int GetIndex(Vector3 pos);
  void Step();
  void Output() override; // オーバーライド
public:
  HDDPoweredSimulator(ArgumentInterpreter arguments);
};