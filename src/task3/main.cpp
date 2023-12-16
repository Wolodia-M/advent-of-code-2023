#include "../loader.hpp"
#include "numeric"
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
  game_state global_state = {.red = 12, .blue = 14, .green = 13};
  std::vector<int> proper;
  std::string line;
  std::stringstream data_stream(DATA);
  while (std::getline(data_stream, line, '\n')) {
    int cnt = 5;
    std::string tmp = line.substr(cnt, line.find(':') - cnt);
    cnt += (line.find(':') - cnt + 2);
    line = line.substr(cnt);
    int iter = std::atoi(tmp.c_str());
    bool err = false;
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
            if (std::atoi(line1.c_str()) > global_state.red) {
              err = true;
            }
          } else if ((line2 == std::string("green")) ||
                     (line2 == std::string("green,"))) {
            if (std::atoi(line1.c_str()) > global_state.green) {
              err = true;
            }
          } else if ((line2 == std::string("blue")) ||
                     (line2 == std::string("blue,"))) {
            if (std::atoi(line1.c_str()) > global_state.blue) {
              err = true;
            }
          }
        }
        if (pos == std::string::npos) {
          throw std::exception();
        }
      }
    } catch (std::exception &e) {
    }
    if (!err) {
      proper.push_back(iter);
    }
  }
  printf("Sum: %d\n", std::accumulate(proper.begin(), proper.end(), 0));
  return 0;
}