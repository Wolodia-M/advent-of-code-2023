#pragma once
#include "../loader.hpp"
#include "algorithm"
#include "cstdio"
#include "cstdlib"
#include "sstream"
#include "string"
#include "vector"
enum Tile : char {
  FOREST = '#',
  PATH = '.',
  SLOPE_NORTH = '^',
  SLOPE_SOUTH = 'v',
  SLOPE_EAST = '>',
  SLOPE_WEST = '<',
};
struct Position {
  int row;
  int column;
  int idx;
  const bool operator==(const Position other) {
    if (this->row == other.row && this->column == other.column) {
      return true;
    }
    return false;
  }
};
using Path = std::vector<Position>;
class Map {
private:
  int rows;
  int columns;
  Tile *map;
  int idx(int x, int y) { return x + y * this->columns; }
  void pathfind(Position start, Position end, Path current, int x, int y,
                std::vector<Path> *store) {
    auto contains = [&current](Position t) -> bool {
      //   return (std::find(current.begin(), current.end(), t) !=
      //   current.end());
      for (auto elem : current) {
        if (elem == t) {
          return true;
        }
      }
      return false;
    };
    auto valid = [this](int x, int y) -> bool {
      return (((x >= 0) && (x < this->columns)) &&
              ((y >= 0) && (y < this->rows)));
    };
    auto valid_next = [&x, &y, this](int new_x, int new_y) -> bool {
      if (this->map[this->idx(new_x, new_y)] == Tile::PATH) {
        return true;
      }
      if ((new_x == x + 1) &&
          (this->map[this->idx(new_x, new_y)] == Tile::SLOPE_EAST)) {
        return true;
      }
      if ((new_x == x - 1) &&
          (this->map[this->idx(new_x, new_y)] == Tile::SLOPE_WEST)) {
        return true;
      }
      if ((new_y == y + 1) &&
          (this->map[this->idx(new_x, new_y)] == Tile::SLOPE_SOUTH)) {
        return true;
      }
      if ((new_y == y - 1) &&
          (this->map[this->idx(new_x, new_y)] == Tile::SLOPE_NORTH)) {
        return true;
      }
      return false;
    };
    Position curr = {.row = y, .column = x, .idx = this->idx(x, y)};
    current.push_back(curr);
    static const int dr[] = {-1, 0, 1, 0};
    static const int dc[] = {0, 1, 0, -1};
    for (int i = 0; i < 4; ++i) {
      int new_y = y + dr[i];
      int new_x = x + dc[i];
      if (curr == end) {
        store->push_back(current);
        // printf("1\n");
        // fflush(stdout);
        break;
      }
      if (valid(new_x, new_y) &&
          !contains({.row = new_y,
                     .column = new_x,
                     .idx = this->idx(new_x, new_y)}) &&
          valid_next(new_x, new_y)) {
        // printf("current_size: %lu, x: %d, y: %d\n", current.size(), new_x,
        //    new_y);
        this->pathfind(start, end, current, new_x, new_y, store);
      }
    }
    current.pop_back();
    return;
  }

public:
  Map(std::string file) {
    std::string FILE_DATA = load_file(file);
    std::stringstream FILE_DATA_STREAM(FILE_DATA);
    std::string line;
    this->rows = std::ranges::count(FILE_DATA, '\n') + 1;
    this->columns = FILE_DATA.substr(0, FILE_DATA.find('\n')).size() + 1;
    printf("Rows: %d, columns: %d\n", this->rows, this->columns);
    this->map = (Tile *)malloc(this->columns * this->rows * sizeof(Tile));
    int y = 0;
    while (std::getline(FILE_DATA_STREAM, line, '\n')) {
      for (int x = 0; x < line.size(); x++) {
        this->map[this->idx(x, y)] = static_cast<Tile>(line.at(x));
      }
      y++;
    }
    printf("Map:\n");
    for (int i = 0; i < this->rows; i++) {
      for (int j = 0; j < this->columns; j++) {
        printf("%c", this->map[this->idx(j, i)]);
      }
      printf("\n");
    }
  }
  Position get_start_pos() {
    for (int x = 0; x < this->columns; x++) {
      if (this->map[this->idx(x, 0)] == Tile::PATH) {
        return {.row = 0, .column = (int)x, .idx = this->idx(x, 0)};
      }
    }
    return {0, 0, 0};
  }
  Position get_end_pos() {
    for (int x = 0; x < this->columns; x++) {
      if (this->map[this->idx(x, this->rows - 1)] == Tile::PATH) {
        return {.row = this->rows - 1,
                .column = (int)x,
                .idx = this->idx(x, this->rows - 1)};
      }
    }
    return {0, 0, 0};
  }
  Path find_path(Position start, Position end) {
    std::vector<Path> found;
    printf("Running pathfinding...\n");
    Path curr = {};
    this->pathfind(start, end, curr, start.column, start.row, &found);
    Path ret = {};
    for (Path p : found) {
      if (ret.empty()) {
        ret = p;
      } else {
        if (p.size() > ret.size()) {
          ret = p;
        }
      }
    }
    return ret;
  }
  ~Map() { free(this->map); }
};