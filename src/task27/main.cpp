#include "cstdio"
#include "plane.hpp"
int main(int argc, char **argv) {
  Plane plane("test27_28");
  plane.tilt_north();
  printf("\nLoad: %lu\n", plane.calculate_load_north());
  return 0;
}