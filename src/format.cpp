#include <string>

#include "format.h"

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function

string Format::AddPrecidingZero(long time) {
  std::string result;
  if(time < 10)
    result = "0" + to_string(time);
  else
    result = to_string(time);
  return result;
}

string Format::ElapsedTime(long seconds) {
  int hours = seconds / 3600;
  seconds = seconds % 3600;
  int minutes = seconds / 60;
  seconds = seconds % 60;
  return AddPrecidingZero(hours) + ":" + AddPrecidingZero(minutes) + ":" + AddPrecidingZero(seconds);
 }


 string Format::RamInKB(string ram) {
   int kb;
   try {
     kb = stoi(ram);
   }
   catch(...) {
     return "NA";
   }
   int mb = kb / 1024;
   return to_string(mb);
 }
