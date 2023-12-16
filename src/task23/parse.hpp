#include "../loader.hpp"
#include "algorithm"
#include "cstdio"
#include "regex"
#include "sstream"
#include "string"
#include "vector"

struct sequence {
  enum seq_type { OPERATIONAL, DAMAGED, UNDEFINED } type;
  std::size_t len;
  std::size_t start_idx;
  bool operator==(const sequence &other) { return (this->type == other.type); }
  bool operator==(const sequence::seq_type &other) {
    return (this->type == other);
  }
  std::string to_string() {
    return std::to_string(this->start_idx) + "->" +
           std::string(this->type == OPERATIONAL ? "'+'"
                       : this->type == DAMAGED   ? "'-'"
                                                 : "'?'") +
           ":" + std::to_string(this->len);
  }
};
struct report {
  std::vector<sequence> data;
  std::string orig_data;
  std::vector<size_t> rle;
  struct {
    size_t data;
    size_t rle;
  } ptrs;
  std::string to_string() {
    std::string ret = "";
    ret += "Data: [";
    for (auto seq : this->data) {
      ret += seq.to_string();
      ret += ", ";
    }
    ret = ret.substr(0, ret.find_last_of(','));
    ret += "], rle: [";
    for (auto num : this->rle) {
      ret += std::to_string(num);
      ret += ", ";
    }
    ret = ret.substr(0, ret.find_last_of(','));
    ret += "]\n";
    return ret;
  }
};
class DataRestorer {
private:
  std::vector<report> file;
  std::vector<sequence> tokenize(std::string data) {
    sequence::seq_type now;
    size_t last_len = 0;
    size_t last_pos = 0;
    char prev_char = ' ';
    std::vector<sequence> seqs;
    bool first = true;
    for (size_t i = 0; i < data.size(); i++) {
      char c = data[i];
      if (c != prev_char) {
        if (!first) {
          seqs.push_back({.type = now, .len = last_len, .start_idx = last_pos});
        }
        first = false;
        prev_char = c;
        last_len = 1;
        last_pos = i;
        switch (c) {
        case '.':
          now = sequence::OPERATIONAL;
          break;
        case '#':
          now = sequence::DAMAGED;
          break;
        case '?':
          now = sequence::UNDEFINED;
          break;
        }
      } else {
        last_len++;
      }
    }
    seqs.push_back({.type = now, .len = last_len, .start_idx = last_pos});
    return seqs;
  }
  std::string unfold(std::string in) {
    std::string ret;
    for (int i = 0; i < 5; i++) {
      ret += in;
      ret += '?';
    }
    return ret.substr(0, ret.find_last_of('?'));
  }
  std::vector<size_t> unfold(std::vector<size_t> in) {
    std::vector<size_t> ret;
    for (int i = 0; i < 5; i++) {
      ret.insert(ret.end(), in.begin(), in.end());
    }
    return ret;
  }

public:
  DataRestorer(std::string path) {
    std::string FILE_DATA = load_file(path);
    std::stringstream FILE_DATA_STREAM(FILE_DATA);
    std::string line, data, num_seq;
    while (std::getline(FILE_DATA_STREAM, line, '\n')) {
      data = line.substr(0, line.find(' '));
      num_seq = line.substr(line.find(' ') + 1);
      printf("Data: '%s', num_seq: '%s'\n", data.c_str(), num_seq.c_str());
      std::stringstream NUM_STREA(num_seq);
      std::vector<size_t> nums;
      std::string num;
      while (std::getline(NUM_STREA, num, ',')) {
        nums.push_back(std::strtoul(num.c_str(), NULL, 10));
      }
      file.push_back(
          {.orig_data = data, .rle = nums, .ptrs = {.data = 0, .rle = 0}});
    }
    // for (auto line : file) {
    // printf("%s", line.to_string().c_str());
    // }
  }
  size_t get_posibilites() {
    size_t sum = 0;
    size_t iter = 0;
    for (auto line : this->file) {
      printf("Iter %lu:\n", iter);
      fflush(stdout);
      std::string tmp = line.orig_data;
      std::string regex = "\\.*";
      for (size_t num : line.rle) {
        for (size_t k = 0; k < num; k++) {
          regex.push_back('#');
        }
        regex += "\\.+";
      }
      regex.pop_back();
      regex.push_back('*');
      std::regex test(regex);
      printf("Regex: %s\n", regex.c_str());
      fflush(stdout);
      size_t undefineds = std::ranges::count(tmp, '?');
      for (size_t i = 0; i <= ((size_t)1 << undefineds) - 1; i++) {
        std::string tmp1 = tmp;
        size_t found = 0;
        for (size_t j = 0; j < tmp1.size(); j++) {
          if (tmp1[j] == '?') {
            tmp1[j] = (i & (0x1 << found)) ? '.' : '#';
            found++;
          }
        }
        if (std::regex_match(tmp1, test)) {
          sum++;
        }
        // printf("Test line: %s\n", tmp1.c_str());
        // fflush(stdout);
        auto tokenized = this->tokenize(tmp1);
        size_t tokens_matched = 0;
        line.ptrs.rle = 0;
        line.ptrs.data = 0;
      }
      iter++;
    }
    return sum;
  }
};