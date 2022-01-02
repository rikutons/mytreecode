#include <iostream>
#include "argument_interpreter.hpp"
using namespace std;

ArgumentInterpreter::ArgumentInterpreter(int argc, char* argv[]) 
{
  if (argc >= 2)
  {
    string arg1 = argv[1];
    if (arg1 == "help" || arg1 == "-h" || arg1 == "h")
    {
      cout << "-------- help of arguments --------" << endl;
      cout << "Time:              " << "t (default is 1)" << endl;
      cout << "Delta Time:        " << "dt (default is 0.01)" << endl;
      cout << "Input Filename:    " << "input_filename (default is \"\")" << endl;
      cout << "  If input filename is \"\", this program generate particles randomly." << endl;
      cout << "Output Filename:   " << "output_filename (default is output.csv)" << endl;
      cout << "Num of Particles:  " << "n (default is 1000)" << endl;
      cout << " Ignored if input filename is not \"\"." << endl;
      cout << "Generation Radius: " << "rsize (default is 1)" << endl;
      cout << " Ignored if input filename is not \"\"." << endl;
      cout << "Softening ^ 2:     " << "eps_square (defalut is 0.1 ^ 2)" << endl;
      cout << "Theta ^ 2:         " << "theta_square (default is 0.5)" << endl;
      cout << "[Example] ./a.out t 2 dt 0.01 n 10000 rsize 10 output_filename output.csv" << endl;
      cout << "------------------------------------" << endl;
      exit(0);
    }
  }
  if(argc % 2 != 1)
  {
    cout << "[ERROR] Illegal arguments!" << endl;
    cout << "The number of arguments must be even number(exclude ./a.out)." << endl;
    exit(1);
  }
  for (int i = 1; i < argc; i += 2)
  {
    string name = argv[i];
    string data = argv[i + 1];
    if ("t" == name)
      t = stod(data);
    else if ("dt" == name)
      dt = stod(data);
    else if ("in" == name || "input" == name || "input_filename" == name)
      input_filename = data;
    else if ("out" == name || "output" == name || "output_filename" == name)
      output_filename = data;
    else if ("eps_square" == name)
      eps_square = stod(data);
    else if ("theta_square" == name)
      theta_square = stod(data);
  // 以下はinputfileが決まっている場合無効となる
    else if ("n" == name)
      n = stoi(data);
    else if ("rsize" == name)
      rsize = stod(data);
  }
  cout << "-------- initial conditions --------" << endl;
  cout << "Time:              " << t << endl;
  cout << "Delta Time:        " << dt << endl;
  if(input_filename != "")
    cout << "Input Filename:    " << input_filename << endl;
  else
  {
    cout << "Num of Particles:  " << n << endl;
    cout << "Generation Radius: " << rsize << endl;
  }
  cout << "Output Filename:   " << output_filename << endl;
  cout << "Softening ^ 2:     " << eps_square << endl;
  cout << "Theta ^ 2:         " << theta_square << endl;
  cout << "------------------------------------" << endl;
}
