#include "sub_area.hpp"

double SubArea::read_time = 0;
double SubArea::write_time = 0;

void SubArea::BeginWrite() 
{
  start = system_clock::now();
  output_file.open(tmpfile_path);
  first_write_flag = true;  // 最終行が空行になってしまう現象をフラグを用いて回避する
  end = system_clock::now();
  AddWriteTime();
}

void SubArea::Write(string s) 
{
  start = system_clock::now();
  if (first_write_flag)
  {
    output_file << s;
    first_write_flag = false;
  }
  else
  {
    output_file << endl << s;
  }
  end = system_clock::now();
  AddWriteTime();
}

void SubArea::EndWrite() 
{
  start = system_clock::now();
  output_file.close();
  end = system_clock::now();
  AddWriteTime();
}

/*
  小領域内の粒子データを読み込み、引数の配列に渡す関数
  返り値は小領域の粒子数
*/
int SubArea::Read(Particle particles[]) 
{
  start = system_clock::now();
  int i = 0;
  input_file.open(tmpfile_path);
  while(!input_file.eof())
  {
    input_file >> particles[i].index >> particles[i].mass >> particles[i].pos.x >> particles[i].pos.y >> particles[i].pos.z >> particles[i].velocity.x >> particles[i].velocity.y >> particles[i].velocity.z >> particles[i].acceralation.x >> particles[i].acceralation.y >> particles[i].acceralation.z;
    particles[i].phi = 0;
    i++;
  }
  input_file.close();
  end = system_clock::now();
  AddReadTime();
  return n = i;
}

int SubArea::MakeLET(BHNode node) 
{
  int size = 0;
  Vector3 dx = center_pos - node.pos;
  double r_square = dx * dx;
  if ((r_square * theta_square > node.size * node.size) || (node.nparticle == 1)){
    LET.push_back({node.mass, node.pos});
    return 1;
  }
  for (int i = 0; i < 8; i++)
    if (node.child[i] != NULL)
      size += MakeLET(*node.child[i]);
  return size;
}

/*
  小領域内の各粒子について、LETの各ノードからかかる力を計算する関数
  計算量は LETのサイズ * 小領域の粒子数
*/
void SubArea::AccumulateLETGravity(Particle particles[]) 
{
    for (int i = 0; i < n; i++)
    {
      for (auto node : LET)
      {
          Vector3 dx = particles[i].pos - node.pos;
          double r = sqrt(dx * dx + eps_square);
          particles[i].phi -= node.mass / r;
          particles[i].acceralation -= node.mass * dx / powl(r, 3);
      }
    }
    LET.clear();
}

void SubArea::UseQueue() 
{
  if(particle_queue.empty())
    return;
  output_file.open(tmpfile_path, ios_base::app);
  while (!particle_queue.empty()) {
    Particle p = particle_queue.front();
    particle_queue.pop();
    Write(p.ToDataString());
    n++;
  }
  output_file.close();
}

void SubArea::AddReadTime() 
{
  read_time += static_cast<double>(duration_cast<microseconds>(end - start).count() / 1000.0);
}

void SubArea::AddWriteTime() 
{
  write_time += static_cast<double>(duration_cast<microseconds>(end - start).count() / 1000.0);
}