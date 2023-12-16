#include "../loader.hpp"
#include "cstdint"
#include "cstdlib"
#include "vector"
class Extrapolator {
private:
  std::vector<int32_t> input_data;
  std::vector<std::vector<int32_t>> extrapolated_values;
  size_t get_diff_count(size_t deep) { return this->input_data.size() - deep; }
  std::vector<int32_t> *get_deep_ptr(int deep) {
    if (deep == 0) {
      return &(this->input_data);
    } else {
      return &(this->extrapolated_values.data()[deep - 1]);
    }
  }

public:
  Extrapolator(std::vector<int32_t> values) {
    this->input_data = values;
    int deep = 1;
    while (true) {
      this->extrapolated_values.push_back({});
      auto in_ptr = get_deep_ptr(deep - 1);
      auto out_ptr = get_deep_ptr(deep);
      out_ptr->resize(this->get_diff_count(deep));
      fflush(stdout);
      for (size_t i = 0; i < out_ptr->size(); i++) {
        int32_t val1 = in_ptr->at(i);
        int32_t val2 = in_ptr->at(i + 1);
        out_ptr->at(i) = val2 - val1;
      }
      bool zero = true;
      for (size_t i = 0; i < out_ptr->size(); i++) {
        if (out_ptr->at(i) != 0) {
          zero = false;
        }
      }
      if (zero) {
        break;
      }
      deep++;
    }
    printf("Input: ");
    for (size_t i = 0; i < this->input_data.size(); i++) {
      printf("%d ", this->input_data[i]);
    }
    printf("\nExtrapolated values: \n");
    for (size_t i = 1; i < this->extrapolated_values.size(); i++) {
      auto ptr = this->get_deep_ptr(i);
      for (size_t j = 0; j < this->get_diff_count(i); j++) {
        printf("%d ", ptr->at(j));
      }
      printf("\n");
    }
  }
  int32_t predict() {
    int32_t ret;
    int32_t last_diff = 0;
    size_t i = this->extrapolated_values.size();
    while (i > 0) {
      i--;
      // printf("i: %lu\n", i);
      last_diff = this->extrapolated_values.at(i).back() + last_diff;
      // printf("last_diff: %d\n", last_diff);
    }
    return this->input_data.back() + last_diff;
  }
};
