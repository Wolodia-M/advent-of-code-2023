#include "../loader.hpp"
#include "../utils.hpp"
#include "cstdio"
#include "sstream"
#include "string"
#include "vector"
typedef struct card {
  int id;
  int wins;
  int copies;
  std::vector<int> nums;
  std::vector<int> win_nums;
} card;
int main(int argc, char **argv) {
  std::vector<card> cards;
  std::string FILE_DATA = load_file("test7_8");
  std::stringstream FILE_DATA_STREAM(FILE_DATA);
  std::string line;
  while (std::getline(FILE_DATA_STREAM, line, '\n')) {
    line = line.substr(5);
    int id = std::atoi(line.substr(0, 1).c_str());
    line = line.substr(3);
    std::stringstream line_stream(line);
    std::vector<int> nums;
    std::vector<int> win_nums;
    std::string num;
    bool second_phase = false;
    while (std::getline(line_stream, num, ' ')) {
      if (num == std::string("|")) {
        second_phase = true;
        continue;
      }
      if (std::isdigit(num[0])) {
        if (second_phase) {
          nums.push_back(std::atoi(num.c_str()));
        } else {
          win_nums.push_back(std::atoi(num.c_str()));
        }
      }
    }
    int wins = 0;
    for (int number : nums) {
      for (int win : win_nums) {
        if (number == win) {
          wins++;
        }
      }
    }
    cards.push_back({.id = id,
                     .wins = wins,
                     .copies = 1,
                     .nums = nums,
                     .win_nums = win_nums});
  }
  for (int i = 0; i < cards.size(); i++) {
    card c = cards.at(i);
    if (c.wins > 0) {
      for (int j = 1; j <= c.wins; j++) {
        if ((i + j) < cards.size()) {
          cards.at(i + j).copies += c.copies;
        }
      }
    }
  }
  for (auto c : cards) {
    printf("Card: %d, copies: %d\n", c.id, c.copies);
  }
  printf("Sum: %d\n", sum_of_vec_op<card>(&cards, [](card elem) -> int {
           return elem.copies;
         }));
}