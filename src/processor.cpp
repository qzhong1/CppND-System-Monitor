#include <string>
#include <sstream>
#include "processor.h"
#include "linux_parser.h"

using std::string;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    string line, key;
    float NonIdle, Total, totald, idled;
    float user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
    float cpu_percentage = 0.0;
    std::ifstream stream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
    if (stream.is_open()){
        while (std::getline(stream, line)) {
            std::istringstream linestream(line);
            linestream >> key >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
            if (key == "cpu")
                break;
        }
        // Calculate CPU usage percentage
        this->PrevIdle = this->PrevIdle + this->PrevIowait;
        idle = idle + iowait;
        NonIdle = user + nice + system + irq + softirq + steal;
        this->PrevTotal = this->PrevIdle + this->PrevNonIdle;
        Total = idle + NonIdle;
        totald = Total - this->PrevTotal;
        idled = idle - this->PrevIdle;
        cpu_percentage = (totald - idled) / totald;
        // Update previous values
        this->PrevIdle = idle;
        this->PrevIowait = iowait;
        this->PrevTotal = Total;
        this->PrevNonIdle = NonIdle;
    }
    return cpu_percentage; }