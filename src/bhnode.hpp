#pragma once
#include <vector>
#include "vector3.hpp"
#include "particle.hpp"

struct BHNode
{
  Vector3 centerPos;
  double size;
  BHNode *child[8];
  Particle *pfirst;
  int nparticle;
  Vector3 pos; // 重心の位置
  double mass;
  static long long int calc_count;
  BHNode()
  {
    for (int i = 0; i < 8; i++)
      child[i] = NULL;
    pfirst = NULL;
  }

  void AssignRoot(Vector3, double, Particle *, int);
  void AssignRootWithLET(Vector3, double, Particle *, vector<Particle*>, int);
  void CreateTreeRecursive(BHNode *&, int &);
  void AssignChild(int, BHNode *&, int &);
  void DumpTree(int indent = 0);
  void CalcPhysicalQuantity();
  void CalcGravityUsingTree(Particle &, double, double);
  void AccumulateForceFromPoint(Vector3, double, double, Particle &);
  void AccumulateForceFromTree(Particle &, double, double);
};