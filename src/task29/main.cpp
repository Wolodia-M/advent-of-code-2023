#include "../loader.hpp"
#include "sstream"
#include "string"

size_t HASH(std::string input) {
  size_t ret = 0;
  for (char c : input) {
    ret += c;
    ret *= 17;
    ret %= 256;
  }
  return ret;
}

int main(int argc, char **argv) {
  std::string DATA = load_file("test29_30");
  std::string line;
  std::stringstream DATA_STREAM(DATA);
  std::size_t sum = 0;
  while (std::getline(DATA_STREAM, line, ',')) {
    size_t hash = HASH(line);
    printf("'%s'\t=> %lu\n", line.c_str(), hash);
    sum += hash;
  }
  printf("Sum: %lu\n", sum);
  return 0;
}