#include "cstdio"
#include "plane.hpp"
int main(int argc, char **argv) {
  Plane plane("test27_28");
  printf("\nLoad: %lu\n", plane.do_work());
  return 0;
}