#include "../loader.hpp"
#include "algorithm"
#include "cstdio"
#include "cstring"
#include "sstream"
#include "string"
#include "vector"
struct pos {
  size_t x;
  size_t y;
};
class GalaxyMap {
private:
  char *map;
  size_t columns;
  size_t rows;
  size_t idx(size_t x, size_t y) { return x + y * this->columns; }
  bool check_row(size_t row) {
    bool galaxy = false;
    for (size_t i = 0; i < this->columns; i++) {
      if (this->map[this->idx(i, row)] == '#') {
        galaxy = true;
      }
    }
    return galaxy;
  }
  bool check_column(size_t column) {
    bool galaxy = false;
    // printf("Checking column %lu\n", column);
    for (size_t i = 0; i < this->rows; i++) {
      //   printf("%c", this->map[this->idx(column, i)]);
      if (this->map[this->idx(column, i)] == '#') {
        galaxy = true;
      }
    }
    // printf("\n%s\n", galaxy ? "Found galaxy" : "All clear");
    return galaxy;
  }

public:
  GalaxyMap(std::string path) {
    std::string FILE_DATA = load_file(path);
    auto lines = std::ranges::count(FILE_DATA, '\n');
    this->rows = lines;
    this->columns = FILE_DATA.find('\n');
    this->map = (char *)malloc(this->rows * this->columns);
    std::stringstream FILE_DATA_STREAM(FILE_DATA);
    std::string line;
    size_t y = 0;
    while (std::getline(FILE_DATA_STREAM, line, '\n')) {
      if ((line[0] == '.') || (line[0] == '#')) {
        for (size_t x = 0; x < line.size(); x++) {
          this->map[this->idx(x, y)] = line[x];
        }
      }
      y++;
    }
    this->print_map(this->map, this->rows, this->columns);
    std::vector<pos> galaxies;
    for (size_t i = 0; i < this->rows; i++) {
      for (size_t j = 0; j < this->columns; j++) {
        if (this->map[this->idx(j, i)] == '#') {
          galaxies.push_back({.x = j, .y = i});
        }
      }
    }
    unsigned long long sum = 0;
    for (auto g1 : galaxies) {
      for (auto g2 : galaxies) {
        if ((g1.x != g2.x) || (g1.y != g2.y)) {
          unsigned long long x1, x2, y1, y2; // Left, right, top, bottom
          x1 = std::min(g1.x, g2.x);
          x2 = std::max(g1.x, g2.x);
          y1 = std::min(g1.y, g2.y);
          y2 = std::max(g1.y, g2.y);
          // printf("x1: %lu, x2: %lu, y1: %lu, y2: %lu\n", x1, x2, y1, y2);
          unsigned long long plusx = 0;
          unsigned long long plusy = 0;
          for (size_t row = y1; row < y2; row++) {
            if (!this->check_row(row)) {
              plusy += 1;
            }
          }
          for (size_t col = x1; col < x2; col++) {
            if (!this->check_column(col)) {
              plusx += 1;
            }
          }
          // printf("plusx: %lu, plusy: %lu\n", plusx, plusy);
          unsigned long long distx = x2 - x1 + plusx;
          unsigned long long disty = y2 - y1 + plusy;
          unsigned long long dist = distx + disty;
          // printf("Distance: %lu\n", dist);
          sum += dist;
        }
      }
    }
    printf("Sum: %llu\n", sum / 2);
  }
  void print_map(char *ptr, size_t rows, size_t cols) {
    printf("Galaxy map:\n");
    printf("Rows: %lu, columns: %lu\n", rows, cols);
    for (size_t i = 0; i < rows; i++) {
      for (size_t j = 0; j < cols; j++) {
        printf("%c", ptr[this->idx(j, i)]);
      }
      printf("\n");
    }
  }
  void expand() {
    for (size_t row = 0; row < this->rows; row++) {
      if (!this->check_row(row)) {
        printf("Row: %lu\n", row);
        char *tmp = (char *)malloc((this->rows + 1) * this->columns);
        memset(tmp, ' ', (this->rows + 1) * this->columns);
        memcpy(tmp, this->map, (row + 1) * this->columns);
        memcpy(tmp + ((row + 1) * this->columns),
               this->map + row * this->columns,
               (this->columns * (this->rows - row)));
        this->rows++;
        row++;
        free(this->map);
        this->map = tmp;
        this->print_map(this->map, this->rows, this->columns);
      }
    }
    for (size_t column = 0; column < this->columns; column++) {
      if (!this->check_column(column)) {
        printf("Column: %lu\n", column);
        char *tmp = (char *)malloc(this->rows * (this->columns + 1));
        memset(tmp, '.', this->rows * (this->columns + 1));
        for (size_t i = 0; i < this->rows; i++) {
          for (size_t j = 0; j < (this->columns + 1); j++) {
            if (j <= column) {
              tmp[i * (this->columns + 1) + j] = this->map[this->idx(j, i)];
            } else if (j == column + 1) {
              tmp[i * (this->columns + 1) + j] = '.';
            } else if (j > (column + 1)) {
              tmp[i * (this->columns + 1) + j] = this->map[this->idx(j - 1, i)];
            }
          }
        }
        this->print_map(tmp, this->rows, this->columns + 1);
      }
    }
  }
  ~GalaxyMap() { free(this->map); }
};