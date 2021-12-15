#include <iostream>
#include <chrono>
#include "tree_simulator.hpp"
#include "simple_simulator.hpp"

using namespace std;
using namespace chrono;

void show_time(system_clock::time_point start, system_clock::time_point end){
    double time = static_cast<double>(duration_cast<microseconds>(end - start).count() / 1000.0);
    cout << "time" << time << "[ms]" << endl;
}

int main()
{
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  system_clock::time_point start, end;
  start = system_clock::now();

  auto simulator = new TreeSimulator();
  // auto simulator = new SimpleSimulator();

  end = system_clock::now();
  show_time(start, end);
  start = end;

  simulator->Simulate();

  end = system_clock::now();
  show_time(start, end);

  return 0;
}
