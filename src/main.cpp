#include <iostream>
#include <chrono>
#include "argument_interpreter.hpp"
#include "simulators/tree_simulator.hpp"
#include "simulators/simple_simulator.hpp"
#include "simulators/hdd_powered_simulator.hpp"
#include "simulators/hdd_async_simulator.hpp"
#include "sub_area.hpp"
#include "bhnode.hpp"
#include "particle.hpp"

using namespace std;
using namespace chrono;

double sim_time = 0;
double write_time = 0;
double read_time = 0;
double error = 0;
long long int calc_count = 0;

double show_time(string message, system_clock::time_point start, system_clock::time_point end){
    double time = static_cast<double>(duration_cast<microseconds>(end - start).count() / 1000.0);
#ifdef DETAIL
    cout << message << ": " << time << "[ms]" << endl;
#endif
    return time;
}

void Simulate(int mode, ArgumentInterpreter arguments)
{
  system_clock::time_point start, end;
  SimulatorBase *simulator;
#ifdef DETAIL
  cout << "  Initialization Start" << endl;
#endif
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
  case 3:
    simulator = new HDDAsyncSimulator(arguments);
    break;
  default:
    break;
  }

  end = system_clock::now();
#ifdef DETAIL
  show_time("  Initialization End", start, end);
#endif

#ifdef DETAIL
  cout << "  Simulation " << " Start" << endl;
#endif
  start = system_clock::now();
  simulator->Simulate();
  end = system_clock::now();
  error += simulator->error;
#ifdef DETAIL
  cout << "energy error: " << simulator->error << "[J]" << endl;
#endif

  sim_time += show_time("  Simulation End", start, end);
#ifdef WATCH_IO
  if(mode == 2 || mode == 3)
  {
#ifdef DETAIL
    cout << "  write: " << SubArea::write_time << "[ms]" << endl;
#endif
    write_time += SubArea::write_time;
    SubArea::write_time = 0;
#ifdef DETAIL
    cout << "  read: " << SubArea::read_time << "[ms]" << endl;
#endif
    read_time += SubArea::read_time;
    SubArea::read_time = 0;
  }
#endif
#ifdef DETAIL
  cout << "  the number of calc: " << BHNode::calc_count << endl;
#endif
  calc_count += BHNode::calc_count;
  BHNode::calc_count = 0;
}

int main(int argc, char* argv[])
{
  ios::sync_with_stdio(false);
  // Particle* p = new Particle[60000000];

  ArgumentInterpreter arguments(argc, argv);

  int mode = arguments.simulator_mode;
  cout << "0. Simple Simulator" << endl;
  cout << "1. Tree Simulator" << endl;
  cout << "2. HDD Powered Tree Simulator" << endl;
  cout << "3. HDD Powered Tree Simulator(Async I/O)" << endl;
  cout << "Select Simulator: ";
  if(mode == -1) {
    cin >> mode;
  }
  cout << mode << " was selected." << endl;
  for (int i = 0; i < arguments.cnt; i++)
  {
    cout << endl << "Start task " << i + 1 << endl;
    Simulate(mode, arguments);
  }

  cout << "Avarage Parameters on " << arguments.cnt << " times execution" << endl;
  cout << "energy error: " << error / arguments.cnt << "[J]" << endl;
  cout << "simulation: " << sim_time / arguments.cnt << "[ms]" << endl;
#ifdef WATCH_IO
  if (mode == 2 || mode == 3)
  {
    cout << "write: " << write_time / arguments.cnt << "[ms]" << endl;
    cout << "read: " << read_time / arguments.cnt << "[ms]" << endl;
  }
#endif
  cout << "the number of calc: " << calc_count / arguments.cnt << endl;
  calc_count += BHNode::calc_count;
}
