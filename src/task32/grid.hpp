#include "../loader.hpp"
#include "algorithm"
#include "cstdint"
#include "cstdio"
#include "cstdlib"
#include "sstream"
#include "string"
#include "sys/resource.h"
#include "vector"
enum ObjectType {
  NONE = '.',
  SPLITTER_VERTICAL = '|',
  SPLITTER_HORIZONTAL = '-',
  MIRROR_LEFT_RIGHT = '/',
  MIRROR_RIGHT_LEFT = '\\',
};
enum Direction { UP, RIGHT, DOWN, LEFT };
class Object {
protected:
  bool enrgized;
  std::vector<Direction> incoming_light;

public:
  Object() {
    this->enrgized = false;
    this->incoming_light = {};
  }
  virtual std::vector<Direction> reflect_light(Direction incoming_dir) = 0;
  virtual char print() = 0;
  bool get_status() { return this->enrgized; }
  void clear() {
    this->enrgized = false;
    this->incoming_light = {};
  }
};
class ObjectNone : public Object {
public:
  std::vector<Direction> reflect_light(Direction incoming_dir) override {
    for (auto dir : this->incoming_light) {
      if (dir == incoming_dir) {
        return {};
      }
    }
    this->enrgized = true;
    this->incoming_light.push_back(incoming_dir);
    return {incoming_dir};
  }
  char print() override { return ObjectType::NONE; }
};
class ObjectSplitterVertical : public Object {
public:
  std::vector<Direction> reflect_light(Direction incoming_dir) override {
    for (auto dir : this->incoming_light) {
      if (dir == incoming_dir) {
        return {};
      }
    }
    this->enrgized = true;
    this->incoming_light.push_back(incoming_dir);
    if (incoming_dir == Direction::LEFT || incoming_dir == Direction::RIGHT) {
      return {Direction::UP, Direction::DOWN};
    } else {
      return {incoming_dir};
    }
  }
  char print() override { return ObjectType::SPLITTER_VERTICAL; }
};
class ObjectSplitterHorizontal : public Object {
public:
  std::vector<Direction> reflect_light(Direction incoming_dir) override {
    for (auto dir : this->incoming_light) {
      if (dir == incoming_dir) {
        return {};
      }
    }
    this->enrgized = true;
    this->incoming_light.push_back(incoming_dir);
    if (incoming_dir == Direction::UP || incoming_dir == Direction::DOWN) {
      return {Direction::RIGHT, Direction::LEFT};
    } else {
      return {incoming_dir};
    }
  }
  char print() override { return ObjectType::SPLITTER_HORIZONTAL; }
};
// incoming_dir means direction of incoming light (eg. RIGHT means
// right-movinf light, so it need to enter on left side)
class ObjectMirrorLeftRight : public Object {
public:
  std::vector<Direction> reflect_light(Direction incoming_dir) override {
    for (auto dir : this->incoming_light) {
      if (dir == incoming_dir) {
        return {};
      }
    }
    this->enrgized = true;
    this->incoming_light.push_back(incoming_dir);
    switch (incoming_dir) {
    case Direction::UP:
      return {Direction::RIGHT};
      break;
    case Direction::RIGHT:
      return {Direction::UP};
      break;
    case Direction::DOWN:
      return {Direction::LEFT};
      break;
    case Direction::LEFT:
      return {Direction::DOWN};
      break;
    }
    return {incoming_dir};
  }
  char print() override { return ObjectType::MIRROR_LEFT_RIGHT; }
};
// incoming_dir means direction of incoming light (eg. RIGHT means
// right-movinf light, so it need to enter on left side)
class ObjectMirrorRightLeft : public Object {
public:
  std::vector<Direction> reflect_light(Direction incoming_dir) override {
    for (auto dir : this->incoming_light) {
      if (dir == incoming_dir) {
        return {};
      }
    }
    this->enrgized = true;
    this->incoming_light.push_back(incoming_dir);
    switch (incoming_dir) {
    case Direction::UP:
      return {Direction::LEFT};
      break;
    case Direction::RIGHT:
      return {Direction::DOWN};
      break;
    case Direction::DOWN:
      return {Direction::RIGHT};
      break;
    case Direction::LEFT:
      return {Direction::UP};
      break;
    }
    return {incoming_dir};
  }
  char print() override { return ObjectType::MIRROR_RIGHT_LEFT; }
};
class Grid {
private:
  Object **grid;
  size_t columns;
  size_t rows;
  size_t idx(size_t x, size_t y) { return x + y * this->columns; }
  void emmit_light_internal(size_t x, size_t y, Direction dir) {
    // printf("x: %lu, y: %lu, dir: %s\n", x, y,
    //        [&dir]() -> std::string {
    //          switch (dir) {
    //          case UP:
    //            return "UP";
    //          case RIGHT:
    //            return "RIGHT";
    //          case DOWN:
    //            return "DOWN";
    //          case LEFT:
    //            return "LEFT";
    //          }
    //        }()
    //                        .c_str());
    // fflush(stdout);
    auto dirs = this->grid[this->idx(x, y)]->reflect_light(dir);
    for (auto dir1 : dirs) {
      size_t new_x, new_y;
      switch (dir1) {
      case UP:
        if (y > 0) {
          new_x = x;
          new_y = y - 1;
          this->emmit_light_internal(new_x, new_y, dir1);
        }
        break;
      case RIGHT:
        if (x < (this->columns - 1)) {
          new_x = x + 1;
          new_y = y;
          this->emmit_light_internal(new_x, new_y, dir1);
        }
        break;
      case DOWN:
        if (y < (this->rows - 1)) {
          new_x = x;
          new_y = y + 1;
          this->emmit_light_internal(new_x, new_y, dir1);
        }
        break;
      case LEFT:
        if (x > 0) {
          new_x = x - 1;
          new_y = y;
          this->emmit_light_internal(new_x, new_y, dir1);
        }
        break;
      }
    }
  }

public:
  Grid(std::string grid) {
    std::string FILE_DATA = load_file(grid);
    this->rows = std::ranges::count(FILE_DATA, '\n');
    this->columns = FILE_DATA.substr(0, FILE_DATA.find('\n')).size();
    this->grid =
        (Object **)malloc(this->rows * this->columns * sizeof(Object *));
    std::stringstream FILE_DATA_STREAM(FILE_DATA);
    size_t y = 0;
    std::string line;
    while (std::getline(FILE_DATA_STREAM, line, '\n')) {
      for (size_t x = 0; x < this->columns; x++) {
        this->grid[this->idx(x, y)] = [&line, &x]() -> Object * {
          switch (line[x]) {
          case ObjectType::NONE:
            return new ObjectNone;
          case ObjectType::SPLITTER_VERTICAL:
            return new ObjectSplitterVertical;
          case ObjectType::SPLITTER_HORIZONTAL:
            return new ObjectSplitterHorizontal;
          case ObjectType::MIRROR_LEFT_RIGHT:
            return new ObjectMirrorLeftRight;
          case ObjectType::MIRROR_RIGHT_LEFT:
            return new ObjectMirrorRightLeft;
          }
          return new ObjectNone;
        }();
      }
      y++;
    }
    printf("Grid:\n");
    for (size_t i = 0; i < this->rows; i++) {
      for (size_t j = 0; j < this->columns; j++) {
        printf("%c", this->grid[this->idx(j, i)]->print());
      }
      printf("\n");
    }
  }
  size_t emmit_light(size_t x, size_t y, Direction dir) {
    size_t kStackSize = 1024L * 1024L * 1024L;
    struct rlimit rl;
    int result;
    result = getrlimit(RLIMIT_STACK, &rl);
    {
      if (rl.rlim_cur < kStackSize) {
        rl.rlim_cur = kStackSize;
        result = setrlimit(RLIMIT_STACK, &rl);
        if (result != 0) {
          printf("setrlimit returned result = %d\n", result);
          fflush(stdout);
        }
      }
    }
    auto dirs = this->grid[this->idx(x, y)]->reflect_light(dir);
    for (auto dir1 : dirs) {
      size_t new_x, new_y;
      switch (dir1) {
      case UP:
        if (y > 0) {
          new_x = x;
          new_y = y - 1;
          this->emmit_light_internal(new_x, new_y, dir1);
        }
        break;
      case RIGHT:
        if (x < (this->columns - 1)) {
          new_x = x + 1;
          new_y = y;
          this->emmit_light_internal(new_x, new_y, dir1);
        }
        break;
      case DOWN:
        if (y < (this->rows - 1)) {
          new_x = x;
          new_y = y + 1;
          this->emmit_light_internal(new_x, new_y, dir1);
        }
        break;
      case LEFT:
        if (x > 0) {
          new_x = x - 1;
          new_y = y;
          this->emmit_light_internal(new_x, new_y, dir1);
        }
        break;
      }
    }
    size_t ret = 0;
    printf("Energized grid:\n");
    for (size_t i = 0; i < this->rows; i++) {
      for (size_t j = 0; j < this->columns; j++) {
        if (this->grid[this->idx(j, i)]->get_status()) {
          printf("#");
          ret++;
        } else {
          printf(".");
        }
      }
      printf("\n");
    }
    return ret;
  }
  size_t get_columns() { return this->columns; }
  size_t get_rows() { return this->rows; }
  void clear() {
    for (size_t i = 0; i < this->rows; i++) {
      for (size_t j = 0; j < this->columns; j++) {
        this->grid[this->idx(j, i)]->clear();
      }
    }
  }
};