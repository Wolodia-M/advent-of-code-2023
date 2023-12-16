#include "../loader.hpp"
#include "cstdio"
#include "map.hpp"
#include "sstream"
#include "string"
#include "vector"

int main(int argc, char **argv) {
  Map map("test25_26");
  size_t sum = 0;
  for (size_t i = 0; i < map.patterns_count(); i++) {
    sum += map.get_pattern_refrection(i);
  }
  printf("Sum: %lu\n", sum);
  return 0;
}