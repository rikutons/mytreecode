#pragma once
#include <fstream>
#include "bhnode.hpp"

/*
  抽象クラス。
  dt毎に実行されるStep関数を継承先で実装すること。
*/
class SimulatorBase
{
private:
  void OpenFiles();
  void InitParticlesOnSphereShape();
  int AskSeeParticles();
  void Output(double t);
  void PrintMassHeart(Particle *particles, int n);
  void PrintEnergies(Particle *particles, int n);
  virtual void Step() = 0;

protected:
  /* data */
  int n; // 粒子の総数
  double power_index;
  double rsize;
  BHNode *nodes;
  int nnodes;
  double eps_square = 0.1 * 0.1;
  double theta_square = 0.5; // [rad^2]
  double dt = 0.01;
  double end_time = 1;
  int step = 0;
  Particle *particles = new Particle[n];
  ofstream output_file;
  ofstream status_output_file;

public:
  SimulatorBase();
  void Simulate();
};