#include "tree_simulator.hpp"

void TreeSimulator::Step() 
{
  Integrate(nodes, nnodes, particles, n, eps_square, theta_square, dt);
}

void TreeSimulator::Integrate(BHNode *bn, int nnodes, Particle *particles, int n, double eps_square, double theta, double dt) 
{
  for (int i = 0; i < n; i++)
    particles[i].Predict(dt);
  CalculateGravity(bn, nnodes, particles, n, eps_square, theta);
  for (int i = 0; i < n; i++)
    particles[i].Correct(dt);
  
}

void TreeSimulator::ClearAccAndPhi(Particle *parray, int n)
{
  Particle *p = parray;
  for (int i = 0; i < n; i++)
  {
    p->acceralation = Vector3();
    p->phi = 0;
    p++;
  }
}

/*
  最も外側にある粒子を内包するような最小の立方体の大きさ( ÷ 2)を返す。
  2の整数乗の値のみを持つ。
*/
double TreeSimulator::CalculateSize(Particle *p, int n)
{
  double rsize = 1;
  for (int i = 0; i < n; i++)
  {
    Vector3 ppos = p[i].pos;
    for (int k = 0; k < 3; k++)
      if (fabs(ppos[k]) > rsize)
        rsize *= 2;
  }
  return rsize;
}

void TreeSimulator::CalculateGravity(BHNode *bn, int nnodes, Particle *particles, int n, double eps_square, double theta)
{
  double rsize = CalculateSize(particles, n);
  bn->AssignRoot(Vector3(), rsize * 2, particles, n);
  int heap_remainder = nnodes - 1;
  BHNode *btmp = bn + 1;
  bn->CreateTreeRecursive(btmp, heap_remainder);
  //    bn->dump();
  //    PRL(bn->sanity_check());
  bn->CalcPhysicalQuantity();
  ClearAccAndPhi(particles, n);

  for (int i = 0; i < n; i++)
    bn->CalcGravityUsingTree(particles[i], eps_square, theta);
    //	PR(i); PR(p->pos);PR(p->phi); PRL(p->acc);
}