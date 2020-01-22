#ifndef PROCESS_H
#define PROCESS_H

#include <string>
using std::string;
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process ();
  Process(int Pid, string User, string Command, float Cpu_Utilization, string Ram, long int Up_Time);
  int Pid();
  void Pid(int Pid);                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

  // TODO: Declare any necessary private members
 private:
   int pid;
   string user;
   string command;
   float cpu_usage;
   string ram;
   long int up_time;
};

#endif
