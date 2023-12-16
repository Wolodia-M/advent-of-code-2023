#include "parse.hpp"

int main(int argc, char **argv) {
  DataRestorer data("test23_24");
  printf("Posibilites: %lu\n", data.get_posibilites());
  return 0;
}