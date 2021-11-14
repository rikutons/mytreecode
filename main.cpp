#include <iostream>
#include <random>
#include "common.h"
#include "particle.hpp"
#include "bhnode.hpp"

using namespace std;

void create_uniform_sphere(Particle* particles, int n, double r){
  random_device rnd;
  mt19937 mt(rnd()); 
  uniform_real_distribution<> rand_real(-1, 1);
  for (int i = 0; i < n; i++){
    Vector3 p(1,1,1);
    while (p * p > 1)
      for (int i = 0; i < 3; i++)
        p[i] = rand_real(mt);
    particles[i].pos = r * p;
  }
}

int main()
{
  int n;
  cerr << "Enter n:";
  cin >> n;
  Particle *particles = new Particle[n], *now;
  double rsize = 10.0;
  create_uniform_sphere(particles, n, rsize);
  now = particles;

  for (int i = 0; i < n; i++)
  {
    PRC(i);
    PRL(particles[i].pos);
  }
  BHNode *nodes = NULL;
  int nnodes = n * 2;
  nodes = new BHNode[nnodes];
  nodes->AssignRoot(Vector3(), rsize * 2, particles, n);
  int heap_remainder = nnodes - 1;
  BHNode *btmp = nodes + 1;
  nodes->CreateTreeRecursive(btmp, heap_remainder);
  // PRL(root->sanity_check());
  nodes->DumpTree();
  return 0;
}
