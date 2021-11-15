#include <iostream>
#include <random>
#include "common.h"
#include "particle.hpp"
#include "bhnode.hpp"

using namespace std;

void accumulate_mutual_gravity(Particle & p1,
			       Particle & p2,
			       double eps2)
{
    Vector3 dx = p1.pos-p2.pos;
    double r2inv = 1/(dx*dx+eps2);
    double rinv  = sqrt(r2inv);
    double r3inv = r2inv*rinv;
    p1.phi -= p2.mass*rinv;
    p2.phi -= p1.mass*rinv;
    p1.acceralation -= p2.mass*r3inv*dx;
    p2.acceralation += p1.mass*r3inv*dx;
}

void clear_acc_and_phi(Particle *parray, int n)
{
  Particle *p = parray;
  for (int i = 0; i < n; i++) {
    p->acceralation = Vector3();
    p->phi = 0;
    p++;
  }
}

void calculate_uncorrected_gravity_direct(Particle * parray, int n, double eps_square)
{
    clear_acc_and_phi(parray, n);
    int i, j;
    Particle * pi;
    Particle * pj;
    for (i = 0, pi = parray; i < n - 1; i++, pi++)
    {
      for (j = i + 1, pj = pi + 1; j < n; j++, pj++)
      {
        accumulate_mutual_gravity(*pi, *pj, eps_square);
      }
    }
}


void create_uniform_sphere(Particle* particles, int n, double r){
  random_device rnd;
  // mt19937 mt(rnd()); 
  mt19937 mt(0); 
  uniform_real_distribution<> rand_real(-1, 1);
  for (int i = 0; i < n; i++){
    Vector3 p(1,1,1);
    while (p * p > 1)
      for (int i = 0; i < 3; i++)
        p[i] = rand_real(mt);
    particles[i].pos = r * p;
    particles[i].mass = 1.0 / n;
  }
}

int main()
{
  int n;
  cerr << "Enter n:";
  cin >> n;
  Particle *particles = new Particle[n];
  double rsize = 1.0;
  create_uniform_sphere(particles, n, rsize);

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
  nodes->CalcPhysicalQuantity();
  // nodes->DumpTree();
  double eps_square = 0.1 * 0.1;
  double theta_square = 0.5; // [rad^2]
  // calculate_uncorrected_gravity_direct(particles, n, eps_square);
  for (int i = 0; i < n; i++)
  {
    auto p = particles[i];
    nodes->CalcGravityUsingTree(p, eps_square, theta_square);
    PR(i); PR(p.pos);PR(p.phi); PRL(p.acceralation);
  }

  return 0;
}
