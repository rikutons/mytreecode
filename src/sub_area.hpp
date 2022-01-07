#pragma once
#include <fstream>
#include <vector>
#include <queue>
#include <chrono>
#include "vector3.hpp"
#include "particle.hpp"
#include "bhnode.hpp"
using namespace chrono;

class SubArea
{
  ifstream input_file;
  ofstream output_file;
  bool first_write_flag;
  system_clock::time_point start, end;
  void AddReadTime();
  void AddWriteTime();

public:
  vector<Particle*> LET;
  static double read_time;
  static double write_time;
  queue<string> particle_queue;
  int n;
  double theta_square;
  double eps_square;
  string tmpfile_path;
  double mass;
  Vector3 center_pos;

  void BeginWrite();
  void Write(string s);
  void EndWrite();
  int Read(Particle[]);
  int MakeLET(BHNode);
  void DeleteLET();
  void UseQueue();
};