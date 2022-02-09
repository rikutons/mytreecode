#include <vector>
#include <bitset>
#include "tree_simulator.hpp"
#include "../common.h"

TreeSimulator::TreeSimulator(ArgumentInterpreter arguments) : SimulatorBase(arguments)
{
  nnodes = n * 2 + 100;
  nodes = new BHNode[nnodes];
}

void TreeSimulator::Step() 
{
  #pragma omp parallel for schedule(dynamic)
  for (int i = 0; i < n; i++)
    particles[i].Predict(dt);
  CalculateGravity();
  #pragma omp parallel for schedule(dynamic)
  for (int i = 0; i < n; i++)
    particles[i].Correct(dt);
}

void TreeSimulator::ClearAccAndPhi()
{
  Particle *p = particles;
#pragma omp parallel for schedule(dynamic)
  for (int i = 0; i < n; i++)
  {
    particles[i].acceralation = Vector3();
    particles[i].phi = 0;
  }
}

/*
  最も外側にある粒子を内包するような最小の立方体の大きさ( ÷ 2)を返す。
  2の整数乗の値のみを持つ。
*/
double TreeSimulator::CalculateSize()
{
  double rsize = 1;
#pragma omp parallel for schedule(dynamic)
  for (int i = 0; i < n; i++)
  {
    Vector3 ppos = particles[i].pos;
    for (int k = 0; k < 3; k++)
      if (fabs(ppos[k]) > rsize)
        rsize *= 2;
  }
  return rsize;
}

void TreeSimulator::CalculateGravity()
{
  double rsize = CalculateSize();
  nodes->AssignRoot(Vector3(), rsize * 2, particles, n);
  int heap_remainder = nnodes - 1;
  BHNode *btmp = nodes + 1;
  nodes->CreateTreeRecursive(btmp, heap_remainder);
  //    bn->dump();
  //    PRL(bn->sanity_check());
  nodes->CalcPhysicalQuantity();
  ClearAccAndPhi();

  vector<Particle*> groupArrays[GROUP_NUM];
  for (int i = 0; i < n; i++) {
    groupArrays[particles[i].CalcGroupIndex(nodes[0].centerPos, nodes[0].size)].emplace_back(&particles[i]);
  }

#pragma omp parallel for schedule(dynamic)
  for (int i = 0; i < GROUP_NUM; i++) {
    auto LET = nodes[0].MakeLET(Particle::CalcGroupCenter(i, nodes[0].size), eps_square, theta_square);
    // cout << "Group: " << std::bitset<6>(i) << ", Particle: " << groupArrays[i].size() << ", LET: " << LET.size() << endl;
    for (auto p : groupArrays[i])
    {
      for (auto node : LET)
      {
        Vector3 dx = node->pos - p->pos;
        node->AccumulateForceFromPoint(dx, dx * dx, eps_square, *p);
      }
    }
  }
  // int poop; cin >> poop;
  // nodes[0].CalcGravityUsingTree(particles[i], eps_square, theta_square);
  //	PR(i); PR(p->pos);PR(p->phi); PRL(p->acc);
}