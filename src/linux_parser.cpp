#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

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
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
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

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string line, key;
  float value, mem_total, mem_free;
  float mem_usage = 0;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()){
    while (std::getline(stream, line)){ 
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "MemTotal:")
        mem_total = value;
      if (key == "MemFree:")
        mem_free = value;
    }
    mem_usage = (mem_total-mem_free)/mem_total;
  }
  return mem_usage; }

// Read and return the system uptime
long LinuxParser::UpTime() { 
  string line;
  long uptime = 0;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
  return uptime; 
  }

// Read and return CPU utilization
vector<float> LinuxParser::CpuUtilization(int pid) {
  string line, s;
  vector<float> time;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i = 0; i<22; i++){
      linestream >> s;
      if (i == 13 || i == 14 || i == 15 || i == 16 || i == 21)
        time.push_back(stof(s));
    }
  }
  return time; }

// Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line, key;
  int value = 0;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "processes")
        break;
    }
  }
  return value;
  }

// Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line, key;
  int value = 0;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "procs_running")
        break;
    }
  }
  return value;
  }

// Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string command;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()){
    std::getline(stream, command);
  }
  return command; }

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  string line, key;
  int value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "VmSize:")
        break;
    }
  }
  return to_string(value/1000); }

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string line, key, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()){
    while (std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "Uid:")
        break;
    }
  }
  return value; }

// Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  string user, x, pid_, line;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()){
    while (std::getline(stream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> x >> pid_;
      if (pid_ == LinuxParser::Uid(pid))
        break;
    }
  }
  return user; }

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  string line;
  string uptime_str;
  long uptime;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()){
    getline(stream, line);
    std::istringstream streamline(line);
    for (int i = 0; i<22; i++){
      streamline >> uptime_str;
    }
    uptime = stol(uptime_str)/sysconf(_SC_CLK_TCK);
  }
  return uptime; 
  }
