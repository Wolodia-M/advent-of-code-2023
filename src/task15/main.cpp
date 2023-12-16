#include "../utils.hpp"
#include "map.hpp"
#include "sstream"
#include "stdio.h"
#include "string"
#include "vector"
int main(int argc, char **argv) {
  Map map("task15_16");
  MapEntry *curr_node = map.get_start_node();
  size_t counter = 0;
  size_t step_ptr = 0;
  std::string FILE_DATA = load_file("test15_part1");
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
  while (true) {
    if (map.is_end_node(curr_node)) {
      break;
    }
    curr_node = map.traverse_map(curr_node, list[step_ptr]);
    step_ptr++;
    if (step_ptr == len) {
      step_ptr = 0;
    }
    counter++;
  }
  printf("Steps needed: %lu\n", counter);
  return 0;
}