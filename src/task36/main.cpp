#include "grid.hpp"
int main(int argc, char **argv) {
  Grid grid("tesk35_36");
  grid.perform_digging();
  printf("Cubic meters: %f\n", grid.get_area());
  return 0;
}