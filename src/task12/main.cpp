#include "../loader.hpp"
#include "../utils.hpp"
#include "stdio.h"
#include "string"
int main(int argc, char **argv) {
  std::string FILE_DATA = load_file("test11_12");
  std::string LINE1 = FILE_DATA.substr(0, FILE_DATA.find('\n'));
  std::string LINE2 = FILE_DATA.substr(FILE_DATA.find('\n') + 1);
  LINE1 = LINE1.substr(10);
  LINE2 = LINE2.substr(10);
  LINE1 = string_replace(LINE1, " ", "");
  LINE2 = string_replace(LINE2, " ", "");
  unsigned long int distance = std::strtoul(LINE2.c_str(), NULL, 10);
  unsigned long int time = std::strtoul(LINE1.c_str(), NULL, 10);
  printf("Time: %lums, distance: %lumm\n", time, distance);
  unsigned long int win_counts = 0;
  for (unsigned long int i = 0; i <= time; i++) {
    unsigned long int speed = i;
    unsigned long int run_time = time - i;
    unsigned long int distance_traveled = speed * run_time;
    if (distance_traveled > distance) {
      win_counts++;
    }
  }
  printf("Win posibilites: %lu\n", win_counts);
}