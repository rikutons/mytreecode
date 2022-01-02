#pragma once
#include <string>
using namespace std;

class ArgumentInterpreter
{
public:
  double dt = 0.01;
  double t = 1;
  double eps_square = 0.1 * 0.1;
  double theta_square = 0.5; // [rad^2]
  string input_filename = "";
  string output_filename = "output.csv";
  // 以下はinputfileが決まっている場合無効となる
  int n = 1000;
  double rsize = 1.0;
  ArgumentInterpreter(int argc, char *argv[]);
};