#include "../loader.hpp"
#include "../utils.hpp"
#include "cstdint"
#include "cstdio"
#include "cstdlib"
#include "sstream"
#include "stdexcept"
#include "string"
#include "vector"
const int GRID_COLS = 1024;
const int GRID_ROWS = 1024;
enum Direction {
  UP = 'U',
  RIGHT = 'R',
  DOWN = 'D',
  LEFT = 'L',
};
struct DigEntry {
  Direction dir;
  int len;
  struct {
    uint8_t R;
    uint8_t G;
    uint8_t B;
  } color;
};
class Grid {
private:
  char grid[GRID_COLS * GRID_ROWS];
  std::vector<DigEntry> commands;
  size_t idx(int x, int y) { return x + y * GRID_COLS; }
  void dfs_explore(std::vector<std::vector<char>> *nodes,
                   std::vector<std::vector<bool>> *visited, size_t stx,
                   size_t sty) {
    auto isValid = [](int row, int col, int numRows, int numCols) -> bool {
      return (row >= 0 && row < numRows && col >= 0 && col < numCols);
    };
    auto isFloor = [](char c) -> bool { return (c == '.'); };
    visited->at(sty).at(stx) = true;
    static const int dr[] = {-1, 0, 1, 0};
    static const int dc[] = {0, 1, 0, -1};
    for (int i = 0; i < 4; ++i) { // Explore in all directions
      int y = sty + dr[i];
      int x = stx + dc[i];
      printf("x: %d, y: %d\n", x, y);
      if (isValid(y, x, nodes->size(), nodes->at(0).size()) &&
          !visited->at(y).at(x) && isFloor(nodes->at(sty).at(stx))) {
        dfs_explore(nodes, visited, x, y);
      }
    }
  }

public:
  Grid(std::string file) {
    for (size_t i = 0; i < GRID_COLS * GRID_ROWS; i++) {
      this->grid[i] = '.';
    }
    std::string FILE_DATA = load_file(file);
    std::stringstream FILE_DATA_STREAM(FILE_DATA);
    std::string line;
    std::string elem;
    while (std::getline(FILE_DATA_STREAM, line, '\n')) {
      std::stringstream line_stream(line);
      int step = 0;
      DigEntry dge;
      while (std::getline(line_stream, elem, ' ')) {
        if (step == 0) {
          dge.dir = static_cast<Direction>(elem[0]);
          step = 1;
        } else if (step == 1) {
          if (std::isdigit(elem[0])) {
            dge.len = std::atoi(elem.c_str());
            step = 2;
          }
        } else if (step == 2) {
          elem = elem.substr(2, elem.size() - 2);
          dge.color.R = std::strtol(elem.substr(0, 2).c_str(), NULL, 16);
          dge.color.G = std::strtol(elem.substr(1, 3).c_str(), NULL, 16);
          dge.color.B = std::strtol(elem.substr(4, 5).c_str(), NULL, 16);
        }
      }
      this->commands.push_back(dge);
    }
  }
  void print_grid() {
    int start_x = GRID_COLS + 1;
    int start_y = GRID_ROWS + 1;
    int end_x = 0;
    int end_y = 0;
    for (int i = 0; i < GRID_ROWS; i++) {
      for (int j = 0; j < GRID_COLS; j++) {
        if (this->grid[this->idx(j, i)] == '#') {
          if (j < start_x) {
            start_x = j;
          }
          if (j > end_x) {
            end_x = j;
          }
          if (i < start_y) {
            start_y = i;
          }
          if (i > end_y) {
            end_y = i;
          }
        }
      }
    }
    printf("X:[%d, %d], Y:[%d, %d]\n", start_x, end_x, start_y, end_y);
    for (int i = start_y; i <= end_y; i++) {
      for (int j = start_x; j <= end_x; j++) {
        printf("%c", this->grid[this->idx(j, i)]);
      }
      printf("\n");
    }
  }
  void perform_digging() {
    int x = GRID_COLS / 2;
    int y = GRID_ROWS / 2;
    this->grid[this->idx(x, y)] = '#';
    for (auto dge : this->commands) {
      switch (dge.dir) {
      case UP: {
        for (int i = 0; i <= dge.len; i++) {
          if (y - i < 0) {
            throw std::runtime_error(
                std::string("Error, direction up, y == ") + std::to_string(y) +
                std::string(", y - i == ") + std::to_string(y - i));
          }
          this->grid[this->idx(x, y - i)] = '#';
        }
        y -= dge.len;
      } break;
      case RIGHT: {
        for (int i = 0; i < dge.len; i++) {
          if (x + i >= GRID_COLS) {
            throw std::runtime_error(
                std::string("Error, direction up, x == ") + std::to_string(x) +
                std::string(", x + i == ") + std::to_string(x + i));
          }
          this->grid[this->idx(x + i, y)] = '#';
        }
        x += dge.len;
      } break;
      case DOWN: {
        for (int i = 0; i <= dge.len; i++) {
          if (y + i >= GRID_ROWS) {
            throw std::runtime_error(
                std::string("Error, direction up, y == ") + std::to_string(y) +
                std::string(", y + i == ") + std::to_string(y + i));
          }
          this->grid[this->idx(x, y + i)] = '#';
        }
        y += dge.len;
      } break;
      case LEFT: {
        for (int i = 0; i < dge.len; i++) {
          if (x - i < 0) {
            throw std::runtime_error(
                std::string("Error, direction up, x == ") + std::to_string(x) +
                std::string(", x - i == ") + std::to_string(x - i));
          }
          this->grid[this->idx(x - i, y)] = '#';
        }
        x -= dge.len;
      } break;
      }
    }
    this->print_grid();
  }
  void dig_inside() {
    stack_1gb();
    int start_x = GRID_COLS + 1;
    int start_y = GRID_ROWS + 1;
    int end_x = 0;
    int end_y = 0;
    for (int i = 0; i < GRID_ROWS; i++) {
      for (int j = 0; j < GRID_COLS; j++) {
        if (this->grid[this->idx(j, i)] == '#') {
          if (j < start_x) {
            start_x = j;
          }
          if (j > end_x) {
            end_x = j;
          }
          if (i < start_y) {
            start_y = i;
          }
          if (i > end_y) {
            end_y = i;
          }
        }
      }
    }
    // std::vector<int> xs;
    int x = 0;
    int y = 0;
    bool found_1 = false;
    std::vector<std::vector<char>> nodes;
    std::vector<std::vector<bool>> visited;
    for (int i = start_y; i <= end_y; i++) {
      nodes.push_back({});
      visited.push_back({});
      for (int j = start_x; j <= end_x; j++) {
        char *c = &this->grid[this->idx(j, i)];
        if (!found_1 && *c == '#') {
          found_1 = true;
          x = j + 2;
          y = i + 2;
          // xs.push_back(j);
        }
        nodes.at(i - start_y).push_back(*c);
        visited.at(i - start_y).push_back(false);
      }
      // int k = 0;
      // while (k < (int)xs.size() - 1) {
      //   int x1 = xs.at(k);
      //   k++;
      //   int x2 = xs.at(k);
      //   k++;
      //   if (x2 == x1 + 1) {
      //     k--;
      //     continue;
      //   }
      //   for (int j = x1; j <= x2; j++) {
      //     this->grid[this->idx(j, i)] = '#';
      //   }
      // }
      // xs.clear();
    }
    printf("DFS\n");
    fflush(stdout);
    this->dfs_explore(&nodes, &visited, x - start_x, y - start_y);
    printf("Writeback\n");
    fflush(stdout);
    for (size_t i = 0; i < nodes.size(); i++) {
      for (size_t j = 0; j < nodes[0].size(); j++) {
        if (visited[i][j]) {
          this->grid[this->idx(j + start_x, i + start_y)] = '#';
        }
      }
    }
    this->print_grid();
  }
  size_t get_volume() {
    size_t vol = 0;
    for (int i = 0; i < GRID_ROWS; i++) {
      for (int j = 0; j < GRID_COLS; j++) {
        if (this->grid[this->idx(j, i)] == '#') {
          vol++;
        }
      }
    }
    return vol;
  }
};