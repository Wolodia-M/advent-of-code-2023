#include "cstdint"
#include "cstdio"
#include "grid.hpp"
int main(int argc, char **argv) {
  Grid grid("test31_32");
  size_t max = 0;
  // Tow row
  for (size_t i = 1; i < grid.get_columns() - 1; i++) {
    size_t et = grid.emmit_light(i, 0, Direction::DOWN);
    if (max < et) {
      max = et;
    }
    grid.clear();
  }
  // Bottom row
  for (size_t i = 1; i < grid.get_columns() - 1; i++) {
    size_t et = grid.emmit_light(i, grid.get_rows() - 1, Direction::UP);
    if (max < et) {
      max = et;
    }
    grid.clear();
  }
  // Left column
  for (size_t i = 1; i < grid.get_rows() - 1; i++) {
    size_t et = grid.emmit_light(0, i, Direction::RIGHT);
    if (max < et) {
      max = et;
    }
    grid.clear();
  }
  // Right column
  for (size_t i = 1; i < grid.get_rows() - 1; i++) {
    size_t et = grid.emmit_light(grid.get_columns() - 1, i, Direction::LEFT);
    if (max < et) {
      max = et;
    }
    grid.clear();
  }
  // Top-left corner
  {
    size_t et = grid.emmit_light(0, 0, Direction::RIGHT);
    if (max < et) {
      max = et;
    }
    grid.clear();
  }
  {
    size_t et = grid.emmit_light(0, 0, Direction::DOWN);
    if (max < et) {
      max = et;
    }
    grid.clear();
  }
  // Top-right corner
  {
    size_t et = grid.emmit_light(grid.get_columns() - 1, 0, Direction::LEFT);
    if (max < et) {
      max = et;
    }
    grid.clear();
  }
  {
    size_t et = grid.emmit_light(grid.get_columns() - 1, 0, Direction::DOWN);
    if (max < et) {
      max = et;
    }
    grid.clear();
  }
  // Bottom-left corner
  {
    size_t et = grid.emmit_light(0, grid.get_rows() - 1, Direction::RIGHT);
    if (max < et) {
      max = et;
    }
    grid.clear();
  }
  {
    size_t et = grid.emmit_light(0, grid.get_rows() - 1, Direction::UP);
    if (max < et) {
      max = et;
    }
    grid.clear();
  }
  // Bottom-right corner
  {
    size_t et = grid.emmit_light(grid.get_columns() - 1, grid.get_rows() - 1,
                                 Direction::LEFT);
    if (max < et) {
      max = et;
    }
    grid.clear();
  }
  {
    size_t et = grid.emmit_light(grid.get_columns() - 1, grid.get_rows() - 1,
                                 Direction::UP);
    if (max < et) {
      max = et;
    }
    grid.clear();
  }
  printf("Max energized tiles: %lu\n", max);
  return 0;
}