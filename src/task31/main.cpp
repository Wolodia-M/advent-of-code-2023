#include "cstdint"
#include "cstdio"
#include "grid.hpp"
int main(int argc, char **argv) {
  Grid grid("task31_32");
  printf("Energized tiles: %lu\n", grid.emmit_light(0, 0, Direction::RIGHT));
  return 0;
}