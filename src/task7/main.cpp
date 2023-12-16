#include "../loader.hpp"
#include "../utils.hpp"
#include "cstdio"
#include "sstream"
#include "string"
#include "vector"
int main(int argc, char **argv) {
  std::string data = load_file("test7_8");
  std::stringstream stream(data);
  std::string line;
  std::vector<int> cards;
  while (std::getline(stream, line, '\n')) {
    cards.push_back(0);
    line = line.substr(line.find(':') + 2);
    printf("Wins | card: %s\n", line.c_str());
    std::vector<int> wins, nums;
    std::stringstream line_stream(line);
    std::string num;
    int state = 0; // 0 -> win numbers, 1 - card numbers
    while (std::getline(line_stream, num, ' ')) {
      if (num == std::string("|")) {
        state = 1;
        continue;
      }
      if ((state == 0) && std::isdigit(num[0])) {
        wins.push_back(std::atoi(num.c_str()));
      } else if (state == 1) {
        nums.push_back(std::atoi(num.c_str()));
      }
    }
    for (int number : nums) {
      for (int win : wins) {
        if (number == win) {
          printf("Num: %d, win_num: %d\n", number, win);
          if (cards.back() == 0) {
            cards.back() = 1;
          } else {
            cards.back() = cards.back() * 2;
          }
        }
      }
    }
    printf("Card sum: %d\n", cards.back());
  }
  printf("Sum: %d\n", sum_of_vec(&cards));
}