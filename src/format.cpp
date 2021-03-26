#include <string>

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
    return to_string(HH_num) + ":" + to_string(MM_num) + ":" + to_string(SS_num); 
    }