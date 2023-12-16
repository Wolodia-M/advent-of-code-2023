#pragma once
#include "../loader.hpp"
#include "cstdio"
#include "optional"
#include "sstream"
#include "string"
#include "vector"
class Grid {
private:
  // grid.at(row).at(column) y->row x->column
  std::vector<std::vector<char>> grid;
  void alloc(int rows, int columns) {
    for (int i = 0; i <= rows; i++) {
      std::vector<char> row;
      for (int j = 0; j <= columns; j++) {
        row.push_back('.');
      }
      this->grid.push_back(row);
    }
  }
  bool write_at(int x, int y, char val) {
    try {
      this->grid.at(y).at(x) = val;
      return true;
    } catch (std::exception &e) {
      return false;
    }
  }

public:
  Grid(std::string file) {
    std::string data = load_file(file);
    std::string line;
    std::getline(std::stringstream(data), line, '\n');
    int cols = line.size();
    int rows = data.size() / (cols + 1);
    this->alloc(rows, cols);
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        if (!this->write_at(j, i, data.at(((cols + 1) * i) + j))) {
          std::printf("ERR parsing file");
        }
      }
    }
  }
  std::optional<char> at(int x, int y) {
    try {
      char ret = this->grid.at(y).at(x);
      return ret;
    } catch (std::exception &e) {
      return {};
    }
  }
};