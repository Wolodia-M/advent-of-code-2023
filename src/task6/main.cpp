#include "../utils.hpp"
#include "cstdio"
#include "filegrid.hpp"
#include "string"
#include "vector"
typedef struct possible_gear {
  int num;
  int x;
  int y;
  bool done;
} possible_gear;
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
  std::vector<possible_gear> posibilites;
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
        int push = false;
        std::vector<int> gears_x;
        std::vector<int> gears_y;
        for (int y = i1 - 1; y <= i1 + 1; y++) {
          for (int x = st_offcet - 1; x <= st_offcet + len; x++) {
            auto val2 = grid.at(x, y);
            char c = val2.value_or('.');
            if (c == '*') {
              push = true;
              gears_x.push_back(x);
              gears_y.push_back(y);
            }
          }
        }
        if (push) {
          for (int cnt = 0; cnt < gears_x.size(); cnt++) {
            posibilites.push_back({.num = std::atoi(num.c_str()),
                                   .x = gears_x.at(cnt),
                                   .y = gears_y.at(cnt),
                                   .done = false});
          }
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
  // Find real gears and i's ratios
  std::vector<int> ratios;
  for (int cnt1 = 0; cnt1 < posibilites.size(); cnt1++) {
    auto elem1 = posibilites.at(cnt1);
    if (elem1.done == false) {
      for (int cnt2 = 0; cnt2 < posibilites.size(); cnt2++) {
        auto elem2 = posibilites.at(cnt2);
        if (elem2.done == false) {
          if (cnt1 != cnt2 && elem1.x == elem2.x && elem1.y == elem2.y) {
            ratios.push_back(elem1.num * elem2.num);
          }
        }
      }
    }
  }
  printf("Sum: %d\n", sum_of_vec(&ratios) / 2);
}