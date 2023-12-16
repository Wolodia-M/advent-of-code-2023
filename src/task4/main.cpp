#include "../loader.hpp"
#include "../utils.hpp"
#include "sstream"
#include "stdio.h"
#include "string"
#include "vector"

struct game_state {
  int red;
  int blue;
  int green;
};

int main(int argc, char **argv) {
  std::string DATA = load_file("test3_4");
  DATA += '\n';
  printf("%s\n", DATA.c_str());
  std::vector<game_state> games;
  std::string line;
  std::stringstream data_stream(DATA);
  while (std::getline(data_stream, line, '\n')) {
    games.push_back({0, 0, 0});
    int cnt = 5;
    std::string tmp = line.substr(cnt, line.find(':') - cnt);
    cnt += (line.find(':') - cnt + 2);
    line = line.substr(cnt);
    int iter = std::atoi(tmp.c_str());
    try {
      while (true) {
        cnt = 0;
        int pos = line.find(';');
        std::string game = line.substr(cnt, line.find(';'));
        cnt += (line.find(';') - cnt + 2);
        line = line.substr(cnt);
        printf("Remaining line: \"%s\", current game: \"%s\"\n", line.c_str(),
               game.c_str());
        std::stringstream stream(game);
        std::string line1, line2;
        while (std::getline(stream, line1, ' ')) {
          std::getline(stream, line2, ' ');
          printf("Num: %d, color: %s\n", std::atoi(line1.c_str()),
                 line2.c_str());
          if ((line2 == std::string("red")) || (line2 == std::string("red,"))) {
            if (std::atoi(line1.c_str()) > games.back().red) {
              games.back().red = std::atoi(line1.c_str());
            }
          } else if ((line2 == std::string("green")) ||
                     (line2 == std::string("green,"))) {
            if (std::atoi(line1.c_str()) > games.back().green) {
              games.back().green = std::atoi(line1.c_str());
            }
          } else if ((line2 == std::string("blue")) ||
                     (line2 == std::string("blue,"))) {
            if (std::atoi(line1.c_str()) > games.back().blue) {
              games.back().blue = std::atoi(line1.c_str());
            }
          }
        }
        if (pos == std::string::npos) {
          throw std::exception();
        }
      }
    } catch (std::exception &e) {
    }
  }
  // int sum = 0;
  // std::ranges::for_each(games, [&sum](const game_state &val) {
  //   sum += val.red * val.blue * val.green;
  // });
  // printf("Sum: %d\n", sum);
  printf("Sum: %d\n",
         sum_of_vec_op<game_state>(&games, [](game_state val) -> int {
           return val.red * val.blue * val.green;
         }));
  return 0;
}