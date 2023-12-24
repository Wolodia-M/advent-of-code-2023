#include "cstdio"
#include "path.hpp"

int main(int argc, char **argv) {
  Map map("task45_46");
  Position start = map.get_start_pos();
  Position end = map.get_end_pos();
  printf("Start: [x: %d, y: %d] at %d\n", start.column, start.row, start.idx);
  printf("End: [x: %d, y: %d] at %d\n", end.column, end.row, end.idx);
  auto found_path = map.find_path(start, end);
  printf("Longest path: %lu\n", found_path.size() - 1);
  return 0;
}