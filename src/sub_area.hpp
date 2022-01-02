#include <fstream>
#include <vector>
#include "vector3.hpp"
#include "particle.hpp"
#include "bhnode.hpp"

struct Object
{
  double mass;
  Vector3 pos;
};

class SubArea
{
  ifstream input_file;
  ofstream output_file;
  bool first_write_flag;
  vector<Object> LET;
  int n;

public:
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
  void AccumulateLETGravity(Particle[]);
};