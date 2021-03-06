#ifndef FORMAT_H
#define FORMAT_H

#include <string>

namespace Format {

  std::string AddPrecidingZero(long time);

  std::string ElapsedTime(long times);


  std::string RamInKB(std::string ram);

};

#endif
