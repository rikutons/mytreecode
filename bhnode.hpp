#pragma once
#include "vector3.hpp"
#include "particle.hpp"

class BHNode
{
  Vector3 centerPos;
  double size;
  BHNode *child[8];
  Particle *pfirst;
  int nparticle;
  Vector3 pos;
  double mass;

public:
  BHNode()
  {
    for (int i = 0; i < 8; i++)
      child[i] = NULL;
    pfirst = NULL;
  }

  void AssignRoot(Vector3, double, Particle*, int);
  void CreateTreeRecursive(BHNode* &, int &);
  void AssignChild(int, BHNode* &, int &);
  void DumpTree(int indent = 0);
  void CalcPhysicalQuantity();
};