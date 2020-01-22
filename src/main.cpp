#include "ncurses_display.h"
#include "system.h"
#include "linux_parser.h"

int main() {
  System system;
  system.Processes(LinuxParser::AllProcesses());
  NCursesDisplay::Display(system, system.Processes().size());
}
