#pragma once
#include "cstddef"
#include "stdexcept"
template <class T, size_t max_x, size_t max_y, size_t max_z> class Grid {
private:
  T grid[max_x][max_y][max_z];

public:
  T get(size_t x, size_t y, size_t z) {
    if (x >= max_x) {
      throw std::out_of_range("x >= max_x");
    }
    if (y >= max_y) {
      throw std::out_of_range("y >= max_y");
    }
    if (z >= max_z) {
      throw std::out_of_range("z >= max_z");
    }
    return this->grid[x][y][z];
  }
  void set(size_t x, size_t y, size_t z, T val) {
    if (x >= max_x) {
      throw std::out_of_range("x >= max_x");
    }
    if (y >= max_y) {
      throw std::out_of_range("y >= max_y");
    }
    if (z >= max_z) {
      throw std::out_of_range("z >= max_z");
    }
    this->grid[x][y][z] = val;
  }
  T *ref(size_t x, size_t y, size_t z) {
    if (x >= max_x) {
      throw std::out_of_range("x >= max_x");
    }
    if (y >= max_y) {
      throw std::out_of_range("y >= max_y");
    }
    if (z >= max_z) {
      throw std::out_of_range("z >= max_z");
    }
    return &this->grid[x][y][z];
  }
};