#include "../loader.hpp"
#include "sstream"
#include "string"
#include "vector"

size_t HASH(std::string input) {
  size_t ret = 0;
  for (char c : input) {
    ret += c;
    ret *= 17;
    ret %= 256;
  }
  return ret;
}

struct LENS {
  std::string id;
  size_t focal_len;
  bool operator==(LENS &other) { return this->id == other.id; }
  bool operator>(LENS &other) { return this->focal_len > other.focal_len; }
  bool operator<(LENS &other) { return this->focal_len < other.focal_len; }
};

struct BOX {
  std::vector<LENS> lenses;
  void add_lens(LENS lens) {
    for (size_t i = 0; i < this->lenses.size(); i++) {
      if (this->lenses[i] == lens) {
        this->lenses[i].focal_len = lens.focal_len;
        return;
      }
    }
    this->lenses.push_back(lens);
  }
  void remove_lens(LENS lens) {
    std::vector<LENS> new_vec;
    for (auto l : this->lenses) {
      if (l == lens) {
      } else {
        new_vec.push_back(l);
      }
      this->lenses = new_vec;
    }
  }
  size_t get_focusing_power(size_t box_id) {
    if (this->lenses.empty()) {
      return 0;
    }
    size_t ret = 0;
    for (size_t i = 0; i < this->lenses.size(); i++) {
      size_t power = (1 + box_id) * (1 + i) * this->lenses[i].focal_len;
      ret += power;
    }
    return ret;
  }
};

class HASHMAP {
private:
  std::vector<BOX> boxes;

public:
  HASHMAP() {
    for (int i = 0; i < 256; i++) {
      this->boxes.push_back({.lenses = {}});
    }
  }
  void add_lens(size_t box_id, std::string lens, size_t fl) {
    this->boxes.at(box_id).add_lens({.id = lens, .focal_len = fl});
  }
  void remove_lens(size_t box_id, std::string lens) {
    this->boxes.at(box_id).remove_lens({.id = lens, .focal_len = 0});
  }
  void print() {
    for (int i = 0; i < 256; i++) {
      auto box = this->boxes[i];
      if (!box.lenses.empty()) {
        printf("Box %d: ", i);
        for (auto lens : box.lenses) {
          printf("[%s %lu] ", lens.id.c_str(), lens.focal_len);
        }
        printf("\n");
      }
    }
  }
  size_t get_focusing_power() {
    size_t ret = 0;
    for (size_t i = 0; i < this->boxes.size(); i++) {
      ret += this->boxes[i].get_focusing_power(i);
    }
    return ret;
  }
};

int main(int argc, char **argv) {
  HASHMAP boxes;
  std::string DATA = load_file("test29_30");
  std::string line;
  std::stringstream DATA_STREAM(DATA);
  while (std::getline(DATA_STREAM, line, ',')) {
    printf("Line: '%s'\n", line.c_str());

    std::string lens_id = [&line]() -> std::string {
      if (line.find('-') != std::string::npos) {
        return line.substr(0, line.find('-'));
      } else if (line.find('=') != std::string::npos) {
        return line.substr(0, line.find('='));
      } else {
        return "";
      }
    }();
    printf("Lens id: '%s'\n", lens_id.c_str());
    size_t box_id = HASH(lens_id);
    printf("Box id: '%lu'\n", box_id);
    char op = (line.find('-') != std::string::npos) ? '-' : '=';
    printf("Operation: '%c'\n", op);
    int focal_len = [&line]() -> int {
      if (line.find('=') != std::string::npos) {
        return std::atoi(line.substr(line.find('=') + 1).c_str());
      } else {
        return -1;
      }
    }();
    printf("Focal length: '%d'\n", focal_len);
    if (op == '-') {
      boxes.remove_lens(box_id, lens_id);
    } else if (op == '=') {
      boxes.add_lens(box_id, lens_id, focal_len);
    }
    boxes.print();
    fflush(stdout);
  }
  printf("Focusing power: %lu\n", boxes.get_focusing_power());
  return 0;
}