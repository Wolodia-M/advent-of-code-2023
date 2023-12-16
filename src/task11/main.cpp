#include "../loader.hpp"
#include "../utils.hpp"
#include "sstream"
#include "stdio.h"
#include "string"
#include "vector"
typedef struct race_info {
  int time;
  int distance;
} race_info;
int main(int argc, char **argv) {
  std::vector<race_info> races;
  std::string FILE_DATA = load_file("test11_12");
  std::string LINE1 = FILE_DATA.substr(0, FILE_DATA.find('\n'));
  std::string LINE2 = FILE_DATA.substr(FILE_DATA.find('\n') + 1);
  printf("Line1: %s,\nLine2: %s\n", LINE1.c_str(), LINE2.c_str());
  std::stringstream LINE1_STREAM(LINE1);
  std::stringstream LINE2_STREAM(LINE2);
  std::string tmp;
  while (std::getline(LINE1_STREAM, tmp, ' ')) {
    if (std::isdigit(tmp[0])) {
      races.push_back({.time = std::atoi(tmp.c_str()), .distance = -1});
    }
  }
  while (std::getline(LINE2_STREAM, tmp, ' ')) {
    if (std::isdigit(tmp[0])) {
      for (int i = 0; i < races.size(); i++) {
        if (races.at(i).distance == -1) {
          races.at(i).distance = std::atoi(tmp.c_str());
          break;
        }
      }
    }
  }
  for (auto elem : races) {
    printf("Race: distance - %dmm, time - %dms\n", elem.distance, elem.time);
  }
  std::vector<int> win_conditions;
  for (auto race : races) {
    win_conditions.push_back(0);
    for (int i = 0; i <= race.time; i++) {
      int speed = i;
      int run_time = race.time - i;
      int distance_traveled = speed * run_time;
      if (distance_traveled > race.distance) {
        win_conditions.back()++;
      }
    }
  }
  printf("Mul: %d\n", std::reduce(win_conditions.begin(), win_conditions.end(),
                                  1, std::multiplies<>()));
}