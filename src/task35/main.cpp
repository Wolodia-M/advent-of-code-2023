#include "grid.hpp"
int main(int argc, char **argv) {
  Grid grid("test35_36");
  grid.perform_digging();
  grid.dig_inside();
  printf("Cubic meters: %lu\n", grid.get_volume());
  return 0;
}