#include <random>
#include <iomanip>
#include "../common.h"
#include "simulator_base.hpp"

SimulatorBase::SimulatorBase(ArgumentInterpreter arguments, bool allocate_memory)
{
  if(arguments.input_filename == "")
  {
    n = arguments.n;
    rsize = arguments.rsize;

    // cerr << "Enter power index: ";
    // cin >> power_index;

    if(allocate_memory)
      particles = new Particle[n];

    InitParticlesOnAlignedShape();
    if (AskSeeParticles() == 1)
      for (int i = 0; i < n; i++)
      {
        PRC(i);
        PRL(particles[i].pos);
      }
  }
  else
  {
    input_file.open("../input/" + arguments.input_filename);
    input_file >> n >> rsize >> true_energy;
    string buf;
    getline(input_file, buf); // 粒子数が書いてある行を破棄する
    cout << n << " particles" << endl;
    if(allocate_memory)
    {
      particles = new Particle[n];
      ReadParticles();
    }
  }

  eps_square = arguments.eps_square;
  theta_square = arguments.theta_square; // [rad^2]
  dt = arguments.dt;
  end_time = arguments.t;

  OpenFiles(arguments.output_filename);
}

void SimulatorBase::Simulate()
{
  for (t = 0; t <= end_time; t += dt, step++)
  {
    Step();
    Output();
  }
  output_file.close();
  status_output_file.close();
  error = true_energy - energy;
}

void SimulatorBase::OpenFiles(string filename) 
{
  output_file.open("../output/" + filename, ios::out);
  output_file << "id,t,x,y,z" << endl;

  status_output_file.open("../output/status_" + filename, ios::out);
  status_output_file << fixed << setprecision(20) << "t,m_x,m_y,m_z,m_vx,m_vy,m_vz,total_energy" << endl;
}

void SimulatorBase::InitParticlesOnSphereShape() 
{
  random_device rnd;
  // mt19937 mt(rnd());
  mt19937 mt(0);
  uniform_real_distribution<> rand_real(-1, 1);
  // ofstream input;
  // input.open("../input/1000n_minus2p5_sphere.csv", ios::out);
  // input << n << " " << rsize << endl;
  for (int i = 0; i < n; i++)
  {
    Vector3 p(1, 1, 1);
    while (p * p > 1)
      for (int j = 0; j < 3; j++)
        p[j] = rand_real(mt);
    p *= powl(p * p, 3.0 / (power_index + 3) - 1);
    particles[i].pos = rsize * p;
    particles[i].mass = 1.0 / n;
    // input << particles[i].mass << " " << to_string(particles[i].pos) << endl;
  }
}

void SimulatorBase::InitParticlesOnAlignedShape() 
{
  random_device rnd;
  mt19937 mt(rnd());
  uniform_real_distribution<> rand_real(0, 1);
  // ofstream input;
  // input.open("../input/1000n_aligned.csv", ios::out);
  // input << n << " " << rsize << endl;
  for (int i = 0; i < n; i++)
  {
    int index = 8 * i / n;
    Vector3 p;
    for (int j = 0; j < 3; j++)
    {
      p[j] = rand_real(mt);
      if(index & (1 << j))
        p[j] *= -1;
    }
    particles[i].pos = rsize * p;
    particles[i].mass = 1.0 / n;
    // input << particles[i].mass << " " << to_string(particles[i].pos) << endl;
  }
}

int SimulatorBase::AskSeeParticles() 
{
  int r;
  cerr << "Do u wanna see particle's data? (yes:1, no:0): ";
  cin >> r;
  return r;
}

void SimulatorBase::Output() 
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
  energy = ke + pe;
  status_output_file << "," << energy << endl;
}

void SimulatorBase::ReadParticles() 
{
  for (int i = 0; i < n; i++)
  {
    double mass;
    Vector3 pos;
    input_file >> mass >> pos[0] >> pos[1] >> pos[2];
    particles[i].mass = mass;
    particles[i].pos = pos;
    particles[i].index = i;
  }
}