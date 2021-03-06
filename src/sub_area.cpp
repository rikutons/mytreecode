#include "sub_area.hpp"

double SubArea::read_time = 0;
double SubArea::write_time = 0;

void SubArea::BeginWrite() 
{
#ifdef WATCH_IO
  start = system_clock::now();
#endif
  try
  {
  output_file.open(tmpfile_path, ios::binary);
    output_file.exceptions(std::ios_base::failbit);
  } catch (const std::exception& e) {
    cout << e.what() << endl;
  }
  if (output_file.fail())
  {
    cout << "failed." << endl;
  }
  first_write_flag = true;  // 最終行が空行になってしまう現象をフラグを用いて回避する
#ifdef WATCH_IO
  end = system_clock::now();
  AddWriteTime();
#endif
}

void SubArea::Write(Particle particle) 
{
#ifdef WATCH_IO
  start = system_clock::now();
#endif
  output_file.write((char *)&particle, sizeof(Particle));
#ifdef WATCH_IO
  end = system_clock::now();
  AddWriteTime();
#endif
}

void SubArea::EndWrite() 
{
#ifdef WATCH_IO
  start = system_clock::now();
#endif
  output_file.close();
#ifdef WATCH_IO
  end = system_clock::now();
  AddWriteTime();
#endif
}

/*
  小領域内の粒子データを読み込み、引数の配列に渡す関数
  返り値は小領域の粒子数
*/
int SubArea::Read(Particle particles[]) 
{
#ifdef WATCH_IO
  start = system_clock::now();
#endif
  int i = 0;
  input_file.open(tmpfile_path, ios::binary);
  do
  {
    // input_file >> particles[i].index >> particles[i].mass >> particles[i].pos.x >> particles[i].pos.y >> particles[i].pos.z >> particles[i].velocity.x >> particles[i].velocity.y >> particles[i].velocity.z >> particles[i].acceralation.x >> particles[i].acceralation.y >> particles[i].acceralation.z;
    input_file.read((char *)&particles[i], sizeof(Particle));
    particles[i].phi = 0;
    i++;
  } while(!input_file.eof());
  input_file.close();
#ifdef WATCH_IO
  end = system_clock::now();
  AddReadTime();
#endif
  return n = i - 1; // 最後に不要な空のパーティクルを1つ読んでしまうので無くす
}

int SubArea::MakeLET(BHNode node) 
{
  int size = 0;
  Vector3 dx = center_pos - node.pos;
  double r_square = dx * dx;
  if ((r_square * theta_square > node.size * node.size) || (node.nparticle == 1)){
    LET.push_back(new Particle(node.pos, node.mass));
    return 1;
  }
  for (int i = 0; i < 8; i++)
    if (node.child[i] != NULL)
      size += MakeLET(*node.child[i]);
  return size;
}

void SubArea::DeleteLET() 
{
  for (auto p : LET)
  {
    delete p;
  }
  LET.clear();
}

void SubArea::UseQueue() 
{
  if(particle_queue.empty())
    return;
  output_file.open(tmpfile_path, ios_base::app | ios_base::binary);
  while (!particle_queue.empty()) {
    Particle p = particle_queue.front();
    particle_queue.pop();
    Write(p);
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