#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include "common.h"
#include "particle.hpp"
#include "bhnode.hpp"

using namespace std;

void accumulate_mutual_gravity(Particle & p1, Particle & p2, double eps_square)
{
    Vector3 dx = p1.pos - p2.pos;
    double r = sqrt(dx * dx + eps_square);
    p1.phi -= p2.mass / r;
    p2.phi -= p1.mass / r;
    p1.acceralation -= p2.mass * dx / powl(r, 3);
    p2.acceralation += p1.mass * dx / powl(r, 3);
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


void CreateUniformSphere(Particle* particles, int n, double power_index, double r){
  random_device rnd;
  // mt19937 mt(rnd()); 
  mt19937 mt(0); 
  uniform_real_distribution<> rand_real(-1, 1);
  for (int i = 0; i < n; i++){
    Vector3 p(1,1,1);
    while (p * p > 1)
      for (int i = 0; i < 3; i++)
        p[i] = rand_real(mt);
    p *= powl(x * x, 3.0 / (power_index + 3) - 1);
    particles[i].pos = r * p;
    particles[i].mass = 1.0 / n;
  }
}

double CalculateSize(Particle * p, int n)
{
  double rsize = 1;
  for (int i = 0; i < n; i++)
  {
    Vector3 ppos = p[i].pos;
    for (int k = 0; k < 3; k++)
    {
      if (fabs(ppos[k]) > rsize)
      {
        rsize *= 2;
      }
    }
  }
  return rsize;
}

void CalculateGravity(BHNode *bn, int nnodes, Particle *pp, int n, double eps_square, double theta)
{
  double rsize = CalculateSize(pp, n);
  bn->AssignRoot(Vector3(), rsize * 2, pp, n);
  int heap_remainder = nnodes - 1;
  BHNode *btmp = bn + 1;
  bn->CreateTreeRecursive(btmp, heap_remainder);
  //    bn->dump();
  //    PRL(bn->sanity_check());
  bn->CalcPhysicalQuantity();
  clear_acc_and_phi(pp, n);

  Particle *p = pp;
  for (int i = 0; i < n; i++)
  {
    bn->CalcGravityUsingTree(*p, eps_square, theta);
    //	PR(i); PR(p->pos);PR(p->phi); PRL(p->acc);
    p++;
  }
}

void Integrate(BHNode * bn, int nnodes, Particle * particles, int n, double eps_square, double theta, double dt)
{
  for (int i = 0; i < n; i++) particles[i].Predict(dt);
  CalculateGravity(bn, nnodes, particles, n, eps_square, theta);
  for (int i = 0; i < n; i++) particles[i].Correct(dt);
}

int main()
{
  int n;
  cerr << "Enter n(number of particles): ";
  cin >> n;
  double power_index;
  cerr << "Enter power index: ";
  cin >> power_index;
  Particle *particles = new Particle[n];
  double rsize = 1.0;
  CreateUniformSphere(particles, n, power_index, rsize);

  {
    int a;
    cerr << "Do u wanna see particle's data? (yes:1, no:0): "
    cin >> a;
    if(a == 1)
      for (int i = 0; i < n; i++)
      {
        PRC(i);
        PRL(particles[i].pos);
      }
  }


  BHNode *nodes = NULL;
  int nnodes = n * 2 + 100;
  nodes = new BHNode[nnodes];
  double eps_square = 0.1 * 0.1;
  double theta_square = 0.5; // [rad^2]
  double dt = 0.01;
  double end_time = 1;
  int step = 0;
  ofstream output_file;
  string filename = "output.csv";
  output_file.open(filename, ios::out);
  output_file << "id,t,x,y,z" << endl;

  for (double t = 0; t <= end_time; t += dt, step++)
  {
    Integrate(nodes, nnodes, particles, n, eps_square, theta_square, dt);
    for (int i = 0; i < n; i++)
    {
      output_file << i << "," << t << "," << particles[i].pos << endl;
    }
  }
  return 0;
}
