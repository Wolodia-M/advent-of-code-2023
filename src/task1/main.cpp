#include "cstdio"
#include "numeric"
#include "string"
#include "vector"
// Paste test data in this string
const std::string DATA = "";

int main(int argc, char **argv) {
  std::vector<int> matches;
  std::vector<int> partial_sums;
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