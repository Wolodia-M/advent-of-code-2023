#include "../loader.hpp"
#include "../utils.hpp"
#include "cstdint"
#include "extrapolator.hpp"
#include "sstream"
#include "string"
#include "vector"
int main(int argc, char **argv) {

  std::string FILE_DATA = load_file("test17_18");
  std::stringstream FILE_DATA_STREAM(FILE_DATA);
  std::string LINE, number;
  std::vector<int32_t> predicts;
  while (std::getline(FILE_DATA_STREAM, LINE, '\n')) {
    std::stringstream LINE_STREAM(LINE);
    std::vector<int32_t> nums;
    while (std::getline(LINE_STREAM, number, ' ')) {
      int32_t num32 = std::strtod(number.c_str(), NULL);
      nums.push_back(num32);
    }
    Extrapolator e(nums);
    // printf("Predict: %d\n", e.predict());
    predicts.push_back(e.predict());
  }
  printf("Sum: %d\n", sum_of_vec(&predicts));
}