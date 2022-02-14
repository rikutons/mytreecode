#include "hdd_async_simulator.hpp"
#include "../common.h"

HDDAsyncSimulator::HDDAsyncSimulator(ArgumentInterpreter arguments) :
  SimulatorBase(arguments, false),
  div_num(8)
{
  sub_areas = new SubArea[div_num];

  for (int i = 0; i < div_num; i++)
  {
    sub_areas[i].theta_square = theta_square;
    sub_areas[i].eps_square = eps_square;
    sub_areas[i].tmpfile_path = "../input/" + arguments.input_filename + to_string(i) + ".tmp";
    // 小領域が8個であるとき限定の処理
    sub_areas[i].center_pos = rsize / 2 * Vector3(1, 1, 1);
    sub_areas[i].size = rsize;
    for (int j = 0; j < 3; j++)
      if(i & (1 << j))
        sub_areas[i].center_pos[j] *= -1;
    // sub_areas[i].center_pos = Vector3();
    // ここまで
    sub_areas[i].BeginWrite();
  }

  nnodes = n / div_num * 2 + n / 10;
  nodes = new BHNode[nnodes];
  particle_maxnum = n / div_num + n / 10;
  particles = new Particle[particle_maxnum]; // !注意! 粒子の交換に対してn / 100粒子分しか余裕を持たせていません
  next_particles = new Particle[particle_maxnum];

  for (int i = 0; i < n; i++)
  {
    Particle p;
    input_file >> p.mass >> p.pos[0] >> p.pos[1] >> p.pos[2];
    p.index = i;
    int area_index = GetIndex(p.pos);
    // cout << "i: " << i << ", mass: " << mass << ", pos: " << pos << ", index: " << index << endl;
    sub_areas[area_index].Write(p); // 速度と加速度の初期値(0, 0, 0)を書き込んでおく
  }
  input_file.close();

  for (int i = 0; i < div_num; i++)
    sub_areas[i].EndWrite();

  for (int i = 0; i < div_num; i++)
  {
    int num = sub_areas[i].Read(particles);
    sub_areas[i].BeginWrite();
    for (int j = 0; j < num; j++)
    {
      particles[j].Predict(dt);
      sub_areas[i].Write(particles[j]);
    }
    sub_areas[i].EndWrite();
  }
}

void HDDAsyncSimulator::Step() 
{
  // cout << "Start Step " << step << endl;
  // cout << "Phase 1: Make LET" << endl;
  double ke = 0;
  double pe = 0;

  sub_areas[0].Read(particles);
  for (int i = 0; i < div_num; i++)
  {
    #pragma omp parallel
    {
      #pragma omp single nowait
      {
        this->sub_areas[(i + 1) % this->div_num].Read(this->next_particles);
      }
      #pragma omp single
      {
        nodes->AssignRoot(Vector3(), rsize * 2, particles, sub_areas[i].n);

        int heap_remainder = nnodes - 1;
        BHNode *btmp = nodes + 1;
        nodes->CreateTreeRecursive(btmp, heap_remainder);
        nodes->CalcPhysicalQuantity();
      }
      #pragma omp for schedule(dynamic)
      for (int j = 0; j < div_num; j++)
      {
        if(i != j)
          sub_areas[j].MakeLET(nodes[0]);
        // if(i != j)
        //   cout << sub_areas[j].MakeLET(nodes[0]);
        // else
        //   cout << "-";
        // if(j != div_num - 1)
        //   cout << ", ";
      }
      // cout << "]" << endl;
    }
    SwapParticles(&particles, &next_particles);
  }

  // cout << "Phase 2: Calc Pos" << endl;
  for (int i = 0; i < div_num; i++)
  {
    vector<Particle*> groupArrays[GROUP_NUM];
    #pragma omp parallel
    {
      #pragma omp single nowait
      {
        if(i != 0)
          Write(i - 1);
        if(i != div_num - 1)
          sub_areas[i + 1].Read(next_particles);
      }
      #pragma omp single
      {
        nodes->AssignRootWithLET(Vector3(), rsize * 2, particles, sub_areas[i].LET, sub_areas[i].n);

        int heap_remainder = nnodes - 1;
        BHNode *btmp = nodes + 1;
        nodes->CreateTreeRecursive(btmp, heap_remainder);
        nodes->CalcPhysicalQuantity();
#ifndef NO_SHARE_LET
        for (int j = 0; j < sub_areas[i].n; j++)
        {
          particles[j].acceralation = Vector3();
          particles[j].phi = particles[j].mass / sqrt(eps_square);
          groupArrays[particles[j].CalcGroupIndex(sub_areas[i].center_pos, sub_areas[i].size)].emplace_back(&particles[j]);
        }
#endif
      }
#ifdef NO_SHARE_LET
      #pragma omp for schedule(dynamic)
      for (int j = 0; j < sub_areas[i].n; j++)
        nodes->CalcGravityUsingTree(particles[j], eps_square, theta_square);
#else
      #pragma omp for schedule(dynamic)
      for (int j = 0; j < GROUP_NUM; j++)
      {
        auto LET = nodes[0].MakeLET(Particle::CalcGroupCenter(j, sub_areas[i].size) + sub_areas[i].center_pos, eps_square, theta_square);
        for (auto p : groupArrays[j])
          for (auto node : LET)
          {
            Vector3 dx = node->pos - p->pos;
            node->AccumulateForceFromPoint(dx, dx * dx, eps_square, *p);
          }
      }
#endif

    }
    sub_areas[i].DeleteLET();

    for (int j = 0; j < sub_areas[i].n; j++)
    {
      particles[j].Correct(dt);
#ifdef OUTPUT
      output_file << particles[j].index << "," << t << "," << particles[j].pos << endl;
#endif
      ke += particles[j].CalcKineticEnergy();
      pe += particles[j].CalcPotentialEnergy();
      particles[j].Predict(dt);
      double max = abs(particles[j].pos.max());
      if(max > rsize)
      {
        for (int k = 0; k < div_num; k++)
        {
          sub_areas[k].size = max;
          sub_areas[k].center_pos *= (max / rsize);
        }
        rsize = max;
      }
    }
    SwapParticles(&particles, &next_particles);
  }
  Write(div_num - 1);

  #pragma omp parallel for
  for (int i = 0; i < div_num; i++)
  {
    sub_areas[i].UseQueue();
    if(sub_areas[i].n > particle_maxnum)
    {
      cout << "[ERROR] Particle's array size is not enough!" << endl;
      cout << "Subarea" << i << " have " << sub_areas[i].n << " particles but array size is " << particle_maxnum << "." << endl;
      cout << "Check particle_maxnum on hdd_powered_simulator.cpp." << endl;
    }
  }
  
  energy = ke + pe;
}

void HDDAsyncSimulator::Write(int i) 
{
  sub_areas[i].BeginWrite();
  for (int j = 0; j < sub_areas[i].n; j++)
  {
    int index = GetIndex(next_particles[j].pos);
    if (i == index)
      sub_areas[i].Write(next_particles[j]);
    else
      sub_areas[index].particle_queue.push(next_particles[j]);
  }
  sub_areas[i].EndWrite();
}

void HDDAsyncSimulator::Output()
{
#ifdef OUTPUT
  // Phase 2の最中に書き込みを行うため、こちらの関数ではエネルギーだけを書くようにオーバーライドする(重心計算はとりあえず面倒なので端折る)
  status_output_file << t << ", 0, 0, 0, 0, 0, 0, " << energy << endl;
#endif
}

// 小領域が8個であるとき限定の関数
int HDDAsyncSimulator::GetIndex(Vector3 pos) 
{
  // return 0;
  int ret = 0;
  for (int i = 0; i < 3; i++)
    if (pos[i] < 0)
      ret |= (1 << i);
  return ret;
}

void HDDAsyncSimulator::SwapParticles(Particle **a, Particle **b) 
{
    Particle *tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}
