#include "../utils.hpp"
#include "cstdio"
#include "grid.hpp"
#include "sstream"
#include "string"
#include "vector"
int main(int argc, char **argv) {
  PipeGrid p("test20_part1");
  p.calc_loop();
  printf("Closed places: %lu\n", p.get_closed_dots_count());
}