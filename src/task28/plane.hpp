#include "../loader.hpp"
#include "algorithm"
#include "cstdio"
#include "cstdlib"
#include "cstring"
#include "sstream"
#include "string"
#include "vector"
enum entry : signed char { ROCK = '#', BOULDER = 'O', NONE = '.' };
class Plane {
private:
  entry *plane;
  size_t rows;
  size_t columns;
  std::vector<std::string> done;
  size_t idx(size_t x, size_t y) { return x + y * this->columns; }
  void tilt_north_internal() {
    for (size_t i = 1; i < this->rows; i++) {
      for (size_t j = 0; j < this->columns; j++) {
        if (this->plane[this->idx(j, i)] == entry::BOULDER) {
          if (this->plane[this->idx(j, i - 1)] == entry::NONE) {
            this->plane[this->idx(j, i - 1)] = entry::BOULDER;
            this->plane[this->idx(j, i)] = entry::NONE;
          }
        }
      }
    }
  }
  void tilt_south_internal() {
    for (size_t i = 0; i < this->rows - 1; i++) {
      for (size_t j = 0; j < this->columns; j++) {
        if (this->plane[this->idx(j, i)] == entry::BOULDER) {
          if (this->plane[this->idx(j, i + 1)] == entry::NONE) {
            this->plane[this->idx(j, i + 1)] = entry::BOULDER;
            this->plane[this->idx(j, i)] = entry::NONE;
          }
        }
      }
    }
  }
  void tilt_east_internal() {
    for (size_t i = 0; i < this->rows; i++) {
      for (size_t j = 0; j < this->columns - 1; j++) {
        if (this->plane[this->idx(j, i)] == entry::BOULDER) {
          if (this->plane[this->idx(j + 1, i)] == entry::NONE) {
            this->plane[this->idx(j + 1, i)] = entry::BOULDER;
            this->plane[this->idx(j, i)] = entry::NONE;
          }
        }
      }
    }
  }
  void tilt_west_internal() {
    for (size_t i = 0; i < this->rows; i++) {
      for (size_t j = 1; j < this->columns; j++) {
        if (this->plane[this->idx(j, i)] == entry::BOULDER) {
          if (this->plane[this->idx(j - 1, i)] == entry::NONE) {
            this->plane[this->idx(j - 1, i)] = entry::BOULDER;
            this->plane[this->idx(j, i)] = entry::NONE;
          }
        }
      }
    }
  }

public:
  Plane(std::string path) {
    std::string FILE_DATA = load_file(path);
    std::stringstream FILE_DATA_STREAM(FILE_DATA);
    std::string line;
    this->rows = std::ranges::count(FILE_DATA, '\n');
    this->columns = FILE_DATA.substr(0, FILE_DATA.find('\n')).size();
    printf("Rows: %lu, columns: %lu\n", this->rows, this->columns);
    this->plane = (entry *)malloc(this->rows * this->columns);
    memcpy(
        this->plane,
        [&FILE_DATA, &line]() -> char * {
          line = FILE_DATA;
          while (line.find('\n') != std::string::npos) {
            line.replace(line.find('\n'), 1, "");
          }
          return line.data();
        }(),
        this->rows *this->columns);
    for (size_t i = 0; i < this->rows; i++) {
      for (size_t j = 0; j < this->columns; j++) {
        printf("%c", this->plane[this->idx(j, i)]);
      }
      printf("\n");
    }
  }
  void tilt_north() {
    for (size_t i = 0; i < this->rows; i++) {
      this->tilt_north_internal();
    }
    // printf("Plane after tilt north:\n");
    // for (size_t i = 0; i < this->rows; i++) {
    // for (size_t j = 0; j < this->columns; j++) {
    // printf("%c", this->plane[this->idx(j, i)]);
    // }
    // printf("\n");
    // }
  }
  void tilt_south() {
    for (size_t i = 0; i < this->rows; i++) {
      this->tilt_south_internal();
    }
    // printf("Plane after tilt south:\n");
    // for (size_t i = 0; i < this->rows; i++) {
    // for (size_t j = 0; j < this->columns; j++) {
    // printf("%c", this->plane[this->idx(j, i)]);
    // }
    // printf("\n");
    // }
  }
  void tilt_east() {
    for (size_t i = 0; i < this->rows; i++) {
      this->tilt_east_internal();
    }
    // printf("Plane after tilt east:\n");
    // for (size_t i = 0; i < this->rows; i++) {
    // for (size_t j = 0; j < this->columns; j++) {
    // printf("%c", this->plane[this->idx(j, i)]);
    // }
    // printf("\n");
    // }
  }
  void tilt_west() {
    for (size_t i = 0; i < this->rows; i++) {
      this->tilt_west_internal();
    }
    // printf("Plane after tilt west:\n");
    // for (size_t i = 0; i < this->rows; i++) {
    // for (size_t j = 0; j < this->columns; j++) {
    // printf("%c", this->plane[this->idx(j, i)]);
    // }
    // printf("\n");
    // }
  }
  size_t calculate_load_north(size_t id) {
    size_t ret = 0;
    entry *tmp = (entry *)malloc(this->rows * this->columns);
    for (size_t i = 0; i < this->rows; i++) {
      for (size_t j = 0; j < this->columns; j++) {
        tmp[this->idx(j, i)] =
            static_cast<entry>(this->done.at(id)[this->idx(j, i)]);
      }
    }
    for (size_t i = 0; i < this->rows; i++) {
      for (size_t j = 0; j < this->columns; j++) {
        if (tmp[this->idx(j, i)] == entry::BOULDER) {
          ret += this->rows - i;
        }
      }
    }
    return ret;
  }
  size_t do_work() {
    size_t i;
    size_t start;
    printf("Tilting...\n");
    for (i = 0; i < 1000000000; i++) {
      if (i % 1000000 == 0) {
        printf(".");
        fflush(stdout);
      }
      this->tilt_north();
      this->tilt_west();
      this->tilt_south();
      this->tilt_east();
      std::string str_data((char *)this->plane, this->rows * this->columns);
      std::string idx = str_data;
      for (size_t j = 0; j < this->done.size(); j++) {
        if (this->done[j] == idx) {
          start = j;
          break;
        }
      }
      this->done.push_back(idx);
    }
    printf("\n");
    return this->calculate_load_north((1000000000 - i) % (start - i) + i - 1);
  }
  ~Plane() { free(this->plane); }
};
