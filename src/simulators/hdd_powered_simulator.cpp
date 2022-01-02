#include "hdd_powered_simulator.hpp"

HDDPoweredSimulator::HDDPoweredSimulator(ArgumentInterpreter arguments) :
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
    for (int j = 0; j < 3; j++)
      if(i & (1 << j))
        sub_areas[i].center_pos[j] *= -1;
    // sub_areas[i].center_pos = Vector3();
    // ここまで
    sub_areas[i].BeginWrite();
  }

  nnodes = n / div_num * 2 + 100;
  nodes = new BHNode[nnodes];
  particles = new Particle[n / div_num + 1];

  for (int i = 0; i < n; i++)
  {
    double mass;
    Vector3 pos;
    input_file >> mass >> pos[0] >> pos[1] >> pos[2];
    int index = GetIndex(pos);
    // cout << "i: " << i << ", mass: " << mass << ", pos: " << pos << ", index: " << index << endl;
    sub_areas[index].Write(to_string(i) + " " + to_string(mass) + " " + to_string(pos) + " 0 0 0 0 0 0"); // 速度と加速度の初期値(0, 0, 0)を書き込んでおく
  }
  for (int i = 0; i < div_num; i++)
    sub_areas[i].EndWrite();
}

void HDDPoweredSimulator::Step() 
{
  // cout << "Start Step " << step << endl;
  // cout << "Phase 1: Make LET" << endl;
  double ke = 0;
  double pe = 0;

  for (int i = 0; i < div_num; i++)
  {
    int num = sub_areas[i].Read(particles);
    sub_areas[i].BeginWrite();
    for (int j = 0; j < num; j++)
    {
      particles[j].Predict(dt);
      sub_areas[i].Write(particles[j].ToDataString());
    }
    sub_areas[i].EndWrite();
  }

  for (int i = 0; i < div_num; i++)
  {
    // #pragma omp parallel
    // {
    //   if(omp_get_thread_num() == 0){
    //     // 領域 i のパーティクルを読みこむ
    //     ReadParticles(i + 1);
    //   }
    //   else{
    //     #pragma omp for
    //     for (int j = 0; j < div_num; j++){
    //       // 領域 j から見たLETを作ってメモリに保存する
    //       GetCenterPos(j);
    //     }
    //   }
    // }
    // delete[] particles;
    // particles = next_particles;
    int num = sub_areas[i].Read(particles);
    // cout << "SubArea " << i << " have " << num << " particles  |  ";
    nodes->AssignRoot(sub_areas[i].center_pos, rsize * 2, particles, num);

    int heap_remainder = nnodes - 1;
    BHNode *btmp = nodes + 1;
    nodes->CreateTreeRecursive(btmp, heap_remainder);
    nodes->CalcPhysicalQuantity();

    // cout << "LET Size: [";
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

  // cout << "Phase 2: Calc Pos" << endl;
  for (int i = 0; i < div_num; i++)
  {
    int num = sub_areas[i].Read(particles);
    // cout << "SubArea " << i << " LET :";
    nodes->AssignRoot(sub_areas[i].center_pos, rsize * 2, particles, num);

    int heap_remainder = nnodes - 1;
    BHNode *btmp = nodes + 1;
    nodes->CreateTreeRecursive(btmp, heap_remainder);
    nodes->CalcPhysicalQuantity();

    for (int j = 0; j < num; j++)
      nodes->CalcGravityUsingTree(particles[j], eps_square, theta_square);
    sub_areas[i].AccumulateLETGravity(particles);

    for (int j = 0; j < num; j++)
      particles[j].Correct(dt);

    sub_areas[i].BeginWrite();
    for (int j = 0; j < num; j++)
    {
      sub_areas[i].Write(particles[j].ToDataString());
      output_file << particles[j].index << "," << t << "," << particles[j].pos << endl;
      ke += particles[j].CalcKineticEnergy();
      pe += particles[j].CalcPotentialEnergy();
    }
    sub_areas[i].EndWrite();
  }
  energy = ke + pe;
}

void HDDPoweredSimulator::Output()
{
  // Phase 2の最中に書き込みを行うため、こちらの関数ではエネルギーだけを書くようにオーバーライドする(重心計算はとりあえず面倒なので端折る)
  status_output_file << t << ", 0, 0, 0, 0, 0, 0, " << energy << endl;
}

// 小領域が8個であるとき限定の関数
int HDDPoweredSimulator::GetIndex(Vector3 pos) 
{
  // return 0;
  int ret = 0;
  for (int i = 0; i < 3; i++)
    if (pos[i] < 0)
      ret |= (1 << i);
  return ret;
}
