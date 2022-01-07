#include <iostream>
#include <chrono>
#include "argument_interpreter.hpp"
#include "simulators/tree_simulator.hpp"
#include "simulators/simple_simulator.hpp"
#include "simulators/hdd_powered_simulator.hpp"
#include "sub_area.hpp"
#include "bhnode.hpp"

using namespace std;
using namespace chrono;

double sim_time = 0;
double write_time = 0;
double read_time = 0;
double error = 0;
long long int calc_count = 0;

double show_time(string message, system_clock::time_point start, system_clock::time_point end){
    double time = static_cast<double>(duration_cast<microseconds>(end - start).count() / 1000.0);
    cout << message << ": " << time << "[ms]" << endl;
    return time;
}

void Simulate(int mode, ArgumentInterpreter arguments)
{
  system_clock::time_point start, end;
  SimulatorBase *simulator;
  cout << "  Initialization Start" << endl;
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
  show_time("  Initialization End", start, end);

  cout << "  Simulation " << " Start" << endl;
  start = system_clock::now();
  simulator->Simulate();
  end = system_clock::now();
  error += simulator->error;
  cout << "energy error: " << simulator->error << "[J]" << endl;

  sim_time += show_time("  Simulation End", start, end);
  if(mode == 2)
  {
    cout << "  write: " << SubArea::write_time << "[ms]" << endl;
    write_time += SubArea::write_time;
    SubArea::write_time = 0;
    cout << "  read: " << SubArea::read_time << "[ms]" << endl;
    read_time += SubArea::read_time;
    SubArea::read_time = 0;
  }
  cout << "  the number of calc: " << BHNode::calc_count << endl;
  calc_count += BHNode::calc_count;
  BHNode::calc_count = 0;
}

int main(int argc, char* argv[])
{
  ios::sync_with_stdio(false);

  ArgumentInterpreter arguments(argc, argv);

  int mode;
  cout << "0. Simple Simulator" << endl;
  cout << "1. Tree Simulator" << endl;
  cout << "2. HDD Powered Tree Simulator" << endl;
  cout << "Select Simulator: ";
  cin >> mode;
  for (int i = 0; i < arguments.cnt; i++)
  {
    cout << endl << "Start task " << i + 1 << endl;
    Simulate(mode, arguments);
  }

  cout << "Avarage Parameters on " << arguments.cnt << " times execution" << endl;
  cout << "energy error: " << error / arguments.cnt << "[J]" << endl;
  cout << "simulation: " << sim_time / arguments.cnt << "[ms]" << endl;
  if (mode == 2)
  {
    cout << "write: " << write_time / arguments.cnt << "[ms]" << endl;
    cout << "read: " << read_time / arguments.cnt << "[ms]" << endl;
  }
  cout << "the number of calc: " << calc_count / arguments.cnt << endl;
  calc_count += BHNode::calc_count;
}
