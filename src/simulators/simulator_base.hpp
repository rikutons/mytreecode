#pragma once
#include <fstream>
#include "../bhnode.hpp"
#include "../argument_interpreter.hpp"

/*
  抽象クラス。
  dt毎に実行されるStep関数を継承先で実装すること。
*/
class SimulatorBase
{
  void OpenFiles(string);
  void InitParticlesOnSphereShape();
  void InitParticlesOnAlignedShape();
  int AskSeeParticles();
  void PrintMassHeart(Particle *particles, int n);
  void PrintEnergies(Particle *particles, int n);
  virtual void Step() = 0;
  void ReadParticles();
  double true_energy = 0;

protected:
  virtual void Output();
  /* data */
  double energy;
  int n; // 粒子の総数
  double t;
  double power_index;
  double rsize;
  BHNode *nodes;
  int nnodes;
  double eps_square = 0.1 * 0.1;
  double theta_square = 0.5; // [rad^2]
  double dt;
  double end_time;
  int step = 0;
  Particle *particles;
  ifstream input_file;
  ofstream output_file;
  ofstream status_output_file;

public:
  double error;
  SimulatorBase(ArgumentInterpreter, bool allocate_memory = true);
  void Simulate();
};