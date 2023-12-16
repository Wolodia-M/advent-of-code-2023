#include "../utils.hpp"
#include "cstdio"
#include "grid.hpp"
#include "sstream"
#include "string"
#include "vector"
int main(int argc, char **argv) {
  PipeGrid p("test19_part1");
  printf("Max distance: %lu\n", p.find_largest_distance());
}