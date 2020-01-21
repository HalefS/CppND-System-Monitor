#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string kernel;
  string line;
  std::ifstream stream(kProcDirectory + kKernelFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() {
  string available_mem_line, total_mem_line;
  string total_mem, available_mem;
  std::istringstream s;
  std::ifstream stream (kProcDirectory+kMeminfoFilename);
  if(stream) {
    std::getline(stream, total_mem_line);
    //Get third string on the proc file meminfo
    std::getline(stream, available_mem_line);
    std::getline(stream, available_mem_line);
    std::istringstream s1 (total_mem_line);
    std::istringstream s2 (available_mem_line);
    s1 >> total_mem >> total_mem;
    s2 >> available_mem >> available_mem;
  }
  return (((stof(total_mem) - stof(available_mem))) / stof(total_mem));
}

long LinuxParser::UpTime() {
  string line;
  long up_time = 0;
  std::ifstream stream(kProcDirectory+kUptimeFilename);
  if(stream) {
    std::getline(stream, line);
    // uptime procfile is defined as "[uptime] "space" [idletime]"
    up_time = std::stol(line.substr(0, line.find(" ")));
  }
  return up_time;
 }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0;}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }


vector<string> LinuxParser::CpuUtilization() {
  string line, user, nice , guest, guest_nice, iowait, irq, system,
          idle, softirq, steal;
  std::ifstream stream (kProcDirectory+kStatFilename);
  if(stream.is_open()) {
    std::getline(stream, line);
    std::istringstream cpu_info_stream(line);
    cpu_info_stream >> user >> nice >> system >> idle >> iowait >> irq
    >> softirq >> steal >> guest >> guest_nice;
  }
  vector<string> cpu_stats {user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice};
  return cpu_stats;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line, processes, value;
  std::ifstream proc_stream (kProcDirectory+kStatFilename);
  if(proc_stream.is_open()) {
    while(std::getline(proc_stream, line)) {
        std::istringstream line_stream (line);
        line_stream >> processes >> value;
        if(processes == "processes")
          break;
      }
    }
  return std::stoi(value);
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line, key, procs_running;
  std::ifstream proc_stream (kProcDirectory+kStatFilename);
  if(proc_stream.is_open()) {
    while(std::getline(proc_stream, line)) {
      std::istringstream string_stream (line);
      string_stream >> key >> procs_running;
      if(key == "procs_running") {
        break;
      }
    }
  }
  return std::stoi(procs_running);
}


string LinuxParser::Command(int pid) {
  string line;
  std::ifstream stream(kProcDirectory+std::to_string(pid)+kCmdlineFilename);
  if(stream)
    std::getline(stream, line);
  return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }

vector<Process> LinuxParser::AllProcesses(vector<int> pids) {
  vector<Process> processes = {};
  for(int pid : pids) {
      string ram = Ram(pid);
      string uid = Uid(pid);
      string user = User(pid);
      long process_up_time = UpTime(pid);
      string command = Command(pid);
      Process p {};
      processes.push_back(p);
  }
  return processes;
}
