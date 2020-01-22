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

int Process::Pid() { return pid; }

void Process::Pid(int Pid) {
  pid = Pid;
}

float Process::CpuUtilization() {
    return 0;

}

string Process::Command() { return command; }

string Process::Ram() { return ram; }

string Process::User() { return user; }

long int Process::UpTime() { return up_time; }

bool Process::operator<(Process const& a) const { return true; }
