#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include <thread>

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
      if (std::all_of(filename.begin(), filename.end(), isdigit) && std::stoi(file->d_name) > 1000) {
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

string LinuxParser::Ram(int pid) {
  string line, key, ram_usage;
  std::ifstream stream (kProcDirectory+std::to_string(pid)+kStatusFilename);
  if(stream.is_open()) {
    // Go to the line beggining with VmSize and extract the ram usage
    while(std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream ram_stream (line);
      while(ram_stream >> key >> ram_usage) {
        if(key == "VmSize" && std::all_of(ram_usage.begin(), ram_usage.end(), isdigit)) {
          return ram_usage;
        }
      }
    }
  }
  return ram_usage;
}

string LinuxParser::Uid(int pid) {
  string line, key, value;
  std::ifstream stream (kProcDirectory+to_string(pid)+kStatusFilename);
  if(stream.is_open()) {
    while(std::getline(stream,line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream uid_line_stream(line);
      uid_line_stream >> key >> value;
      if(key == "Uid")
        return value;
    }
  }
  return "NA";
}

string LinuxParser::User(int pid) {
  string line, user, permission, user_id;
  std::ifstream stream (kPasswordPath);
  if(stream.is_open()) {
    while(std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream user_stream(line);
      user_stream >> user >> permission >> user_id;
      if(user_id == Uid(pid))
        return user;
    }
  }
  return "NA";
}

long LinuxParser::UpTime(int pid) {
  string line, time_elapsed;
  std::ifstream stream (kProcDirectory+to_string(pid)+kStatFilename);
  if(stream.is_open()) {
    std::getline(stream, line);
    std::istringstream time_stream(line);
    // extract pieces from the stream untill we reach the column with value 20
    while(time_stream >> time_elapsed) {
      if(time_elapsed == "20") {
        // From there move four columns and get the value (column 22 overall)
        for(int i = 0; i < 4; i++) {
          time_stream >> time_elapsed;
        }
        // Convert clock_ticks to seconds
        return std::stol(time_elapsed) / sysconf(_SC_CLK_TCK);
      }
    }
  }
  return 0;
}

vector<Process> LinuxParser::AllProcesses() {
  vector<Process> all_processes;
  for(int pid : Pids()) {
      Process p (pid, User(pid), Command(pid), 13.2, Ram(pid), UpTime(pid));
      all_processes.push_back(p);
    }
  return all_processes;
}
