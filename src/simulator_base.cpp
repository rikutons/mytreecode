#include <random>
#include "common.h"
#include "simulator_base.hpp"

SimulatorBase::SimulatorBase()
{
  cerr << "Enter n(number of particles): ";
  cin >> n;

  cerr << "Enter power index: ";
  cin >> power_index;

  particles = new Particle[n];

  rsize = 1.0;
  InitParticlesOnSphereShape();

  if (AskSeeParticles() == 1)
    for (int i = 0; i < n; i++)
    {
      PRC(i);
      PRL(particles[i].pos);
    }

  nodes = NULL;
  nnodes = n * 2 + 100;
  nodes = new BHNode[nnodes];
  eps_square = 0.1 * 0.1;
  theta_square = 0.5; // [rad^2]
  dt = 0.01;
  end_time = 1;
  step = 0;

  OpenFiles();
}

void SimulatorBase::Simulate()
{
  for (double t = 0; t <= end_time; t += dt, step++)
  {
    Step();
    Output(t);
  }
}

void SimulatorBase::OpenFiles() 
{
  output_file.open("../output/output.csv", ios::out);
  output_file << "id,t,x,y,z" << endl;

  status_output_file.open("../output/status_output.csv", ios::out);
  status_output_file << "t,m_x,m_y,m_z,m_vx,m_vy,m_vz,total_energy" << endl;
}

void SimulatorBase::InitParticlesOnSphereShape() 
{
  random_device rnd;
  // mt19937 mt(rnd());
  mt19937 mt(0);
  uniform_real_distribution<> rand_real(-1, 1);
  for (int i = 0; i < n; i++)
  {
    Vector3 p(1, 1, 1);
    while (p * p > 1)
      for (int i = 0; i < 3; i++)
        p[i] = rand_real(mt);
    p *= powl(p * p, 3.0 / (power_index + 3) - 1);
    particles[i].pos = rsize * p;
    particles[i].mass = 1.0 / n;
  }
}

int SimulatorBase::AskSeeParticles() 
{
  int r;
  cerr << "Do u wanna see particle's data? (yes:1, no:0): ";
  cin >> r;
  return r;
}

void SimulatorBase::Output(double t) 
{
  for (int i = 0; i < n; i++)
    output_file << i << "," << t << "," << particles[i].pos << endl;
  status_output_file << t << ",";
  PrintMassHeart(particles, n);
  PrintEnergies(particles, n);
}

void SimulatorBase::PrintMassHeart(Particle *particles, int n) 
{
  Vector3 cmpos = Vector3();
  Vector3 cmvel = Vector3();
  double cmmass = 0.0;
  for (int i = 0; i < n; i++)
  {
    cmpos += particles[i].mass * particles[i].pos;
    cmvel += particles[i].mass * particles[i].velocity;
    cmmass += particles[i].mass;
  }
  cmpos /= cmmass;
  cmvel /= cmmass;
  // cerr << "cm pos = " << cmpos << " vel = " << cmvel << endl;
  status_output_file << cmpos << "," << cmvel;
}

void SimulatorBase::PrintEnergies(Particle *particles, int n) 
{
  double ke = 0, pe = 0;
  for (int i = 0; i < n; i++)
  {
    ke += particles[i].CalcKineticEnergy();
    pe += particles[i].CalcPotentialEnergy();
  }
  double total_energy = ke + pe;
  status_output_file << "," << total_energy << endl;
}