#include "../loader.hpp"
#include "../utils.hpp"
#include "algorithm"
#include "string"
#include "vector"

struct Node {
  bool visited;
  size_t x;
  size_t y;
  size_t weight;
  size_t curr_distance;
};
class Graph {
private:
  std::vector<Node> grid;
  size_t rows;
  size_t cols;
  size_t idx(size_t x, size_t y) { return x + y * this->cols; }
  void dijkstra(size_t prev_x, size_t prev_y, size_t prev_prev_x,
                size_t prev_prev_y, size_t new_x, size_t new_y, size_t dest_x,
                size_t dest_y) {
    size_t x = new_y;
    size_t y = new_x;
    std::vector<std::pair<size_t, size_t>> edges;
    if (this->grid[this->idx(x, y)].visited) {
      return;
    }
    bool found = false;
    this->grid.at(this->idx(x, y)).visited = true;
    if (x < (this->cols - 1)) {
      if (!this->grid.at(this->idx(x + 1, y)).visited) {
        size_t new_distance = this->grid.at(this->idx(x + 1, y)).weight +
                              this->grid.at(this->idx(x, y)).curr_distance;
        if (this->grid.at(this->idx(x + 1, y)).curr_distance > new_distance) {
          this->grid.at(this->idx(x + 1, y)).curr_distance = new_distance;
        }
        edges.push_back(std::make_pair(x + 1, y));
      }
    }
    if (x > 0) {
      if (!this->grid.at(this->idx(x - 1, y)).visited) {
        size_t new_distance = this->grid.at(this->idx(x - 1, y)).weight +
                              this->grid.at(this->idx(x, y)).curr_distance;
        if (this->grid.at(this->idx(x - 1, y)).curr_distance > new_distance) {
          this->grid.at(this->idx(x - 1, y)).curr_distance = new_distance;
        }
        edges.push_back(std::make_pair(x - 1, y));
      }
    }
    if (y < (this->cols - 1)) {
      if (!this->grid.at(this->idx(x, y + 1)).visited) {
        size_t new_distance = this->grid.at(this->idx(x, y + 1)).weight +
                              this->grid.at(this->idx(x, y)).curr_distance;
        if (this->grid.at(this->idx(x, y + 1)).curr_distance > new_distance) {
          this->grid.at(this->idx(x, y + 1)).curr_distance = new_distance;
        }
        edges.push_back(std::make_pair(x, y + 1));
      }
    }
    if (y > 0) {
      if (!this->grid.at(this->idx(x, y - 1)).visited) {
        size_t new_distance = this->grid.at(this->idx(x, y - 1)).weight +
                              this->grid.at(this->idx(x, y)).curr_distance;
        if (this->grid.at(this->idx(x, y - 1)).curr_distance > new_distance) {
          this->grid.at(this->idx(x, y - 1)).curr_distance = new_distance;
        }
        edges.push_back(std::make_pair(x, y - 1));
      }
    }
    for (auto next : edges) {
      printf("X:%lu,Y:%lu,E:%lu\n", x, y, edges.size());
      if (!(next.first == x && x == prev_x && prev_x == prev_prev_x) ||
          !(next.second == y && y == prev_y && prev_y == prev_prev_y)) {
        this->dijkstra(x, y, prev_x, prev_y, next.first, next.second, dest_x,
                       dest_y);
      }
    }
  }

public:
  Graph(std::string file) {
    std::string FILE_DATA = load_file(file);
    this->rows = std::ranges::count(FILE_DATA, '\n');
    this->cols = FILE_DATA.substr(0, FILE_DATA.find('\n')).size();
    printf("Rows %lu, cols: %lu\n", rows, cols);
    size_t x = 0;
    size_t y = 0;
    for (char c : FILE_DATA) {
      if (std::isdigit(c)) {
        this->grid.push_back({.visited = false,
                              .x = x,
                              .y = y,
                              .weight = static_cast<size_t>((c - 0x30)),
                              .curr_distance = 0xFFFFFFFFFFFFFFFF});
        x++;
      } else if (c == '\n') {
        y++;
        x = 0;
      }
    }
    for (size_t i = 0; i < this->rows; i++) {
      for (size_t j = 0; j < this->cols; j++) {
        printf("%c", (char)(this->grid[this->idx(j, i)].weight + 0x30));
      }
      printf("\n");
    }
  }
  size_t lowest_heat_loss(size_t x, size_t y, size_t dest_x, size_t dest_y) {
    this->grid.at(this->idx(x, y)).curr_distance = 0;
    stack_1gb();
    this->dijkstra(0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
                   0xFFFFFFFFFFFFFFFF, 0, 0, dest_x, dest_y);
    for (size_t i = 0; i < this->rows; i++) {
      for (size_t j = 0; j < this->cols; j++) {
        printf("%c", this->grid[this->idx(j, i)].visited ? '#' : '.');
      }
      printf("\n");
    }
    return this->grid[this->idx(dest_x, dest_y)].curr_distance;
  }
  size_t get_cols() { return this->cols; }
  size_t get_rows() { return this->rows; }
};