#include "../utils.hpp"
#include "map.hpp"
#include "sstream"
#include "stdio.h"
#include "string"
#include "vector"
int main(int argc, char **argv) {
  Map map("task15_16");
  std::vector<MapEntry *> curr_nodes = map.get_start_node();
  std::string FILE_DATA = load_file("test16");
  FILE_DATA = FILE_DATA.substr(0, FILE_DATA.find('\n'));
  size_t len = FILE_DATA.size();
  Direction list[len];
  for (int i = 0; i < len; i++) {
    if (FILE_DATA[i] == 'R') {
      list[i] = Direction::Right;
    } else if (FILE_DATA[i] == 'L') {
      list[i] = Direction::Left;
    }
  }
  std::vector<size_t> counters;
  for (int i = 0; i < curr_nodes.size(); i++) {
    size_t counter = 0;
    size_t step_ptr = 0;
    while (true) {
      if (map.is_end_node(curr_nodes.at(i))) {
        break;
      }
      curr_nodes.at(i) = map.traverse_map(curr_nodes.at(i), list[step_ptr]);
      step_ptr++;
      if (step_ptr == len) {
        step_ptr = 0;
      }
      counter++;
    }
    counters.push_back(counter);
  }
  size_t steps = std::lcm(counters.at(0), counters.at(1));
  for (int i = 2; i < counters.size(); i++) {
    steps = std::lcm(steps, counters.at(i));
  }
  printf("Steps needed: %lu\n", steps);
  return 0;
}