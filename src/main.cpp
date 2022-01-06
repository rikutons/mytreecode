#include <iostream>
#include <chrono>
#include "argument_interpreter.hpp"
#include "simulators/tree_simulator.hpp"
#include "simulators/simple_simulator.hpp"
#include "simulators/hdd_powered_simulator.hpp"
#include "sub_area.hpp"

using namespace std;
using namespace chrono;

void show_time(string message, system_clock::time_point start, system_clock::time_point end){
    double time = static_cast<double>(duration_cast<microseconds>(end - start).count() / 1000.0);
    cout << message << ": " << time << "[ms]" << endl;
}

int main(int argc, char* argv[])
{
  ios::sync_with_stdio(false);

  system_clock::time_point start, end;
  ArgumentInterpreter arguments(argc, argv);

  int mode;
  cout << "0. Simple Simulator" << endl;
  cout << "1. Tree Simulator" << endl;
  cout << "2. HDD Powered Tree Simulator" << endl;
  cout << "Select Simulator: ";
  cin >> mode;
  SimulatorBase *simulator;
  cout << "Initialization Start" << endl;
  start = system_clock::now();
  switch (mode)
  {
  case 0:
    /* code */
    simulator = new SimpleSimulator(arguments);
    break;
  case 1:
    simulator = new TreeSimulator(arguments);
    break;
  case 2:
    simulator = new HDDPoweredSimulator(arguments);
    break;
  default:
    break;
  }

  end = system_clock::now();
  show_time("Initialization End", start, end);

  cout << "Simulation Start" << endl;
  start = system_clock::now();
  simulator->Simulate();
  end = system_clock::now();
  show_time("Simulation End", start, end);
  if(mode == 2)
  {
    cout << "write: " << SubArea::write_time << "[ms]" << endl;
    cout << "read: " << SubArea::read_time << "[ms]" << endl;
  }

  return 0;
}
