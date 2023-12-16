#include "../utils.hpp"
#include "cstdio"
#include "filegrid.hpp"
#include "string"
#include "vector"
int main(int argc, char **argv) {
  Grid grid("test5_6");
  // Print
  printf("Schematic:\n");
  int once = 0;
  int i = 0;
  while (true) {
    int j = 0;
    while (true) {
      auto val = grid.at(j, i);
      if (!val.has_value()) {
        break;
      }
      once = 0;
      printf("%c", val.value());
      j++;
    }
    printf("\n");
    i++;
    once++;
    if (once > 1) {
      break;
    }
  }
  // Parse
  std::vector<int> parts;
  int once1 = 0;
  int i1 = 0;
  while (true) {
    int j1 = 0;
    while (true) {
      auto val = grid.at(j1, i1);
      if (!val.has_value()) {
        break;
      }
      std::string num = "";
      once1 = 0;
      if (std::isdigit(val.value())) {
        num += val.value();
        int len = 1;
        int st_offcet = j1;
        while (true) {
          j1++;
          auto val1 = grid.at(j1, i1);
          if (!val.has_value()) {
            break;
          }
          if (!std::isdigit(val1.value())) {
            break;
          }
          num += val1.value();
          len++;
        }
        bool push = false;
        for (int y = i1 - 1; y <= i1 + 1; y++) {
          for (int x = st_offcet - 1; x <= st_offcet + len; x++) {
            auto val2 = grid.at(x, y);
            char c = val2.value_or('.');
            if ((std::isdigit(c) == false) && (c != '.')) {
              push = true;
            }
          }
        }
        if (push) {
          parts.push_back(std::atoi(num.c_str()));
        }
      }
      j1++;
    }
    i1++;
    once1++;
    if (once1 > 1) {
      break;
    }
  }
  printf("Sum: %d\n", sum_of_vec(&parts));
}