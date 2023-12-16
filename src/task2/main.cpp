#include "cstdio"
#include "map"
#include "numeric"
#include "string"
#include "vector"

// Paste test data in this string
std::string DATA = "";

std::string replace(std::string in, std::string token, std::string data) {
  std::string out = "";
  while (in.find(token) != std::string::npos) {
    in.replace(in.find(token), token.size(), data);
  }
  return in;
}

int main(int argc, char **argv) {
  std::vector<int> matches;
  std::vector<int> partial_sums;
  DATA = replace(DATA, "one", "o1e");
  DATA = replace(DATA, "two", "t2o");
  DATA = replace(DATA, "three", "t3e");
  DATA = replace(DATA, "four", "f4r");
  DATA = replace(DATA, "five", "f5e");
  DATA = replace(DATA, "six", "s6x");
  DATA = replace(DATA, "seven", "s7n");
  DATA = replace(DATA, "eight", "e8t");
  DATA = replace(DATA, "nine", "n9e");
  for (char c : DATA) {
    if (std::isdigit(c)) {
      matches.push_back(c - '0');
    } else if (c == '\n') {
      std::string number = std::to_string(matches.at(0)) +
                           std::to_string(matches.at(matches.size() - 1));
      int num = std::atoi(number.c_str());
      partial_sums.push_back(num);
      matches.clear();
      printf("Number: %d\n", num);
    }
  }
  printf("Sum: %d\n",
         std::accumulate(partial_sums.begin(), partial_sums.end(), 0));
}