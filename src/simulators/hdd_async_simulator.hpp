#include <string>
#include <omp.h>
#include "simulator_base.hpp"
#include "../sub_area.hpp"

class HDDAsyncSimulator : public SimulatorBase
{
  int div_num; // 分割数 8以外考えたくない！
  Particle *next_particles;
  SubArea *sub_areas;
  int particle_maxnum;

private:
  void SwapParticles(Particle **, Particle **);
  int GetIndex(Vector3 pos);
  void Step();
  void Write(int i);
  void Output() override; // オーバーライド
public:
  HDDAsyncSimulator(ArgumentInterpreter arguments);
};