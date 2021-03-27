#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// Return this process's ID
int Process::Pid() { return this->pid_; }

// Return this process's CPU utilization
float Process::CpuUtilization() { 
    vector<float> time = LinuxParser::CpuUtilization(Process::Pid()); 
    float total_time = time[0] + time[1];
    total_time = total_time + time[2] + time[3];
    float hertz = sysconf(_SC_CLK_TCK);
    long uptime = LinuxParser::UpTime();
    float seconds = uptime - (time[4]/hertz);
    this->cpu_ = (total_time/hertz)/seconds;
    return this->cpu_;
    }

// Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_) ; }

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { return cpu_ < a.cpu_; }
