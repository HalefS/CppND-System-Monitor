#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int Pid, string User, string Command, float Cpu_Utilization, string Ram, long int Up_Time) :
pid(Pid), user(User), command(Command), cpu_usage(Cpu_Utilization), ram(Ram), up_time(Up_Time) {}

Process::Process() {}

// TODO: Return this process's ID
int Process::Pid() { return pid; }

void Process::Pid(int Pid) {
  pid = Pid;
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
    return 0;

}

// TODO: Return the command that generated this process
string Process::Command() { return command; }

// TODO: Return this process's memory utilization
string Process::Ram() { return ram; }

// TODO: Return the user (name) that generated this process
string Process::User() { return user; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return up_time; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }
