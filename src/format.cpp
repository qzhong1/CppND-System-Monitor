#include <string>
#include <iomanip>

#include "format.h"

using std::string;
using std::to_string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) { 
    long HH_num, MM_num, SS_num;
    HH_num = seconds/3600;
    MM_num = (seconds - HH_num*3600)/60;
    SS_num = seconds - HH_num*3600 - MM_num*60;
    std::ostringstream elapsedTime;
    elapsedTime << std::setw(2) << std::setfill('0') << HH_num 
              << ":" << std::setw(2) << std::setfill('0') << MM_num
              << ":" << std::setw(2) << std::setfill('0') << SS_num;
    return elapsedTime.str();
    }